#include "contiki.h"
#include <stdio.h>
#include "nrf_pwr_mgmt.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/*---------------------------------------------------------------------------*/
PROCESS(low_power, "LOW power Process");
AUTOSTART_PROCESSES(&low_power);
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

static void idle_state_handle(void);

PROCESS_THREAD(low_power, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Wait 2 seconds for the SR04 sensor to be ready */
  etimer_set(&timer, CLOCK_SECOND * 5);

  /* Wait for the periodic timer to expire */
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));


  nrf_pwr_mgmt_init();
  /* Setup a periodic timer that expires after 1 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 10);
  while(1) {
    nrf_pwr_mgmt_run();
    /* Wait for the periodic timer to expire and then restart the timer. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);
  }
 
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
