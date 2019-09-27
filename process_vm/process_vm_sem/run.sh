#!/usr/bin/env bash

test -e /tmp/ipcb_psvm_file_1 && rm -f /tmp/ipcb_psvm_file_1
test -e /tmp/ipcb_psvm_file_2 && rm -f /tmp/ipcb_psvm_file_2
test -e /tmp/fifo_starttime-shm && rm -f /tmp/fifo_starttime-shm
test -e /dev/shm/sem.ipcb_semaphore_example && rm -f /dev/shm/sem.ipcb_semaphore_example

# touch /tmp/ipcb_psvm_file_1
# touch /tmp/ipcb_psvm_file_2
echo 0 > /tmp/ipcb_psvm_file_1
echo 0 > /tmp/ipcb_psvm_file_2

# Run the main test app
$PWD/psvm_sem_master

if [ -e "/tmp/ipcb_psvm_file_1" ]; then
	rm /tmp/ipcb_psvm_file_1
fi
if [ -e "/tmp/ipcb_psvm_file_2" ]; then
	rm /tmp/ipcb_psvm_file_2
fi
if [ -e "/tmp/fifo_starttime-shm" ]; then
	rm /tmp/fifo_starttime-shm
fi
if [ -e "/dev/shm/sem.ipcb_semaphore_example" ]; then
	sudo rm /dev/shm/sem.ipcb_semaphore_example
fi

# sudo kill $(pidof ipcb_xpmem_master)