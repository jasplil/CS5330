#include <opencv2/opencv.hpp>
#include "filter.hpp"

int main(int argc, char *argv[]) {
    cv::VideoCapture *capdev;
    
    // open the video device
    capdev = new cv::VideoCapture(0);

    // check if video device can be opened with the given index
    if( !capdev->isOpened() ) {
      printf("Unable to open video device\n");
      return(-1);
    }
    
    // get the width/height and the frame rate of the video
    cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                  (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
                  
    printf("Expected size: %d %d\n", refS.width, refS.height);

    // create a window
    cv::namedWindow("Video", 1); // identifies a window

    cv::Mat frame;
    bool hasFilter = false;
    std::string currFilter = "none";

    for(;;) {
      *capdev >> frame; // get a new frame from the camera, treat as a stream

      if(frame.empty()) {
        printf("frame is empty\n");
        break;
      }

      // see if there is a waiting keystroke
      char key = cv::waitKey(10);

      if( key == 'q') {
        break;
      }

      if (key == 's' || key == 'S') {
          // Save the current frame to a file
          cv::imwrite("captured_image.jpg", frame);
          std::cout << "Image saved as captured_image.jpg" << std::endl;
      }
      
      cv::Mat keyFrame;

      keyFrame = cv::Mat::zeros(frame.size(), frame.type());
      frame.copyTo(keyFrame);

      // Call filter functions here
      if (hasFilter && currFilter == "greyscaleAvg") {
          cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
      }

      if (hasFilter && currFilter == "greyscaleAvg") {
          std::cout << "greyScale func called" << std::endl;
          greyscaleAvg(frame, keyFrame);
      }

      if (hasFilter && currFilter == "blur5x5") {
          std::cout << "blur func called" << std::endl;
          blur5x5(frame, keyFrame);
      }

      if (hasFilter && currFilter == "sobelX3x3") {
          cv::Mat sobelx;
          std::cout << "sobelX3x3 func called" << std::endl;
          sobelX3x3(frame, sobelx);
          cv::convertScaleAbs(sobelx, keyFrame);
      }

      if (hasFilter && currFilter == "sobelY3x3") {
          cv::Mat sobely;
          std::cout << "sobelY3x3 func called" << std::endl;
          sobelY3x3(frame, sobely);
          cv::convertScaleAbs(sobely, keyFrame);
      }

      if (hasFilter && currFilter == "magnitude") {
        cv::Mat sobelx;
        cv::Mat sobely;

        sobelX3x3(frame, sobelx);
        sobelY3x3(frame, sobely);
        magnitude(sobelx, sobely, keyFrame);
      }

      if (hasFilter && currFilter == "blurQuantize") {
        std::cout << "blurQuantize func called" << std::endl;
        blurQuantize(frame, keyFrame, 15);
      }

      if (hasFilter && currFilter == "cartoon") {
        std::cout << "cartoon func called" << std::endl;
        cartoon(frame, keyFrame, 15, 20);
      }

      if (key == 'a') {
        hasFilter = !hasFilter;
        currFilter = "greyscaleAvg";
      }

      if (key == 'b') {
        hasFilter = !hasFilter;
        currFilter = "blur5x5"; 
      }

      if (key == 'g') {
        hasFilter = !hasFilter;
        currFilter = "cvGreyScale";
      }

      if (key == 'x') {
        hasFilter = !hasFilter;
        currFilter = "sobelX3x3";
      }

      if (key == 'y') {
        hasFilter = !hasFilter;
        currFilter = "sobelX3x3";
      }

      if (key == 'm') {
        hasFilter = !hasFilter;
        currFilter = "magnitude";
      }

      if (key == 'l') {
        hasFilter = !hasFilter;
        currFilter = "blurQuantize";
      }

      if (key == 'c') {
        hasFilter = !hasFilter;
        currFilter = "cartoon";
      }

      cv::imshow("Video", keyFrame);
    }
}