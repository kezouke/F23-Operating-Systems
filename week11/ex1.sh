#!/bin/bash

# Function to get shared libraries of a binary
get_libs() {
    # Run 'ldd' on the specified binary and use 'awk' to extract only shared libraries
    ldd "$1" | awk 'BEGIN{ORS=" "}$1~/^\/lib/{print $1}$2~/=>/ && $3~/^\/lib/{print $3}'
}

# Compile ex1.c to create the ex1 executable
gcc ex1.c -o ex1

# Create a file of at least 50 MiB
dd if=/dev/zero of=lofs.img bs=1M count=50

# Setup a loop device on the created file
LOOP_DEVICE=$(sudo losetup --find --show lofs.img)
# Get the loop device number (needed to associate it with LOFS)
LOOP_NUMBER=${LOOP_DEVICE#/dev/loop}

# Create a Loop File System (LOFS) ext4 on the created file
sudo mkfs.ext4 "$LOOP_DEVICE"

# Create a new empty directory ./lofsdisk
mkdir -p ./lofsdisk

# Mount the created filesystem on the mount point ./lofsdisk
sudo mount lofs.img ./lofsdisk

# Add file1 with my name to the LOFS
echo "Elisei" | sudo tee ./lofsdisk/file1 > /dev/null
# Add file2 with my surname to the LOFS
echo "Smirnov" | sudo tee ./lofsdisk/file2 > /dev/null

# Copy the necessary binaries and their shared libraries to the LOFS
for a in bash cat echo ls; do
    # save absolute path for command we are iterating now
    cmd=/bin/$a
    # output that we are dealing with command now
    echo "Processing command: $cmd"
    # Get shared libraries for the command
    res=$(get_libs "$cmd")
    for i in $res; do
        # output that we are dealing with shared library now
        echo "Processing library: $i"
        # Create the directory structure in the LOFS for the library
        sudo mkdir -p "./lofsdisk$(dirname $i)"
        # Copy the library to the LOFS
        sudo cp "$i" "./lofsdisk$i"
    done
    # Create the directory structure in the LOFS for the command
    sudo mkdir -p "./lofsdisk$(dirname $cmd)"
    # Copy the command to the LOFS
    sudo cp "$cmd" "./lofsdisk$cmd"
done

# Copy the ex1 executable to the LOFS
sudo cp ex1 ./lofsdisk/ex1

# add "line-explanation" for human to ex1.txt
echo "ex1.c output with changing the root directory:" > ex1.txt
# Change the root directory of the process to the mount point of the LOFS
sudo chroot ./lofsdisk ./ex1 >> ex1.txt

# Unmount the filesystem
sudo umount ./lofsdisk

# Clean up the loop device
sudo wipefs -a "/dev/loop$LOOP_NUMBER"

# Remove the directory
sudo rm -r ./lofsdisk

# add "line-explanation" for human to ex1.txt
echo "ex1.c output without changing the root directory:" >> ex1.txt
# Run ex1 again without changing the root directory
./ex1 >> ex1.txt

<<multilinecomment
_______________________________
Additional Explanations (the same as in ex1.txt):

1) With Changing the Root Directory:
The output reflects the contents of the chrooted environment starting from the root (/) within the LOFS.
It includes directories like bin, lib, lib64, file1, file2, etc., which are part of the new root directory.

2) Without Changing the Root Directory:
The output reflects the contents of the actual root directory of the system.
It includes typical system directories like bin, lib, lib64, home, var, etc, etc., which are not part of the chrooted environment.

3) Key Findings:
  a) Difference Explanation: 
    This difference is because the chroot command changes the perceived root directory for the current running process and its children. A program that is run in such a modified environment cannot access files and commands outside that environmental directory tree. This modified environment is called a chroot jail. Changing the root directory is commonly done for system maintenance, such as recovering from a system crash, and for computer security, such as running internet-facing services in a chroot jail.

  b) Chrooted Environment Output: The first case shows only the contents of the chrooted environment, isolating the process to a specific directory (./lofsdisk). This allows the execution of a program (ex1) with a modified root directory.

  c) System Root Output: The second case reflects the actual system root directory, showing a broader view of the system's file structure.

In summary, the primary difference lies in the scope of the file system hierarchy that the program (ex1) can access and interact with. The chrooted environment constrains the view to a specific directory, while running without changing the root directory gives access to the entire system's root.
multilinecomment
