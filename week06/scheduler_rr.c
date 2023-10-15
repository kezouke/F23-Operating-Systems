#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

#define PS_MAX 10

// holds the scheduling data of one process
typedef struct {
    int idx; // process idx (index)
    int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
    int burst;
    int remaining_quantum;
} ProcessData;

int quantum;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time = 0; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

int process_queue[PS_MAX];
int queue_size = 0; // current size of the process queue

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;

void read_file(FILE *file) {

    int idx, at, bt;
    data_size = 0;

    char buffer[100];
    fgets(buffer, sizeof(buffer), file);

    while (fscanf(file, "%d %d %d", &idx, &at, &bt) == 3) {

        if (data_size < PS_MAX) {
            data[data_size].idx = idx;
            data[data_size].at = at;
            data[data_size].bt = bt;
            data[data_size].rt = 0;
            data[data_size].wt = 0;
            data[data_size].ct = 0;
            data[data_size].tat = 0;
            data[data_size].burst = bt;
            // Increment data size
            data_size++;
        } else {
            printf("Too many processes. Maximum limit reached.\n");
            break;
        }
    }

    for (int i = 0; i < PS_MAX; i++) {
        ps[i] = 0;
        process_queue[i] = -1;
    }

}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
    // TODO: send signal SIGCONT to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    if (kill(process, 0) != -1) {
        kill(process, SIGCONT);
    }
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
    // TODO: send signal SIGTSTP to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    if (kill(process, 0) != -1) {
        kill(process, SIGTSTP);
    }
}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    if (kill(process, 0) != -1) {
        kill(process, SIGTERM);
    }
}

// create a process using fork
void create_process(int new_process) {

    // TODO: stop the running process

    // TODO: fork a new process and add it to ps array

    // TODO: Now the idx of the running process is new_process

    // TODO: The scheduler process runs the program "./worker {new_process}"
    // using one of the exec functions like execvp
    // 1. Stop the running process (if any)
    if (ps[new_process] != 0) {
        resume(ps[new_process]);
        printf("Scheduler: Resuming Process %d (Remaining Time: %d)", running_process, data[running_process].burst);
    } else {
        pid_t child_pid = fork();
        if (child_pid == 0) {
            // This is the child process

            // Prepare command for exec (e.g., "./worker" "new_process")
            char process_idx_str[20];
            sprintf(process_idx_str, "%d", new_process);
            char *args[] = {"./worker", process_idx_str, NULL};

            // Execute the worker program for the new process
            execvp(args[0], args);

            // If execvp returns, it failed
            perror("Exec failed");
            exit(EXIT_FAILURE);
        } else {
            // 3. Add the child process to the ps array
            ps[new_process] = child_pid;

            // 4. Update the running process
            running_process = new_process;
            data[new_process].rt = total_time - data[new_process].at;
            printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process,
                   data[running_process].burst);
        }
    }
}

// find next process for running
ProcessData find_next_process() {

    // location of next process in {data} array
    for (int i = 0; i < data_size; i++) {
        if (data[i].at <= total_time && data[i].burst > 0) {
            int found = 0;
            // Check if the process is already in the queue
            for (int j = 0; j < queue_size; j++) {
                if (process_queue[j] == data[i].idx) {
                    found = 1;
                    break;
                }
            }
            // If not in the queue, add it
            if (!found) {
                process_queue[queue_size++] = data[i].idx;
            }
        }
        if (data[i].burst == 0) {
            // remove it from queue
            for (int j = 0; j < queue_size; j++) {
                if (process_queue[j] == data[i].idx) {
                    process_queue[j] = -1;
                }
            }
        }
    }

    // Find the next process to run
    int location = -1;
    if (queue_size > 0) {
        location = process_queue[0];
        // Shift the queue to remove the first element
        for (int i = 0; i < queue_size - 1; i++) {
            process_queue[i] = process_queue[i + 1];
        }
        queue_size--;
    }

    // if next_process did not arrive so far,
    // then we recursively call this function after incrementing total_time
    if (location == -1) {
        for(int i = 0; i < data_size; i++) {
            if(data[i].burst > 0) {
                location = i;
                break;
            }
        }

        for(int i=0; i < data_size; i++) {
            // Check if the process has arrived and hasn't been completed
            if (data[i].burst > 0 && data[i].at <= data[location].at) {
                // If the location is not set (i.e., it's the first valid process found), or
                // it's an FCFS algorithm and the current process arrived before the one in location,
                // update the location to the current process
                location = i;
            }
        }

        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);

        // increment the time
        total_time++;
        return find_next_process();
    }

    //return the data of next process
    return data[location];
}


