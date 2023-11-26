#include <cstring>
#include <opencv2/opencv.hpp>
#include <utility>
#include <vector>

int baselineMatching(cv::Mat const target, std::vector<std::pair<std::string, cv::Mat>> const imageDatabase, std::vector<std::pair<std::string, float>> &distanceArr);