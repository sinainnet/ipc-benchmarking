/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: process_vm/process_vm.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#define _GNU_SOURCE
#include <sys/uio.h>
#include <sys/types.h>       
#include <unistd.h>

#include "process_vm.h"

/*
 *  Lorem Ipsum
 */
size_t
ipcb_process_vm_readv (pid_t pid,
                                char* localData,
                                unsigned long lDataRow,
				unsigned long lDataCol,
                                char** remoteData,
                                unsigned long rDataRow,
                                unsigned long rDataCol) {

	int written, idx = 0, nread = 0;
	unsigned long lReverseRowCnt = lDataRow, lReverseColSize = lDataCol;
	unsigned long rReverseRowCnt = rDataRow, rReverseColSize = rDataCol;
	long int pageCounter = lReverseRowCnt - 1;

	if (lDataRow < rDataRow && lDataCol < rDataCol ||
			lDataRow * lDataCol < rDataRow * rDataCol)
		ipcb_print_error("process_vm_readv:size");

	
	struct iovec localIov[] = {
                {
                        .iov_base = localData,
                        // .iov_base = localData[pageCounter - 1],
                        .iov_len = lReverseColSize,
                },
        };
    	struct iovec remoteIov[] = {
                {
                        .iov_base = remoteData[pageCounter - 1],
                        .iov_len = rReverseColSize,
                },
        };
        
        while (0 <= pageCounter)
    	{
		written = process_vm_readv(pid, localIov, 1, remoteIov, 1, 0);
		if (written <= 0)
                	return ipcb_print_error("process_vm_readv:readv");

                if ((size_t) written >= rReverseColSize) {
		        nread+=written;
                        pageCounter - 1;
                        localIov[idx].iov_len = lReverseColSize;
                        localIov[idx].iov_base = localData;
                        // localIov[idx].iov_base = localData[pageCounter];
                        remoteIov[idx].iov_len = lReverseColSize;
	                remoteIov[idx].iov_base = remoteData[pageCounter];
                } 
		else {
            		nread+=written;
                        localIov[idx].iov_len -= written;
                        localIov[idx].iov_base += written;

			remoteIov[idx].iov_len -= written;
                        remoteIov[idx].iov_base += written;
                }
        }

	if(nread < 0)
        	return ON_ERROR;
        return nread;
}


/*
 *  Lorem Ipsum
 */
size_t
ipcb_process_vm_writev (pid_t pid,
                                char** localData,
                                unsigned long lDataRow,
                                unsigned long lDataCol,
                                char* remoteData,
                                unsigned long rDataRow,
                                unsigned long rDataCol) {
        // printf("me\n");
        long int written, idx = 0, nread = 0;
        unsigned long lReverseRowCnt = lDataRow, lReverseColSize = lDataCol;
        unsigned long rReverseRowCnt = rDataRow, rReverseColSize = rDataCol;
	long int pageCounter = lReverseRowCnt - 1;
        // printf("me2, pagec: %ld.\n", pageCounter);
        if (lDataRow > rDataRow && lDataCol > rDataCol ||
                        lDataRow * lDataCol > rDataRow * rDataCol)
                ipcb_print_error("process_vm_writev:size");


        struct iovec localIov[] = {
                {
                        .iov_base = localData[pageCounter-1],
                        .iov_len = lReverseColSize,
                },
        };
        struct iovec remoteIov[] = {
                {
                        .iov_base = remoteData,
                        .iov_len = lReverseColSize,
                },
        };
	
	while (0 <= pageCounter)
        {
        	// printf("%ld",pageCounter);
		written = process_vm_writev(pid, localIov, 1, remoteIov, 1, 0);
                if (written <= 0)
                        return ipcb_print_error("process_vm_writev:writev");

                if ((size_t) written >= lReverseColSize) {
                        nread+=written;
                        pageCounter--;
                        localIov[idx].iov_len = lReverseColSize;
                        localIov[idx].iov_base = localData[pageCounter];
                        remoteIov[idx].iov_len = lReverseColSize;
                        remoteIov[idx].iov_base += written;
                }
                else {
                        nread+=written;
                        localIov[idx].iov_len -= written;
                        localIov[idx].iov_base += written;

                        remoteIov[idx].iov_len -= written;
                        remoteIov[idx].iov_base += written;
                }
        }

        if(nread < 0)
                return ON_ERROR;
        return nread;
}
