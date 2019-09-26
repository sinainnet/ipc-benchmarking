#!/usr/bin/env bash

test -e /tmp/ipcb_shm_file && rm -f /tmp/ipcb_shm_file
test -e /tmp/fifo_twoway-shm && rm -f /tmp/fifo_twoway-shm
test -e /tmp/fifo_starttime-shm && rm -f /tmp/fifo_starttime-shm
test -e /dev/shm/sem.ipcb_semaphore_example && rm -f /dev/shm/sem.ipcb_semaphore_example

echo 0 > /tmp/ipcb_shm_file
echo 0 > /tmp/fifo_twoway-shm

# Run the main test app
$PWD/shm_master

if [ -e "/tmp/ipcb_shm_file" ]; then
	rm /tmp/ipcb_shm_file
fi
if [ -e "/tmp/fifo_twoway-shm" ]; then
	rm /tmp/fifo_twoway-shm
fi
if [ -e "/tmp/fifo_starttime-shm" ]; then
	rm /tmp/fifo_starttime-shm
fi
if [ -e "/dev/shm/sem.ipcb_semaphore_example" ]; then
	sudo rm /dev/shm/sem.ipcb_semaphore_example
fi

# sudo kill $(pidof ipcb_xpmem_master)