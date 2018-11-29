#pragma once
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QMetaEnum>

class horizontDetector: public QObject{
    Q_OBJECT
public:
    enum horizontStatus{
        DETECTED, NOT_DETECTED
    };
    struct horizontInfo{
        horizontStatus status;
        cv::Point2f centerPt;
        float angle;
        std::vector<cv::Point2f> detectedHorizPts;
        cv::Mat transformation;
        horizontInfo() :
            status(horizontStatus::NOT_DETECTED),
            centerPt(cv::Point2f()),
            angle(0),
            detectedHorizPts(std::vector<cv::Point2f>()),
            transformation(cv::Mat::zeros(2,3, CV_64F))
        {
            transformation.at<double>(0,0)=1.0;
            transformation.at<double>(1,1)=1.0;
        }
    };

    horizontInfo calcHorizont(cv::Mat i_img,float i_imgScaling);
    cv::Mat drawHorizontDebugImg(const cv::Mat &i_img, cv::Point2f i_horizontCenterPt, float i_horizontAngle);
    int getGradientTreshold() const;
    void setGradientTreshold(int gradientTreshold);

private:

    void calcGradientHorizont(cv::Mat i_img, int i_sampleCount, cv::Point2f& o_horizontCenterPt, float& o_horizontAngle, std::vector<cv::Point2f>& o_detectedHorizPts);
    void calcGradientImg(cv::Mat i_img, cv::Mat &o_img);
    int getFirstColPointAboveTresh(const cv::Mat& i_colData, int tresh);
    int m_gradientTreshold;


//public slots:
//    void receiveThreshold(const int );

};




