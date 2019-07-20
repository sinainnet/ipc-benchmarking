
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: vmsplice/vmsplice.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>

#include "vmsplice.h"


/*
 *  Lorem Ipsum
 */
int 
ipcb_vmsplicer(int fd, char** data, int rowSize, int colSize, int flag) {
	int pageCounter = rowSize - 1;
	int written, idx = 0, nread = 0;
        struct iovec iov[] = {
                {
                        .iov_base = data[pageCounter],
                        .iov_len = colSize,
                },
        };
    
    	while (pageCounter >= 0) {
                written = vmsplice(fd, &iov[idx], 1, flag);
        
                if (written <= 0)
                        return ipcb_print_error("ipcb_vmsplicer:vmsplice");

                if ( (size_t)written >= iov[idx].iov_len ) {
                        int extra = written - iov[idx].iov_len;

			nread+=written;
                        pageCounter--;
                        iov[idx].iov_len = colSize;
                        iov[idx].iov_base = data[pageCounter];
                } 
		else {
            		nread+=written;
                        iov[idx].iov_len -= written;
                        iov[idx].iov_base += written;
                }
        }

    	if(nread < 0)
        	return -1;
    	return nread;

}


/*
 *  Lorem Ipsum
 */
int
ipcb_splicer(int fdIn, int fdOut, int rowSize, int colSize, int flag) {
	struct pollfd pFdOut = { .fd = fdOut, .events = POLLOUT, };
    	int len = rowSize * colSize;
    	int written = 0, nread = 0;

        while (len > 0) {
                if (poll(&pFdOut, 1, -1) < 0)
                        return ipcb_print_error("ipcb_splicer:poll");
                written = splice(fdIn, NULL, fdOut, NULL, len, flag);
        	if (written < 0)
            		return ipcb_print_error("ipcb_splicer:splice");
        	len -= written;
		
        }
    	return len;
}



