#pragma once
#include <mutex>
#include "my_const.h"

/**
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