#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include "filter.h"

/*
  Function that calculates the horizontal gradient
  src is the input image
  dst is the output image, allocated by the function
*/

int gradx( const cv::Mat &src, cv::Mat &dst ) {

  // allocate the dst image
  // bits-Type-C-#channels
  // 8UC1  8-bit unsigned char with 1 channel (greyscale image)
  // 8UC3  8-bit unsigned char with 3 channels (BGR/RGB image)
  // 16SC1  16-bit signed short with 1 channel
  // 16SC3  16-bit signed short with 3 channels (-32k to 32k)
  // 32FC3  32-bit floating point value with 3 channels
  dst = cv::Mat::zeros( src.size(), CV_16SC3 );

  // execute the filter: horizontal Sobel  (X-Sobel)
  // -1 0 1
  // -2 0 2
  // -1 0 1
  for(int i=1;i<src.rows-1;i++) {

    // set up 3 row points (above, middle, below)
    const cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i-1);
    const cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
    const cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i+1);

    // destination pointer, which is type short Vec3s
    cv::Vec3s *dptr = dst.ptr<cv::Vec3s>(i);

    for(int j=1;j<src.cols-1;j++) {

      // over all color channels
      for(int c=0;c<2;c++) {

	dptr[j][c] = ( -1 * (short)rptrm1[j-1][c] + 1 * rptrm1[j+1][c] +
		       -2 * rptr[j-1][c]   + 2 * rptr[j+1][c]   +
		       -1 * rptrp1[j-1][c] + 1 * rptrp1[j+1][c] ) / 4;
	// divide by 4 to normalize back to [-255,255]
      }
    }
  }
    

  return(0);
}
