#ifndef HELPINGFUNCTIONS
#define HELPINGFUNCTIONS

#endif // HELPINGFUNCTIONS
#include "opencv2/opencv.hpp"
#include <QImage>
cv::Mat qimage_to_mat_ref(QImage &img, int format);
QImage mat_to_qimage_ref(cv::Mat &mat, QImage::Format format);
