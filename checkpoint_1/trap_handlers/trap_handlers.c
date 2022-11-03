#include <ykernel.h>
#include "../kernel_start.h"
#include "../kernel_utils.h"
#include "trap_handlers.h"
#include "../syscalls/io_syscalls.h"
#include "../syscalls/ipc_syscalls.h"
#include "../syscalls/process_syscalls.h"
#include "../syscalls/sync_syscalls.h"
#include "../data_structures/queue.h"
#include "../debug_utils/debug.h"

/*
 * Handle traps to the kernel
 */
void handle_trap_kernel(UserContext* context) {
  int rc = 0;
  int trap_type = context->code;
  TracePrintf(1, "Handling kernel trap with code %x\n", trap_type);

  // switch based on the trap type
  switch (trap_type) {
    // process syscalls
    case YALNIX_FORK:
      rc = handle_Fork();
      break;
    case YALNIX_EXEC:
      rc = handle_Exec((char *)context->regs[0], (char **) context->regs[1]); //TODO cast args as temporary solution
      context->pc = running_process->uctxt->pc;
      context->sp = running_process->uctxt->sp;
      break;
    case YALNIX_EXIT:
      handle_Exit(context->regs[0]);
      break;
    case YALNIX_WAIT:
      handle_Wait((int *)context->regs[0]); //TODO cast args as temporary solution
      break;
    case YALNIX_GETPID:
      rc = handle_GetPid();
      break;
    case YALNIX_BRK:
      handle_Brk((void *)context->regs[0]); //TODO cast args as temporary solution
      break;
    case YALNIX_DELAY:
      handle_Delay(context->regs[0]);
      break;

    // TTY Syscalls
    case YALNIX_TTY_READ:
//      handle_TtyRead(context->regs[0], context->regs[1], context->regs[2]);
      break;
    case YALNIX_TTY_WRITE:
//      handle_TtyWrite(context->regs[0], context->regs[1], context->regs[2]);
      break;

    // TODO -- what are YALNIX_REGISTER etc?
    // TODO -- what are YALNIX_READ_SECTOR etc?

    // IPC
    case YALNIX_PIPE_INIT:
//      handle_PipeInit(context->regs[0], context->regs[1]);
      break;
    case YALNIX_PIPE_READ:
//      handle_PipeRead(context->regs[0]);
      break;
    case YALNIX_PIPE_WRITE:
//      handle_PipeWrite(context->regs[0], context->regs[1], context->regs[2]);
      break;

    // NOP
    case YALNIX_NOP:
      // do nothing!
      break;

    // TODO -- semaphore stuff?

    case YALNIX_LOCK_INIT:
//      handle_LockInit(context->regs[0]);
      break;
    case YALNIX_LOCK_ACQUIRE:
//      handle_Acquire(context->regs[0]);
      break;
    case YALNIX_LOCK_RELEASE:
//      handle_Release(context->regs[0]);
      break;
    case YALNIX_CVAR_INIT:
//      handle_CvarInit(context->regs[0]);
      break;
    case YALNIX_CVAR_SIGNAL:
//      handle_CvarSignal(context->regs[0]);
      break;
    case YALNIX_CVAR_BROADCAST:
//      handle_CvarBroadcast(context->regs[0]);
      break;
    case YALNIX_CVAR_WAIT:
//      handle_CvarWait(context->regs[0], context->regs[1]);
      break;
    // TODO -- YALNIX_RECLAIM

    // TODO -- YALNIX_ABORT
    // TODO -- YALNIX_BOOT
  }

  context->regs[0] = rc;
}

/*
 * Handle traps to clock -- starts the next process in the ready queue
 */
