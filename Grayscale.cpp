#include<iostream>
#include"Image_Class.h"
using namespace std;

void Grayscaleflow(Image& img, string& imageName)
{
    for (int i = 0; i < img.width; i++)
        for (int j = 0; j < img.height; j++){
            unsigned int avg = 0;
            for (int k = 0; k < img.channels; k++)
                avg += img(i, j, k);

            avg = avg / 3;

            for (int k = 0; k < img.channels; k++)
                img(i, j, k) = avg;
        }      
}
