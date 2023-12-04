#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>

int displayMultiImg(std::vector<std::string> imagePaths);
int calculateNormalizedHistogram(cv::Mat const target, cv::Mat &res, int histSize);
int calcRGBHistogram(cv::Mat const target, cv::Mat &res, int histSize);

float histogramIntersection(cv::Mat const histogram1, cv::Mat const histogram2);
float euclideanDistance(cv::Mat const histogram1, cv::Mat const histogram2);
