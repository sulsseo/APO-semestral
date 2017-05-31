#define _POSIX_C_SOURCE 200112L

#include "my_knobs.h"

/*******************************************************************
  Simple program to demostrate binary reprezentation on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_binrep.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

const char *memdev = "/dev/mem";

#define SPILED_REG_BASE_PHYS 0x43c40000
#define SPILED_REG_SIZE      0x00004000

#define SPILED_REG_LED_LINE_o           0x004
#define SPILED_REG_LED_RGB1_o           0x010
#define SPILED_REG_LED_RGB2_o           0x014
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018

#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020
#define SPILED_REG_KNOBS_8BIT_o         0x024

unsigned char *mem_base;
long double initX;
long double initY;

uint32_t get_knobs_value(unsigned char *mem_base) {
    if (mem_base == NULL) exit(1);

    uint32_t rgb_knobs_value;
    rgb_knobs_value = *(volatile uint32_t *) (mem_base + SPILED_REG_KNOBS_8BIT_o);

    *(volatile uint32_t *) (mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;
    *(volatile uint32_t *) (mem_base + SPILED_REG_LED_RGB1_o) = rgb_knobs_value;
    *(volatile uint32_t *) (mem_base + SPILED_REG_LED_RGB2_o) = rgb_knobs_value;

    return rgb_knobs_value;
}

long double divide_value(long double knob_value) {
    return (knob_value - 128.0) / 64.0;
}

void handle_knobs(long double *positionX,
                  long double *positionY,
                  long double *constantImaginary,
                  int *red_click,
                  int *green_click,
                  int *blue_click,
                  std::mutex *mutex,
                  bool *finished) {

    std::unique_lock<std::mutex> thread_lock(*mutex);
    thread_lock.unlock();
    while (!*finished) {
        thread_lock.lock();

        uint32_t rgb_knobs_value = get_knobs_value(mem_base);

        *red_click = (rgb_knobs_value >> 24 & 0b00000100);
        *green_click = (rgb_knobs_value >> 24 & 0b00000010);
        *blue_click = (rgb_knobs_value >> 24 & 0b00000001);

        *positionX = divide_value((uint8_t) (rgb_knobs_value >> 16));
        *positionY = divide_value((uint8_t) (rgb_knobs_value >> 8));

        *constantImaginary = divide_value((uint8_t) rgb_knobs_value);

        thread_lock.unlock();
    }
}

void get_init_position() {
    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

    uint32_t knobs_value = get_knobs_value(mem_base);

    initX = divide_value(knobs_value >> 16);
    initY = divide_value(knobs_value >> 8);
}
