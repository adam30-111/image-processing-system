#include <iostream>
#include <algorithm>
#include "Image_Class.h"
using namespace std;

void Resize(Image& img) {
    int newWidth = 0, newHeight = 0;

    while (newWidth <= 0 || newHeight <= 0) {
        cout << "enter width and height : \n";
        cin >> newWidth >> newHeight;
        if (cin.fail()) {       
            cin.clear();
            cin.ignore(1000, '\n');
            newWidth = newHeight = 0;
        }
        if (newWidth <= 0 || newHeight <= 0) cout << "Invalid size, try again.\n";
    }

    Image newImage(newWidth, newHeight);

    double widthRatio = (double)img.width / newWidth;
    double heightRatio = (double)img.height / newHeight;

    for (int y = 0; y < newHeight; y++) {
        int oldY = min((int)(heightRatio * y), img.height - 1);
        for (int x = 0; x < newWidth; x++) {
            int oldX = min((int)(widthRatio * x), img.width - 1);

            newImage(x, y, 0) = img(oldX, oldY, 0);
            newImage(x, y, 1) = img(oldX, oldY, 1);
            newImage(x, y, 2) = img(oldX, oldY, 2);
        }
    }

    img = newImage;
}