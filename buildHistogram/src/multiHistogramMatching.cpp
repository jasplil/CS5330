#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>
#include "matchMethods.hpp"
#include "util.hpp"

int topBtmCntRGBHistogramMatching(cv::Mat const target, std::vector<std::pair<std::string, cv::Mat>> const imageDatabase, std::vector<std::pair<std::string, float>> &distanceArr, int histSize) {
  // get the top and bottom halves and the center of the target image
  cv::Mat targetTop = target(cv::Rect(0, 0, target.cols, target.rows / 2));
  cv::Mat targetBtm = target(cv::Rect(0, target.rows / 2, target.cols, target.rows / 2));
  cv::Mat targetCnt = target(cv::Rect(target.cols * 3 / 8, target.rows * 3 / 8, target.cols / 4, target.rows / 4));

  // get the RGB histogram of the top half of the target image
  cv::Mat targetHistogramTop;
  getRGBHistogram(targetTop, targetHistogramTop, histSize);
  // get the RGB histogram of the bottom half of the target image
  cv::Mat targetHistogramBtm;
  getRGBHistogram(targetBtm, targetHistogramBtm, histSize);
  // get the RGB histogram of the center of the target image
  cv::Mat targetHistogramCnt;
  getRGBHistogram(targetCnt, targetHistogramCnt, histSize);

  // loop through all the images in the database
  for (int i = 0; i < imageDatabase.size(); i++)
  {
    // get the image
    cv::Mat image = imageDatabase[i].second;

    // get the top and bottom halves and the center of the target image
    cv::Mat imageTop = image(cv::Rect(0, 0, image.cols, image.rows / 2));
    cv::Mat imageBtm = image(cv::Rect(0, image.rows / 2, image.cols, image.rows / 2));
    cv::Mat imageCnt = image(cv::Rect(image.cols * 3 / 8, image.rows * 3 / 8, image.cols / 4, image.rows / 4));

    // get the RGB histogram of the top half of the image
    cv::Mat imageHistogramTop;
    getRGBHistogram(imageTop, imageHistogramTop, histSize);
    // get the RGB histogram of the bottom half of the image
    cv::Mat imageHistogramBtm;
    getRGBHistogram(imageBtm, imageHistogramBtm, histSize);
    // get the RGB histogram of the center of the image
    cv::Mat imageHistogramCnt;
    getRGBHistogram(imageCnt, imageHistogramCnt, histSize);

    // calculate the histogram distances of the three sections
    float distanceTop = euclideanDistance(targetHistogramTop, imageHistogramTop);
    float distanceBtm = euclideanDistance(targetHistogramBtm, imageHistogramBtm);
    float distanceCnt = euclideanDistance(targetHistogramCnt, imageHistogramCnt);

    // calculate the weighted mean histogram distance
    float distance = distanceTop + distanceBtm + 2.0 * distanceCnt;

    // store the filename along with the distance value
    distanceArr.push_back(std::make_pair(imageDatabase[i].first, distance));
  }

  return (0);
}