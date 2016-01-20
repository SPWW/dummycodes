#ifndef _optical_flow_
#define _optical_flow_
#include <opencv2/opencv.hpp>
#include <iostream>
#include <functional>

class optical_flow{
protected:
  cv::VideoCapture input_video;
  int move_state;
  double distance_up_threshold = 2;
  double distance_low_threshold = 0.5;

  void switch_state(){
    move_state = move_state==1?0:1;
    return;
  }
public:
  optical_flow(cv::VideoCapture& video):input_video(video),move_state(0){
    if(!input_video.isOpened()){
      std::cout << "open video filed!" << std::endl;
    }
  }

  void detact_motion();
  virtual void detact_callback(int index,double distance) = 0;

  virtual ~optical_flow(){
    input_video.release();
  }
};
#endif
