#include "of.h"
#include <string>
#include <vector>
using namespace std;
using namespace cv;

void optical_flow::detact_motion(){
  Mat imgA,imgB;

  //read first frame into image A
  input_video.read(imgA);
  //convert image to gray
  cvtColor(imgA,imgA,CV_BGR2GRAY);
  cv::Rect myROI(50, 140, 670, 230);
  imgA = imgA(myROI);
  int i = 0;
  int last_corners = 0;
  while(input_video.read(imgB)){
      input_video.grab();
      input_video.grab();
      input_video.grab();
      input_video.grab();
    Mat imgx(imgB);
    cvtColor(imgB,imgB,CV_BGR2GRAY);
    imgB = imgB(myROI);
    imgx = imgx(myROI);

    int win_size = 5;
    int maxCorners = 20;
    double qualityLevel = 0.01;
    double minDistance = 0.5;

    std::vector<cv::Point2f> cornersA;
    cornersA.reserve(maxCorners);
  	std::vector<cv::Point2f> cornersB;
    cornersB.reserve(maxCorners);



  	//Determines strong corners on an image.
  	goodFeaturesToTrack( imgA,cornersA,maxCorners,qualityLevel,minDistance);
  	goodFeaturesToTrack( imgB,cornersB,maxCorners,qualityLevel,minDistance);

  	//Refines the corner locations.
  	cornerSubPix( imgA, cornersA, Size( win_size, win_size ), Size( -1, -1 ),
  				  TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03 ) );

  	cornerSubPix( imgB, cornersB, Size( win_size, win_size ), Size( -1, -1 ),
  				  TermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03 ) );

  	// Call Lucas Kanade algorithm

  	std::vector<uchar> features_found;
    features_found.reserve(maxCorners);
  	std::vector<float> feature_errors;
  	feature_errors.reserve(maxCorners);

  	calcOpticalFlowPyrLK( imgA, imgB, cornersA, cornersB, features_found, feature_errors ,
		Size( win_size, win_size ), 5,
		cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3 ), 0 );


    double max_distance = 0;
    std::vector<double> vd;
    for(int i=0;i<features_found.size();++i){
      if(features_found[i]==0)continue;
  		//cout<<"Got it"<<endl;
  		Point p0( ceil( cornersA[i].x ), ceil( cornersA[i].y ) );
  		Point p1( ceil( cornersB[i].x ), ceil( cornersB[i].y ) );

      double distance = norm(p0-p1);
      //max_distance = max_distance>distance?max_distance:distance;
      if(distance <100){vd.push_back(distance);
  			line( imgx, p0, p1, CV_RGB(0,255,0), 2 );
			circle( imgx, p0, 2, CV_RGB(255,0,0), 5 );
			circle( imgx, p1, 2, CV_RGB(0,0,255), 5 );
        }
    }
    for(auto each:vd)max_distance+=each;
    if(vd.size()!=0)max_distance = max_distance/vd.size();
    //std::cout << "distance per frame:"<< max_distance << std::endl;
    int diff = vd.size()>last_corners?vd.size()-last_corners:last_corners-vd.size();
    if(diff<5 && vd.size()>10){
        if(move_state == 0 && max_distance>distance_up_threshold){
          switch_state();
          detact_callback(i,max_distance);

          	vector<int> compression_params;
            compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
            compression_params.push_back(9);
            string name("optical_flow_");
        	name+=to_string(i);
        	name+=".png";
        	cout << name << " " << vd.size()<< endl;
        	imwrite(name, imgx, compression_params);

        }else if(move_state==1 && max_distance<distance_low_threshold){
          switch_state();
          detact_callback(i,max_distance);

        	vector<int> compression_params;
            compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
            compression_params.push_back(9);
            string name("optical_flow_");
        	name+=to_string(i);
        	name+=".png";
        	cout << name << " " << vd.size()<< endl;
        	imwrite(name, imgx, compression_params);

        }
    }
    last_corners = vd.size();
    imgA = imgB;
    ++i;
  }





}
