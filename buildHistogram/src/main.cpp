#include <algorithm>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include "matchMethods.hpp"
using namespace std;

int main(int argc, char *argv[]) {
  // target image path
  char targetPath[256];
  // get the filename
  strcpy(targetPath, argv[1]);

  // read the target image
  cv::Mat target = cv::imread(targetPath);
  // error checking
  if (target.data == NULL) {
    printf("error: unable to read image %s\n", targetPath);
    return (-2);
  }

  // image database directory path
  char dirname[256];
  strcpy(dirname, argv[2]);
  DIR *dirp = opendir(dirname);

  if (dirp == NULL) {
    printf("error: unable to open directory %s\n", dirname);
    exit(-3);
  }

  // create a vector to store the database image paths
  std::vector<std::pair<std::string, cv::Mat>> imageDatabase;
  // loop over all the files in the image database directory
  struct dirent *dp;
  while ((dp = readdir(dirp)) != NULL) {
    // check if the file is an image
    if (strstr(dp->d_name, ".jpg") ||
        strstr(dp->d_name, ".png") ||
        strstr(dp->d_name, ".ppm") ||
        strstr(dp->d_name, ".tif")) {
      // build the overall filename
      char buffer[256];
      strcpy(buffer, dirname);
      strcat(buffer, "/");
      strcat(buffer, dp->d_name);

      // read the image
      cv::Mat image = cv::imread(buffer);

      // error checking
      if (image.data == NULL) {
        printf("error: unable to read image %s\n", buffer);
        return (-4);
      }

      // store the images
      imageDatabase.push_back(std::make_pair(std::string(buffer), image));
    }
  }

  // close the directory
  closedir(dirp);

  // get the matching method
  std::string matchingMethod = argv[3];

  // the vector to store the distances
  std::vector<std::pair<std::string, float>> distanceArr;

  if (matchingMethod == "baseline") {
    baselineMatching(target, imageDatabase, distanceArr);
  } else {
    printf("error: invalid matching method\n");
    return (-5);
  }

  for (int i = 0; i < distanceArr.size(); i++) {
    printf("%s %f\n", distanceArr[i].first.c_str(), distanceArr[i].second);
  }
 
  // sort the vector of pairs by the ssd value in ascending order
  std::sort(distanceArr.begin(), distanceArr.end(),
            [](std::pair<std::string, float> const &a, std::pair<std::string, float> const &b)
            {
              return a.second < b.second;
            });

  // get the output number
  int outputNumber = atoi(argv[4]);

  // error checking
  if (outputNumber < 1) {
    printf("error: output number must be positive integer\n");
    return (-6);
  }

  // // display the target image
  // cv::namedWindow("Target image", 0);

  // // display the image
  // cv::imshow("Target image", target);

  // select the top N images
  std::vector<std::string> topNImages;
  for (int i = 1; i < outputNumber + 1; i++) {
    topNImages.push_back(distanceArr[i].first);
  }

  // // display the top N images
  // // display5ImagesInARow(topNImages);

  // // print out top N images paths
  // for (int i = 0; i < topNImages.size(); i++)
  // {
  //   std::string name = topNImages[i].c_str();
  //   std::istringstream iss(name);
  //   std::string token;
  //   std::string lastToken;

  //   while (std::getline(iss, token, '/')) {
  //       if (!token.empty()) {
  //           lastToken = token;
  //       }
  //   }

  //   printf("%s %s\n", topNImages[i].c_str(), lastToken.c_str());
  // }

  // wait for a key
  // while (true) {
  //   int key = cv::waitKey(1);
  //   if (key == 'q' || key == 'Q') {
  //     break;
  //   }
  // }

  cout << "Project2";
  return 0;
}