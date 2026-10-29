#include "Image_Class.h"
using namespace std;

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
int main() {
    string sore ;
    cout << "enter image name" << endl;
    cin >> sore ;
    Image image(sore);

    purpleFilter(image);

    image.saveImage("purple.jpg");
    return 0;
            }

