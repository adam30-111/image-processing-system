#include <iostream>
#include "Image_Class.h"
using namespace std;

void wanoSunlightFlow(Image& img) {
    int channel;
    
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            for (int c = 0; c < img.channels; c++) {

                if (c == 0) {
                    channel = img(x, y, c);
                    channel += 30;
                    if (channel > 255) 
                        channel = 255;
                    
                    img(x, y, c) = channel;
                }
                else if (c == 1) {
                    channel = img(x, y, c);
                    channel += 20;
                    if (channel > 255) channel = 255;
                    img(x, y, c) = channel;
                }
                else if (c == 2) { 
                    channel = img(x, y, c);
                    channel -= 10;
                    if (channel < 0) channel = 0;
                    img(x, y, c) = channel;
                }
            }
        }
    }
}