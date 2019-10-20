#!/usr/bin/env bash

test -e /tmp/ipcb_xpmem.share && rm -f /tmp/ipcb_xpmem.share
test -e /tmp/ipcb_mw_xpmem.lock && rm -f /tmp/ipcb_mw_xpmem.lock
test -e /tmp/ipcb_rw_xpmem.lock && rm -f /tmp/ipcb_rw_xpmem.lock
test -e /dev/shm/sem.ipcb_semaphore_example && rm -f /dev/shm/sem.ipcb_semaphore_example

# create TMP_SHARE_SIZE bytes defined in xpmem_test.h
for i in `seq 0 31` ; do
	echo -n 0 >> /tmp/ipcb_xpmem.share
done
echo 0 > /tmp/ipcb_mw_xpmem.lock
echo 0 > /tmp/ipcb_rw_xpmem.lock

# Run the main test app
$PWD/ipcb_xpmem_master

# Tests complete -- now check for memory leaks
#echo "==== test_mem_leak STARTS ===="
#PINNED=$(grep "pages pinned" /proc/xpmem/* | sed -e 's/^.*: //')
#UNPINNED=$(grep "pages unpinned" /proc/xpmem/* | sed -e 's/^.*: //')
#echo "all pinned pages = $PINNED"
#echo "all unpinned pages = $UNPINNED"
#if [ "$PINNED" -eq "$UNPINNED" ]; then
#	echo "==== test_mem_leak PASSED ===="
#else
#	echo "==== test_mem_leak FAILED ===="
#fi

if [ -e "/tmp/ipcb_xpmem.share" ]; then
	rm /tmp/ipcb_xpmem.share
fi
if [ -e "/tmp/ipcb_mw_xpmem.lock" ]; then
	rm /tmp/ipcb_mw_xpmem.lock
fi
if [ -e "/tmp/ipcb_rw_xpmem.lock" ]; then
	rm /tmp/ipcb_rw_xpmem.lock
fi
if [ -e "/dev/shm/sem.ipcb_semaphore_example" ]; then
	sudo rm /dev/shm/sem.ipcb_semaphore_example
fi

# sudo kill $(pidof ipcb_xpmem_master)