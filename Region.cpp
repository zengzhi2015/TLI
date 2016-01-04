/*
 * Region.cpp
 *
 *      Author: ZENG
 */

#include "Region.h"

cRegion::cRegion() {
  ;
}

cRegion::~cRegion() {
  ;
}

void cRegion::fSeedfill() {
  const int H = mBinImage.rows;
  const int W = mBinImage.cols;
  mLabels.create(H,W,CV_32S);
  mArea.create(H,W,CV_32S);
  mBinImage.convertTo(mLabels, CV_32SC1, 0.003921);
  mAreaMap.clear();

  int label = 2;
  int Area;
  int *pLabels;
  int *pArea;

  // positive regions
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      pLabels = (int*)mLabels.data + i*W + j;
      if (*pLabels == 1) {
        Area = 0;
        stack<pair<int,int>> neighborPixels;
        neighborPixels.push(pair<int,int>(i,j));
        while(!neighborPixels.empty()) {
          pair<int,int> curPixel = neighborPixels.top();
          int curY = curPixel.first;
          int curX = curPixel.second;
          *((int*)mLabels.data + curY*W + curX) = label;
          Area += 1;
          neighborPixels.pop() ;
          if(curX==0) { // left bound
            if(curY==0) { // left and up bound
              if(*((int*)mLabels.data + curY*W + curX+1) == 1) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 1) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) { // left and down bound
              if(*((int*)mLabels.data + curY*W + curX+1) == 1) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 1) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) { // left bound only
              if(*((int*)mLabels.data + curY*W + curX+1) == 1) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 1) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 1) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
          if(curX==W-1) { // right bound
            if(curY==0) { // right and up bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 1) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 1) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) { // right and down bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 1) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 1) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) { // right bound only
              if(*((int*)mLabels.data + curY*W + curX-1) == 1) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 1) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 1) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
          if((curX<W-1)&(curX>0)) {
            if(curY==0) { // up bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 1) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + curY*W + curX+1) == 1) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 1) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) { // down bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 1) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + curY*W + curX+1) == 1) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 1) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) { // no bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 1) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + curY*W + curX+1) == 1) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 1) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 1) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
        }
        mAreaMap[label] = Area;
        label += 1;
      }
    }
  }

  // negative regions
  label = -1;
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      pLabels = (int*)mLabels.data + i*W + j;
      if (*pLabels == 0) {
        Area = 0;
        stack<pair<int,int>> neighborPixels;
        neighborPixels.push(pair<int,int>(i,j));
        while(!neighborPixels.empty()) {
          pair<int,int> curPixel = neighborPixels.top();
          int curY = curPixel.first;
          int curX = curPixel.second;
          *((int*)mLabels.data + curY*W + curX) = label;
          Area += 1;
          neighborPixels.pop() ;
          if(curX==0) { // left bound
            if(curY==0) { // left and up bound
              if(*((int*)mLabels.data + curY*W + curX+1) == 0) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 0) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) { // left and down bound
              if(*((int*)mLabels.data + curY*W + curX+1) == 0) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 0) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) { // left bound only
              if(*((int*)mLabels.data + curY*W + curX+1) == 0) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 0) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 0) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
          if(curX==W-1) { // right bound
            if(curY==0) { // right and up bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 0) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 0) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) { // right and down bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 0) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 0) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) { // right bound only
              if(*((int*)mLabels.data + curY*W + curX-1) == 0) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 0) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 0) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
          if((curX<W-1)&(curX>0)) {
            if(curY==0) { // up bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 0) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + curY*W + curX+1) == 0) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 0) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) { // down bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 0) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + curY*W + curX+1) == 0) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 0) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) { // no bound
              if(*((int*)mLabels.data + curY*W + curX-1) == 0) {// left pixel
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)mLabels.data + curY*W + curX+1) == 0) {// right pixel
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)mLabels.data + (curY-1)*W + curX) == 0) {// up pixel
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)mLabels.data + (curY+1)*W + curX) == 0) {// down pixel
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
        }
        mAreaMap[label] = Area;
        label -= 1;
      }
    }
  }

  pLabels = (int*)mLabels.data;
  pArea = (int*)mArea.data;
  int tMax = H*W;
  for(int i = 0; i < tMax; i++) {
    if(*pLabels > 0) {
      *pArea = mAreaMap[*pLabels];
    }
    else {
      *pArea = -mAreaMap[*pLabels];
    }

    pLabels += 1;
    pArea += 1;
  }

}

void cRegion::Analysis(Mat &binImg, Mat &results, Mat &labels, map<int, int> &areaMap) {
  binImg.copyTo(mBinImage);
  medianBlur(mBinImage, mBinImage, 5);
  fSeedfill();

  const int H = mBinImage.rows;
  const int W = mBinImage.cols;
  int size = W*H/1000;
  mBinImage = (mArea > size) | ((mArea > -2*size) & (mArea < 0));
  fSeedfill();

  mBinImage.copyTo(results);
  mLabels.copyTo(labels);
  areaMap.clear();
  areaMap = mAreaMap;

  results = mArea > 0;
  imshow("Regions&Area", results);
}



