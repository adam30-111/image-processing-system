#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <cmath>
#include "Image_Class.h"
using namespace std;

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool hassuitableextension(string imageName)
{
    size_t dotPos = imageName.find_last_of('.');
    if (dotPos == string::npos) {
        return false;
    }
    string extension = imageName.substr(dotPos + 1);
    return (extension == "jpeg" || extension == "jpg" || extension == "bmp" || extension == "png" || extension == "tga");
}

void loadImageFlow(Image& img, string& imageName)
{
    while (true)
    {
        cout << "Please enter image name to load: ";
        getline(cin, imageName);
        if (!hassuitableextension(imageName)) {
            cout << "Invalid image extension. Please try again.\n" << endl;
            continue;
        }
        if (!fileExists(imageName)) {
            cout << "File not found. Please try again.\n" << endl;
            continue;
        }
        img.loadNewImage(imageName);
        break;
    }
}

void saveImageFlow(Image& img, string& imageName)
{
    string saveChoice;
                cout << "\n";
                cout << "Do you want to save it under a new name? (y/n): ";
                cin >> saveChoice;
                cin.ignore();

                if (saveChoice == "y" || saveChoice == "Y") {
                    while (true) {
                        string newImageName;
                        cout << "Enter new image name: ";
                        getline(cin, newImageName);
                        if (!hassuitableextension(newImageName)) {
                            cout << "Invalid image extension. Please try again.\n" << endl;
                            continue;
                        }
                        img.saveImage(newImageName);
                        imageName = newImageName;
                        break;
                    }
                } 
                else if (saveChoice == "n" || saveChoice == "N") {
                    img.saveImage(imageName);
                }
                else {
                    cout << "Invalid choice. Please try again.\n" << endl;
                }
}

void Grayscaleflow(Image& img)
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

void BlackandWhiteflow(Image& img) {
    int threshold = 128;
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < img.channels; ++k)
                avg += img(i, j, k);
            
            avg /= img.channels;

            unsigned char value = (avg >= threshold) ? 255 : 0;

            for (int k = 0; k < img.channels; ++k) {
                img(i, j, k) = value;
            }
        }
    }
}

void invertColorsflow(Image& img) {
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            unsigned char r = img.getPixel(x, y, 0);
            unsigned char g = img.getPixel(x, y, 1);
            unsigned char b = img.getPixel(x, y, 2);
            
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            img.setPixel(x, y, 0, r);
            img.setPixel(x, y, 1, g);
            img.setPixel(x, y, 2, b);
        }
    }
}

void addFrame(Image& img, int choice) {
    int w = img.width; 
    int h = img.height;
    int t = min(20, min(w, h) / 2);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            bool inX = (x < t || x >= w - t);
            bool inY = (y < t || y >= h - t);
            if (!inX && !inY) 
                continue;           

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

void addframeflow(Image& img) {
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
    addFrame(img, choice);
}

void flipImage(Image& img, bool horizontal)
{
    if (horizontal) {
        for (int i = 0; i < img.width / 2; i++)
            for (int j = 0; j < img.height; j++)
                for (int k = 0; k < img.channels; k++)
                    swap(img(i, j, k), img(img.width - 1 - i, j, k));
    }
    else {
        for (int i = 0; i < img.width; i++)
            for (int j = 0; j < img.height / 2; j++)
                for (int k = 0; k < img.channels; k++)
                    swap(img(i, j, k), img(i, img.height - 1 - j, k));
    }
}

void flipImageFlow(Image& img)
{
    cout << "Choose flip direction:\n";
    cout << "Horizontally or Vertically (h/v): ";
    string flipDirection;
    cin >> flipDirection;
    cin.ignore();

    if (flipDirection == "h" || flipDirection == "H")
        flipImage(img, true);
    else if (flipDirection == "v" || flipDirection == "V")
        flipImage(img, false);
    else
        cout << "Invalid choice. Please try again.\n" << endl;
}

void rotateImage(Image& img, int angle) {
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

void RotateImageFlow(Image& img) {
    int angle;
    cout << "Choose rotation angle (90 / 180 / 270): ";
    cin >> angle;

    while (angle != 90 && angle != 180 && angle != 270) {
        cout << "Invalid choice! Please enter 90, 180 or 270: ";
        cin >> angle;
    }
    rotateImage(img, angle);
}

void adjustBrightness(Image& img, bool lighten, int percentage) {
    double fraction = percentage / 100.0;

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            unsigned char r = img.getPixel(x, y, 0);
            unsigned char g = img.getPixel(x, y, 1);
            unsigned char b = img.getPixel(x, y, 2);

            if (!lighten) {
                r = r * (1 - fraction);
                g = g * (1 - fraction);
                b = b * (1 - fraction);
            } else {
                r = r + (255 - r) * fraction;
                g = g + (255 - g) * fraction;
                b = b + (255 - b) * fraction;
            }

            img.setPixel(x, y, 0, r);
            img.setPixel(x, y, 1, g);
            img.setPixel(x, y, 2, b);
        }
    }
}

void adjustBrightnessflow(Image& img) {
    int choice;
    cout << "option (1 to darken, 2 to lighten): ";
    cin >> choice;
    if (choice != 1 && choice != 2) {
        cout << "Invalid choice. Please enter 1 or 2." << endl;
        return;
    }

    int percentage;
    cout << "Enter the percentage from 0 to 100: ";
    cin >> percentage;
    if (percentage < 0 || percentage > 100) {
        cout << "Invalid percentage." << endl;
        return;
    }
    adjustBrightness(img, choice == 2, percentage);
}

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

