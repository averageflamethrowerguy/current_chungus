//
// Created by smooth_operator on 10/19/22.
//

#ifndef CURRENT_CHUNGUS_TTY
#define CURRENT_CHUNGUS_TTY

#include <ykernel.h>
#include "queue.h"
#include "stdbool.h"
#define MAX_BUFFER_LEN 100
/*
* This struct holds the metadata about each terminal, allowing us to read and write to it. Currently, we only allow a single
* process to read from a terminal at a time, and a single process to write to a terminal at a time.
*/
typedef struct tty_object {
  int id;
  lock_t *lock;
  int num_unconsumed_chars;
  char buf[MAX_BUFFER_LEN];
  queue_t* blocked_reads; 
  queue_t* blocked_writes;
} tty_object_t;

tty_object_t *init_tty_object(int id);

tty_object_t *get_tty_object(int id);

/*
 * Gets the pcb_t* at head of tty queue, places it in ready queue, returns a pointer to it
 */
pcb_t* unblock_pcb_on_tty(int tty_id);


// // PIPE FUNCTIONS //

// bool is_full(int tty_id);

// bool is_empty(int tty_id);

// char read_byte(int tty_id);

// int write_byte(int tty_id);

// int block_pcb_on_pipe(int tty_id, pcb_t* process_block);


#endif //CURRENT_CHUNGUS_TTY