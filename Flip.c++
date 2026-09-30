#include<iostream>
#include"Image_Class.h"
using namespace std;

void flipImageFlow(Image& img, string& imageName)
{
    cout << "Choose flip direction:\n";
    cout << "Horizontally or Vertically (h/v): ";
    char flipDirection;
    cin >> flipDirection;
    cin.ignore();

    if (flipDirection == 'h' || flipDirection == 'H') {
        for (int i = 0; i < img.width / 2; i++)
            for (int j = 0; j < img.height; j++)
                for (int k = 0; k < img.channels; k++)
                    swap(img(i, j, k), img(img.width - 1 - i, j, k));
    }

    else if (flipDirection == 'v' || flipDirection == 'V') {
        for (int i = 0; i < img.width; i++)
            for (int j = 0; j < img.height / 2; j++)
                for (int k = 0; k < img.channels; k++)
                    swap(img(i, j, k), img(i, img.height - 1 - j, k));
    }
    else
        cout << "Invalid choice. Please try again.\n" << endl;
}