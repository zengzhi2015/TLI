/*
 * SOF.cpp
 *
 *      Author: ZENG
 */
#include "SOF.h"

cSOF::cSOF() {
  ;
}

cSOF::~cSOF() {
  ;
}

void cSOF::fSetName(String name) {
  mName = name;
}

void cSOF::fDoubleFlow() {
  calcOpticalFlowPyrLK(mPreviousImage, mNextImage, mPreviousPoints, mNextPoints, mStatus, mError, mWinSize, mMaxLevel, mTermcrit,
      mFlag, mMinEigThreshold);
  calcOpticalFlowPyrLK(mNextImage, mPreviousImage, mNextPoints, mBackPoints, mBackStatus, mBackError, mWinSize, mMaxLevel, mTermcrit,
        mFlag, mMinEigThreshold);
}

void cSOF::fPostProcessing() {
  vector<Point2f>::iterator it1,it2,it3;
  vector<uchar>::iterator it4, it5;
  vector<String>::iterator it6;

  mInformation.assign(mStatus.size(),"Null");

  it1 = mPreviousPoints.begin();
  it2 = mNextPoints.begin();
  it3 = mBackPoints.begin();
  it4 = mStatus.begin();
  it5 = mBackStatus.begin();
  it6 = mInformation.begin();

  double Thr = 2;

  while(it1 != mPreviousPoints.end()) {

    if(norm(*it1 - *it3) <= Thr && *it4 == 1 && *it5 == 1) {
      *it6 = "Correct";
    }
    if(norm(*it1 - *it3) <= Thr && *it4 == 1 && *it5 == 0) {
      *it6 = "BackNotConverge";
    }

    if(norm(*it1 - *it3) <= Thr && *it4 == 0 && *it5 == 1) {
      *it6 = "ForeNotConverge";
    }
    if(norm(*it1 - *it3) <= Thr && *it4 == 0 && *it5 == 0) {
      *it6 = "BothNotConverge";
    }
    if(norm(*it1 - *it3) > Thr && *it4 == 1 && *it5 == 1) {
      *it6 = "Occlude";
    }
    if(norm(*it1 - *it3) > Thr && *it4 == 0 && *it5 == 1) {
      *it6 = "Occlude&ForeNotConverge";
    }
    if(norm(*it1 - *it3) > Thr && *it4 == 1 && *it5 == 0) {
      *it6 = "Occlude&BackNotConverge";
    }
    if(norm(*it1 - *it3) > Thr && *it4 == 0 && *it5 == 0) {
      *it6 = "Occlude&BothNotConverge";
    }

    it1++;
    it2++;
    it3++;
    it4++;
    it5++;
    it6++;
  }
}

void cSOF::fCalculation(Mat &PreviousImage, Mat &NextImage, vector<Point2f> &PreviousPoints, vector<Point2f> &NextPoints, vector<String> &Info) {
  cvtColor(PreviousImage, mPreviousImage, COLOR_BGR2GRAY);
  cvtColor(NextImage, mNextImage, COLOR_BGR2GRAY);
  mPreviousPoints.assign(PreviousPoints.begin(),PreviousPoints.end());
  fDoubleFlow();
  fPostProcessing();
  NextPoints.assign(mNextPoints.begin(),mNextPoints.end());
  Info.assign(mInformation.begin(),mInformation.end());
}


