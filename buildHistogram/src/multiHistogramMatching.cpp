#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>
#include "matchMethods.hpp"
#include "util.hpp"

int topBtmRGBHistogramMatching(cv::Mat const target, std::vector<std::pair<std::string, cv::Mat>> const imageDatabase, std::vector<std::pair<std::string, float>> &distanceArr, int histSize) {
  // split the target image into top and bottom halves
  cv::Mat targetTop = target(cv::Rect(0, 0, target.cols, target.rows / 2));
  cv::Mat targetBtm = target(cv::Rect(0, target.rows / 2, target.cols, target.rows / 2));

  // get the RGB histogram of the top half of the target image
  cv::Mat targetHistogramTop;
  calcRGBHistogram(targetTop, targetHistogramTop, histSize);
  // get the RGB histogram of the bottom half of the target image
  cv::Mat targetHistogramBtm;
  calcRGBHistogram(targetBtm, targetHistogramBtm, histSize);

  // loop through all the images in the database
  for (int i = 0; i < imageDatabase.size(); i++) {
    // get the image
    cv::Mat image = imageDatabase[i].second;

    // split the image into top and bottom halves
    cv::Mat imageTop = image(cv::Rect(0, 0, image.cols, image.rows / 2));
    cv::Mat imageBtm = image(cv::Rect(0, image.rows / 2, image.cols, image.rows / 2));

    // get the RGB histogram of the top half of the image
    cv::Mat imageHistogramTop;
    calcRGBHistogram(imageTop, imageHistogramTop, histSize);
    // get the RGB histogram of the bottom half of the image
    cv::Mat imageHistogramBtm;
    calcRGBHistogram(imageBtm, imageHistogramBtm, histSize);

    // calculate the histogram distances of the two halves
    float distanceTop = histogramIntersection(targetHistogramTop, imageHistogramTop);
    float distanceBtm = histogramIntersection(targetHistogramBtm, imageHistogramBtm);

    // calculate the mean histogram distance
    float distance = distanceTop + distanceBtm;

    // store the filename along with the distance value
    distanceArr.push_back(std::make_pair(imageDatabase[i].first, distance));
  }

  return (0);
}