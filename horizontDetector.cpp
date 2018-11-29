#include <horizontDetector.h>
#include "opencv2/highgui.hpp"
#include <iostream>

horizontDetector::horizontInfo horizontDetector::calcHorizont(cv::Mat i_img,float i_imgScaling){
    cv::Mat scaledImg;
    cv::cvtColor(i_img,scaledImg,CV_RGB2GRAY);
    cv::resize(i_img,scaledImg,cv::Size(),i_imgScaling,i_imgScaling);
    int sampleCount = 20;

    cv::Point2f horizontCenterPt;
    float horizontAngle;
    std::vector<cv::Point2f> detectedHorizPts;
    calcGradientHorizont(scaledImg, sampleCount, horizontCenterPt, horizontAngle,detectedHorizPts);

    horizontCenterPt.x = horizontCenterPt.x*1/i_imgScaling;
    horizontCenterPt.y = horizontCenterPt.y*1/i_imgScaling;
    for (cv::Point2f& ptIt : detectedHorizPts){
        ptIt.x = ptIt.x*1/i_imgScaling;
        ptIt.y = ptIt.y*1/i_imgScaling;
    }

    horizontInfo horizont;
    horizont.angle = horizontAngle;
    horizont.centerPt = horizontCenterPt;
    horizont.detectedHorizPts = detectedHorizPts;
    horizont.status = horizontStatus::DETECTED;
    return horizont;
}

void horizontDetector::calcGradientHorizont(cv::Mat i_img, int i_sampleCount, cv::Point2f& o_horizontCenterPt, float& o_horizontAngle, std::vector<cv::Point2f>& o_detectedHorizPts){
    cv::Mat imgYGradient;
    calcGradientImg(i_img,imgYGradient);

    int imgWidth = imgYGradient.cols;

    int sampleDist = imgWidth/i_sampleCount;
    std::vector<cv::Point2f> horizontSamplePts(i_sampleCount);

    for (int i = 0; i < i_sampleCount; i++){
        int colIdx = i*sampleDist;
        int aboveThreshRowIdx = getFirstColPointAboveTresh(imgYGradient.col(colIdx),m_gradientTreshold);
        horizontSamplePts[i] = cv::Point2f(colIdx,aboveThreshRowIdx);
    }

    std::vector<float> lineValues;
    cv::fitLine(horizontSamplePts,lineValues,cv::DIST_L1,0,0.01,0.01);

    cv::Vec2f lineSlope(lineValues[0],lineValues[1]);
    cv::Vec2f lineBasePt(lineValues[2],lineValues[3]);

    int lineCenterX=imgWidth/2;
    float lambda = (lineCenterX-lineBasePt[0])/lineSlope[0];
    int lineCenterY=lineBasePt[1]+lambda*lineSlope[1];
    o_horizontCenterPt.x = lineCenterX;
    o_horizontCenterPt.y = lineCenterY;

    o_detectedHorizPts = horizontSamplePts;
    o_horizontAngle=(tan(lineSlope[1]/lineSlope[0])*180.0)/M_PI;
}

void horizontDetector::calcGradientImg(cv::Mat i_img, cv::Mat& o_img)
{
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    int dx=0;
    int dy=1;
    int ksize = 3;
    cv::Sobel(i_img,o_img,ddepth,dx,dy,ksize,scale,delta,cv::BORDER_DEFAULT);
    cv::convertScaleAbs(o_img,o_img);
}

int horizontDetector::getFirstColPointAboveTresh(const cv::Mat &i_colData, int tresh)
{
    int i =0;
    for(cv::MatConstIterator_<uchar> it = i_colData.begin<uchar>(); it != i_colData.end<uchar>(); ++it){
        if(*it > tresh)
            break;
        i++;
    }
    return i;
}

int horizontDetector::getGradientTreshold() const
{
    return m_gradientTreshold;
}

void horizontDetector::setGradientTreshold(int gradientTreshold)
{
    m_gradientTreshold = gradientTreshold;
}

cv::Mat horizontDetector::drawHorizontDebugImg(const cv::Mat &i_img, cv::Point2f i_horizontCenterPt, float i_horizontAngle)
{
    float i_horizontAngleRad = (i_horizontAngle*M_PI)/180.0;
    float slopeX = 1.0;
    float slopeY = slopeX * tan(i_horizontAngleRad);
    cv::Vec2f lineBasePtVec(i_horizontCenterPt.x,i_horizontCenterPt.y);
    cv::Vec2f lineSlopeVec(slopeX,slopeY);
    cv::Vec2f horizontPtVec0 = lineBasePtVec-500*lineSlopeVec;
    cv::Vec2f horizontPtVec1 = lineBasePtVec+500*lineSlopeVec;

    cv::Point2f horizontPt0;
    horizontPt0.x = horizontPtVec0[0];
    horizontPt0.y = horizontPtVec0[1];
    cv::Point2f horizontPt1;
    horizontPt1.x = horizontPtVec1[0];
    horizontPt1.y = horizontPtVec1[1];

    cv::Mat imgCopy;
    i_img.copyTo(imgCopy);

    cv::circle(imgCopy,i_horizontCenterPt,4,cv::Scalar(0,0,255),3);
    cv::line(imgCopy,horizontPt0,horizontPt1,cv::Scalar(0,255,0),2);

    //for (const auto ptIt : m_detectedHorizPts)
    //    cv::circle(imgCopy,ptIt,2,cv::Scalar(255,0,0),2);
    return imgCopy;
}
