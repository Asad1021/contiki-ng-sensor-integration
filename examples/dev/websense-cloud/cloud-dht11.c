#include "contiki.h"
#include "rpl.h"
#include "httpd-simple.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dht11-sensor.h"
#include "dev/leds.h"
#include "sys/log.h"

/* Log configuration */
#define LOG_MODULE "Web Sense DB"
#define LOG_LEVEL LOG_LEVEL_INFO

/* GPIO configuration for DHT11 */
#define DHT11_GPIO_PORT (0)
#define DHT11_GPIO_PIN  (20)

/*---------------------------------------------------------------------------*/
static
PT_THREAD(generate_routes(struct httpd_state *s))
{
  char buff[50];

  PSOCK_BEGIN(&s->sout);

  /* Activate the DHT11 sensor */
  SENSORS_ACTIVATE(dht11_sensor);

  /* Read the temperature and humidity from DHT11 sensor */
  switch(dht11_sensor.status(0)) {
    case DHT11_STATUS_OKAY:
      sprintf(buff,"{\"temp\":%d.%d,\"hum\":%d.%d}",
              dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER),
              dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL),
              dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER),
              dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL));
      LOG_INFO("Sensor readings: %s\n", buff);
      break;
    case DHT11_STATUS_CHECKSUM_FAILED:
      sprintf(buff, "{\"error\":\"Check sum failed\"}");
      LOG_WARN("DHT11 Checksum failed\n");
      break;
    case DHT11_STATUS_TIMEOUT:
      sprintf(buff, "{\"error\":\"Reading timed out\"}");
      LOG_WARN("DHT11 Reading timed out\n");
      break;
    default:
      sprintf(buff, "{\"error\":\"Unknown error\"}");
      LOG_WARN("DHT11 Unknown error\n");
      break;
  }

  printf("Sending JSON response to requester\n");

  SEND_STRING(&s->sout, buff);

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
PROCESS(webserver_nogui_process, "Web Sense-db server");
PROCESS_THREAD(webserver_nogui_process, ev, data)
{
  PROCESS_BEGIN();

  httpd_init();

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    httpd_appcall(data);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
httpd_simple_script_t
httpd_simple_get_script(const char *name)
{
  return generate_routes;
}
/*---------------------------------------------------------------------------*/
/* Declare and auto-start this file's process */
PROCESS(web_sense_db, "Web Sense-db");
AUTOSTART_PROCESSES(&web_sense_db);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(web_sense_db, ev, data)
{
  PROCESS_BEGIN();
  leds_on(LEDS_BLUE);

  /* Initialize the DHT11 sensor */
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  /* Start the web server process */
  process_start(&webserver_nogui_process, NULL);

  LOG_INFO("Web Sense started\n");

  PROCESS_END();
}
