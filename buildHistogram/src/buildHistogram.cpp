/*
  Bruce Maxwell
  CS 5330 F23
  Building and viewing a rg histogram
*/

#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {

  cv::Mat src; // input image
  cv::Mat dst; // view image
  cv::Mat hist; // histogram

  char filename[256];
  float max;
  const int histsize = 256;

  // error checking
  if(argc < 2) {
    printf("usage: %s <image filename>\n", argv[0]);
    return(-1);
  }

  // grab the filename
  strcpy( filename, argv[1] );

  // read the file
  src = cv::imread( filename );

  if( src.data == NULL ) {
    printf("error: unable to read image %s\n", filename);
    return(-2);
  }

  // build the histogram
  // allocate the histogram, 2D single channel floating point array
  // initialied to zero
  hist = cv::Mat::zeros( cv::Size( histsize, histsize ), CV_32FC1 );

  // build the histogram
  max = 0; // keep track of the largest bucket
  for( int i=0;i<src.rows;i++) {
    for(int j=0;j<src.cols;j++) {

      // grab the data
      float B = src.at<cv::Vec3b>(i, j)[0];
      float G = src.at<cv::Vec3b>(i, j)[1];
      float R = src.at<cv::Vec3b>(i, j)[2];

      // compute rg standard chromaticity
      float divisor = (R + G + B);
      divisor = divisor > 0.0 ? divisor : 1.0;
      float r = R / (R + G + B);
      float g = G / (R + G + B);

      // compute indexes, r, g are in [0, 1]
      int rindex = (int)( r * (histsize-1) + 0.5); // rounds to nearest value
      int gindex = (int)( g * (histsize-1) + 0.5);

      // increment the histogram
      hist.at<float>(rindex, gindex)++;
      float newvalue = hist.at<float>(rindex, gindex);

      // conditional assignment
      max = newvalue > max ? newvalue : max;
    }
  }

  // histogram is complete but not normalized
  // hist /= max; // divides the whole Mat by max, which is one method of normalization
  hist /= (src.rows * src.cols); // divides by the number of pixels, which makes it a probability 

  // histogram is all in the range [0, 1]

  dst.create( hist.size(), CV_8UC3 );

  // loop over histogram
  for(int i=0;i<hist.rows;i++) {
    for(int j=0;j<hist.cols;j++) {
      if( i + j > hist.rows ) {
	dst.at<cv::Vec3b>(i, j) = cv::Vec3b( 200, 120, 60 );
      }
      else {
	float rcolor = (float)i / histsize;
	float gcolor = (float)j / histsize;
	float bcolor = (float)((histsize) - i - j) / histsize;
	dst.at<cv::Vec3b>(i, j)[0] = hist.at<float>(i, j) > 0 ? (unsigned char)((hist.at<float>(i, j) * 128 + 127))*bcolor : 0;
	dst.at<cv::Vec3b>(i, j)[1] = hist.at<float>(i, j) > 0 ? (unsigned char)((hist.at<float>(i, j) * 128 + 127))*gcolor : 0;
	dst.at<cv::Vec3b>(i, j)[2] = hist.at<float>(i, j) > 0 ? (unsigned char)((hist.at<float>(i, j) * 128 + 127))*rcolor : 0;
      }
    }
  }

  // write the image to a file
  cv::imwrite("histogram.png", dst);

  return(0);
}
