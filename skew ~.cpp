#include <iostream>
#include <cmath>
#include "Image_Class.h"
using namespace std;

void skewImage(Image &img, double angleDeg) {
    const double PI = 3.14159265358979323846;
    double t = tan(angleDeg * PI / 180.0);

    int w = img.width, h = img.height;
    int newW = w + (int)round(h * t);

    Image result(newW, h);

    for (int x = 0; x < newW; x++)
        for (int y = 0; y < h; y++)
            for (int c = 0; c < 3; c++)
                result(x, y, c) = 255;

    for (int y = 0; y < h; y++) {
        int shift = (int)round((h - 1 - y) * t);
        for (int x = 0; x < w; x++)
            for (int c = 0; c < 3; c++)
                result(x + shift, y, c) = img(x, y, c);
    }

    img = result;
}

int main() {
    string file;
    double angle;

    cout << "Enter the file name: ";
    cin >> file;
    Image img(file);

    cout << "Enter skew angle(0 - 89): ";
    cin >> angle;

    skewImage(img, angle);

    img.saveImage("skewed_" + file);
    cout << "Saved as skewed_" << file << endl;
    return 0;
}