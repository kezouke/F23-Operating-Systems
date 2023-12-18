#!/bin/bash

# Example 2: Checking inode, blocks, links, and file information

# Check the inode, number of blocks, block size, total size, and permissions of ex1
# Using stat command with specific format options
stat -c "Inode: %i, Number of Blocks: %b, Block Size: %B, Total Size: %s bytes, Permissions: %A" ex1
# Inode: 536138, Number of Blocks: 32, Block Size: 512, Total Size: 15960 bytes, Permissions: -rwxrwxr-x

# Copy ex1 to ex2
cp ex1 ex2

# Check the number of links for ex2
# Using stat command to get the link count
links_ex2=$(stat -c "%h" ex2)
echo "Number of links for ex2: $links_ex2"
# Number of links for ex2: 1

# Check if ex1 and ex2 have the same inode numbers
# Using stat command to get the inode numbers
stat -c "%i" ex1
# 536138
stat -c "%i" ex2
# 536812

# Explanation:
# They have different inode numbers since we just copied one file to another one without making any type
# of link. This are 2 completely different files.

# Identify files in /etc with 3 links
# Using stat command with specific format options and grep
echo "Files in /etc with 3 links:"
stat -c "%h - %n" /etc/* | grep "^3"
<<output
Files in /etc with 3 links:
3 - /etc/acpi
3 - /etc/alsa
3 - /etc/apparmor
3 - /etc/avahi
3 - /etc/ca-certificates
3 - /etc/default
3 - /etc/emacs
3 - /etc/firefox
3 - /etc/fwupd
3 - /etc/gdb
3 - /etc/glvnd
3 - /etc/gss
3 - /etc/ifplugd
3 - /etc/libblockdev
3 - /etc/libreoffice
3 - /etc/perl
3 - /etc/pm
3 - /etc/sane.d
3 - /etc/sgml
3 - /etc/ssh
3 - /etc/ufw
3 - /etc/update-manager
output

# Explanation of the number of links (3 links)
# The number '3' represents the number of hard links to each file. A hard link is a directory entry that associates a name with an inode.
<<comments
When we create a hard link to an existing file, we essentially
create another directory entry that references the same inode. 
Both the original file and the new hard link share the same set
of data blocks on the disk. From the file system's perspective, 
there is no distinction between the original file and its hard 
links—they are simply different names for the same data.

Now, the number "3" as the link count for a file means that 
there are three directory entries (filenames) pointing to the
same inode. Each of these entries is a hard link to the file. 
The link count includes the original filename and any additional
hard links created.

For example, if you have a file in the /etc directory with a link
count of 3, it implies that there are three different filenames 
within /etc that point to the same file data. These filenames 
could be hard links created manually or by system processes.
comments
