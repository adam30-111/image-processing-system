#include <iostream>
#include <string>
#include <cmath>
#include "Image_Class.h"
using namespace std;

void detectEdges(Image &img, int threshold = 150) {
    int width = img.width;
    int height = img.height;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int avg = 0;
            for (int k = 0; k < 3; k++) avg += img(i, j, k);
            avg /= 3;
            for (int k = 0; k < 3; k++) img(i, j, k) = avg;
        }
    }

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    Image result(width, height);
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            for (int k = 0; k < 3; k++)
                result(i, j, k) = 255;


    for (int i = 1; i < width - 1; i++) {
        for (int j = 1; j < height - 1; j++) {
            int sx = 0, sy = 0;
            for (int a = -1; a <= 1; a++) {
                for (int b = -1; b <= 1; b++) {
                    int p = img(i + a, j + b, 0);
                    sx += p * Gx[b + 1][a + 1];  // row = y offset, col = x offset
                    sy += p * Gy[b + 1][a + 1];
                }
            }
            double mag = sqrt(sx * sx + sy * sy);
            int val = (mag > threshold) ? 0 : 255;
            for (int k = 0; k < 3; k++) result(i, j, k) = val;
        }
    }

    img = result;
}

int main() {
    int threshold = 150;
    string name;
    cout << "Enter image name ('luffy.jpg') ";
    cin >> name;

    Image img(name);
    detectEdges(img, threshold);

    img.saveImage("edges_" + name);
    cout << "Saved as edges1_" << name << endl;
    return 0;
}
