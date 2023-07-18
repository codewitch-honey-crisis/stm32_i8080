#include <gpio.hpp>
using namespace cmsis;
#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT) ((REG) & (BIT))

port_name_t cmsis::gpio_port_t::name() const {
    if (reg_base == gpio_ports[1].reg_base) {
        return port_name_t::b;
    }
#if defined(GPIOC)
    else if (reg_base == gpio_ports[2].reg_base) {
        return port_name_t::c;
    }
#endif
#if defined(GPIOD)
    else if (reg_base == gpio_ports[3].reg_base) {
        return port_name_t::d;
    }
#endif
#if defined(GPIOE)
    else if (reg_base == gpio_ports[4].reg_base) {
        return port_name_t::e;
    }
#endif
#if defined(GPIOF)
    else if (reg_base == gpio_ports[5].reg_base) {
        return port_name_t::f;
    }
#endif
#if defined(GPIOG)
    else if (reg_base == gpio_ports[6].reg_base) {
        return port_name_t::g;
    }
#endif
#if defined(GPIOH)
    else if (reg_base == gpio_ports[7].reg_base) {
        return port_name_t::h;
    }
#endif
#if defined(GPIOI)
    else if (reg_base == gpio_ports[8].reg_base) {
        return port_name_t::i;
    }
#endif
#if defined(GPIOJ)
    else if (reg_base == gpio_ports[9].reg_base) {
        return port_name_t::j;
    }
#endif
#if defined(GPIOK)
    else if (reg_base == gpio_ports[10].reg_base) {
        return port_name_t::k;
    } else {
        return port_name_t::a;
    }
#endif
}

void cmsis::gpio_port_t::clock_enabled(bool value) {
    uint32_t r;
    __IO uint32_t tmpreg;
    switch (name()) {
        case port_name_t::b:
            r = RCC_AHB1ENR_GPIOBEN;
            break;
#if defined(GPIOC)
        case port_name_t::c:
            r = RCC_AHB1ENR_GPIOCEN;
            break;
#endif
#if defined(GPIOD)
        case port_name_t::d:
            r = RCC_AHB1ENR_GPIODEN;
            break;
#endif
#if defined(GPIOE)
        case port_name_t::e:
            r = RCC_AHB1ENR_GPIOEEN;
            break;
#endif
#if defined(GPIOF)
        case port_name_t::f:
            r = RCC_AHB1ENR_GPIOFEN;
            break;
#endif
#if defined(GPIOG)
        case port_name_t::g:
            r = RCC_AHB1ENR_GPIOGEN;
            break;
#endif
#if defined(GPIOH)
        case port_name_t::h:
            r = RCC_AHB1ENR_GPIOHEN;
            break;
#endif
#if defined(GPIOI)
        case port_name_t::i:
            r = RCC_AHB1ENR_GPIOIEN;
            break;
#endif
#if defined(GPIOJ)
        case port_name_t::j:
            r = RCC_AHB1ENR_GPIOJEN;
            break;
#endif
#if defined(GPIOK)
        case port_name_t::k:
            r = RCC_AHB1ENR_GPIOKEN;
            break;
#endif           
        default:  // port_name_t::a:
            r = RCC_AHB1ENR_GPIOAEN;
            break;
    }
    if(value) {
      SET_BIT(RCC->AHB1ENR, r);
    } else {
      CLEAR_BIT(RCC->AHB1ENR, r);
    }
    tmpreg = READ_BIT(RCC->AHB1ENR, r);  // delay
    (void)tmpreg;
}
bool cmsis::gpio_port_t::clock_enabled() const {
    uint32_t r;
    __IO uint32_t result;
    switch (name()) {
        case port_name_t::b:
            r = RCC_AHB1ENR_GPIOBEN;
            break;
#if defined(GPIOC)
        case port_name_t::c:
            r = RCC_AHB1ENR_GPIOCEN;
            break;
#endif
#if defined(GPIOD)
        case port_name_t::d:
            r = RCC_AHB1ENR_GPIODEN;
            break;
#endif
#if defined(GPIOE)
        case port_name_t::e:
            r = RCC_AHB1ENR_GPIOEEN;
            break;
#endif
#if defined(GPIOF)
        case port_name_t::f:
            r = RCC_AHB1ENR_GPIOFEN;
            break;
#endif
#if defined(GPIOG)
        case port_name_t::g:
            r = RCC_AHB1ENR_GPIOGEN;
            break;
#endif
#if defined(GPIOH)
        case port_name_t::h:
            r = RCC_AHB1ENR_GPIOHEN;
            break;
#endif
#if defined(GPIOI)
        case port_name_t::i:
            r = RCC_AHB1ENR_GPIOIEN;
            break;
#endif
#if defined(GPIOJ)
        case port_name_t::j:
            r = RCC_AHB1ENR_GPIOJEN;
            break;
#endif
#if defined(GPIOK)
        case port_name_t::k:
            r = RCC_AHB1ENR_GPIOKEN;
            break;
#endif           
        default:  // port_name_t::a:
            r = RCC_AHB1ENR_GPIOAEN;
            break;
    }
    result = READ_BIT(RCC->AHB1ENR, r);  // delay
    return !!result;
}

