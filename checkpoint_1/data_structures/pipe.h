//
// Created by smooth_operator on 10/19/22.
//

#ifndef CURRENT_CHUNGUS_PIPE
#define CURRENT_CHUNGUS_PIPE

#include "queue.h"
#include "lock.h"
#include <yalnix.h>

typedef struct pipe {
  int pipe_id;
  char buf[PIPE_BUFFER_LEN];
  int start_id=0;
  int end_id=0;
  int max_size = PIPE_BUFFER_LEN;
  int cur_size = 0;
  struct pipe next_pipe = NULL;                   // the next pipe in the global pipe list
  lock_t* read_lock = NULL;                       // the lock for reading this pipe
  queue_t* blocked_read_queue = NULL;             // the queue of processes blocked on reads on this pipe
  lock_t* write_lock = NULL;                      // the lock for writing this pipe
  queue_t* blocked_write_queue = NULL;            // the queue of processes blocked on writes on this pipe
} pipe_t;

/*
 * Creates a new pipe with this id
 */
pipe_t* create_pipe(int pipe_id)

pipe_t* find_pipe(int pipe_id);

bool is_full(pipe_t* pipe);

bool pipe_is_empty(pipe_t* pipe);

char read_byte(pipe_t* pipe);

int write_byte(pipe_t* pipe, char byte);

int block_pcb_on_pipe_read(pipe_t* pipe, pcb_t* process_block)

int block_pcb_on_pipe_write(pipe_t* pipe, pcb_t* process_block)

/********** unblock_pcb_on_pipe *************/
/*
 * Gets the pcb_t* at head of pipe, places it in ready queue, returns a pointer to it
 */
pcb_t* unblock_pcb_on_pipe(pipe_t* pipe);

void delete_pipe(pipe_t* pipe);

#endif //CURRENT_CHUNGUS_PIPE