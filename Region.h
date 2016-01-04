/*
 * Region.h
 *
 *      Author: ZENG
 */

#ifndef BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_REGION_H_
#define BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_REGION_H_

#include "CommonLibs.h"

class cRegion {
private:
  Mat mBinImage;
  Mat mLabels; // different regions with different labels
  Mat mArea;
  map<int, int> mAreaMap;
public:
  cRegion();
  ~cRegion();
public:
  void Analysis(Mat &binImg, Mat &results, Mat &labels, map<int, int> &areaMap);
private:
  void fSeedfill();
};

#endif /* BACKGROUNDSUBTRACTION_C_MOTION_TLI_1_0_REGION_H_ */
