/*
 * TLI.cpp
 *
 *      Author: ZENG
 */
#include "TLI.h"

cTLI::cTLI() {
  ;
}

cTLI::~cTLI() {
  ;
}

void cTLI::fInitializeLattice(Mat &BGR) {
  BGR.copyTo(mNextBGR);
  // ////////////////////////////
  mInitialLattice.clear();
  for(int i = 0; i < BGR.rows; i += mInterval) {
    for(int j = 0; j < BGR.cols; j += mInterval) {
      mInitialLattice.push_back(Point2f(j,i));
    }
  }
  mNextLattice.assign(mInitialLattice.begin(),mInitialLattice.end());
  mInfo.clear();
  mInitialized = true;
  // /////////////////////////////
  mKernel.create(2*mInterval-1,2*mInterval-1,CV_64FC1);
  double x1,y1,x2,y2,d1,d2,z1,z2;
  for(int i = 0; i<mKernel.rows; i++) {
    for(int j = 0; j<mKernel.cols; j++) {
      x1 = (double)j - mInterval + 1;
      y1 = (double)i - mInterval + 1;
      x2 = sqrt(2)/2*(x1 - y1);
      y2 = sqrt(2)/2*(x1 + y1);
      d1 = mInterval;
      d2 = mInterval/sqrt(2);
      z1 = MAX(0,MIN(d1-abs(x1),d1-abs(y1)))/d1;
      z2 = MAX(0,MIN(d2-abs(x2),d2-abs(y2)))/d2;
      mKernel.at<double>(i,j) = (z1 + z2)/2;
    }
  }
}

void cTLI::fTrackLattice(Mat &BGR) {
  mNextBGR.copyTo(mPreBGR);
  BGR.copyTo(mNextBGR);
  swap(mNextLattice, mPreLattice);
  mFlow.fCalculation(mPreBGR, mNextBGR, mPreLattice, mNextLattice, mInfo);
}

void cTLI::fAnalyseMask(Mat &mask) {
  mask.copyTo(mBackground);
  mRegion.Analysis(mBackground, mRegions, mLabels, mAreaMap);
}

void cTLI::fModifyLattice() {
  double dist;
  vector<Point2f>::iterator it1 = mInitialLattice.begin();
  vector<Point2f>::iterator it2 = mNextLattice.begin();
  while(it1 != mInitialLattice.end()) {
    if(mRegions.at<uchar>((int)round(it1->y),(int)round(it1->x)) == 0) {
      *it2 = *it1;
    }
    if(it2->x < 0) {
      it2->x = 0;
    }
    if(it2->x > mNextBGR.cols - 1) {
      it2->x = mNextBGR.cols - 1;
    }
    if(it2->y < 0) {
      it2->y = 0;
    }
    if(it2->y > mNextBGR.rows - 1) {
      it2->y = mNextBGR.rows - 1;
    }

    it1++;
    it2++;
  }
}

void cTLI::fCalDensity() {
  Mat tDensity(mPreBGR.rows + mInterval*2 - 2,mPreBGR.cols + mInterval*2 - 2,CV_64FC1,Scalar(0.));
  mDensity.release();
  mDensity.create(mPreBGR.rows,mPreBGR.cols,CV_64FC1);

  vector<Point2f>::iterator it1;
  for(it1 = mNextLattice.begin(); it1 != mNextLattice.end(); it1++) {
    if(it1->x < mPreBGR.cols && it1->x >= 0 && it1->y < mPreBGR.rows && it1->y >= 0) {
      tDensity(Rect(it1->x,it1->y,mKernel.cols,mKernel.rows)) += mKernel;
    }
  }
  mDensity = tDensity(Rect(mInterval-1,mInterval-1,mDensity.cols,mDensity.rows));

  Mat DensityShow(mPreBGR.size(),CV_8UC1,Scalar(0));
  mDensity.convertTo(DensityShow,CV_8UC1,100,0);
  imshow("DensityShow",DensityShow);
}

