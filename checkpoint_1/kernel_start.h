//
// Created by smooth_operator on 10/8/22.
//

#ifndef CURRENT_CHUNGUS_KERNEL_START_H
#define CURRENT_CHUNGUS_KERNEL_START_H

#include <ykernel.h>
#include "trap_handlers/trap_handlers.h"
#include "data_structures/pcb.h"
#include "data_structures/queue.h"

// globals for the entire kernel
pcb_t* running_process;
queue_t* ready_queue;
char *frame_table;

/********** KernelStart ***********/
/*
 * The start function for CurrentChungus
 */
void KernelStart(char *cmd_args[],
                 unsigned int pmem_size,
                 UserContext *uctxt);

/*
* Set the kernel brk based on whether or not virtual memory is enabled. This will be a syscall used by malloc and other
* higher-level user/library dynamic allocation calls.
* In case of any error, the value ERROR is returned.
*/
int SetKernelBrk(void *addr);

// "user text" for our idle PCB
void DoIdle(void);

#endif //CURRENT_CHUNGUS_KERNEL_START_H
