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
    bool isGray = false;

    for(;;) {
      *capdev >> frame; // get a new frame from the camera, treat as a stream

      if(frame.empty()) {
        printf("frame is empty\n");
        break;
      }

      // if (isGray) {
      //     // Convert the frame to grayscale
      //     cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
      // }

      // cv::imshow("Video", frame);

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

      // if (key == 'g' || key == 'G') {
      //   isGray = !isGray;
      // }
      cv::Mat keyFrame;

      keyFrame = cv::Mat::zeros(frame.size(), frame.type());
      frame.copyTo(keyFrame);

      if (isGray) {
          // std::cout << "greyScale func called" << std::endl;
          // greyscaleAvg(frame, keyFrame);
          std::cout << "blur func called" << std::endl;
          gaussianFilter(frame, keyFrame);
      }

      if (key == 'a') {
        // isGray = !isGray;
      }

      if (key == 'b') {
        isGray = !isGray;
      }

      cv::imshow("Video", keyFrame);
    }
}