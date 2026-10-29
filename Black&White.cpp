#include <iostream>
#include <string>
#include "Image_Class.h"
using namespace std;

void applyBWFilter(Image& img, int threshold = 128) {
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < img.channels; ++k) {
                avg += img(i, j, k);
            }
            avg /= img.channels;

            unsigned char value = (avg >= threshold) ? 255 : 0;

            for (int k = 0; k < img.channels; ++k) {
                img(i, j, k) = value;
            }
        }
    }




