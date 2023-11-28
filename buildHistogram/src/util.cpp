#include <algorithm>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>
#include "util.hpp"

int displayMultiImg(std::vector<std::string> imagePaths) {
  // the vector to store the images
  std::vector<cv::Mat> imageVector;

  // read the images
  for (const auto &path : imagePaths) {
    // read the image
    cv::Mat image = cv::imread(path);

    // error checking
    if (image.data == NULL) {
      printf("error: unable to read image %s\n", path.c_str());
      return (-1);
    }

    // store the image
    imageVector.push_back(image);
  }

  // calculate the number of rows, 5 rows maximum
  int m = (imageVector.size() + 4) / 5;
  int n = std::min((int)imageVector.size(), 5);

  // calculate canvas width, which is 2/5 of first image width times the number of columns
  int canvasWidth = imageVector[0].cols / 5 * 2 * n;
  int canvasHeight = imageVector[0].rows / 5 * 2 * m;
  // create the canvas
  cv::Mat canvas(canvasHeight, canvasWidth, imageVector[0].type(), cv::Scalar(255, 255, 255));

  for (int i = 0; i < imageVector.size(); i++) {
    int row = i / 5;
    int col = i % 5;

    cv::Mat resizedImage;
    cv::resize(imageVector[i], resizedImage, cv::Size(canvasWidth / n, canvasHeight / m));
    cv::Rect roi(col * canvasWidth / n, row * canvasHeight / m, canvasWidth / n, canvasHeight / m);
    resizedImage.copyTo(canvas(roi));
  }

  char windowName[256];
  snprintf(windowName, sizeof(windowName), "Top %d images", (int)imageVector.size());
  cv::namedWindow(windowName, 1);

  cv::imshow(windowName, canvas);

  return (0);
}

float histogramIntersection(cv::Mat const hist1, cv::Mat const hist2) {
  if (hist1.rows != hist2.rows || hist1.cols != hist2.cols) {
    printf("error: histograms have different dimensions\n");
    printf("hist1: %d x %d\n", hist1.rows, hist1.cols);
    printf("hist2: %d x %d\n", hist2.rows, hist2.cols);
    return (-1);
  }

  float intersection = 0.0;
  for (int i = 0; i < hist1.rows; i++) {
    const float *row1 = hist1.ptr<float>(i);
    const float *row2 = hist2.ptr<float>(i);
    for (int j = 0; j < hist1.cols; j++) {
      intersection += std::min(row1[j], row2[j]);
    }
  }
  
  return 1.0 - intersection;
}

float euclideanDistance(cv::Mat const hist1, cv::Mat const hist2) {
  float distance = 0.0;
  for (int i = 0; i < hist1.rows; i++) {
    const float *row1 = hist1.ptr<float>(i);
    const float *row2 = hist2.ptr<float>(i);
    for (int j = 0; j < hist1.cols; j++) {
      distance += (row1[j] - row2[j]) * (row1[j] - row2[j]);
    }
  }

  return std::sqrt(distance);
}

int calculateNormalizedHistogram(cv::Mat const target, cv::Mat &res, int histSize) {
  // error checking
  if (histSize < 1 || histSize > 256) {
    printf("out of bound\n");
    return (-1);
  }

  // initialize the histogram array to 0
  res = cv::Mat::zeros(cv::Size(histSize, histSize), CV_32FC1);
  int totalPixels = target.rows * target.cols;

  for (int i = 0; i < target.rows; i++) {
    // get the ith row of the feature vectors
    const cv::Vec3b *row = target.ptr<cv::Vec3b>(i);
    for (int j = 0; j < target.cols; j++) {
      // get each channel
      float B = row[j][0];
      float G = row[j][1];
      float R = row[j][2];

      // compute rg standard chromaticity
      float divisor = (R + G + B);
      divisor = divisor > 0.0 ? divisor : 1.0;
      float r = R / divisor;
      float g = G / divisor;

      // compute indexes, r, g are in [0, 1]
      int rIndex = (int)(r * (histSize - 1) + 0.5); // rounds to nearest value
      int gIndex = (int)(g * (histSize - 1) + 0.5);

      // increment the histogram
      res.at<float>(rIndex, gIndex)++;
    }
  }

  // normalize histogram
  res /= (totalPixels);

  return (0);
}

int calcRGBHistogram(cv::Mat const target, cv::Mat &targetHistogram, int histSize) {
  if (histSize < 1 || histSize > 256) {
    printf("error: out of bound\n");
    return (-1);
  }

  // allocate the histogram as 2D one channels floating point array
  // and initialzed to zero
  targetHistogram = cv::Mat::zeros(cv::Size(histSize * histSize * histSize, 1), CV_32FC1);

  // build the histogram
  for (int i = 0; i < target.rows; i++)
  {
    // get the ith row of the feature vectors
    const cv::Vec3b *row = target.ptr<cv::Vec3b>(i);

    for (int j = 0; j < target.cols; j++)
    {
      // get each channel
      float B = row[j][0];
      float G = row[j][1];
      float R = row[j][2];

      // compute indexes
      int bIndex = (int)(B / 255 * (histSize - 1) + 0.5); // rounds to nearest value
      int gIndex = (int)(G / 255 * (histSize - 1) + 0.5);
      int rIndex = (int)(R / 255 * (histSize - 1) + 0.5);

      // increment the histogram
      targetHistogram.at<float>(0, bIndex * histSize * histSize + gIndex * histSize + rIndex)++;
    }
  }

  // normalize histogram by divided by the number of pixels, which makes it a probability
  targetHistogram /= (target.rows * target.cols);

  return (0);
}