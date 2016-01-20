#include "of.h"
using namespace cv;
using namespace std;

class detacter:public optical_flow{
public:
    detacter(VideoCapture& video):optical_flow(video){
    }

    void detact_callback(int index,double distance){

        cout << "moving state change. time:" <<index <<" distance: "<< distance<< endl;
        return;
    }

};

int main(int argc,char* argv[]){
    VideoCapture cap(argv[1]);

    detacter d(cap);
    d.detact_motion();
    return 0;
}
