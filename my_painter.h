#pragma once

// some c++ logic
#include <mutex>
#include <cmath>
#include <iostream>
#include <thread>
#include <complex>

// my headers
#include "my_painter.h"
#include "my_const.h"

// libraries
#include "mzapo_parlcd.h"
#include "mzapo_regs.h"
#include "mzapo_phys.h"

// text
#include "font_types.h"


/**
 * TODO docu
 *
 * @param julia_set
 * @param positionX
 * @param positionY
 * @param const_real
 * @param const_imag
 */
void get_julia(uint16_t *julia_set,
               long double *positionX,
               long double *positionY,
               long double *const_real,
               long double *const_imag);

/**
 * TODO docu
 *
 * @param positionX
 * @param positionY
 * @param const_real
 * @param const_imag
 * @param red_click
 * @param green_click
 * @param blue_click
 * @param mutex
 * @param finished
 */
void draw_fractal(long double *positionX,
                  long double *positionY,
                  long double *const_real,
                  long double *const_imag,
                  int *red_click,
                  int *green_click,
                  int *blue_click,
                  std::mutex *mutex,
                  bool *finished);

/**
 * TODO docu
 *
 * @param data
 * @param c
 * @param row
 * @param column
 * @param color
 * @param background
 * @param scale
 */
void put_char_there(uint16_t data[HEIGHT*WIDTH],
                    char c,
                    int row,
                    int column,
                    uint16_t color,
                    uint16_t background,
                    int scale);

/**
 * TODO docu
 *
 * @param data
 * @param single
 * @param row
 * @param col
 * @param color
 * @param background
 * @param scale
 * @param size
 */
void write_to_data(uint16_t data[HEIGHT*WIDTH],
                   char *single,
                   int row,
                   int col,
                   uint16_t color,
                   uint16_t background,
                   int scale,
                   int size);

/**
 * TODO docu
 *
 * @param julia_set
 * @param real
 * @param imag
 * @param const_real
 * @param const_imag
 * @param i
 * @param j
 */
void point_iteration(uint16_t *julia_set,
                     long double real,
                     long double imag,
                     long double *const_real,
                     long double *const_imag,
                     int i,
                     int j);