void Resizeflow(Image& img) {
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
    resizeImage(img, newWidth, newHeight);
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

void mergeimagesflow(Image& img) {
    Image img2;
    string imageName2;

    cout << "Please enter the second image name to merge: ";
    loadImageFlow(img2, imageName2);

    bool scaleUp = true;
    if (img.width != img2.width || img.height != img2.height) {
        char mergeChoice;
        cout << "Do you want to scale up to the largest size, or crop to the common area?" << endl;
        cout << "1. Scale up" << endl;
        cout << "2. Crop" << endl;
        cout << "Enter your choice: ";
        cin >> mergeChoice;
        cin.ignore();

        if (mergeChoice == '1') scaleUp = true;
        else if (mergeChoice == '2') scaleUp = false;
        else {
            cout << "Invalid choice, merge cancelled" << endl;
            return;
        }
    }
    mergeImages(img, img2, scaleUp);
}

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
                    sx += p * Gx[b + 1][a + 1];
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

void cropImage(Image& img, int x, int y, int W, int H) {
    Image cropped(W, H);
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            for (int k = 0; k < 3; k++)
                cropped(i, j, k) = img(i + x, j + y, k);
        }
    }
    img = cropped;
}

Image cropFilter(Image& image) {
    int x, y, W, H;
    cout << "enter x,y,W,H" << endl;
    cin >> x >> y >> W >> H;
    while (x < 0 || y < 0 || W <= 0 || H <= 0 || x + W > image.width || y + H > image.height) {
        cout << "Invalid dimensions" << endl;
        cout << "enter x,y,W,H" << endl;
        cin >> x >> y >> W >> H;
    }
    Image result = image;
    cropImage(result, x, y, W, H);
    return result;
}

void blurWithLevel(Image& image, int level) {
    int w = image.width;
    int h = image.height;

    Image original = image;

    for (int x = level; x < w - level; x++) {
        for (int y = level; y < h - level; y++) {
            for (int c = 0; c < 3; c++) {

                int sum = 0;
                int count = 0;

                for (int i = -level; i <= level; i++) {
                    for (int j = -level; j <= level; j++) {

                        int value = original(x + i, y + j, c);
                        sum += value;
                        count++;
                    }
                }

                int average = sum / count;
                image(x, y, c) = average;
            }
        }
    }
}

void blurimages(Image& image) {
    int level;
    cout << "Enter blur level: ";
    cin >> level;
    if (level < 1) {
        cout << "Blur level must be 1 or more." << endl;
        return;
    }

    blurWithLevel(image, level);
}

void wanoSunlightFlow(Image& img) {
    int channel;
    
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            for (int c = 0; c < img.channels; c++) {

                if (c == 0) {
                    channel = img(x, y, c);
                    channel += 30;
                    if (channel > 255) 
                        channel = 255;
                    
                    img(x, y, c) = channel;
                }
                else if (c == 1) {
                    channel = img(x, y, c);
                    channel += 20;
                    if (channel > 255) channel = 255;
                    img(x, y, c) = channel;
                }
                else if (c == 2) { 
                    channel = img(x, y, c);
                    channel -= 10;
                    if (channel < 0) channel = 0;
                    img(x, y, c) = channel;
                }
            }
        }
    }
}

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
    }

}

void purpleFilter(Image& image) {

    for (int i=0; i<image.width; i++ ) {
        for (int j=0; j<image.height; j++ ) {
            int R = image(i, j, 0);
            int G = image(i, j, 1);
            int B = image(i, j, 2);

            R = R * 1.2;
            if (R > 255) R = 255;
            image(i, j, 0) = R;

            G = G * 0.8;
            image(i, j, 1) = G;


            B = B * 1.2;
            if (B > 255) B = 255;
            image(i, j, 2) = B;
        }

    }
}

void infrared(Image& image){
    int w=image.width;
    int h=image.height;

    for(int x=0;x<w;x++){
        for(int y=0;y<h;y++){

            int red = image(x,y,0);
            int green = image(x,y,1);
            int blue = image(x,y,2);

            red=red*2;

            if(red>255){
                red=255;
            }

            green=255-red;
            blue=255-red;

            image(x,y,0)=red;
            image(x,y,1)=green;
            image(x,y,2)=blue;
        }
    }
}

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

void oilPaint(Image& image) {
    const int radius = 3;
    const int levels = 20;
    Image result(image.width, image.height);

    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {

            int count[levels] = {0};
            int sumR[levels] = {0};
            int sumG[levels] = {0};
            int sumB[levels] = {0};

            for (int dx = -radius; dx <= radius; dx++) {
                for (int dy = -radius; dy <= radius; dy++) {
                    int nx = i + dx;
                    int ny = j + dy;

                    if (nx < 0 || ny < 0 || nx >= image.width || ny >= image.height)
                        continue;

                    int R = image(nx, ny, 0);
                    int G = image(nx, ny, 1);
                    int B = image(nx, ny, 2);

                    int intensity = (R + G + B) / 3;
                    int level = intensity * levels / 256;

                    count[level]++;
                    sumR[level] += R;
                    sumG[level] += G;
                    sumB[level] += B;
                }
            }
            int best = 0;
            for (int l = 1; l < levels; l++) {
                if (count[l] > count[best]) best = l;
            }
            result(i, j, 0) = sumR[best] / count[best];
            result(i, j, 1) = sumG[best] / count[best];
            result(i, j, 2) = sumB[best] / count[best];
        }
    }
    image = result;
}