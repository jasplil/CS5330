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

  cv::Mat cleanedImage;
  cv::morphologyEx(thresholdedImg, cleanedImage, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

  cv::Mat labels, stats, centroids;
  int output = cv::connectedComponentsWithStats(cleanedImage, labels, stats, centroids, 8, CV_32S);

  std::vector<int> sortedLabels = std::vector<int>(stats.rows);
  for (int i = 0; i < sortedLabels.size(); ++i) {
    sortedLabels[i] = i;
  }

  int columnToSortBy = cv::CC_STAT_AREA;
  std::sort(sortedLabels.begin(), sortedLabels.end(), [&stats, columnToSortBy](int i, int j)
            { return stats.at<int>(i, columnToSortBy) > stats.at<int>(j, columnToSortBy); });

  for (int i = 0; i < 10; ++i) {
    std::cout << "label: " << sortedLabels[i] << ", area: " << stats.at<int>(sortedLabels[i], cv::CC_STAT_AREA) << std::endl;
  }

  cv::Mat dst = cv::Mat(labels.rows, labels.cols, CV_8UC3, cv::Scalar(0, 0, 0));
  for (int i = 0; i < dst.rows; ++i) {
    int const *labelsRowPtr = labels.ptr<int>(i);
    cv::Vec3b *segmentedImageRowPtr = dst.ptr<cv::Vec3b>(i);
    for (int j = 0; j < dst.cols; ++j) {
      int label = labelsRowPtr[j];
      if (label == 0)
        continue;
      if (stats.at<int>(label, cv::CC_STAT_AREA) >= 968)
        // segmentedImageRowPtr[j] = cv::Vec3b(255, 182, 193);
        segmentedImageRowPtr[j] = cv::Vec3b(255, 255, 255);
    }
  }


  cv::namedWindow("original", 1);
  cv::imshow("original", cleanedImage);

  while (true) {
    int key = cv::waitKey(1);
    if (key == 'q' || key == 'Q') {
      break;
    }
  }

  return 0;
}