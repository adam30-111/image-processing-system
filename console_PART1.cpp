#include <iostream>
#include <algorithm>
#include <fstream>
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

void BlackandWhiteflow(Image& img, string& imageName) {
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

void invertColorsflow(Image& img, string& imageName) {
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

void addframeflow(Image& img, string& imageName) {
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

void flipImageFlow(Image& img, string& imageName)
{
    cout << "Choose flip direction:\n";
    cout << "Horizontally or Vertically (h/v): ";
    string flipDirection;
    cin >> flipDirection;
    cin.ignore();

    if (flipDirection == "h" || flipDirection == "H") {
        for (int i = 0; i < img.width / 2; i++)
            for (int j = 0; j < img.height; j++)
                for (int k = 0; k < img.channels; k++)
                    swap(img(i, j, k), img(img.width - 1 - i, j, k));
    }

    else if (flipDirection == "v" || flipDirection == "V") {
        for (int i = 0; i < img.width; i++)
            for (int j = 0; j < img.height / 2; j++)
                for (int k = 0; k < img.channels; k++)
                    swap(img(i, j, k), img(i, img.height - 1 - j, k));
    }
    else
        cout << "Invalid choice. Please try again.\n" << endl;
}

void RotateImageFlow(Image& img, string& imageName) {
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

void adjustBrightnessflow(Image& img, string& imageName) {
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

void Resizeflow(Image& img, string& imageName) {
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

int main()
{
    int choice;
    string saveChoice;
    string imageName;
    Image img;

    loadImageFlow(img, imageName);
    do
    {
        cout << "=== Image Processing ===" << endl;
        cout << "\n";
        cout << "22. Load New Image" << endl;
        cout << "0. Save Image" << endl;
        cout << "\n";
        cout << "============================" << endl;
        cout << "Filter Options:" << endl;
        cout << "1. Grayscale Conversion" << endl;
        cout << "2. Black and White Conversion" << endl;
        cout << "3. Invert Image" << endl;
        cout << "4. Adding a Frame to the Picture" << endl;
        cout << "5. Flip Image" << endl;
        cout << "6. Rotate Image" << endl;
        cout << "7. Darken and Lighten Image" << endl;
        cout << "8. Resizing Images" << endl;
        cout << "============================" << endl;
        cout << "99. Exit" << endl;
        cout << "\n";

        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
            case 22:
                cout << "Do you want to save the current image first? (y/n): ";
                cin >> saveChoice;
                cin.ignore();

                if (saveChoice == "y" || saveChoice == "Y") {
                    saveImageFlow(img, imageName);
                }
                else if (saveChoice == "n" || saveChoice == "N") {
                    loadImageFlow(img, imageName);
                }
                else {
                    cout << "Invalid choice. Please try again.\n" << endl;
                    break;
                }
                break;

            case 0:
                saveImageFlow(img, imageName);
                break;

            case 1:
                Grayscaleflow(img, imageName);
                break;
            
            case 2:
                BlackandWhiteflow(img, imageName);
                break;

            case 3:
                invertColorsflow(img, imageName);
                break;

            case 4:
                addframeflow(img, imageName);
                break;

            case 5:
                flipImageFlow(img, imageName);
                break;

            case 6:
                RotateImageFlow(img, imageName);
                break;
            
            case 7:
                adjustBrightnessflow(img, imageName);
                break;    
            
            case 8:
                Resizeflow(img, imageName);
                break;

            case 99:
                cout << "Do you want to save before exiting? (y/n): ";
                cin >> saveChoice;
                cin.ignore();

                if (saveChoice == "y" || saveChoice == "Y") {
                    saveImageFlow(img, imageName);
                }
                else if (saveChoice == "n" || saveChoice == "N") {
                    cout << "Exiting without saving." << endl;
                }
                else {
                    cout << "Invalid choice. Please try again.\n" << endl;
                    break;
                }
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n" << endl;
        }
    } while (choice != 99);
}
