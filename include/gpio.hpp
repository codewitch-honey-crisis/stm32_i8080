#pragma once
#include <stddef.h>
#include <stm32f767xx.h>
#include <stm32f7xx.h>
namespace cmsis {

typedef enum struct pin_mode : uint32_t {
    input = (0x00000000U),           /*!< Select input mode */
    output = GPIO_MODER_MODER0_0,    /*!< Select output mode */
    alternate = GPIO_MODER_MODER0_1, /*!< Select alternate function mode */
    analog = GPIO_MODER_MODER0       /*!< Select analog mode */
} pin_mode_t;

typedef enum struct pin_pull_mode : uint32_t {
    none = (0x00000000U),           /*!< Select I/O no pull */
    pull_up = GPIO_PUPDR_PUPDR0_0,  /*!< Select I/O pull up */
    pull_down = GPIO_PUPDR_PUPDR0_1 /*!< Select I/O pull down */
} pin_pull_mode_t;

typedef enum struct pin_output_type : uint32_t {
    push_pull = (0x00000000U),    /*!< Select push-pull as output type */
    open_drain = GPIO_OTYPER_OT_0 /*!< Select open-drain as output type */
} pin_output_type_t;

typedef enum struct pin_speed : uint32_t {
    low = (0x00000000U),               /*!< Select I/O low output speed    */
    medium = GPIO_OSPEEDER_OSPEEDR0_0, /*!< Select I/O medium output speed */
    high = GPIO_OSPEEDER_OSPEEDR0_1,   /*!< Select I/O fast output speed   */
    very_high = GPIO_OSPEEDER_OSPEEDR0 /*!< Select I/O high output speed   */
} pin_speed_t;

typedef enum struct port_name {
    first = 0x00,
    a = first,
    b,
#if defined GPIOC_BASE
    c,
#endif
#if defined GPIOD_BASE
    d,
#endif
#if defined GPIOE_BASE
    e,
#endif
#if defined GPIOF_BASE
    f,
#endif
#if defined GPIOG_BASE
    g,
#endif
#if defined GPIOH_BASE
    h,
#endif
#if defined GPIOI_BASE
    i,
#endif
#if defined GPIOJ_BASE
    j,
#endif
#if defined GPIOK_BASE
    k,
#endif
#if defined GPIOZ_BASE
    z,
#endif
    end,
    last = (end - 1)
} port_name_t;

typedef struct gpio_port {
  GPIO_TypeDef *reg_base;
  port_name_t name() const;
  bool clock_enabled() const;
  void clock_enabled(bool value);
} gpio_port_t;

typedef struct gpio_pin {
    port_name_t port;
    uint8_t index;
    bool port_clock_enabled() const;
    void port_clock_enabled(bool value);
    bool input() const;
    bool output() const;
    void output(bool value);
    pin_output_type_t output_type() const;
    void output_type(pin_output_type_t value);
    pin_mode_t mode() const;
    void mode(pin_mode_t value);
    pin_pull_mode_t pull_mode() const;
    void pull_mode(pin_pull_mode_t value);
    pin_speed_t speed() const;
    void speed(pin_speed_t value);
} gpio_pin_t;
constexpr static const size_t gpio_ports_size = (size_t)port_name_t::last - (size_t)port_name_t::first + 1;
extern gpio_port_t gpio_ports[];

template<port_name_t Port, uint8_t Index>
struct static_gpio_pin {
  using type = static_gpio_pin;
  constexpr static const port_name_t port = Port;
  constexpr static const uint8_t index = Index;
  static void port_clock_enabled(bool value) {
      gpio_ports[(size_t)port].clock_enabled(value);
  }
  static bool port_clock_enabled() {
      return gpio_ports[(size_t)port].clock_enabled();
  }
  static bool input() {
    constexpr static const unsigned shl = 1 << index;
    return (READ_BIT(gpio_ports[(size_t)port].reg_base->IDR, shl) == shl);
  }
  static bool output() {
    constexpr static const unsigned shl = 1 << index;
    uint32_t odr = READ_REG(gpio_ports[(size_t)port].reg_base->ODR);
    return !!(odr & shl);
  }
  static void output(bool value) {
    constexpr static const unsigned shl = 1 << index;
    uint32_t val = 0;
    val = shl << (16*!value);
    WRITE_REG(gpio_ports[(size_t)port].reg_base->BSRR, val); 
  }
  static pin_output_type_t output_type() {
    constexpr static const unsigned shl = 1 << index;
    return (pin_output_type_t)(READ_BIT(gpio_ports[(size_t)port].reg_base->OTYPER, shl) >> POSITION_VAL(shl));
  }
  static void output_type(pin_output_type_t value) {
    constexpr static const unsigned shl = 1 << index;
    MODIFY_REG(gpio_ports[(size_t)port].reg_base->OTYPER, shl, (shl * uint32_t(value)));
  }
  static pin_mode_t mode() {
    constexpr static const unsigned shl = 1 << index;
    return (pin_mode_t)(READ_BIT(gpio_ports[(size_t)port].reg_base->MODER,
                                 (GPIO_MODER_MODER0 << (POSITION_VAL(shl) * 2U))) >>
                        (POSITION_VAL(shl) * 2U));
  }
  static void mode(pin_mode_t value) {
    constexpr static const unsigned shl = 1 << index;
    MODIFY_REG(gpio_ports[(size_t)port].reg_base->MODER, (GPIO_MODER_MODER0 << (POSITION_VAL(shl) * 2U)), (uint32_t(value) << (POSITION_VAL(shl) * 2U)));
  }
  static pin_pull_mode_t pull_mode() {
    constexpr static const unsigned shl = 1 << index;
    return (pin_pull_mode_t)(READ_BIT(gpio_ports[(size_t)port].reg_base->PUPDR,
                                      (GPIO_PUPDR_PUPDR0 << (POSITION_VAL(shl) * 2U))) >>
                             (POSITION_VAL(shl) * 2U));
  }
  static void pull_mode(pin_pull_mode_t value) {
    constexpr static unsigned shl = 1 << index;
    MODIFY_REG(gpio_ports[(size_t)port].reg_base->PUPDR, (GPIO_PUPDR_PUPDR0 << (POSITION_VAL(shl) * 2U)), (uint32_t(value) << (POSITION_VAL(shl) * 2U)));
  }
  static pin_speed_t speed() {
    constexpr static const unsigned shl = 1 << index;
    return (pin_speed_t)(READ_BIT(gpio_ports[(size_t)port].reg_base->OSPEEDR,
                                  (GPIO_OSPEEDER_OSPEEDR0 << (POSITION_VAL(shl) * 2U))) >>
                         (POSITION_VAL(shl) * 2U));
  }
  static void speed(pin_speed_t value) {
    constexpr static const unsigned shl = 1 << index;
    MODIFY_REG(gpio_ports[(size_t)port].reg_base->OSPEEDR, (GPIO_OSPEEDER_OSPEEDR0 << (POSITION_VAL(shl) * 2U)),
               (uint32_t(value) << (POSITION_VAL(shl) * 2U)));
  }
};
#if defined(GPIOA)
using pa0_t = static_gpio_pin<port_name_t::a,0>;
using pa1_t = static_gpio_pin<port_name_t::a,1>;
using pa2_t = static_gpio_pin<port_name_t::a,2>;
using pa3_t = static_gpio_pin<port_name_t::a,3>;
using pa4_t = static_gpio_pin<port_name_t::a,4>;
using pa5_t = static_gpio_pin<port_name_t::a,5>;
using pa6_t = static_gpio_pin<port_name_t::a,6>;
using pa7_t = static_gpio_pin<port_name_t::a,7>;
using pa8_t = static_gpio_pin<port_name_t::a,8>;
using pa9_t = static_gpio_pin<port_name_t::a,9>;
using pa10_t = static_gpio_pin<port_name_t::a,10>;
using pa11_t = static_gpio_pin<port_name_t::a,11>;
using pa12_t = static_gpio_pin<port_name_t::a,12>;
using pa13_t = static_gpio_pin<port_name_t::a,13>;
using pa14_t = static_gpio_pin<port_name_t::a,14>;
using pa15_t = static_gpio_pin<port_name_t::a,15>;
#endif

#if defined(GPIOB)
using pb0_t = static_gpio_pin<port_name_t::b,0>;
using pb1_t = static_gpio_pin<port_name_t::b,1>;
using pb2_t = static_gpio_pin<port_name_t::b,2>;
using pb3_t = static_gpio_pin<port_name_t::b,3>;
using pb4_t = static_gpio_pin<port_name_t::b,4>;
using pb5_t = static_gpio_pin<port_name_t::b,5>;
using pb6_t = static_gpio_pin<port_name_t::b,6>;
using pb7_t = static_gpio_pin<port_name_t::b,7>;
using pb8_t = static_gpio_pin<port_name_t::b,8>;
using pb9_t = static_gpio_pin<port_name_t::b,9>;
using pb10_t = static_gpio_pin<port_name_t::b,10>;
using pb11_t = static_gpio_pin<port_name_t::b,11>;
using pb12_t = static_gpio_pin<port_name_t::b,12>;
using pb13_t = static_gpio_pin<port_name_t::b,13>;
using pb14_t = static_gpio_pin<port_name_t::b,14>;
using pb15_t = static_gpio_pin<port_name_t::b,15>;
#endif

#if defined(GPIOC)
using pc0_t = static_gpio_pin<port_name_t::c,0>;
using pc1_t = static_gpio_pin<port_name_t::c,1>;
using pc2_t = static_gpio_pin<port_name_t::c,2>;
using pc3_t = static_gpio_pin<port_name_t::c,3>;
using pc4_t = static_gpio_pin<port_name_t::c,4>;
using pc5_t = static_gpio_pin<port_name_t::c,5>;
using pc6_t = static_gpio_pin<port_name_t::c,6>;
using pc7_t = static_gpio_pin<port_name_t::c,7>;
using pc8_t = static_gpio_pin<port_name_t::c,8>;
using pc9_t = static_gpio_pin<port_name_t::c,9>;
using pc10_t = static_gpio_pin<port_name_t::c,10>;
using pc11_t = static_gpio_pin<port_name_t::c,11>;
using pc12_t = static_gpio_pin<port_name_t::c,12>;
using pc13_t = static_gpio_pin<port_name_t::c,13>;
using pc14_t = static_gpio_pin<port_name_t::c,14>;
using pc15_t = static_gpio_pin<port_name_t::c,15>;
#endif

#if defined(GPIOD)
using pd0_t = static_gpio_pin<port_name_t::d,0>;
using pd1_t = static_gpio_pin<port_name_t::d,1>;
using pd2_t = static_gpio_pin<port_name_t::d,2>;
using pd3_t = static_gpio_pin<port_name_t::d,3>;
using pd4_t = static_gpio_pin<port_name_t::d,4>;
using pd5_t = static_gpio_pin<port_name_t::d,5>;
using pd6_t = static_gpio_pin<port_name_t::d,6>;
using pd7_t = static_gpio_pin<port_name_t::d,7>;
using pd8_t = static_gpio_pin<port_name_t::d,8>;
using pd9_t = static_gpio_pin<port_name_t::d,9>;
using pd10_t = static_gpio_pin<port_name_t::d,10>;
using pd11_t = static_gpio_pin<port_name_t::d,11>;
using pd12_t = static_gpio_pin<port_name_t::d,12>;
using pd13_t = static_gpio_pin<port_name_t::d,13>;
using pd14_t = static_gpio_pin<port_name_t::d,14>;
using pd15_t = static_gpio_pin<port_name_t::d,15>;
#endif

#if defined(GPIOE)
using pe0_t = static_gpio_pin<port_name_t::e,0>;
using pe1_t = static_gpio_pin<port_name_t::e,1>;
using pe2_t = static_gpio_pin<port_name_t::e,2>;
using pe3_t = static_gpio_pin<port_name_t::e,3>;
using pe4_t = static_gpio_pin<port_name_t::e,4>;
using pe5_t = static_gpio_pin<port_name_t::e,5>;
using pe6_t = static_gpio_pin<port_name_t::e,6>;
using pe7_t = static_gpio_pin<port_name_t::e,7>;
using pe8_t = static_gpio_pin<port_name_t::e,8>;
using pe9_t = static_gpio_pin<port_name_t::e,9>;
using pe10_t = static_gpio_pin<port_name_t::e,10>;
using pe11_t = static_gpio_pin<port_name_t::e,11>;
using pe12_t = static_gpio_pin<port_name_t::e,12>;
using pe13_t = static_gpio_pin<port_name_t::e,13>;
using pe14_t = static_gpio_pin<port_name_t::e,14>;
using pe15_t = static_gpio_pin<port_name_t::e,15>;
#endif

#if defined(GPIOF)
using pf0_t = static_gpio_pin<port_name_t::f,0>;
using pf1_t = static_gpio_pin<port_name_t::f,1>;
using pf2_t = static_gpio_pin<port_name_t::f,2>;
using pf3_t = static_gpio_pin<port_name_t::f,3>;
using pf4_t = static_gpio_pin<port_name_t::f,4>;
using pf5_t = static_gpio_pin<port_name_t::f,5>;
using pf6_t = static_gpio_pin<port_name_t::f,6>;
using pf7_t = static_gpio_pin<port_name_t::f,7>;
using pf8_t = static_gpio_pin<port_name_t::f,8>;
using pf9_t = static_gpio_pin<port_name_t::f,9>;
using pf10_t = static_gpio_pin<port_name_t::f,10>;
using pf11_t = static_gpio_pin<port_name_t::f,11>;
using pf12_t = static_gpio_pin<port_name_t::f,12>;
using pf13_t = static_gpio_pin<port_name_t::f,13>;
using pf14_t = static_gpio_pin<port_name_t::f,14>;
using pf15_t = static_gpio_pin<port_name_t::f,15>;
#endif

#if defined(GPIOG)
using pg0_t = static_gpio_pin<port_name_t::g,0>;
using pg1_t = static_gpio_pin<port_name_t::g,1>;
using pg2_t = static_gpio_pin<port_name_t::g,2>;
using pg3_t = static_gpio_pin<port_name_t::g,3>;
using pg4_t = static_gpio_pin<port_name_t::g,4>;
using pg5_t = static_gpio_pin<port_name_t::g,5>;
using pg6_t = static_gpio_pin<port_name_t::g,6>;
using pg7_t = static_gpio_pin<port_name_t::g,7>;
using pg8_t = static_gpio_pin<port_name_t::g,8>;
using pg9_t = static_gpio_pin<port_name_t::g,9>;
using pg10_t = static_gpio_pin<port_name_t::g,10>;
using pg11_t = static_gpio_pin<port_name_t::g,11>;
using pg12_t = static_gpio_pin<port_name_t::g,12>;
using pg13_t = static_gpio_pin<port_name_t::g,13>;
using pg14_t = static_gpio_pin<port_name_t::g,14>;
using pg15_t = static_gpio_pin<port_name_t::g,15>;
#endif

#if defined(GPIOH)
using ph0_t = static_gpio_pin<port_name_t::h,0>;
using ph1_t = static_gpio_pin<port_name_t::h,1>;
using ph2_t = static_gpio_pin<port_name_t::h,2>;
using ph3_t = static_gpio_pin<port_name_t::h,3>;
using ph4_t = static_gpio_pin<port_name_t::h,4>;
using ph5_t = static_gpio_pin<port_name_t::h,5>;
using ph6_t = static_gpio_pin<port_name_t::h,6>;
using ph7_t = static_gpio_pin<port_name_t::h,7>;
using ph8_t = static_gpio_pin<port_name_t::h,8>;
using ph9_t = static_gpio_pin<port_name_t::h,9>;
using ph10_t = static_gpio_pin<port_name_t::h,10>;
using ph11_t = static_gpio_pin<port_name_t::h,11>;
using ph12_t = static_gpio_pin<port_name_t::h,12>;
using ph13_t = static_gpio_pin<port_name_t::h,13>;
using ph14_t = static_gpio_pin<port_name_t::h,14>;
using ph15_t = static_gpio_pin<port_name_t::h,15>;
#endif

#if defined(GPIOI)
using pi0_t = static_gpio_pin<port_name_t::i,0>;
using pi1_t = static_gpio_pin<port_name_t::i,1>;
using pi2_t = static_gpio_pin<port_name_t::i,2>;
using pi3_t = static_gpio_pin<port_name_t::i,3>;
using pi4_t = static_gpio_pin<port_name_t::i,4>;
using pi5_t = static_gpio_pin<port_name_t::i,5>;
using pi6_t = static_gpio_pin<port_name_t::i,6>;
using pi7_t = static_gpio_pin<port_name_t::i,7>;
using pi8_t = static_gpio_pin<port_name_t::i,8>;
using pi9_t = static_gpio_pin<port_name_t::i,9>;
using pi10_t = static_gpio_pin<port_name_t::i,10>;
using pi11_t = static_gpio_pin<port_name_t::i,11>;
using pi12_t = static_gpio_pin<port_name_t::i,12>;
using pi13_t = static_gpio_pin<port_name_t::i,13>;
using pi14_t = static_gpio_pin<port_name_t::i,14>;
using pi15_t = static_gpio_pin<port_name_t::i,15>;
#endif

#if defined(GPIOJ)
using pj0_t = static_gpio_pin<port_name_t::j,0>;
using pj1_t = static_gpio_pin<port_name_t::j,1>;
using pj2_t = static_gpio_pin<port_name_t::j,2>;
using pj3_t = static_gpio_pin<port_name_t::j,3>;
using pj4_t = static_gpio_pin<port_name_t::j,4>;
using pj5_t = static_gpio_pin<port_name_t::j,5>;
using pj6_t = static_gpio_pin<port_name_t::j,6>;
using pj7_t = static_gpio_pin<port_name_t::j,7>;
using pj8_t = static_gpio_pin<port_name_t::j,8>;
using pj9_t = static_gpio_pin<port_name_t::j,9>;
using pj10_t = static_gpio_pin<port_name_t::j,10>;
using pj11_t = static_gpio_pin<port_name_t::j,11>;
using pj12_t = static_gpio_pin<port_name_t::j,12>;
using pj13_t = static_gpio_pin<port_name_t::j,13>;
using pj14_t = static_gpio_pin<port_name_t::j,14>;
using pj15_t = static_gpio_pin<port_name_t::j,15>;
#endif

#if defined(GPIOK)
using pk0_t = static_gpio_pin<port_name_t::k,0>;
using pk1_t = static_gpio_pin<port_name_t::k,1>;
using pk2_t = static_gpio_pin<port_name_t::k,2>;
using pk3_t = static_gpio_pin<port_name_t::k,3>;
using pk4_t = static_gpio_pin<port_name_t::k,4>;
using pk5_t = static_gpio_pin<port_name_t::k,5>;
using pk6_t = static_gpio_pin<port_name_t::k,6>;
using pk7_t = static_gpio_pin<port_name_t::k,7>;
using pk8_t = static_gpio_pin<port_name_t::k,8>;
using pk9_t = static_gpio_pin<port_name_t::k,9>;
using pk10_t = static_gpio_pin<port_name_t::k,10>;
using pk11_t = static_gpio_pin<port_name_t::k,11>;
using pk12_t = static_gpio_pin<port_name_t::k,12>;
using pk13_t = static_gpio_pin<port_name_t::k,13>;
using pk14_t = static_gpio_pin<port_name_t::k,14>;
using pk15_t = static_gpio_pin<port_name_t::k,15>;
#endif





}  // namespace cmsis

