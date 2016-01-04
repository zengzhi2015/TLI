/*
 * SOF.h
 *
 *      Author: ZENG
 */

#ifndef BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_SOF_H_
#define BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_SOF_H_

#include "CommonLibs.h"

class cSOF {
private:
  String mName;
private:
  Mat mPreviousImage;
  Mat mNextImage;
  vector<Point2f> mPreviousPoints; // starting points in the first image
  vector<Point2f> mNextPoints;  // moved pints in the second image
  vector<uchar> mStatus;
  int mMaxLevel = 7;
  vector<float> mError;
  TermCriteria mTermcrit = TermCriteria(2, 50, 0.001); // LKP related
  Size mWinSize = Size(21,21); // LKP related
  int mFlag = 0;
  double mMinEigThreshold = 0.0001;
private:
  vector<Point2f> mBackPoints;
  vector<uchar> mBackStatus;
  vector<float> mBackError;
  vector<String> mInformation;
public:
  cSOF();
  ~cSOF();
  void fSetName(String name);
private:
  void fDoubleFlow();
  void fPostProcessing();
public:
  void fCalculation(Mat &PreviousImage, Mat &NextImage, vector<Point2f> &PreviousPoints, vector<Point2f> &NextPoints, vector<String> &Info);
};

#endif /* BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_SOF_H_ */
