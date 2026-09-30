#include <iostream>
#include <algorithm>
#include "Image_Class.h"
using namespace std;

void adjustBrightness(Image& img) {
    int choice;
    cout << "option (1 to darken, 2 to lighten): ";
    cin >> choice;

    int percentage;
    cout << "Enter the percentage from 0 to 100: ";
    cin >> percentage;

    double fraction = percentage / 100.0;

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            unsigned char r = img.getPixel(x, y, 0);
            unsigned char g = img.getPixel(x, y, 1);
            unsigned char b = img.getPixel(x, y, 2);

            if (choice == 1) {
                r = r * (1 - fraction);
                g = g * (1 - fraction);
                b = b * (1 - fraction);
            } else if (choice == 2) {
                r = r + (255 - r) * fraction;
                g = g + (255 - g) * fraction;
                b = b + (255 - b) * fraction;
            }
            else {
                cout << "Invalid choice. Please enter 1 or 2." << endl;
                return;
            }
            
            img.setPixel(x, y, 0, r);
            img.setPixel(x, y, 1, g);
            img.setPixel(x, y, 2, b);
        }
    }
}