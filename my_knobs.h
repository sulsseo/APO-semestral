#pragma once
#include <mutex>

void handle_knobs(long double *positionX,
                  long double *positionY,
                  long double *constantImaginary,
                  int *red_click,
                  int *green_click,
                  int *blue_click,
                  std::mutex *mutex,
                  bool *finished);

void get_init_position();
