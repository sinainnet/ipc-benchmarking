/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: process_vm/process_vm.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include "../commons/commons.h"



/*
 *  Lorem Ipsum
 */
size_t ipcb_process_vm_readv (pid_t pid,
                                char* localData,
                                unsigned long lDataRow,
                                unsigned long lDataCol,
                                char** remoteData,
                                unsigned long rDataRow,
                                unsigned long rDataCol);


/*
 *  Lorem Ipsum
 */
size_t ipcb_process_vm_writev (pid_t pid,
                                char** localData,
                                unsigned long lDataRow,
                                unsigned long lDataCol,
                                char* remoteData,
                                unsigned long rDataRow,
                                unsigned long rDataCol);


