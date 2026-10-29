#include <iostream>
#include "Image_Class.h"
using namespace std;

void resizeImage(Image& img, int newWidth, int newHeight) {
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

void mergeImages(Image& img, Image& img2, bool scaleUp) {
    if (img.width == img2.width && img.height == img2.height) {
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                for (int k = 0; k < img.channels; k++) {
                    img(i, j, k) = (img(i, j, k) + img2(i, j, k)) / 2;
                }
            }
        }
    }
    else if (scaleUp) {
        int targetWidth = max(img.width, img2.width);
        int targetHeight = max(img.height, img2.height);
        resizeImage(img, targetWidth, targetHeight);
        resizeImage(img2, targetWidth, targetHeight);

        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                for (int k = 0; k < img.channels; k++) {
                    img(i, j, k) = (img(i, j, k) + img2(i, j, k)) / 2;
                }
            }
        }
    }
    else {
        int targetWidth = min(img.width, img2.width);
        int targetHeight = min(img.height, img2.height);

        Image resultImage(targetWidth, targetHeight);
        for (int i = 0; i < targetWidth; i++) {
            for (int j = 0; j < targetHeight; j++) {
                for (int k = 0; k < img.channels; k++) {
                    resultImage(i, j, k) = (img(i, j, k) + img2(i, j, k)) / 2;
                }
            }
        }
        img = resultImage;
    }
}