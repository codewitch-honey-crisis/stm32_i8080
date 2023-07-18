#pragma once
#include <stdint.h>
namespace cmsis {
    extern uint32_t sys_ticks();
    extern void sys_tick_delay(uint32_t ticks);
}
