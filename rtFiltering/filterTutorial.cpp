/* Bruce Maxwell
   Fall 2023
   CS 5330
*/
#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include "filter.h"

// using namespace cv

// argc # of command line strings
// argv is an array of the strings
int main(int argc, char *argv[]) {

  // allocate an empty cv::Mat
  cv::Mat src;
  char filename[256];

  if( argc < 2 ) {
    printf("usage: %s <image filename>\n", argv[0] );
    return(-1);
  }

  strcpy( filename, argv[1] );

  // allocates and reads the image data from filename
  // by default imread converts every image to 8-bits/color channel
  src = cv::imread( filename );

  // check if it read the image
  // if it didn't read the image, src.data will be NULL
  if( src.data == NULL ) {
    printf("Error: unable to read image %s\n", filename);
    return(-2);
  }

  // image should be in memory now in BGR format

  // how do we copy an image?
  cv::Mat src2;

  // shallow copy / copies the pointers, does not duplicate the data
  src2 = src;

  // deep copy of the whole image data
  src.copyTo( src2 );

  // display the read image
  // create a window
  cv::namedWindow( "original", 1 );
  cv::imshow( "original", src );

  // // do something to the image
  // for(int i=0;i<src.rows;i++) {

  //   // access the image through row pointers
  //   // treating the image as 3-byte vectors
  //   cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i); // get the pointer to the start row i

  //   // treat the image as an array of bytes
  //   uchar *uptr = src.ptr<uchar>(i); // a uchar pointer to the start of row i 

  //   // memory address just after the last pixel
  //   uchar *end = uptr + src.cols*3;

  //   // while I haven't hit the last pixel
  //   while(uptr < end) {
  //   //for(int j=0;j<src.cols*3;j++) {

  //     // update the pixel
  //     *uptr = 255 - *uptr;

  //     // update the  memory address
  //     uptr++;

  //     /*
  //     uptr[j] = 255 - uptr[j];
  //     */

  //     /*
  //     rptr[j][0] = 255 - rptr[j][0]; // blue
  //     rptr[j][1] = 255 - rptr[j][1]; // green
  //     rptr[j][2] = 255 - rptr[j][2]; // red
  //     */
      
  //     /*
  //     src.at<cv::Vec3b>(i, j)[0] = 255 - src.at<cv::Vec3b>(i, j)[0]; // negative blue channel
  //     src.at<cv::Vec3b>(i, j)[1] = 255 - src.at<cv::Vec3b>(i, j)[1]; // negative green channel
  //     src.at<cv::Vec3b>(i, j)[2] = 255 - src.at<cv::Vec3b>(i, j)[2]; // negative red channel
  //     */
  //   }
  // }

  // cv::namedWindow( "modified", 2 );
  // cv::imshow( "modified", src );

  // // calling the gradx function
  // cv::Mat graddst;
  // gradx( src2, graddst );

  // // convert short image to an unsigned char 3-channel image
  // cv::Mat displayit;
  // cv::convertScaleAbs( graddst, displayit );  // outputs 3-channel byte image

  // cv::namedWindow( "gradx", 3 );
  // cv::imshow( "gradx", displayit );

  // wait for a keystroke
  cv::waitKey(0);

  // // get rid of the window
  // cv::destroyWindow( "original" );

  return(0);
}
