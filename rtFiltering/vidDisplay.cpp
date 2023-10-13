#include <opencv2/opencv.hpp>
// using namespace cv

int main(int argc, char *argv[]) {
    cv::VideoCapture *capdev;
    
    // open the video device
    capdev = new cv::VideoCapture(0);
    if( !capdev->isOpened() ) {
      printf("Unable to open video device\n");
      return(-1);
    }
    // get some properties of the image
    cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
                  (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
                  
    printf("Expected size: %d %d\n", refS.width, refS.height);
    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;
    bool isGray = false;

    for(;;) {
      *capdev >> frame; // get a new frame from the camera, treat as a stream

      if( frame.empty() ) {
        printf("frame is empty\n");
        break;
      }

      if (isGray) {
          // Convert the frame to grayscale
          cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
      }

      cv::imshow("Video", frame);

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

      if (key == 'g' || key == 'G') {
        isGray = !isGray;
      }
    }
}