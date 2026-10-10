#include "Image_Class.h"
using namespace std;

Image cropFilter(Image& image) {

    int x,y,W,H ;
    cout << "enter x,y,W,H" << endl;
    cin >> x >> y >> W >> H ;
    while (x < 0 || y < 0 || W <= 0 || H <= 0  || x+W > image.width || y+H > image.height) {
        cout << "Invalid dimensions" << endl;
        cout << "enter x,y,W,H" << endl;
        cin >> x >> y >> W >> H ;
    }
    Image cropped(W,H);
    for (int i=0; i<W; i++ ) {
        for (int j=0; j<H; j++ ) {
            for (int k=0; k < 3; k++)
                cropped( i, j , k) = image(i + x , j + y , k);
        }
    }
    return cropped;
}
int main() {
    string sore ;
    cout << "enter image name" << endl;
    cin >> sore ;
    Image image(sore);
    cout << image.width << " " << image.height << endl;
    Image cropped = cropFilter(image);
    string outName;
    cout << "enter new image name" << endl;
    cin >> outName;

    cropped.saveImage(outName);

    return 0  ;
}
