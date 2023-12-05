#include "methods.hpp"
#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

int thresholding(const cv::Mat &src, cv::Mat &dst) {
  if (src.data == NULL) {
    printf("error: thresholding() src image is empty\n");
    return (-1);
  }

  // randomly sample a subset of the pixel values from the image
  cv::Mat img = src.clone();
  img.reshape(1, 1);
  cv::randShuffle(img);
  cv::Mat shuffled = img(cv::Rect(0, 0, img.cols / 4, img.rows));

  // convert the vector to CV_32F
  cv::Mat samples(shuffled);
  samples.convertTo(samples, CV_32F);

  // implement k-means algorithm
  // calculate the threshold as the mean of the two cluster centers.
  cv::Mat labels;
  cv::Mat centers;
  int clusterCount = 2;
  int attempts = 3;

  cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 100, 0.01);
  cv::kmeans(samples, clusterCount, labels, criteria, attempts, cv::KMEANS_PP_CENTERS, centers);
  float threshold = (centers.at<float>(0) + centers.at<float>(1)) / 2.0f;

  if (threshold < 0 || threshold > 255) {
    printf("threshold is out of range\n");
    return (-4);
  }

  dst = cv::Mat(src.rows, src.cols, CV_8UC1);

  // apply the thresholding
  for (int i = 0; i < src.rows; i++) {
    const uchar *sptr = src.ptr<uchar>(i);
    uchar *dptr = dst.ptr<uchar>(i);
    for (int j = 0; j < src.cols; j++) {
      if (sptr[j] > threshold)
        dptr[j] = 255;
      else
        dptr[j] = 0;
    }
  }

  return (0);
}