void cmsis::gpio_pin_t::port_clock_enabled(bool value) {
    gpio_ports[(size_t)port].clock_enabled(value);
}
bool cmsis::gpio_pin_t::port_clock_enabled() const {
    return gpio_ports[(size_t)port].clock_enabled();
}
void cmsis::gpio_pin_t::mode(pin_mode_t value) {
    const unsigned shl = 1 << index;
    MODIFY_REG(gpio_ports[(size_t)port].reg_base->MODER, (GPIO_MODER_MODER0 << (POSITION_VAL(shl) * 2U)), (uint32_t(value) << (POSITION_VAL(shl) * 2U)));
}
pin_mode_t cmsis::gpio_pin_t::mode() const {
    const unsigned shl = 1 << index;
    return (pin_mode_t)(READ_BIT(gpio_ports[(size_t)port].reg_base->MODER,
                                 (GPIO_MODER_MODER0 << (POSITION_VAL(shl) * 2U))) >>
                        (POSITION_VAL(shl) * 2U));
}
void cmsis::gpio_pin_t::pull_mode(pin_pull_mode_t value) {
    const unsigned shl = 1 << index;
    MODIFY_REG(gpio_ports[(size_t)port].reg_base->PUPDR, (GPIO_PUPDR_PUPDR0 << (POSITION_VAL(shl) * 2U)), (uint32_t(value) << (POSITION_VAL(shl) * 2U)));
}
pin_pull_mode_t cmsis::gpio_pin_t::pull_mode() const {
    const unsigned shl = 1 << index;
    return (pin_pull_mode_t)(READ_BIT(gpio_ports[(size_t)port].reg_base->PUPDR,
                                      (GPIO_PUPDR_PUPDR0 << (POSITION_VAL(shl) * 2U))) >>
                             (POSITION_VAL(shl) * 2U));
}
bool cmsis::gpio_pin_t::input() const {
    const unsigned shl = 1 << index;
    return (READ_BIT(gpio_ports[(size_t)port].reg_base->IDR, shl) == shl);
}
void cmsis::gpio_pin_t::output(bool value) {
    const unsigned shl = 1 << index;
    uint32_t val = 0;
    val = shl << (16*!value);
    WRITE_REG(gpio_ports[(size_t)port].reg_base->BSRR, val);
}
bool cmsis::gpio_pin_t::output() const {
    const unsigned shl = 1 << index;
    uint32_t odr = READ_REG(gpio_ports[(size_t)port].reg_base->ODR);
    return !!(odr & shl);
}
void cmsis::gpio_pin_t::output_type(pin_output_type_t value) {
    const unsigned shl = 1 << index;
    MODIFY_REG(gpio_ports[(size_t)port].reg_base->OTYPER, shl, (shl * uint32_t(value)));
}
pin_output_type_t cmsis::gpio_pin_t::output_type() const {
    const unsigned shl = 1 << index;
    return (pin_output_type_t)(READ_BIT(gpio_ports[(size_t)port].reg_base->OTYPER, shl) >> POSITION_VAL(shl));
}
void cmsis::gpio_pin_t::speed(pin_speed value) {
    const unsigned shl = 1 << index;
    MODIFY_REG(gpio_ports[(size_t)port].reg_base->OSPEEDR, (GPIO_OSPEEDER_OSPEEDR0 << (POSITION_VAL(shl) * 2U)),
               (uint32_t(value) << (POSITION_VAL(shl) * 2U)));
}
pin_speed_t cmsis::gpio_pin_t::speed() const {
    const unsigned shl = 1 << index;
    return (pin_speed_t)(READ_BIT(gpio_ports[(size_t)port].reg_base->OSPEEDR,
                                  (GPIO_OSPEEDER_OSPEEDR0 << (POSITION_VAL(shl) * 2U))) >>
                         (POSITION_VAL(shl) * 2U));
}

gpio_port_t cmsis::gpio_ports[] = {
    {(GPIO_TypeDef *)GPIOA_BASE},
    {(GPIO_TypeDef *)GPIOB_BASE}
#if defined GPIOC_BASE
    ,
    {(GPIO_TypeDef *)GPIOC_BASE}
#endif
#if defined GPIOD_BASE
    ,
    {(GPIO_TypeDef *)GPIOD_BASE}
#endif
#if defined GPIOE_BASE
    ,
    {(GPIO_TypeDef *)GPIOE_BASE}
#endif
#if defined GPIOF_BASE
    ,
    {(GPIO_TypeDef *)GPIOF_BASE}
#endif
#if defined GPIOG_BASE
    ,
    {(GPIO_TypeDef *)GPIOG_BASE}
#endif
#if defined GPIOH_BASE
    ,
    {(GPIO_TypeDef *)GPIOH_BASE}
#endif
#if defined GPIOI_BASE
    ,
    {(GPIO_TypeDef *)GPIOI_BASE}
#endif
#if defined GPIOJ_BASE
    ,
    {(GPIO_TypeDef *)GPIOJ_BASE}
#endif
#if defined GPIOK_BASE
    ,
    {(GPIO_TypeDef *)GPIOK_BASE}
#endif
#if defined GPIOZ_BASE
    ,
    {(GPIO_TypeDef *)GPIOZ_BASE}
#endif
};