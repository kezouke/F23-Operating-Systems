# Repository Overview

Welcome! This repository is a culmination of diverse hands-on exercises and projects covering a spectrum of topics ranging from Unix-like systems to advanced C programming, interprocess communication, memory management, file systems, input/output handling, deadlock detection, and more. Each folder represents a unique category of exercises, offering a comprehensive exploration of these fundamental concepts from Operating Systems Course.

## Folder Structure and Task Descriptions

### [01.Basic_UNIX_commands](01.Basic_UNIX_commands)
In this section, fundamental Unix commands, shell scripting, and basic C programming are explored. The tasks include creating directories, executing shell commands, recording command history, writing shell scripts, and implementing a simple "Hello World" program in C. The [tasks_description.pdf](01.Basic_UNIX_commands/tasks_description.pdf) file provides detailed explanations for each exercise.

In these exercises, I successfully completed a series of tasks focusing on shell scripting and basic C programming. In Exercise 1, I created a directory named "week01" in my home directory, listed the last 5 entries in /usr/bin containing "gcc" in reverse alphabetical order, and saved the results in a file named "ex1.txt." For Exercise 2, I experimented with various commands, recorded the command history using the "history" command, and saved the output in "ex2.txt," along with a script named "ex2.sh." In Exercise 3, I crafted a shell script, "ex3.sh," that created two files, "root.txt" and "home.txt," within separate folders. The script also included pauses with date stamps and displayed the sorted content of the files and folders. Finally, in Exercise 4, I wrote a simple "Hello World" program in C, saved it as "main.c," compiled it into an executable named "ex4," and successfully ran the program. The files "main.c" and "ex4" were submitted as part of the task. These exercises provided hands-on experience with shell scripting, command execution, and basic C programming concepts.

### [02.Comprehensive_recap_of_C](02.Comprehensive_recap_of_C)
This series of exercises delves deep into C programming concepts. The tasks cover variable declarations, string manipulations, numeral system conversions, character counting, and sequence computations. The [tasks_description.pdf](02.Comprehensive_recap_of_C/tasks_description.pdf) file offers comprehensive guidance for each exercise.

In this series of exercises, I successfully tackled various programming tasks involving C language and shell scripting.

**Exercise 1:** I created a C program named "ex1.c" that declared variables of different types, assigned them maximum values using appropriate constants, and printed their sizes and values. The accompanying script, "ex1.sh," was crafted to execute the program.

**Exercise 2:** I implemented a C program, "ex2.c," which prompted the user to input a string character by character until a dot (.) was entered, and then printed the reverse of the string within double quotation marks. I also created a script, "ex2.sh," to run the program.

**Exercise 3:** This exercise involved writing a function, "convert," to convert a given number from one numeral system to another. The C program "ex3.c" utilized this function to read a long long number and the source/target numeral system specifiers from the user, printing the converted number or an error message. The script "ex3.sh" executed the program.

**Exercise 4:** In the first part, I developed a function "count" in C to return the number of occurrences of a specified character in a string. The second part involved a function "countAll" that printed the number of occurrences of each character in the input string. The C program "ex4.c" accepted an input string from the command line and printed the counts for each character. The corresponding script, "ex4.sh," executed the program.

**Exercise 5:** For the Tribonacci sequence, I crafted a function "tribonacci" in C that computed the value of Tn without using arrays or function recursion. The program "ex5.c" called this function with specific arguments and printed the output. An accompanying script, "ex5.sh," facilitated the compilation and execution of "ex5.c."

These exercises collectively provided a comprehensive exploration of C programming, including variable declarations, string manipulations, numeral system conversions, character counting, and sequence computation.

### [03.Comprehensive_recap_of_C_2.0](03.Comprehensive_recap_of_C_2.0)
Advanced C programming exercises covering pointers, structures, file manipulation, directory organization, array aggregation, and mathematical computations. The [tasks_description.pdf](03.Comprehensive_recap_of_C_2.0/tasks_description.pdf) file provides detailed task descriptions.

In this set of programming exercises, I successfully completed tasks involving pointers, structures, file and directory organization, and array aggregation.

**Exercise 1:** I created a C program, "ex1.c," which declared a constant pointer and three contiguous memory cells. I used the pointer to fill these cells with specific values and printed their memory addresses, checking for contiguity. Additionally, I implemented a function, "const tri," to calculate the Tribonacci number using only the provided cells.

