#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>
#include "matchMethods.hpp"
#include "util.hpp"

// chosse rg channels
// create and normalize the Histogram
// histogram intersection
int rgChromaticityMatching(cv::Mat const target, std::vector<std::pair<std::string, cv::Mat>> const imageDatabase, std::vector<std::pair<std::string, float>> &distanceArr, int histSize) {
  cv::Mat targetHistogram;
  calculateNormalizedHistogram(target, targetHistogram, histSize);

  for (int i = 0; i < imageDatabase.size(); i++) {
    cv::Mat imageHistogram;
    calculateNormalizedHistogram(imageDatabase[i].second, imageHistogram, 16);
    float distance = histogramIntersection(targetHistogram, imageHistogram);
    printf("distance: %f\n", distance);
    distanceArr.push_back(std::make_pair(imageDatabase[i].first, distance));
  }

  return (0);
}