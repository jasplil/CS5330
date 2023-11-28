#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>

int displayMultiImg(std::vector<std::string> imagePaths);
float histogramIntersection(cv::Mat const histogram1, cv::Mat const histogram2);
int calculateNormalizedHistogram(cv::Mat const target, cv::Mat &res, int histSize);