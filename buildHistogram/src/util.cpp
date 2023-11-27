#include <algorithm>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>
#include "util.hpp"

int display5ImagesInARow(std::vector<std::string> imagePaths)
{
  // the vector to store the images
  std::vector<cv::Mat> imageVector;

  // read the images
  for (const auto &path : imagePaths)
  {
    // read the image
    cv::Mat image = cv::imread(path);

    // error checking
    if (image.data == NULL)
    {
      printf("error: unable to read image %s\n", path.c_str());
      return (-1);
    }

    // store the image
    imageVector.push_back(image);
  }

  // calculate the number of rows with 5 images in each row
  int numRows = (imageVector.size() + 4) / 5;
  // calculate the number of columns, 5 columns maximum
  int numCols = std::min((int)imageVector.size(), 5);

  // calculate canvas width, which is 2/5 of first image width times the number of columns
  int canvasWidth = imageVector[0].cols / 5 * 2 * numCols;
  // calculate canvas height, which is 2/5 of first image height times the number of rows
  int canvasHeight = imageVector[0].rows / 5 * 2 * numRows;
  // create the canvas
  cv::Mat canvas(canvasHeight, canvasWidth, imageVector[0].type(), cv::Scalar(255, 255, 255));

  // loop over all the images
  for (int i = 0; i < imageVector.size(); i++)
  {
    // calculate the row index
    int row = i / 5;
    // calculate the column index
    int col = i % 5;

    // the resized image
    cv::Mat resizedImage;
    // resize the image
    cv::resize(imageVector[i], resizedImage, cv::Size(canvasWidth / numCols, canvasHeight / numRows));
    // calculate the ROI
    cv::Rect roi(col * canvasWidth / numCols, row * canvasHeight / numRows, canvasWidth / numCols, canvasHeight / numRows);
    // copy the image to the ROI
    resizedImage.copyTo(canvas(roi));
  }

  // create a window
  char windowName[256];
  snprintf(windowName, sizeof(windowName), "Top %d images", (int)imageVector.size());
  cv::namedWindow(windowName, 1);

  // display the image
  cv::imshow(windowName, canvas);

  return (0);
}