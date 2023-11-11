#!/bin/bash

# Example 1: Check file types, count directories, and compile a C program


# Check file types in /dev and /etc
file /dev/*
file /etc/*
# Explanation: This checks the file types in the specified directories.
# The output may include information about directories, symbolic links, ASCII texts,
# magic text files for the file(1) command, regular files, and more.


# Count the number of directories in /etc
dir_count=$(ls -l /etc | grep ^d | wc -l)
echo "Number of directories in /etc: $dir_count"
# Number of directories in /etc: 128



# Check file type before compilation
file ex1.c
# ex1.c: C source, ASCII text

gcc -o ex1 ex1.c

# Check file type after compilation
file ex1
# ex1: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=6561d5ddb7a60e5a85eedbcfa9fe92d48ebbe3e4, for GNU/Linux 3.2.0, not stripped


# Modify the program to print non-English words
# C file type now is:
# ex1.c: C source, Unicode text, UTF-8 text
# Explanation: The source file now contains Unicode text, specifically UTF-8 encoded.
# This change is due to the introduction of non-English words (ASCII before doesn't have non-English letters, so encoding is changed).

# Compiled C file type now (didn't change):
# ex1: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=385ed8368c0e658455ad51a4eccca71fd9dcf22b, for GNU/Linux 3.2.0, not stripped
# Explanation: The compilation process does not change the file type of the compiled executable.
# The output still indicates it is an ELF 64-bit executable. Since after compilation thi

# The file command examines the content of the file to determine its type based on certain patterns and characteristics.
# The modification to the source code introduces Unicode and UTF-8 characters, leading to the change in recognized file type for the source file.
# However, the compilation process focuses on translating the C source code into machine code and organizing it into the ELF format, but it doesn't alter the characteristics recognized by the file command significantly.
# Therefore, even though the source file's recognized type changes, the compiled binary maintains its ELF format because the compilation primarily deals with translating code, not textual content.
