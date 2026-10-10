#include <iostream>
#include <algorithm>
#include "Image_Class.h"
using namespace std;
void oldTV (Image& img) {
    Image original = img;
    int shift = 3;
    for (int y = 0; y < img . height; y++) {
        float LineFactor = (y % 4 < 2) ? 1.0f : 0.5f ;
        for (int x = 0; x < img . width; x++) {
            int xr = max(0, x - shift);
            int xb = min(img . width - 1, x + shift);
            int noise = rand() % 31 - 15;

            int r = original(xr , y , 0);
            int g = original(x , y , 1);
            int b = original(xb , y , 2);

            r = (int)(r * LineFactor) + noise;
            g = (int)(g * LineFactor) + noise;
            b = (int)(b * LineFactor) + noise;

            img(x , y ,0) = min(255, max(0, r));
            img(x , y ,1) = min(255, max(0, g));
            img(x , y ,2) = min(255, max(0, b));
        }
    };

}
int main() {
    return 0;
}