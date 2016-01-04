/*
 * main.cpp
 *
 *      Author: ZENG
 */
#include "TLI.h"

int main() {

  cout << "Using OpenCV " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << "." << CV_SUBMINOR_VERSION << std::endl;
  cTLI lattice;

  for(int frameNumber = 1; frameNumber <= 30000; frameNumber++) {

    char* data_path = new char[128];
    char* bin_path = new char[128];
    sprintf(data_path, "D:\\BGR\\in%06d.jpg",frameNumber);
    sprintf(bin_path, "D:\\MASK\\bin%06d.png",frameNumber);
    Mat BGR = imread(data_path, CV_LOAD_IMAGE_COLOR);
    Mat MASK = imread(bin_path, CV_LOAD_IMAGE_GRAYSCALE);

    if (BGR.empty()) {
      break;
    }
    if(MAX(BGR.rows,BGR.cols) > 600) {
      pyrDown(BGR,BGR);
      pyrDown(MASK,MASK);
    }
    lattice.fProcess(BGR, MASK);
    waitKey(1);
  }

  return 0;
}
