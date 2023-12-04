#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include "filter.hpp"

int greyscaleAvg(const cv::Mat &src, cv::Mat &dst) {
  dst = cv::Mat::zeros(src.size(), CV_8UC3);
  int m = src.rows;
  int n = src.cols;

  for (int i = 0; i < m; i++) {
    const cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
    cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);
    for (int j = 0; j < n; j++) {
      for (int c = 0; c < 3; c++) {
        dptr[j][c] = std::round((rptr[j][0] + rptr[j][1] + rptr[j][2]) / 3);
      }
    }
  }

  return (0);
}

int blur5x5(const cv::Mat &src, cv::Mat &dst) {
  cv::Mat temp = cv::Mat::zeros(src.size(), CV_8UC3);
  dst = cv::Mat::zeros(src.size(), CV_8UC3);

  // 1D Gaussian kernel
  int kernel[5] = {1, 2, 4, 2, 1};
  int kernelSum = 10;
  int m = src.rows;
  int n = src.cols;

  // Horizontal pass
  for (int i = 0; i < m; i++) {
    cv::Vec3b *tptr = temp.ptr<cv::Vec3b>(i);
    const cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
    // Ignoring filter edge effects
    for (int j = 2; j < n-2; j++) {
        for (int c = 0; c < 3; c++) {
            tptr[j][c] = (rptr[j - 2][c] + rptr[j - 1][c] * 2 + rptr[j][c] * 4 + rptr[j + 1][c] * 2 + rptr[j + 2][c]) / kernelSum;
        }
    }

    for (int j = 0; j < 3; j++) {
      tptr[0][j] = tptr[2][j];
      tptr[1][j] = tptr[2][j];
      tptr[n - 1][j] = tptr[n - 3][j];
      tptr[n - 2][j] = tptr[n - 3][j];
    }
  }

  // Vertical pass
  for (int i = 2; i < m-2; i++) {
    cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);
    // Access rows
    const cv::Vec3b *rptr = temp.ptr<cv::Vec3b>(i - 2);
    for (int j = 0; j < n; j++) {
        for (int c = 0; c < 3; c++) {
            dptr[j][c] = (rptr[j][c] + rptr[n + j][c] * 2 + rptr[2 * n + j][c] * 4 + rptr[3 * n + j][c] * 2 + rptr[4 * n + j][c]) / 10;
        }
    }
  }

  // Process edges
  cv::Vec3b *dptr0 = dst.ptr<cv::Vec3b>(0);
  cv::Vec3b *dptr1 = dst.ptr<cv::Vec3b>(1);
  cv::Vec3b *dptr2 = dst.ptr<cv::Vec3b>(2);
  cv::Vec3b *dptrm1 = dst.ptr<cv::Vec3b>(m - 1);
  cv::Vec3b *dptrm2 = dst.ptr<cv::Vec3b>(m - 2);
  cv::Vec3b *dptrm3 = dst.ptr<cv::Vec3b>(m - 3);

  for (int i = 0; i < src.cols; i++) {
    for (int j = 0; j < 3; j++) {
      dptr0[i][j] = dptr2[i][j];
      dptr1[i][j] = dptr2[i][j];
      dptrm1[i][j] = dptrm3[i][j];
      dptrm2[i][j] = dptrm3[i][j];
    }
  }

  return (0);
}

int sobelX3x3(const cv::Mat &src, cv::Mat &dst) {
  cv::Mat temp = cv::Mat::zeros(src.size(), CV_16SC3);
  int m = src.rows;
  int n = src.cols;

  int sobelxSum = 4;

  // Vertical pass for {1,2,1} blue
  for (int i = 1; i < m-1; i++) {
    cv::Vec3s *tptr = temp.ptr<cv::Vec3s>(i);
    const cv::Vec3b *rowAbove = src.ptr<cv::Vec3b>(i - 1);
    const cv::Vec3b *currRow = src.ptr<cv::Vec3b>(i);
    const cv::Vec3b *rowBelow = src.ptr<cv::Vec3b>(i + 1);
    for (int j = 0; j < n; j++) {
      for (int c = 0; c < 3; c++){
        tptr[j][c] = (rowAbove[j][c] + currRow[j][c] * 2 + rowBelow[j][c]) / sobelxSum;
      }
    }
  }

  dst = cv::Mat::zeros(src.size(), CV_16SC3);

  // Horizontal pass for {-1,0,1} 1st derivative
  for (int i = 0; i < m; i++) {
    cv::Vec3s *dptr = dst.ptr<cv::Vec3s>(i);
    const cv::Vec3s *rptr = temp.ptr<cv::Vec3s>(i);
    for (int j = 1; j < n - 1; j++) {
      for (int c = 0; c < 3; c++) {
        dptr[j][c] = (-1 * rptr[j - 1][c] + rptr[j + 1][c]) / 2;
      }
    }
  }

  return (0);
}

