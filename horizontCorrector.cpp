#include "horizontCorrector.h"
//#include "opencv2/core/ocl.hpp"

void correctHorizont(cv::Mat &io_img, cv::Mat transformation)
{
    //cv::ocl::setUseOpenCL(true);
    int imgX = io_img.cols;
    int imgY = io_img.rows;

    //cv::UMat oclMat;
    //io_img.copyTo(oclMat);

    cv::warpAffine(io_img,io_img,transformation,cv::Size(imgX,imgY));

    //oclMat.copyTo(io_img);
}

cv::Mat calcHorizontCorrectionTransform(cv::Mat &io_img, cv::Point2f i_horizontCenterPt, float i_horizontAngle)
{
    int imgX = io_img.cols;
    int imgY = io_img.rows;

    cv::Mat M_rotate = cv::getRotationMatrix2D(i_horizontCenterPt,i_horizontAngle,1);
    cv::Mat M_translate2Center = cv::Mat::zeros(2,3, CV_64F);
    M_translate2Center.at<double>(0,2)=(imgX/2.0)-i_horizontCenterPt.x;
    M_translate2Center.at<double>(1,2)=(imgY/2.0)-i_horizontCenterPt.y;

    cv::Mat M=M_rotate+M_translate2Center;

    return M;
}
