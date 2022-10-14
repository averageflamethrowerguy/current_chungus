//
// Created by smooth_operator on 10/8/22.
//

#ifndef CURRENT_CHUNGUS_PCB
#define CURRENT_CHUNGUS_PCB

#include "stdbool.h"
// TODO -- include user context
// TODO -- include kernel context

typedef struct pcb {
  int pid;                                             // the process id
  // uspace: some way to store pages (linked list?)
  // uctxt: user context
  KernelContext kctext;
  // kstack: some way to store pages (linked list?)

  bool hasExited;                                      // whether the process is dead yet
  int rc;                                              // the return code of the process
  pcb_t* next_pcb;                                     // the next pcb in the queue
  pcb_t* prev_pcb;                                     // the previous pcb in the queue

  pcb_t* children;                                     // null unless there are children
  int num_children;                                    // 0 unless there are children
  pcb_t* parent;                                       // the parent, if any
} pcb_t;


// TODO -- create an idle_pcb object

#endif //CURRENT_CHUNGUS_PCB