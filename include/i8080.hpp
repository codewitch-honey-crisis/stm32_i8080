#pragma once
#include <gpio.hpp>
namespace cmsis {
template<typename PinCS,typename PinRS,typename PinRD, typename PinWR, typename PinD0, typename PinD1, typename PinD2, typename PinD3, typename PinD4, typename PinD5, typename PinD6, typename PinD7>
class i8080_8 {
public:
    using type = i8080_8;
    using pin_cs = PinCS;
    using pin_dc = PinRS; // for compat with spi
    using pin_rs = PinRS;
    using pin_rd = PinRD;
    using pin_wr = PinWR;
    using pin_d0 = PinD0;
    using pin_d1 = PinD1;
    using pin_d2 = PinD2;
    using pin_d3 = PinD3;
    using pin_d4 = PinD4;
    using pin_d5 = PinD5;
    using pin_d6 = PinD6;
    using pin_d7 = PinD7;
private:
    constexpr static const uint16_t pin_d0_mask = 1 << pin_d0::index;
    constexpr static const uint16_t pin_d1_mask = 1 << pin_d1::index;
    constexpr static const uint16_t pin_d2_mask = 1 << pin_d2::index;
    constexpr static const uint16_t pin_d3_mask = 1 << pin_d3::index;
    constexpr static const uint16_t pin_d4_mask = 1 << pin_d4::index;
    constexpr static const uint16_t pin_d5_mask = 1 << pin_d5::index;
    constexpr static const uint16_t pin_d6_mask = 1 << pin_d6::index;
    constexpr static const uint16_t pin_d7_mask = 1 << pin_d7::index;
    
    // masks for bottom byte of 16 bit value
    constexpr static const uint32_t d0_bsr_mask(uint8_t b) { return (pin_d0_mask<<16)>>((b<<4)&0x10); }
    constexpr static const uint32_t d1_bsr_mask(uint8_t b) { return (pin_d1_mask<<16)>>((b<<3)&0x10); }
    constexpr static const uint32_t d2_bsr_mask(uint8_t b) { return (pin_d2_mask<<16)>>((b<<2)&0x10); }
    constexpr static const uint32_t d3_bsr_mask(uint8_t b) { return (pin_d3_mask<<16)>>((b<<1)&0x10); }
    constexpr static const uint32_t d4_bsr_mask(uint8_t b) { return (pin_d4_mask<<16)>>((b<<0)&0x10); }
    constexpr static const uint32_t d5_bsr_mask(uint8_t b) { return (pin_d5_mask<<16)>>((b>>1)&0x10); }
    constexpr static const uint32_t d6_bsr_mask(uint8_t b) { return (pin_d6_mask<<16)>>((b>>2)&0x10); }
    constexpr static const uint32_t d7_bsr_mask(uint8_t b) { return (pin_d7_mask<<16)>>((b>>3)&0x10); }

    // masks for top byte of 16 bit value
    constexpr static const uint32_t d8_bsr_mask(uint8_t b) { return (pin_d0_mask<<16)>>((b>>4)&0x10); }
    constexpr static const uint32_t d9_bsr_mask(uint8_t b) { return (pin_d1_mask<<16)>>((b>>5)&0x10); }
    constexpr static const uint32_t d10_bsr_mask(uint8_t b) { return (pin_d2_mask<<16)>>((b>>6)&0x10); }
    constexpr static const uint32_t d11_bsr_mask(uint8_t b) { return (pin_d3_mask<<16)>>((b>>7)&0x10); }
    constexpr static const uint32_t d12_bsr_mask(uint8_t b) { return (pin_d4_mask<<16)>>((b>>8)&0x10); }
    constexpr static const uint32_t d13_bsr_mask(uint8_t b) { return (pin_d5_mask<<16)>>((b>>9)&0x10); }
    constexpr static const uint32_t d14_bsr_mask(uint8_t b) { return (pin_d6_mask<<16)>>((b>>10)&0x10); }
    constexpr static const uint32_t d15_bsr_mask(uint8_t b) { return (pin_d7_mask<<16)>>((b>>11)&0x10); }
    
    template<typename Pin> 
    static void init_pin() {
        Pin::port_clock_enabled(true);
        Pin::speed(pin_speed_t::high);
        Pin::mode(pin_mode_t::output);
    }
    static void write8(uint8_t value) {
        gpio_ports[(size_t)pin_d0::port].reg_base->BSRR = d0_bsr_mask(value);
        gpio_ports[(size_t)pin_d1::port].reg_base->BSRR = d1_bsr_mask(value);
        gpio_ports[(size_t)pin_d2::port].reg_base->BSRR = d2_bsr_mask(value);
        gpio_ports[(size_t)pin_d3::port].reg_base->BSRR = d3_bsr_mask(value);
        pin_wr::output(false);
        gpio_ports[(size_t)pin_d4::port].reg_base->BSRR = d4_bsr_mask(value);
        gpio_ports[(size_t)pin_d5::port].reg_base->BSRR = d5_bsr_mask(value);
        gpio_ports[(size_t)pin_d6::port].reg_base->BSRR = d6_bsr_mask(value);
        gpio_ports[(size_t)pin_d7::port].reg_base->BSRR = d7_bsr_mask(value);
        pin_wr::output(false);
        pin_wr::output(false);
        pin_wr::output(false);

        pin_wr::output(true);
        pin_wr::output(true);
        pin_wr::output(true);
        pin_wr::output(true);
        pin_wr::output(true);
        pin_wr::output(true);
    }
    static void write8_slow(uint8_t value) {
        pin_d0::output(value & (1<<0));
        pin_d1::output(value & (1<<1));
        pin_d2::output(value & (1<<2));
        pin_d3::output(value & (1<<3));
        pin_wr::output(false);
        pin_d4::output(value & (1<<4));
        pin_d5::output(value & (1<<5));
        pin_d6::output(value & (1<<6));
        pin_d7::output(value & (1<<7));
        
        pin_wr::output(false);
        pin_wr::output(false);
        pin_wr::output(false);

        pin_wr::output(true);
        pin_wr::output(true);
        pin_wr::output(true);
        pin_wr::output(true);
        pin_wr::output(true);
        pin_wr::output(true);
    }
public:
    static void initialize() {
        init_pin<pin_cs>();
        pin_cs::output(true);
        init_pin<pin_rs>();
        pin_rs::output(true);
        init_pin<pin_rd>();
        pin_rs::output(true);
        init_pin<pin_wr>();
        pin_wr::output(true);
        init_pin<pin_d0>();
        init_pin<pin_d1>();
        init_pin<pin_d2>();
        init_pin<pin_d3>();
        init_pin<pin_d4>();
        init_pin<pin_d5>();
        init_pin<pin_d6>();
        init_pin<pin_d7>();
    }
    static void write_command(uint8_t cmd, const uint8_t* args, size_t arg_count) {
        pin_cs::output(false);
        pin_dc::output(false);
        write8(cmd);
        pin_dc::output(true);
        for(size_t i = 0 ;i < arg_count; ++i) {
            write8(*args++);
        }
        pin_cs::output(true);
    }
    static void write_data(const void* data, size_t size) {
        pin_cs::output(false);
        const uint8_t* p = (const uint8_t*)data;
        for(size_t i = 0 ;i < size; ++i) {
            write8(*p++);
        }
        pin_cs::output(true);
    }

};
}