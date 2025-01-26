#include "contiki.h"
#include <stdio.h>
#define NRF_PWR_MGMT 1
#include "nrf_pwr_mgmt.h"

/*---------------------------------------------------------------------------*/
PROCESS(low_power, "LOW power Process");
AUTOSTART_PROCESSES(&low_power);
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(low_power, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Wait 2 seconds for the SR04 sensor to be ready */
  etimer_set(&timer, CLOCK_SECOND * 2);

  /* Wait for the periodic timer to expire */
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

  nrf_pwr_mgmt_init();

  /* Setup a periodic timer that expires after 1 seconds. */
  // etimer_set(&timer, CLOCK_SECOND * 1);
  while(1) {
    /*
     * Request a fresh read
     */
      nrf_pwr_mgmt_run();

    /* Wait for the periodic timer to expire and then restart the timer. */
    // PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    // etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
