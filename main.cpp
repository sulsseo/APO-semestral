#include <thread>
#include <mutex>
#include <iostream>

#include "my_painter.h"
#include "my_knobs.h"

int main(int argc, char *argv[]) {
    /* init position */
    long double positionX = 0.0;
    long double positionY = 0.0;

    /* C complex definition */
    long double constant_real = -0.678;
    long double constant_imaginary = 0.3125;

//    long double constant_real = 0.285;
//    long double constant_imaginary = 0.01;

    /* button status */
    int red_click = 0;
    int green_click = 0;
    int blue_click = 0;

    get_init_position();

    bool finished = false;
    std::mutex mutex;

    std::thread knobs_thread = std::thread(handle_knobs, &positionX, &positionY, &constant_imaginary, &red_click, &green_click, &blue_click, &mutex, &finished);
    std::thread drawing_thread(draw_fractal, &positionX, &positionY, &constant_real, &constant_imaginary, &red_click, &green_click, &blue_click, &mutex, &finished);

    // close connection on hit some key
    getchar();

    finished = true;

    drawing_thread.join();
    knobs_thread.join();
}
