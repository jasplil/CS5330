#include <iostream>
#include <opencv2/opencv.hpp>
#include "methods.hpp"
using namespace std;

int main(int argc, char *argv[]) {
  char srcPath[256];
  strcpy(srcPath, argv[1]);

  cv::Mat src = cv::imread(srcPath);

  // pre-processing the image
  cv::Mat blurredImg;
  cv::blur(src, blurredImg, cv::Size(5, 5));

  cv::Mat hsvImg;
  cv::cvtColor(blurredImg, hsvImg, cv::COLOR_BGR2HSV);
  for (int i = 0; i < hsvImg.rows; i++) {
    for (int j = 0; j < hsvImg.cols; j++) {
      cv::Vec3b pixel = hsvImg.at<cv::Vec3b>(i, j);
      int saturation = pixel[1];
      if (saturation > 100) {
        pixel[2] = pixel[2] * 0.5;
      }

      hsvImg.at<cv::Vec3b>(i, j) = pixel;
    }
  }

  cv::Mat darkerImg;
  cv::cvtColor(hsvImg, darkerImg, cv::COLOR_HSV2BGR);

  cv::Mat grayscaleImg;
  cv::cvtColor(darkerImg, grayscaleImg, cv::COLOR_BGR2GRAY);

  cv::Mat invertedImg;
  cv::bitwise_not(grayscaleImg, invertedImg);

  cv::Mat thresholdedImg;
  thresholding(invertedImg, thresholdedImg);

  cv::namedWindow("original", 1);
  cv::imshow("original", thresholdedImg);

  while (true) {
    int key = cv::waitKey(1);
    if (key == 'q' || key == 'Q') {
      break;
    }
  }

  return 0;
}