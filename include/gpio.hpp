#pragma once
#include <stddef.h>
#include <stm32f767xx.h>
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
  void clock_enable();

} gpio_port_t;

typedef struct gpio_pin {
    port_name_t port;
    uint8_t index;
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
}  // namespace cmsis