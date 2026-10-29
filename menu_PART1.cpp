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
    char saveChoice;
                cout << "\n";
                cout << "Do you want to save it under a new name? (y/n): ";
                cin >> saveChoice;
                cin.ignore();

                if (saveChoice == 'y' || saveChoice == 'Y') {
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
                else if (saveChoice == 'n' || saveChoice == 'N') {
                    img.saveImage(imageName);
                }
                else {
                    cout << "Invalid choice. Please try again.\n" << endl;
                }
}

int main()
{
    int choice;
    char saveChoice;
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

                if (saveChoice == 'y' || saveChoice == 'Y') {
                    saveImageFlow(img, imageName);
                }
                else if (saveChoice == 'n' || saveChoice == 'N') {
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
                // Grayscale Conversion;
                break;
            
            case 2:
                // Black and White Conversion;
                break;
            case 3:
                // Invert Image;
                break;        
            case 4:
                // Adding a Frame to the Picture;
                break;

            case 5:
                // Flip Image;
                break;
            case 6:
                // Rotate Image;
                break;
            
            case 7:
                // Darken and Lighten Image;
                break;    
            
            case 8:
                // Resizing Images;
                break;

            case 99:
                cout << "Do you want to save before exiting? (y/n): ";
                cin >> saveChoice;
                cin.ignore();

                if (saveChoice == 'y' || saveChoice == 'Y') {
                    saveImageFlow(img, imageName);
                }
                else if (saveChoice == 'n' || saveChoice == 'N') {
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
