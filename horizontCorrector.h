#include <opencv2/opencv.hpp>
#ifndef HORIZONTCORRECTOR
#define HORIZONTCORRECTOR

#endif // HORIZONTCORRECTOR

void correctHorizont(cv::Mat &io_img, cv::Mat transformation);
cv::Mat calcHorizontCorrectionTransform(cv::Mat &io_img, cv::Point2f i_horizontCenterPt, float i_horizontAngle);