// reports the metrics and simulation results
void report() {
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i = 0; i < data_size; i++) {
        printf("process %d: \n", i);
        printf("	at=%d\n", data[i].at);
        printf("	bt=%d\n", data[i].bt);
        printf("	ct=%d\n", data[i].ct);
        printf("	wt=%d\n", data[i].wt);
        printf("	tat=%d\n", data[i].tat);
        printf("	rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %d\n", data_size);
    float avg_wt = (float) sum_wt / data_size;
    float avg_tat = (float) sum_tat / data_size;
    printf("Average results for this run:\n");
    printf("	avg_wt=%f\n", avg_wt);
    printf("	avg_tat=%f\n", avg_tat);
}

void check_burst() {

    for (int i = 0; i < data_size; i++)
        if (data[i].burst > 0)
            return;

    // report simulation results
    report();

    // terminate the scheduler :)
    exit(EXIT_SUCCESS);
}


// This function is called every one second as handler for SIGALRM signal
void schedule_handler(int signum) {
    // increment the total time
    total_time++;



    /* TODO
    1. If there is a worker process running, then decrement its remaining burst
    and print messages as follows:
    "Scheduler: Runtime: {total_time} seconds"
    "Process {running_process} is running with {data[running_process].burst} seconds left"

    1.A. If the worker process finished its burst time, then the scheduler should terminate
    the running process and prints the message:
    "Scheduler: Terminating Process {running_process} (Remaining Time: {data[running_process].burst})"

    then the scheduler waits for its termination and there will be no running processes anymore.
    Since the process is terminated, we can calculate its metrics {ct, tat, wt}

    2. After that, we need to find the next process to run {next_process}.

    // this call will check the bursts of all processes
    check_burst();
    3. If {next_process} is not running, then we need to check the current process
    3.A. If the current process is running, then we should stop the current running process
    and print the message:
    "Scheduler: Stopping Process {running_process} (Remaining Time: {data[running_process].burst})"
    3.B. set current process as the running process.
    3.C.1. then create a new process for {running_process} and print the message:
    "Scheduler: Starting Process {running_process} (Remaining Time: {data[running_process].burst})"
    Here we have the first response to the process {running_process} and we can calculate the metric {rt}
    3.C.2. or resume the process {running_process} if it is stopped and print the message:
    "Scheduler: Resuming Process {running_process} (Remaining Time: {data[running_process].burst})"
    */

    // 1. If there is a worker process running
    if (running_process != -1) {
        data[running_process].remaining_quantum--;
        // Decrement its remaining burst
        data[running_process].burst--;

        // Print messages
        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        // 1.A. If the worker process finished its burst time
        if (data[running_process].burst == 0) {
            // Print termination message
            printf("Scheduler: Terminating Process %d (Remaining Time: 0)\n", running_process);
            terminate(ps[running_process]);
            // Wait for the process to terminate
            int status;
            waitpid(ps[running_process], &status, 0);

            // Calculate metrics for the terminated process (ct, tat, wt)
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;

            // Update the running process to indicate that there is no running process
            running_process = -1;
        }
    }

    if (running_process == -1 || data[running_process].remaining_quantum == 0) {
        if (running_process == -1) {
            check_burst();
        } else {
            suspend(ps[running_process]);
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        }
        // 2. Find the next process to run
        ProcessData next_process = find_next_process();

        running_process = next_process.idx;
        data[running_process].remaining_quantum = quantum;
        create_process(running_process);

        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
    }
}

int read_quantum() {
    int quantum;
    printf("Enter the time quantum: ");
    scanf("%d", &quantum);
    return quantum;
}

int main(int argc, char *argv[]) {

    // read the data file
    FILE *in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);
    }

    quantum = read_quantum();


    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while (1); // infinite loop
}