void handle_trap_clock(UserContext* context) {
  TracePrintf(1, "TRAP_CLOCK: Our kernel hit the clock trap\n");

  if (ready_queue == NULL) {
    TracePrintf(1, "TRAP_CLOCK/DELAY: NULL READY QUEUE\n");
    return;
  }

  // handle Delay:
  if (delayed_processes != NULL) {
    if (delayed_processes->prev_pcb == NULL) {
      TracePrintf(3, "TRAP_CLOCK/DELAY: NULL prev_pcb\n");
    }

    if (delayed_processes->next_pcb == NULL) {
      TracePrintf(3, "TRAP_CLOCK/DELAY: NULL next_pcb\n");
    } else {
      TracePrintf(3, "TRAP_CLOCK/DELAY: Going from %d to %d\n", delayed_processes, delayed_processes->next_pcb);
    }

    // go through all processes in the delay data structure
    pcb_t* next_process = delayed_processes;
    TracePrintf(1, "TRAP_CLOCK/DELAY: Remaining Ticks: %d\n\n", next_process->delayed_clock_cycles);
    while (next_process != NULL) {
      // decrement their delays
      next_process->delayed_clock_cycles--;
      TracePrintf(3, "Delayed process with id %d now has %d clock cycles remaining\n",
                  next_process->pid, next_process->delayed_clock_cycles);

      // if any process gets a delay of 0 or less, put it back into the ready queue
      if (next_process->delayed_clock_cycles <= 0) {
        TracePrintf(3, "Delayed process with id %d will be put in the ready queue\n", next_process->pid);
        add_to_queue(ready_queue, next_process);

        // remove it from the delay data structure
        if (next_process->prev_pcb == NULL) {
//          TracePrintf(1, "PrevPCB is NULL\n");
          // next_pcb may be NULL, this is OK
          if (next_process->next_pcb != NULL) {
            delayed_processes = next_process->next_pcb;
          }
          else {
            delayed_processes = NULL;
            break;
          }
        }
        else {
//          TracePrintf(1, "PrevPCB is NOT NULL\n");
          // muck with pointers to remove our process from the queue
          next_process->prev_pcb->next_pcb = next_process->next_pcb;
          if (next_process->next_pcb != NULL) {
            next_process->next_pcb->prev_pcb = next_process->prev_pcb;
          }
        }
      }

//      TracePrintf(1, "Going from %d to %d\n", next_process, next_process->next_pcb);
      next_process = next_process->next_pcb;
    }
  }

  TracePrintf(3, "TRAP_CLOCK/DELAY: LEFT THE WHILE LOOP, %d\n", delayed_processes);
  pcb_t* old_process = running_process;

  // get the next process from the queue
  install_next_from_queue(old_process, 0);
}

/*
 * Handles all other traps
 */
void handle_trap_unhandled(UserContext* context) {
  TracePrintf(1, "This trap is not yet implemented\n");
  // TODO -- log something (maybe trap id?)
  // TODO -- return to user execution
}

/***************** FUTURE HANDLERS *********************/
/*
 * These will be implemented after Checkpoint 1... but we're still writing pseudocode for it!
 */


/*
 * Abort the current user process
 */
void handle_trap_illegal(UserContext* context) {
  TracePrintf(1, "This trap is not yet implemented\n");
  // abort the current process
  // get the next process from the ready queue
}

/*
 * Enlarges the user memory if it's an implicit request for more memory
 * otherwise kills the process
 */
void handle_trap_memory(UserContext* context) {
  TracePrintf(1, "This trap is not yet implemented\n");
  // implicit request for more memory -- stack, not the heap
  // check if the address being touched is one page or less away from the top of the stack
  // if so:
  //    set the brk to be a page lower
  //    return
  // otherwise:
  //  abort the process
  //  run the next process on the ready queue
  Halt();
}

/*
 * Aborts current user process
 */
void handle_trap_math(UserContext* context) {
  TracePrintf(1, "This trap is not yet implemented\n");
  // abort the user process
  // run the next process on the ready queue
}

/*
 * Hardware detected a new line in the terminal
 */
void handle_trap_tty_receive(UserContext* context) {
  TracePrintf(1, "This trap is not yet implemented\n");
  // int tty_id = context->code;
  // read input from terminal with TtyReceive
  // save into a terminal buffer
  // wake up waiting read processes
}

/*
 * A line being written to the terminal has completed
 */
void handle_trap_tty_transmit(UserContext* context) {
  TracePrintf(1, "This trap is not yet implemented\n");
  // int tty_id = context->code;
}

