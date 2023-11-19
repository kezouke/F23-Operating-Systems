#!/bin/bash

# Function to get shared libraries of a binary
get_libs() {
    ldd "$1" | awk 'BEGIN{ORS=" "}$1~/^\/lib/{print $1}$2~/=>/ && $3~/^\/lib/{print $3}'
}

# Compile ex1.c to create the ex1 executable
gcc ex1.c -o ex1

# Create a file of at least 50 MiB
dd if=/dev/zero of=lofs.img bs=1M count=50

# Setup a loop device on the created file and get the loop device number
LOOP_DEVICE=$(sudo losetup --find --show lofs.img)
LOOP_NUMBER=${LOOP_DEVICE#/dev/loop}

# Create a Loop File System (LOFS) ext4 on the created file
sudo mkfs.ext4 "$LOOP_DEVICE"

# Create a new empty directory ./lofsdisk
mkdir -p ./lofsdisk

# Mount the created filesystem on the mount point ./lofsdisk
sudo mount lofs.img ./lofsdisk

# Add two files file1 and file2 to the LOFS
echo "Elisei" | sudo tee ./lofsdisk/file1 > /dev/null
echo "Smirnov" | sudo tee ./lofsdisk/file2 > /dev/null

# Copy the necessary binaries and their shared libraries to the LOFS
for a in bash cat echo ls; do
    cmd=/bin/$a
    echo "Processing command: $cmd"
    # Get shared libraries for the command
    res=$(get_libs "$cmd")
    for i in $res; do
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

# Change the root directory of the process to the mount point of the LOFS
echo "ex1.c output with changing the root directory:" > ex1.txt
sudo chroot ./lofsdisk ./ex1 >> ex1.txt

# Unmount the filesystem
sudo umount ./lofsdisk

# Clean up the loop device
sudo wipefs -a "/dev/loop$LOOP_NUMBER"

# Remove the directory
sudo rm -r ./lofsdisk

# Run ex1 again without changing the root directory
echo "ex1.c output without changing the root directory:" >> ex1.txt
./ex1 >> ex1.txt
