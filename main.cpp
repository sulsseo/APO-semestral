
#include <thread>
#include <mutex>
#include <iostream>

#include "my_painter.h"
#include "my_knobs.h"
//#include "my_const.h"

int main(int argc, char *argv[]) {

//    long double positionX = 0.0625;
//    long double positionY = -0.1875;

//    long double positionX = 1.3125;
//    long double positionY = -2;

    long double positionX = 0.0;
    long double positionY = 0.0;

    long double constantReal = -0.678;
    long double constantImaginary = 0.3125;

    /* button status */
    int red_click = 0;
    int green_click = 0;
    int blue_click = 0;

    // TODO init position
    get_init_position();

    bool finished = false;
    std::mutex mutex;

    std::thread knobs_thread = std::thread(handle_knobs, &positionX, &positionY, &constantImaginary, &red_click, &green_click, &blue_click, &mutex, &finished);
    std::thread drawing_thread(draw_fractal, &positionX, &positionY, &constantReal, &constantImaginary, &red_click, &green_click, &blue_click, &mutex, &finished);

    // close connection on hit some key
    getchar();

    finished = true;

    drawing_thread.join();
    knobs_thread.join();
}
