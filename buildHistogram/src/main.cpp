#include <algorithm>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include "matchMethods.hpp"
#include "util.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  char targetPath[256];
  strcpy(targetPath, argv[1]);

  // read the target image
  cv::Mat target = cv::imread(targetPath);
  if (target.data == NULL) {
    printf("error: unable to read image %s\n", targetPath);
    return (-2);
  }

  // read image database directory path
  char dirname[256];
  strcpy(dirname, argv[2]);
  DIR *dirp = opendir(dirname);

  if (dirp == NULL) {
    printf("error: unable to open directory %s\n", dirname);
    exit(-3);
  }

  // create a vector to store the database image paths
  std::vector<std::pair<std::string, cv::Mat>> imageDatabase;
  // loops over the directory of images
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

      // read each image
      cv::Mat image = cv::imread(buffer);

      if (image.data == NULL) {
        printf("error: unable to read image %s\n", buffer);
        return (-4);
      }

      imageDatabase.push_back(std::make_pair(std::string(buffer), image));
    }
  }
  closedir(dirp);

  std::string matchingMethod = argv[3];
  std::vector<std::pair<std::string, float>> distanceArr;

  // different matching methods
  if (matchingMethod == "baseline") {
    baselineMatching(target, imageDatabase, distanceArr);
  } else if (matchingMethod == "rgChromaticity") {
    rgChromaticityMatching(target, imageDatabase, distanceArr, 16);
  }
  
  else {
    printf("error: invalid matching method\n");
    return (-5);
  }

  for (int i = 0; i < distanceArr.size(); i++) {
    printf("%s %f\n", distanceArr[i].first.c_str(), distanceArr[i].second);
  }
 
  // sort the vector - ascending order
  std::sort(distanceArr.begin(), distanceArr.end(),
            [](std::pair<std::string, float> const &a, std::pair<std::string, float> const &b)
            {
              return a.second < b.second;
            });

  // get the output number
  int resNum = atoi(argv[4]);

  // select the top N matches
  std::vector<std::string> topMatches;
  for (int i = 1; i < resNum + 1; i++) {
    topMatches.push_back(distanceArr[i].first);
  }

  // display the top N matches
  displayMultiImg(topMatches);

  // print out top N images paths
  for (int i = 0; i < topMatches.size(); i++) {
    std::string name = topMatches[i].c_str();
    std::istringstream iss(name);
    std::string token;
    std::string lastToken;

    while (std::getline(iss, token, '/')) {
        if (!token.empty()) {
            lastToken = token;
        }
    }

    printf("%s %s\n", topMatches[i].c_str(), lastToken.c_str());
  }

  // wait for a key
  while (true) {
    int key = cv::waitKey(1);
    if (key == 'q' || key == 'Q') {
      break;
    }
  }

  cout << "Project2";
  return 0;
}