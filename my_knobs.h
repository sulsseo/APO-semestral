#pragma once
#include <mutex>
#include <sys/mman.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <cstring>
#include <cinttypes>
#include <ctime>

#include "mzapo_phys.h"

/**
 * work for knobhandle thread. listen to knob changes and return values to pointers
 *
 * @param positionX
 * @param positionY
 * @param constantImaginary
 * @param red_click
 * @param green_click
 * @param blue_click
 * @param mutex
 * @param finished
 */
void handle_knobs(long double *positionX,
                  long double *positionY,
                  long double *constantImaginary,
                  int *red_click,
                  int *green_click,
                  int *blue_click,
                  std::mutex *mutex,
                  bool *finished);

/**
 * save initial possition of knob spinners
 */
void get_init_position();
