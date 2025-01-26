#include "contiki.h"

#include <stdio.h>

#include "usb-dfu-trigger.h"
/*---------------------------------------------------------------------------*/
PROCESS(dfu_process, "DFU Process");
AUTOSTART_PROCESSES(&dfu_process);
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(dfu_process, ev, data)
{
  // static struct etimer timer;

  PROCESS_BEGIN();
  /* Wait 2 seconds for the MQ sensor to be ready */
  // etimer_set(&timer, CLOCK_SECOND * 20);
  
  /* Wait for the periodic timer to expire */
  // PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

  /* Setup a periodic timer that expires after 1 seconds. */
  // etimer_set(&timer, CLOCK_SECOND * 1);
  // while(1) {
  //   /*
  //    * Request a fresh read
  //    */
  //   printf("DFU Process\n");
  //   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  //   etimer_reset(&timer);
  // }

  dfu_trigger_usb_init();

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
