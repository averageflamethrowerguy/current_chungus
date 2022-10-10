#include "ipc_syscalls.h"
#include "stdbool.h"
#include "../data_structures/pcb.h"

typedef struct pipe {
  int pipe_id;
  char buf[/* TODO -- buf length */];
  int start_id=0;
  int end_id=0;
  pcb_t* blocked_head;                 // the head of the linked list of blocked processes
  pcb_t* blocked_tail;                 // the tail of the linked list of blocked processes
} pipe_t;

/****************** UTILITY FUNCTIONS ***********************/

bool is_full(int pipe_id)
{
  // check to see if the pipe is full -- i.e., the end index + 1 == the start index
}

bool is_empty(int pipe_id)
{
  // check to see if the pipe is empty -- i.e., the start and end indices of the buffer are the same
}

char read_byte(int pipe_id)
{
  // returns ERROR if there is no byte to read (is_empty)
  // reads a byte from the start_index of the buffer
  // increments the start_index (rolls around if we exceed buf_size)
  // returns the byte
}

int write_byte(int pipe_id)
{
  // returns ERROR if there is no more space in the buffer (is_full)
  // writes a byte one past the end_index of the buffer (rolls around if necessary)
  // increments the end_index (rolls around if necessary)
  // returns 0
}

int block_pcb_on_pipe(int pipe_id, pcb_t* process_block)
{
  // if head is NULL, place process_block there, set up tail
  // otherwise:
  //  get variable old_tail
  //  point tail at the pcb
  //  point old_tail and pcb at each other
  // return SUCCESS or ERROR
}

/********** unblock_pcb_on_pipe *************/
/*
 * Gets the pcb_t* at head of pipe, places it in ready queue, returns a pointer to it
 */
pcb_t* unblock_pcb_on_pipe(int pipe_id)
{
  // gets head of pipe
  // sets head of pipe to head->next_pcb
  // sets the prev_pcb of the new head to NULL
  // sets the next_pcb of the old head to NULL
  // sticks the old head in the ready queue
  // returns the old head
}

/***************** END UTILITY FUNCTIONS *********************/


/*
 * Create a new pipe; save its identifier at *pipe idp. (See the header files for the length of the pipe’s internal
buffer.) In case of any error, the value ERROR is returned.
 */
int PipeInit(int *pipe_idp)
{
  // create a buffer for the pipe
  // store the pipe buffer *somewhere*
}

/*
 * Read len consecutive bytes from the named pipe into the buffer starting at address buf, following the standard
semantics:
– If the pipe is empty, then block the caller.
– If the pipe has plen ≤ len unread bytes, give all of them to the caller and return.
– If the pipe has plen > len unread bytes, give the first len bytes to caller and return. Retain the unread
plen − len bytes in the pipe.
In case of any error, the value ERROR is returned. Otherwise, the return value is the number of bytes read.
 */
int PipeRead(int pipe_id, void *buf, int len)
{
  // if the pipe is empty, block the caller:
  //   put the caller in the blocked queue of the pipe
  //   swap a new process into the ready slot for execution
  // read bytes from the pipe into *buf
}

/*
 * Write the len bytes starting at buf to the named pipe. (As the pipe is a FIFO buffer, these bytes should be
appended to the sequence of unread bytes currently in the pipe.) Return as soon as you get the bytes into the
buffer. In case of any error, the value ERROR is returned. Otherwise, return the number of bytes written.
 */
int PipeWrite(int pipe_id, void *buf, int len)
{
  // checks to see if the buffer has enough free space
  // write all the bytes into the buffer, checking return code
  // put any processes waiting on the pipe back into the ready queue
  // return ERROR or the number of bytes written
}