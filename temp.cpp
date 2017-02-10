#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>     // std::string, std::to_string

using namespace cv;
using namespace std;

int INIT_Background(){
  
  /****** Local variables declaration *******/
  Mat frame;
  
  /****** Code *******/
  VideoCapture cap(1); 				// open the default camera
  if(!cap.isOpened())   			// check if we succeeded
    return -1;

  cap >> frame; 				// get a new frame from camera

  cv::imwrite( "./background.jpg", frame );    // Save background image 
  
  std::cout << "\n\n\n" << "***************************************\n" << "Press SPACE to start surveillance mode...\n"
  << "***************************************" << "\n\n\n" ;

  cv::namedWindow( "Background", WINDOW_AUTOSIZE );	// Create a window for display.
  cv::imshow( "Background", frame );                // Show our image inside it.

  cv::waitKey(0); 	

  cv::destroyAllWindows();

  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}



Mat capture_frame(){
  
  VideoCapture cap(1); 				// open the default camera
  if(!cap.isOpened()){ 				// check if we succeeded
  cv::namedWindow( "Error in capture_frame", WINDOW_AUTOSIZE );	// Create a window for display.
  }

  Mat frame;
  
  cap >> frame; 				// get a new frame from camera
 	
  // the camera will be deinitialized automatically in VideoCapture destructor
  return frame;
}



int process_frame(Mat frame, Mat background){

  /*************** Local variables declaration ***************/
  
  // Images init.
  Mat frame_diff;
  Mat frame_diff_grey;
  Mat frame_diff_grey2;
  Mat frame_bin;
  Mat frame_ok;
  Mat element;
  element = cv::getStructuringElement(cv::MORPH_RECT, Size(3,3), Point(-1,-1));

  // Parameters
  int blur_lenght = 5;
  int gaussian_size = 3;
  int white_value = 255;
  int threshold_value = 40;
  int morphology_iterations = 1;
  
  /***************** Image Processing **********/

  // Compute difference beetwen actual frame and background.
  cv::absdiff(frame,background,frame_diff);

  // RGB2GREY for simplicity and higher CPU speed.
  cv::cvtColor(frame_diff, frame_diff_grey, cv::COLOR_RGB2GRAY);  

  // [OPTIONAL] Low pass filter -> denoising 
  // cv::blur(frame_diff_grey,frame_diff_grey2,cv::Size(blur_lenght,blur_lenght));
  // [NOT STABLE] Adaptative filtering. Better in changing lighting conditions.
  // cv::adaptiveThreshold(diff_grey2, last, white_value, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, gaussian_size, 0);
  // Simple thresholding
  cv::threshold(frame_diff_grey, frame_bin, threshold_value, white_value, cv::THRESH_BINARY);

  // Denoising final frame with morphology -> Opening
  cv::morphologyEx(frame_bin, frame_ok, cv::MORPH_OPEN, element, Point(-1,-1), morphology_iterations, cv::BORDER_REFLECT);

  // Object detection
  int zeros = cv::countNonZero(frame_ok);

  /******* Step-by-Step processing image views (debugging purposes) *******/

  // cv::namedWindow( "Frame2", WINDOW_AUTOSIZE );	// Create a window for display.
  // cv::imshow( "Frame2", frame );                // Show our image inside it.
  // cv::waitKey(0); 

  
  // cv::namedWindow( "Frame_Diff", WINDOW_AUTOSIZE );	// Create a window for display.
  // cv::imshow( "Frame_Diff", frame_diff );                // Show our image inside it.
  // cv::waitKey(0); 

  
  // cv::namedWindow( "Diff_grey", WINDOW_AUTOSIZE );	// Create a window for display.
  // cv::imshow( "Diff_grey", frame_diff_grey );                // Show our image inside it.
  // cv::waitKey(0); 

  
  //cv::namedWindow( "Diff_grey2", WINDOW_AUTOSIZE );	// Create a window for display.
  //cv::imshow( "Diff_grey2", frame_diff_grey2 );                // Show our image inside it.
  //cv::waitKey(0);

  
  // cv::namedWindow( "last", WINDOW_AUTOSIZE );	// Create a window for display.
  // cv::imshow( "last", frame_bin );                // Show our image inside it.
  // cv::waitKey(0);

  
  // cv::namedWindow( "p", WINDOW_AUTOSIZE );	// Create a window for display.
  // cv::imshow( "p", frame_ok);                // Show our image inside it.
  // cv::waitKey(0);

  return zeros;
}

int main(int, char**){
  string str = "capture_";
  char i = '1';

  Mat frame, background;
  int aux,result, num_frame;
  num_frame = 1;
  aux = INIT_Background();
  if(aux != 0)
    std::cout << "ERROR, Camera not ready \n";
  else 
    std::cout << "OK, background saved \n";

  background = cv::imread("background.jpg");
  
  str.push_back(i);
  
  while(1){

    frame = capture_frame();
    result = process_frame(frame, background);
  
    // std::cout << result << "\n" ; debugging purpose
    //std::cout << str << "\n" ;     debugging purpose

    if(result > 100){
      cv::imwrite( str + ".png" , frame);    // Save frame image 

      str.erase(str.length() -1, 1);
      i ++;
      if(i == '9'){
        str.push_back('1');
        i = '0';
      }

      str.push_back(i);
    }
  }
}

