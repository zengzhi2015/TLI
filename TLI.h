/*
 * TLI.h
 *
 *      Author: ZENG
 */

#ifndef BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_TLI_H_
#define BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_TLI_H_

#include "Region.h"
#include "SOF.h"

class cTLI {
private:
  bool mInitialized = false;
private:
  Mat mPreBGR;
  Mat mNextBGR;
  vector<Point2f> mInitialLattice;
  vector<Point2f> mPreLattice;
  vector<Point2f> mNextLattice;
  vector<String> mInfo;
  cSOF mFlow;
private:
  Mat mBackground;
  Mat mRegions;
  Mat mLabels;
  map<int, int> mAreaMap;
  cRegion mRegion;
private:
  double mInterval = 7;
  Mat mKernel;
  Mat mDensity;
  map<int, Point2d>  mInhomogeneity;
public:
  cTLI();
  ~cTLI();
private:
  void fInitializeLattice(Mat &BGR);
  void fTrackLattice(Mat &BGR);
  void fAnalyseMask(Mat &mask);
  void fModifyLattice();
  void fCalDensity();
  void fAnalysisInhomogeneity();
private:
  void fPlotPoints(const String s);
public:
  void fProcess(Mat &BGR, Mat &mask);
};


#endif /* BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_TLI_H_ */