int sobelY3x3(const cv::Mat &src, cv::Mat &dst) {
  cv::Mat temp = cv::Mat::zeros(src.size(), CV_16UC3);
  int m = src.rows;
  int n = src.cols;
  
  for (int i = 1; i < m - 1; i++) {
    cv::Vec3s *tptr = temp.ptr<cv::Vec3s>(i);
    const cv::Vec3b *rptrm1 = src.ptr<cv::Vec3b>(i - 1);
    const cv::Vec3b *rptrp1 = src.ptr<cv::Vec3b>(i + 1);
    for (int j = 0; j < n; j++) {
      for (int c = 0; c < 3; c++) {
        tptr[j][c] = (rptrm1[j][c] - rptrp1[j][c]) / 2;
      }
    }
  }

  dst = cv::Mat::zeros(src.size(), CV_16SC3);
  for (int i = 0; i < m; i++) {
    cv::Vec3s *dptr = dst.ptr<cv::Vec3s>(i);
    const cv::Vec3s *rptr = temp.ptr<cv::Vec3s>(i);
    for (int j = 1; j < n - 1; j++){
      for (int c = 0; c < 3; c++) {
        dptr[j][c] = (rptr[j - 1][c] + 2 * rptr[j][c] + rptr[j + 1][c]) / 4;
      }
    }
  }

  return (0);
}

int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst) {
  dst = cv::Mat::zeros(sx.size(), CV_8UC3);
  for (int i = 0; i < sx.rows; i++) {
    cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);
    const cv::Vec3s *sxptr = sx.ptr<cv::Vec3s>(i);
    const cv::Vec3s *syptr = sy.ptr<cv::Vec3s>(i);
    for (int j = 0; j < sx.cols; j++) {
      for (int c = 0; c < 3; c++) {
        dptr[j][c] = std::sqrt(sxptr[j][c] * sxptr[j][c] + syptr[j][c] * syptr[j][c]);
      }
    }
  }
  return (0);
}

int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels) {
  cv::Mat blurImg;
  blur5x5(src, blurImg);

  int b = 255 / levels;
  int *grid = new int[300];

  for (int i = 0; i < 300; i++) {
    grid[i] = (i / b) * b;
  }

  dst = cv::Mat::zeros(src.size(), CV_8UC3);

  // quantize the image
  for (int i = 0; i < src.rows; i++) {
    cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);
    const cv::Vec3b *rptr = blurImg.ptr<cv::Vec3b>(i);
    for (int j = 0; j < src.cols; j++) {
      for (int c = 0; c < 3; c++) {
        dptr[j][c] = grid[rptr[j][c]];
      }
    }
  }

  return (0);
}

int cartoon(cv::Mat &src, cv::Mat &dst, int levels, int magThreshold) {
  cv::Mat sobelx;
  sobelX3x3(src, sobelx);
  cv::Mat sobely;
  sobelY3x3(src, sobely);
  cv::Mat mag;
  magnitude(sobelx, sobely, mag);
  cv::Mat quantize;
  blurQuantize(src, quantize, levels);

  dst = cv::Mat::zeros(src.size(), CV_8UC3);

  for (int i = 0; i < src.rows; i++) {
    cv::Vec3b *dptr = dst.ptr<cv::Vec3b>(i);
    cv::Vec3b *mptr = mag.ptr<cv::Vec3b>(i);
    cv::Vec3b *qptr = quantize.ptr<cv::Vec3b>(i);

    for (int j = 0; j < src.cols; j++) {
      for (int c = 0; c < 3; c++) {
        // if magnitude is less than threshold, use quantized image
        if (mptr[j][c] <= magThreshold) {
          dptr[j][c] = qptr[j][c];
        }
      }
    }
  }

  return (0);
}

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
