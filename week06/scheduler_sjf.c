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
    int burst; // remaining burst (this should decrement when the process is being executed)
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet


unsigned data_size;

void read_file(FILE *file) {
    int idx, at, bt;
    data_size = 0;
    memset(ps, 0, sizeof(ps));

    fscanf(file, "%*s %*s %*s");
    while (fscanf(file, "%d %d %d", &idx, &at, &bt) != EOF) {
        if (data_size < PS_MAX) {
            data[data_size].idx = idx;
            data[data_size].at = at;
            data[data_size].bt = bt;
            data[data_size].rt = -1;
            data[data_size].wt = 0;
            data[data_size].ct = 0;
            data[data_size].tat = 0;
            data[data_size].burst = bt;
            data_size++;
        } else {
            printf("ERROR: Maximum number of processes reached\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
}


void resume(pid_t process) {
    if (kill(process, 0) != -1) {
        kill(process, SIGCONT);
    }
}


void suspend(pid_t process) {
    if (kill(process, 0) != -1) {
        kill(process, SIGTSTP);
    }
}

void terminate(pid_t process) {
    if (kill(process, 0) != -1) {
        kill(process, SIGTERM);
    }
}

void create_process(int new_process) {
    // stopping the running process omitted because it is non-preemptive SJF algorithm

    pid_t child_pid = fork();
    if (child_pid == 0) {
        char process_index[10];
        sprintf(process_index, "%d", new_process);
        char *args[] = {"./worker", process_index, NULL};
        execvp(args[0],
               args); // scheduler runs the program "./worker {new_process}" using one of the exec functions like execvp
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (child_pid > 0) {
        ps[new_process] = child_pid; // added child_pid to ps array
        running_process = new_process; // now the idx of the running process is new_process
        data[new_process].rt = total_time - data[new_process].at;
    } else {
        perror("ERROR: fork failed\n");
        exit(EXIT_FAILURE);
    }
}

// find next process for running
ProcessData find_next_process() {
    // process_next_idx of next process in {data} array
    int process_next_idx = -1;
    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0 && data[i].at <= total_time) {
            process_next_idx = i;
            break;
        }
    }

    // if no processes arrived yet we find the closest one for printing
    if (process_next_idx == -1) {
        for (int i = 0; i < data_size; i++) {
            if (data[i].burst > 0) {
                process_next_idx = i;
                break;
            }
        }
        for (int i = 0; i < data_size; i++) {
            if (data[i].burst > 0) {
                if (data[i].at - total_time < data[process_next_idx].at - total_time ||
                    (data[i].at - total_time == data[process_next_idx].at - total_time &&
                     data[i].burst < data[process_next_idx].burst)) {
                    process_next_idx = i;
                }
            }
        }
    } else { // find process with minimum burst
        for (int i = 0; i < data_size; i++) {
            if (data[i].burst > 0 && data[i].at <= total_time && data[i].burst < data[process_next_idx].burst) {
                process_next_idx = i;
            }
        }
    }


    // If the next process hasn't arrived yet, increment the total_time and recursively call the function
    if (data[process_next_idx].at > total_time) {
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, process_next_idx);
        total_time++;
        return find_next_process();

    }

    // Return the data of the next process
    return data[process_next_idx];
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

    check_burst();

    if (running_process == -1) {
        ProcessData next_process = find_next_process();

        running_process = next_process.idx;
        create_process(running_process);
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
    }

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
