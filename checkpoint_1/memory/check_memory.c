#include <ykernel.h>
#include <hardware.h>
#include "../kernel_start.h"

/*
 * Checks to see if this memory is valid.
 * Assumes: the program will touch all bytes from mem_loc to mem_loc+mem_size
 *
 * Returns SUCCESS if the entire area of memory is touchable by the user (i.e., all in user space)
 * and has valid page entries through the entire section of memory.
 * Returns ERROR if this is not the case.
 */
int check_memory(void* mem_loc, unsigned int mem_size, bool read_required, bool write_required, bool exec_required) {
  int start_memory_loc_in_region_1 = ((int)mem_loc - VMEM_1_BASE);
  int end_memory_loc_in_region_1 = ((int)mem_loc + mem_size - VMEM_1_BASE);

  // are we in region 0?
  if (start_memory_loc_in_region_1 < 0 || end_memory_loc_in_region_1 < 0) {
    return ERROR;
  }

  int start_page_idx = start_memory_loc_in_region_1 >> PAGESHIFT;
  int end_page_idx = end_memory_loc_in_region_1 >> PAGESHIFT;

  // check all the page table entries between start and end page to see if they're valid
  for (int i = start_page_idx; i <= end_page_idx; i++) {
    if (!running_process->region_1_page_table[i].valid) {
      return ERROR;
    }

    bool is_read_enabled = false;
    bool is_write_enabled = false;
    bool is_exec_enabled = false;
    if (running_process->region_1_page_table[i].prot == (PROT_READ)) {
      is_read_enabled = true;
    } else if (running_process->region_1_page_table[i].prot == (PROT_WRITE)) {
      is_write_enabled = true;
    } else if (running_process->region_1_page_table[i].prot == (PROT_EXEC)) {
      is_exec_enabled = true;
    } else if (running_process->region_1_page_table[i].prot == (PROT_READ | PROT_WRITE)) {
      is_read_enabled = true;
      is_write_enabled = true;
    } else if (running_process->region_1_page_table[i].prot == (PROT_READ | PROT_EXEC)) {
      is_read_enabled = true;
      is_exec_enabled = true;
    } else if (running_process->region_1_page_table[i].prot == (PROT_WRITE | PROT_EXEC)) {
      is_write_enabled = true;
      is_exec_enabled = true;
    } else if (running_process->region_1_page_table[i].prot == (PROT_READ | PROT_WRITE | PROT_EXEC)) {
      is_read_enabled = true;
      is_write_enabled = true;
      is_exec_enabled = true;
    }

    if ((read_required && !is_read_enabled) || (write_required && !is_write_enabled) || (exec_required && !is_exec_enabled)) {
      return ERROR;
    }
  }

  return SUCCESS;
}