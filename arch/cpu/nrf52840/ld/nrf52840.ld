/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = __flash, LENGTH = __flash_size
  RAM (rwx) :  ORIGIN = __ram, LENGTH = __ram_size
}

INCLUDE "nrf_common.ld"

/* Define the program sections */
SECTIONS
{
  /* Other sections defined in nrf_common.ld are included here */

  /* Add the .pwr_mgmt_data section */
  .pwr_mgmt_data ALIGN(4) :
  {
    __start_pwr_mgmt_data = .; /* Mark the start of the section */
    KEEP(*(SORT(.pwr_mgmt_data*))); /* Include all .pwr_mgmt_data sections */
    __stop_pwr_mgmt_data = .;  /* Mark the end of the section */
  } > FLASH /* Place this section in FLASH */
}

/* These symbols are used by the stack check library. */
_stack = end;
_stack_origin = ORIGIN(RAM) + LENGTH(RAM);
_heap = _stack;
_eheap = _stack_origin;