**Exercise 2:** I defined a structure "Point" with two real number fields and implemented functions to compute the Euclidean distance between two points and the area of a triangle formed by three points. The main function defined points A, B, and C and calculated the distance between A and B, as well as the area of triangle ABC. The program was saved as "ex2.c," and a script to run it was saved as "ex2.sh."

**Exercise 3:** In multiple parts, I created structures for files and directories, implemented operations for file and directory manipulation, and wrote a program, "ex3.c," to simulate a simple file system. The root directory had subdirectories "home" and "bin," with files added to these directories. The program included functions to append content to files and print the paths of all files. The script to run the program was saved as "ex3.sh."

**Exercise 4:** I wrote a function, "aggregate," that performed aggregation operations on arrays of double and integer types. The program "ex4.c" tested this function on arrays of 5 doubles and 5 integers, printing the results for each array. The header of the "aggregate" function was fixed and included parameters for array base, size, number of items, initial value, and operation function.

These exercises collectively covered a wide range of C programming concepts, including pointers, structures, file manipulation, directory organization, array aggregation, and mathematical computations.

### [04.Processes_&_Threads](04.Processes_&_Threads)
Programming exercises related to process creation, vector calculations, process observation, and the development of a simplistic shell. [tasks_description.pdf](04.Processes_&_Threads/tasks_description.pdf) offers comprehensive guidance for each exercise.

In these programming exercises, I successfully completed tasks related to process creation, vector calculations, process observation, and the development of a simplistic shell.

**Exercise 1:** I created a C program, "ex1.c," which spawned two new processes without one being a child of the other. The program printed the process IDs and their parent IDs, along with the execution time for each process in milliseconds. I used the `fork` system call and the `clock` function from the `time.h` library. The script "ex1.sh" was provided to run the program.

**Exercise 2:** In two parts, I developed a program, "ex2.c," that created two vectors and calculated their dot product using an array of processes. Each child process contributed to the calculation, and the main process aggregated the results. A shared file, "temp.txt," facilitated communication among processes. The script "ex2.sh" was provided to run the program.

**Exercise 3:** I wrote a C program, "ex3.c," to experiment with the `fork` system call in a loop for a specified number of times. The program ran in the background, and the results were observed using the `pstree` command. The results were documented in "ex3.txt," and the script "ex3.sh" included commands to replicate the experiment for different runs.

**Exercise 4:** I developed a C program, "ex4.c," to create a simplistic shell that read user input and executed commands without arguments. The shell was extended to handle commands with arguments and run processes in the background without using the shell operator "&." The `execve` system call was utilized instead of the `system` library function. The script "ex4.sh" was provided to run the shell program.

These exercises collectively covered various aspects of process creation, inter-process communication, system calls, and command execution in a Unix-like environment.

### [05.Interprocess_Communication](05.Interprocess_Communication)
This series explores tasks related to interprocess communication via pipes, multi-threading, and parallelizing primality checking. Detailed explanations for each exercise can be found in the [tasks_description.pdf](05.Interprocess_Communication/tasks_description.pdf) file.

In this series of programming exercises, I successfully tackled tasks related to inter-process communication via pipes, multi-threading, and parallelizing primality checking.

**Exercise 1 (1/3):** In Part I, I created a C program, "channel.c," that implemented a simple publish/subscribe messaging system via pipes. The publisher and subscriber communicated through an unnamed pipe. In Part II, I developed a program, "publisher.c," which forked n child processes, each opening a named pipe to publish messages to subscribers.

**Exercise 1 (2/3):** I extended the publisher program to accept a maximum number of subscribers (n) as a command-line argument. I also created a program, "subscriber.c," which opened the corresponding named pipe and printed received messages. The script "ex1.sh" ran one publisher and n subscribers, with output analysis and explanations added to "ex1.txt."

**Exercise 1 (3/3):** I assumed all subscribers had subscribed to the channel and created processes, not threads, for inter-process communication. The findings regarding pipe communication and the termination of processes were documented in "ex1.txt." The programs "publisher.c," "subscriber.c," and the script "ex1.sh" were submitted.

**Exercise 2:** I created a C program, "ex2.c," that utilized threads to print messages in a specified order. The threads were based on a structure containing an id, an integer, and a message. The program ensured the order of thread creation, message printing, and thread exit. The script "ex2.sh" was provided to run the program.

**Exercise 3:** In these exercises, I implemented a multi-threaded program, "ex3.c," to count prime numbers within a specified range, distributing the computation equally among threads. A script, "ex3.sh," compiled the program and measured its execution time for various thread counts, storing the results in "ex3_res.txt." Findings and explanations were added to "ex3_exp.txt."

