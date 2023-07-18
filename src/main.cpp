#include <sys_tick.hpp>
#include <i8080.hpp>

#include "support.h"
using namespace cmsis;

/* Private functions ---------------------------------------------------------*/
using bus_t = i8080_8<pf3_t,pc3_t,pa3_t,pc0_t,pf12_t,pd15_t,pf15_t,pe13_t,pf14_t,pe11_t,pe9_t,pf13_t>;
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
using pin_rst = pf5_t;
extern "C" int main(void) {
    /* Enable the CPU Cache */
    CPU_CACHE_Enable();
    /* Configure the system clock to 216 MHz */
    SystemClock_Config();
    //pb0_t::port_clock_enabled(true);
    //pb0_t::mode(pin_mode_t::output);
    //pb0_t::output(true);
    bus_t::initialize();
    pin_rst::port_clock_enabled(true);
    pin_rst::mode(pin_mode_t::output);
    pin_rst::output(true);
    sys_tick_delay(5);
    pin_rst::output(false);
    sys_tick_delay(20);
    pin_rst::output(true);
    sys_tick_delay(120);
    static const uint8_t params1[] = {0x00,0x03,0x09,0x08,0x16,0x0A,0x3F,0x78,0x4C,0x09,0x0A,0x08,0x16,0x1A,0x0F};
    bus_t::write_command(0xE0,params1,sizeof(params1));
    static const uint8_t params2[] = {0x00,0x16,0x19,0x03,0x0F,0x05,0x32,0x45,0x46,0x04,0x0E,0x0D,0x35,0x37,0x0F};
    bus_t::write_command(0xE1,params2,sizeof(params2));
    static const uint8_t params3[] = {0x17,0x15};
    bus_t::write_command(0xC0,params3,sizeof(params3));
    static const uint8_t params4[] = {0x41};
    bus_t::write_command(0xC1,params4,sizeof(params4));
    static const uint8_t params5[] = {0x00,0x12,0x80};
    bus_t::write_command(0xC5,params5,sizeof(params5));
    static const uint8_t params6[] = { 0x48 }; 
    bus_t::write_command(0x36,params6,sizeof(params6));
    static const uint8_t colmod_cal = 0x55; // 16bpp
    bus_t::write_command(0x3A,&colmod_cal,1);
    static const uint8_t params7[] = { 0x00 };
    bus_t::write_command(0xB0,params7,sizeof(params7));
    static const uint8_t params8[] = { 0xA0 };
    bus_t::write_command(0xB1,params8,sizeof(params8));
    static const uint8_t params9[] = { 0x02 };
    bus_t::write_command(0xB4,params9,sizeof(params9));
    static const uint8_t params10[] = { 0x02, 0x02, 0x3B };
    bus_t::write_command(0xB6,params10,sizeof(params10));
    static const uint8_t params11[] = { 0xC6 };
    bus_t::write_command(0xB7,params11,sizeof(params11));
    static const uint8_t params12[] = { 0xA9, 0x51, 0x2C, 0x82 };
    bus_t::write_command(0xF7,params12,sizeof(params12));
    bus_t::write_command(0x11,nullptr,0);
    sys_tick_delay(120);
    bus_t::write_command(0x29,nullptr,0);
    sys_tick_delay(25);
    static const uint8_t params13[] = {0x48};
    bus_t::write_command(0x36,params13,sizeof(params13));
    
    uint16_t x_start = 0;
    uint16_t x_end = 239;
    uint16_t y_start = 0;
    uint16_t y_end = 319;
    uint8_t data1[4];
    data1[0]=(x_start>>8)&0xFF;
    data1[1]=x_start&0xFF;
    data1[2]=((x_end)>>8)&0xFF;
    data1[3]=(x_end)&0xFF;
    bus_t::write_command(0x2A, data1, 4);
    uint8_t data2[4];
    data2[0]=(y_start>>8)&0xFF;
    data2[1]=y_start&0xFF;
    data2[2]=((y_end)>>8)&0xFF;
    data2[3]=(y_end)&0xFF;
    bus_t::write_command(0x2B, data2, 4);

    bus_t::write_command(0x2C,nullptr,0);
    uint8_t coldata[2] = { 0x00,0x00 };
    for(int y = y_start; y<=y_end;++y) {
        for(int x = x_start; x<=x_end;++x) {
            bus_t::write_data(coldata,2);
        }
    }
    while(1);
    /* Infinite loop */
    bool on = false;
    while (1) {
        pb0_t::output(on);
        on=!on;
        sys_tick_delay(250);
    }
}
