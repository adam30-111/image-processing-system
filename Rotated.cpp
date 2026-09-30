#include <iostream>
#include "Image_Class.h"
using namespace std;

void rotate(Image& img) {
    int angle;
    cout << "Choose rotation angle (90 / 180 / 270): ";
    cin >> angle;

    while (angle != 90 && angle != 180 && angle != 270) {
        cout << "Invalid choice! Please enter 90, 180 or 270: ";
        cin >> angle;
    }

    if (angle == 180) {
        Image rotated(img.width, img.height);
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                for (int c = 0; c < 3; c++) {
                    rotated(img.width - 1 - i, img.height - 1 - j, c) = img(i, j, c);
                }
            }
        }
        img = rotated;
    }
    else {
        Image rotated(img.height, img.width);
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                for (int c = 0; c < 3; c++) {
                    if (angle == 90) {
                        rotated(img.height - 1 - j, i, c) = img(i, j, c);
                    }
                    else {
                        rotated(j, img.width - 1 - i, c) = img(i, j, c);
                    }
                }
            }
        }
        img = rotated;

}


}