**Exercise 4:** I developed a program, "ex4.c," to distribute primality checking computation among threads using global state and mutexes. A script, "ex4.sh," compiled the program and measured its execution time for different thread counts, storing results in "ex4_res.txt." Findings and explanations were added to "ex4_exp.txt," comparing them to the results from Exercise 3.

These exercises collectively covered a range of topics, including inter-process communication, multi-threading, synchronization, and parallel computation.

### [06.Processes_scheduling](06.Processes_scheduling)
Exercises covering process and signal handling, scheduling algorithms, and simulation. The [tasks_description.pdf](06.Processes_scheduling/tasks_description.pdf) file provides detailed guidance for each task.

In this series of programming exercises, I successfully tackled tasks related to process and signal handling, scheduling algorithms, and simulation. Here's a brief summary of each exercise:

**Exercise 1:** Created two programs, "agent.c" and "controller.c," along with a file "text.txt." The agent program created a PID file on startup, read and printed the contents of "text.txt," and responded to signals (SIGUSR1 and SIGUSR2). The controller checked for a running agent, interactively communicated with the agent using signals, and terminated gracefully. Programs "agent.c" and "controller.c" were submitted.

**Exercise 2:** Implemented a simulation of the First-Come-First-Served (FCFS) scheduling algorithm using a timer in "scheduler.c." Additionally, created a worker process, "worker.c," responsible for finding and counting triangular numbers. The scheduler scheduled processes based on arrival times and sent signals to control the worker process. Metrics such as response time, completion time, turnaround time, and waiting time were calculated. Results and answers to questions were documented in "ex2.txt."

**Exercise 3:** Modified "scheduler.c" to implement the Shortest Job First (SJF) scheduling algorithm in "scheduler_sjf.c." The non-preemptive version of SJF was considered, and the functions "find_next_process" and "schedule_handler" were adjusted. The output was compared with the results from Exercise 2, and explanations were added to "ex3.txt."

**Exercise 4:** Modified "scheduler.c" to implement the Round-Robin scheduling algorithm in "scheduler_rr.c." The quantum was specified by the user from stdin. Adjustments were made to the "find_next_process" and "schedule_handler" functions. The output was compared with the results from Exercises 2 and 3, and explanations were added to "ex4.txt." A script "ex4.sh" was provided to run the program.

These exercises collectively covered topics such as process communication, signal handling, scheduling algorithms, and simulation. Each exercise involved creating and modifying C programs to achieve specific objectives and analyzing the results.

### [07.Memory_Management](07.Memory_Management)
Tasks covering memory allocation simulation and file processing using memory mapping. The exploration includes memory allocation algorithms, file processing, and system calls. Refer to [tasks_description.pdf](07.Memory_Management/tasks_description.pdf) for detailed task descriptions.

In this set of programming exercises, I successfully completed tasks related to memory allocation simulation and file processing using memory mapping. Here's a brief summary of each exercise:

**Exercise 1:** Implemented a memory allocator simulation in C with three allocation algorithms: First Fit, Best Fit, and Worst Fit. Created a program "allocator.c" that reads queries from "queries.txt" to allocate and clear memory cells. The performance of each algorithm was measured in terms of throughput, considering the number of queries executed and total allocation time. The findings and performance results were documented in "ex1.txt." The program "allocator.c" was submitted.

**Exercise 2:** Developed a program "ex2.c" that used the mmap system call to generate a large text file "text.txt" by reading characters from "/dev/random." The content of "text.txt" was generated, considering printable characters and adding new lines after every 1024 characters. The file was then opened in chunks, and the number of capital letters was counted, printed to stdout, and replaced with lowercase letters. Finally, the mapped memory was unmapped. The program "ex2.c" was submitted.

These exercises covered various aspects of memory management, file processing, and system calls, showcasing the ability to work with memory allocation algorithms and memory mapping in a Unix environment. Each task involved implementing specific functionalities, handling file input/output, and optimizing performance metrics.

### [08.Memory_Management_2.0](08.Memory_Management_2.0)
Extensions of memory management exercises covering Tribonacci calculations, Euclidean distance computation, file system simulation, and array aggregation. [tasks_description.pdf](08.Memory_Management_2.0/tasks_description.pdf) provides detailed task descriptions.

