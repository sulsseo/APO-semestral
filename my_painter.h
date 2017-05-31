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
 * Main method for get new calculation of julia set.
 * Save new picture to given pointer for updated values.
 *
 * @param julia_set - pointer to data structure
 * @param positionX - center position X
 * @param positionY - center position Y
 * @param const_real - real part of c constant
 * @param const_imag - imaginary part of c constant
 */
void get_julia(uint16_t *julia_set,
               long double *positionX,
               long double *positionY,
               long double *const_real,
               long double *const_imag);

/**
 * Drawing thread work function, on changing position pointers repaint new picture.
 * Do some action on knob clicks.
 *
 * @param positionX
 * @param positionY
 * @param const_real
 * @param const_imag
 * @param red_click - state of red button (0 - inactive, >0 click)
 * @param green_click - state of green button
 * @param blue_click - state of blue button
 * @param mutex - locker
 * @param finished - closing variable
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
 * Put single char in data matrix.
 *
 * @param data - prepared data matrix
 * @param c - char to write
 * @param row - coord
 * @param column - coord
 * @param color - char color definition
 * @param background - background color definition
 * @param scale - char size 1 - small, 8 - bi
 */
void put_char_there(uint16_t data[HEIGHT*WIDTH],
                    char c,
                    int row,
                    int column,
                    uint16_t color,
                    uint16_t background,
                    int scale);

/**
 * Add text to given data matrix.
 *
 * @param data - prepared data matrix
 * @param single - char sequence to write
 * @param row - position coords
 * @param col - position coords
 * @param color - color for displayed text
 * @param background - text background
 * @param scale - regulation of text size 1 - small, 8 - big
 * @param size - text buffer size
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
 * Point iteration method for coloring given pixel based on given complex number.
 * Algoirthm given by computation of Julia set definition
 *
 * @param julia_set - pointer to data storage
 * @param real - real part of complex number
 * @param imag - imaginary part of complex number
 * @param const_real - real part of complex number for constant in juliaset
 * @param const_imag - imaginary part of complex number for constant in juliaset
 * @param i - y coord
 * @param j - x coord
 */
void point_iteration(uint16_t *julia_set,
                     long double real,
                     long double imag,
                     long double *const_real,
                     long double *const_imag,
                     int i,
                     int j);