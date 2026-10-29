#include<iostream>
#include"Image_Class.h"
using namespace std;

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
