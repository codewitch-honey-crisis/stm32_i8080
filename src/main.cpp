#include "support.h"
#include <gpio.hpp>
using namespace cmsis;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
extern "C" int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Configure the system clock to 216 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  //LED_Init();
  gpio_ports[(size_t)port_name_t::b].clock_enable();
  // pb0 is LED
  gpio_pin pin = {port_name_t::b,0};
  pin.mode(pin_mode_t::output);
  pin.output_type(pin_output_type_t::push_pull);
  pin.pull_mode(pin_pull_mode_t::none);
  pin.output(true);
  //for(int i = 0;i<500000;++i);
  pin.output(false);
  /* Configure the EXTI Line on User Button */
  Configure_EXTI();

  /* Infinite loop */
  while (1)
  {
  }
}