**Exercise 1:**
- Developed a C program, "ex1.c," that writes its PID in `/tmp/ex1.pid` and generates a random password starting with "pass:". The password consists of 8 printable characters and is stored in memory using mmap or in the heap. The program then waits in an infinite loop.
- Created a script, "ex1_hack.sh," that reads the virtual memory pages of "ex1.c" while it's running, finds the generated password, prints the password and its memory address to stdout, and sends a SIGKILL signal to "ex1.c."

**Exercise 2:**
- Implemented a virtual memory simulation in C using mmap and signals for process communication.
- Created a program, "mmu.c," that accepts command line arguments, checks if a page is in RAM, simulates page faults, updates page tables, and prints information for each memory access. The MMU signals the pager process for loading pages and exits after processing all memory accesses.
- Developed another program, "pager.c," which manages free frames, disk array, and RAM array. The pager process waits for SIGUSR1 signals from the MMU, handles page loading, page replacement, and prints updated page tables. It terminates after no non-zero referenced fields are found.

**Exercise 3:**
- Implemented a C program, "ex3.c," that runs for 10 seconds. Every second, it allocates 10 MB of memory, fills it with zeros, prints memory usage using the getrusage() function, and sleeps for 1 second.
- Compiled and ran the program in the background and simultaneously ran 'vmstat 1' to observe memory changes, paying attention to the "si" and "so" fields.
- Added findings and observations to "ex3.txt" with comments.

These exercises cover topics such as process communication, memory management, virtual memory simulation, and monitoring memory usage. Each program addresses specific tasks and requirements outlined in the exercises.

### [09.Memory_Management_3.0](09.Memory_Management_3.0)
Advanced memory management tasks covering password generation, hacking simulation, virtual memory simulation, and memory usage monitoring. Refer to [tasks_description.pdf](09.Memory_Management_3.0/tasks_description.pdf) for detailed guidance.

**Exercise 1:**
- Extended the implementation of "ex2" from "8.Memory_Management_2.0" folder to include three page replacement algorithms: Random, NFU (Not Frequently Used), and Aging.
- Implemented functions `random`, `nfu`, and `aging` in the "pager.c" program to perform page replacement based on the specified algorithms.
- Modified "mmu.c" to include benchmarking functionality. The program calculates and prints the hit ratio after finishing all requests, considering that a hit occurs when the MMU gets a request to a valid page, and a miss occurs when it gets a request to an invalid page.
- Ran the program three times, once for each algorithm, using the same input. Compared the performance of the algorithms and added findings to "ex1.txt."
- Provided a sample input for the program.

**Exercise 2:**
- Extended the implementation of "ex2" from lab 08 to include a Translation Lookaside Buffer (TLB) functionality in the "mmu.c" program.
- Introduced a TLB represented as an array of struct `TLBentry` with fields `page` and `frame`.
- Stored the most recent mappings between pages and frames in the TLB. In case of a TLB miss, checked the page table.
- Calculated the TLB miss ratio and printed it to stdout.
- Experimented with different values for the number of frames and pages. Compared the results and added findings to "ex2.txt."

These exercises involve extending existing programs to implement additional functionalities, comparing different algorithms, and analyzing performance metrics such as hit ratio and TLB miss ratio. The findings are documented in separate text files for each exercise.

### [10.File_Systems](10.File_Systems)
Exploration of file system operations, directory monitoring, file type analysis, inode, and link examination, symbolic links, and file permissions. [tasks_description.pdf](10.File_Systems/tasks_description.pdf) offers detailed explanations for each exercise.

**Exercise 1: Directory Monitoring**
- Developed a directory monitoring program, `monitor.c`, that watches a specified directory and reports changes.
- Used the inotify API to track events such as file access, creation, deletion, modification, opening, and metadata changes.
- Implemented a companion program, `ex1.c`, responsible for making changes to the watched directory, including creating, modifying, and removing files and folders.
- Implemented functions to find and manage hard links in `ex1.c`.
- Created a test script, `ex1_test.sh`, to simulate various directory changes.
- Monitored changes such as file access, creation, deletion, and modification.
- Evaluated the effectiveness of the monitoring program based on specific scenarios.
- The programs `monitor.c` and `ex1.c`, along with the test script `ex1_test.sh`, were submitted.

**Exercise 2 A: File Type and Directory Counting**
- Checked file types in the paths `/dev` and `/etc`.
- Counted the number of directories in the folder `/etc`.
- Compiled a simple "Hello, World!" program (`ex1.c`) and observed changes in file types using the `file` command.
- Modified the program to print non-English words, compared the results using the `file` command.

