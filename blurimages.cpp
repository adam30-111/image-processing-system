#include<iostream>
#include "Image_Class.h"
using namespace std;

void blurimages(Image& image){

    int level;
    cout<<"Enter blur level: ";
    cin>>level;

    int size=level*2+1;
    int w=image.width;
    int h=image.height;

    Image original=image;

    for(int x=level;x<w-level;x++){
        for(int y=level;y<h-level;y++){
            for(int c=0;c<3;c++){

                int sum=0;
                int count=0;

                for(int i=-level;i<=level;i++){
                    for(int j=-level;j<=level;j++){

                        int value=original(x+i,y+j,c);
                        sum+=value;
                        count++;
                    }
                }

                int average=sum/count;
                image(x,y,c)=average;
            }
        }
    }
}

