#include <stm32f7xx.h>

#include <sys_tick.hpp>
static volatile uint32_t sys_ticks_count = 0;
/*void SysTick_Init(void) {
    SysTick->CTRL = 0;
    sys_ticks_count = 0;
    SysTick->VAL = 0; // Load the SysTick Counter Value 
    SysTick->CTRL = (SysTick_CTRL_TICKINT_Msk   |  // Enable SysTick exception 
                     SysTick_CTRL_ENABLE_Msk) |    // Enable SysTick system timer 
                     SysTick_CTRL_CLKSOURCE_Msk;   // Use processor clock source 
}*/
extern "C" void SysTick_Handler(void) {
		++sys_ticks_count;
}
void cmsis::sys_tick_delay(uint32_t ticks) {
    uint32_t started = sys_ticks_count;
    while((sys_ticks_count-started)<=ticks);
}
uint32_t cmsis::sys_ticks() {
    return sys_ticks_count;
}