void cTLI::fAnalysisInhomogeneity() {

  Mat tBlock;
  Mat tM2;
  Mat tDens;
  Point2d tInhomo;
  mInhomogeneity.clear();

  map<int, int>::iterator it = mAreaMap.begin();
  while(it != mAreaMap.end()) {
    if(it->first >0) {
      tBlock = mLabels == it->first;
      tBlock.convertTo(tBlock, CV_64FC1, 1);
      tBlock /= 255;
      tDens = tBlock.mul(mDensity);
      tInhomo.y = 1 - sum((tDens>0.5))[0]/255/MAX(1,sum(tBlock)[0]);
      tBlock = mLabels == it->first;
      dilate(tBlock,tBlock,Mat(),Point(-1,-1),5);
      tBlock.convertTo(tBlock, CV_64FC1, 1);
      tBlock /= 255;
      tDens = tBlock.mul(mDensity);
      tBlock = tDens > 0.9;
      tBlock.convertTo(tBlock, CV_64FC1, 1);
      tBlock /= 255;
      tDens = tBlock.mul(tDens);
      tInhomo.x = sum(tDens)[0]/MAX(1,sum(tBlock)[0]);
      mInhomogeneity[it->first] = tInhomo;
    }
    it++;
  }

  // Plot
  Mat tChannels[3];
  Mat tMerge;
  tChannels[0].create(mLabels.size(), CV_8UC1); tChannels[0] = 0;
  tChannels[1].create(mLabels.size(), CV_8UC1); tChannels[1] = 0;
  tChannels[2].create(mLabels.size(), CV_8UC1); tChannels[2] = 0;
  map<int, Point2d>::iterator it2 = mInhomogeneity.begin();
  while(it2 != mInhomogeneity.end()) {
    tChannels[2] += (mLabels == it2->first)/255*MIN(255,4*255*it2->second.y);
    tChannels[1] += (mLabels == it2->first)/255*MIN(255,2*255*(it2->second.x-1));
    it2++;
  }
  merge(tChannels,3,tMerge);
  imshow("Inhomogeneity",tMerge);
}


void cTLI::fProcess(Mat &BGR, Mat &mask) {
  if(!mInitialized) {
    fInitializeLattice(BGR);
    fTrackLattice(BGR);
    fAnalyseMask(mask);
    fModifyLattice();
    fCalDensity();
    fAnalysisInhomogeneity();
    mInitialized = true;
  }
  else {
    fTrackLattice(BGR);
    fAnalyseMask(mask);
    fModifyLattice();
    String s = "Trace Lattice";
    fPlotPoints(s);
    fCalDensity();
    fAnalysisInhomogeneity();
  }
}

void cTLI::fPlotPoints(const String s) {
  Mat ShowPoints;
  mNextBGR.copyTo(ShowPoints);
  vector<Point2f>::iterator it2;
  vector<String>::iterator it6;
  it2 = mNextLattice.begin();
  it6 = mInfo.begin();
  while(it2 != mNextLattice.end()) {
    if(*it6 == "Correct") {
      circle(ShowPoints, *it2, 1, Scalar(0,255,0), -1);
    }
    if(*it6 == "BackNotConverge") {
      circle(ShowPoints, *it2, 1, Scalar(255,255,0), -1);
    }
    if(*it6 == "ForeNotConverge") {
      circle(ShowPoints, *it2, 1, Scalar(255,255,0), -1);
    }
    if(*it6 == "BothNotConverge") {
      circle(ShowPoints, *it2, 1, Scalar(255,255,0), -1);
    }
    if(*it6 == "Occlude") {
      circle(ShowPoints, *it2, 1, Scalar(0,0,255), -1);
    }
    if(*it6 == "Occlude&ForeNotConverge") {
      circle(ShowPoints, *it2, 1, Scalar(255,0,255), -1);
    }
    if(*it6 == "Occlude&BackNotConverge") {
      circle(ShowPoints, *it2, 1, Scalar(255,0,255), -1);
    }
    if(*it6 == "Occlude&BothNotConverge") {
      circle(ShowPoints, *it2, 1, Scalar(255,0,255), -1);
    }
    it2++;
    it6++;
  }
  imshow(s, ShowPoints);
}