**Exercise 2 B: Inode and Links**
- Checked the inode, number of blocks, block size, total size, and permissions of a program (`ex1.c`).
- Copied the program to create a new file (`ex2.c`) and checked the number of links.
- Identified files in `/etc` with three links.
- Explored the meaning of the link count.

**Exercise 2 C: File Comparison and Link Analysis**
- Created a script (`gen.sh`) to generate a text file (`ex1.txt`) and created links.
- Compared file content, checked inode numbers, disk usage, and link count.
- Moved a hard link to another folder (`/tmp`), traced links, and checked link counts.
- Removed all links from the file.

**Exercise 2 D: Symbolic Links and Folder Structure**
- Created and manipulated symbolic links, observed differences in folder structures.
- Examined behaviors of symbolic links when accessing sub-folders.
- Explored the effects of deleting folders and symbolic links.

**Exercise 2 E: File Permissions**
- Explored changing file permissions for an empty file (`ex5.txt`).
- Interpreted the meanings of permission values (660, 775, 777).

These exercises cover various aspects of file system operations, directory monitoring, symbolic links, and file permissions. The solutions involve practical implementations and observations of system behaviors.

### [11.Alternative_File_Systems](11.Alternative_File_Systems)
Introduction to File System isolation using chroot, involving setting up a Loop File System and isolating processes within it. [tasks_description.pdf](11.Alternative_File_Systems/tasks_description.pdf) provides comprehensive guidance.

**Exercise 1: File System Isolation with chroot**
In this exercise, the task was to explore the concept of creating a File System within an existing File System using the chroot command. This technique allows the isolation of processes, limiting their view to a specific directory structure. The motivation behind this is to enhance security by controlling what a process can access and modify.

This exercise provided hands-on experience in setting up a Loop File System, using chroot to isolate processes, and understanding the impact on the process's view of the file system. The emphasis on script documentation ensures reproducibility and clear understanding of each step involved.

### [12.Input_&_Output](12.Input_&_Output)
Capturing keyboard events in C programming, involving the creation of a program to capture keyboard events directly from the device and print them to stdout. [tasks_description.pdf](12.Input_&_Output/tasks_description.pdf) provides detailed explanations.

**Exercise 1: Capturing Keyboard Events in C**
In this exercise, the task was to write a C program, `ex1.c`, that captures keyboard events directly from the keyboard device and prints them to stdout. The program uses the `/dev/input/by-path/platform-i8042-serio-0-event-kbd` file (or a similar keyboard event character device file) to endlessly read keyboard events. The input event structure from `linux/input.h` was utilized for handling only PRESSED, REPEATED, and RELEASED events.

This exercise provided hands-on experience in working with keyboard events in a C program and handling specific shortcuts. The program's ability to capture and respond to various keyboard events enhances its functionality, making it a useful tool for understanding and interacting with input devices. The clear documentation ensures that the source code is easily understandable and maintainable. The submission includes `ex1.txt` and `ex1.c` for review.

### [13.Deadlocks](13.Deadlocks)
Tasks related to deadlock detection and avoidance using various algorithms. Programs cover resource allocation with deadlock detection and the application of Banker's Algorithm for deadlock avoidance. Detailed explanations for each exercise can be found in the [tasks_description.pdf](13.Deadlocks/tasks_description.pdf) file.

**Exercise 1: Resource Allocation with Deadlock Detection**
In this exercise, a C program, `ex1.c`, was developed to handle resource allocation and detect deadlocks using the algorithm presented in the provided slides. The program creates multiple threads and processes resource requests concurrently, ensuring that deadlock situations are appropriately handled.

**Exercise 2: Banker's Algorithm for Deadlock Avoidance**
For this exercise, a C program, `ex2.c`, was implemented to apply the Banker's Algorithm for deadlock avoidance. The program reads input from a file (`input.txt`) and processes the resource allocation and request matrices to determine if any deadlock is detected.

Both programs were designed to handle variable numbers of threads, resource types, and processes, making them adaptable for different scenarios. The scripts (`ex1.sh` and `ex2.sh`) were provided to run the respective programs. The submission includes `ex1.c`, `ex1.txt`, `ex1.sh`, `ex2.c`, and `ex2.sh`. Each program's source code is well-documented, and the output files provide insights into the program execution and deadlock detection results.

Feel free to explore each folder, review the source code, and delve into the detailed task descriptions to gain a deeper understanding of the concepts covered in this repository. 
