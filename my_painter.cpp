#include "my_painter.h"

const long double IMAGINARY_START = (JULIA_MAX * HEIGHT) / (WIDTH * -2.0);
const long double REAL_STEP = JULIA_MAX / WIDTH;

const long double IMAGINARY_STEP = (-2.0 * IMAGINARY_START) / HEIGHT;

long double previousX = 999.9, previousY = 999.9, previousImag = 999.9;

long double animation_real[] = {-0.4, 0.285, 0.285, 0.45, -0.70176, -0.835, -0.8, -0.7269, 0};
long double animation_imag[] = {0.6, 0, 0.01, 0.1428, -0.3842, -0.2321, 0.156, 0.1889, -0.8};


int displayInfo = FALSE;
int animation = FALSE;

void get_julia(uint16_t *julia_set,
               long double *positionX,
               long double *positionY,
               long double *const_real,
               long double *const_imag) {

    fprintf(stderr, "X= %Lf, Y= %Lf, c_real= %Lf, c_imag= %Lf\n", *positionX, *positionY, *const_real,
            *const_imag);

    long double imaginary;
    long double real;

    for (int i = 0; i < HEIGHT; i++) {
        imaginary = i * IMAGINARY_STEP + (*positionX - 2.0);


        for (int j = 0; j < WIDTH; j++) {
            real = j * REAL_STEP + (*positionY + IMAGINARY_START);

            point_iteration(julia_set, real, imaginary, const_real, const_imag, i, j);
        }
    }
}

void point_iteration(uint16_t *julia_set,
                     long double real,
                     long double imag,
                     long double *const_real,
                     long double *const_imag,
                     int i,
                     int j) {
    long double real_pow, imag_pow;
    double this_iterations;

    for (this_iterations = 0.0; this_iterations < JULIA_ITERATIONS; ++this_iterations) {
        real_pow = real * real;
        imag_pow = imag * imag;

        if (real_pow + imag_pow > JULIA_MAX) {
            break;
        }

        imag = 2.0 * real * imag + *const_imag;
        real = real_pow - imag_pow + *const_real;
    }

    double value = this_iterations / JULIA_ITERATIONS;

    // coloring inspiration https://cw.fel.cvut.cz/wiki/courses/b3b36prg/semestral-project/start
    unsigned char red = (unsigned char) (9.0 * (1.0 - value) * value * value * value * 255.0);
    unsigned char green = (unsigned char) (15.0 * (1.0 - value) * (1.0 - value) * value * value * 255.0);
    unsigned char blue = (unsigned char) (8.5 * (1.0 - value) * (1.0 - value) * (1.0 - value) * value * 255.0);

    // inspired by colleague
    julia_set[j + i * WIDTH] = (uint16_t) (((red & 0xf8) << 8) | ((green & 0xfc) << 3) | ((blue & 0xf8) >> 3));
}

void draw_fractal(long double *positionX,
                  long double *positionY,
                  long double *const_real,
                  long double *const_imag,
                  int *red_click,
                  int *green_click,
                  int *blue_click,
                  std::mutex *mutex,
                  bool *finished) {

    uint16_t julia_set[HEIGHT * WIDTH];

    // map adress
    unsigned char *parlcd_mem_base;
    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    parlcd_hx8357_init(parlcd_mem_base);

    std::unique_lock<std::mutex> thread_lock(*mutex);
    thread_lock.unlock();

    int index = 0;

    // working thread
    while (!*finished) {
        thread_lock.lock();

        if (!animation) {

            // julia painter
            if ((*red_click > 0) & !displayInfo) {
                displayInfo = TRUE;
                get_julia(julia_set, positionX, positionY, const_real, const_imag);

            } else if ((*red_click > 0) & displayInfo) {
                displayInfo = FALSE;
                get_julia(julia_set, positionX, positionY, const_real, const_imag);

            } else if ((*green_click > 0) & !animation) {
                animation = TRUE;
            }

            if (previousX != *positionX || previousY != *positionY || previousImag != *const_imag) {
                previousX = *positionX;
                previousY = *positionY;
                previousImag = *const_imag;
                get_julia(julia_set, positionX, positionY, const_real, const_imag);
            }

            if (displayInfo) {
                char string[256];
                int n = sprintf(string, "x=%Lf y=%Lf, c=%Lf", *positionX, *positionY, *const_imag);
                write_to_data(julia_set, string, 0, 0, 0xFF00, 0, 1, n);
            }

            draw_display(julia_set, parlcd_mem_base);

            thread_lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

        } else {
            if ((*green_click > 0)) {
                animation = FALSE;
                index = 0;
            }

            get_julia(julia_set, positionX, positionY, &animation_real[index], &animation_imag[index]);
            if (index != 8) {
                index++;
            } else {
                index = 0;
            }

            draw_display(julia_set, parlcd_mem_base);

            thread_lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void put_char_there(uint16_t data[HEIGHT * WIDTH],
                    char c,
                    int row,
                    int column,
                    uint16_t color,
                    uint16_t background,
                    int scale) {

    for (int i = 0; i < 16 * scale; i++) {
        for (int j = 0; j < 8 * scale; j++) {
            if (font_rom8x16.bits[(int) c * 16 + (i / scale)] >> (15 - (j / scale)) & 1) {
                data[(column * 8 + j) + (i + row * 16) * WIDTH] = color;
            }
        }
    }
}

void write_to_data(uint16_t data[HEIGHT * WIDTH],
                   char *single,
                   int row,
                   int col,
                   uint16_t color,
                   uint16_t background,
                   int scale,
                   int size) {
    char c;
    for (int i = 0; i < size; i++) {
        c = single[i];
        put_char_there(data, c, row, (col + i * scale), color, background, scale);
    }
}