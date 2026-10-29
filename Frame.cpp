#include <iostream>
#include <algorithm>
#include "Image_Class.h"
using namespace std;

void frame(Image& img) {
    cout<<"Choose frame type:\n";
    cout<<"1. Simple Frame\n";
    cout<<"2. Fancy Frame\n";
    
    int choice = 0;
    while (choice != 1 && choice != 2) {
        cout << "Enter your choice: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
        }
        if (choice != 1 && choice != 2)
            cout << "Invalid input, try again.\n";
    }

    int w = img.width; 
    int h = img.height;
    int t = min(20, min(w, h) / 2);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            bool inX = (x < t || x >= w - t);
            bool inY = (y < t || y >= h - t);
            if (!inX && !inY) continue;            
            int d = min(min(x, w - 1 - x), min(y, h - 1 - y));   
            int r = 255, g = 0, b = 0;             

            if (choice == 2) {
                if (d < t / 5 || d >= 9 * t / 10)           { r = 20;  g = 25;  b = 60;  } 
                else if (d < 2 * t / 5 || d >= 4 * t / 5)   { r = 212; g = 175; b = 55;  } 
                else if (inX && inY)                        { r = 30;  g = 80;  b = 200; } 
                else                                        { r = 190; g = 30;  b = 45;  }
            }
            img(x, y, 0) = r;
            img(x, y, 1) = g;
            img(x, y, 2) = b;
        }
    }
}
