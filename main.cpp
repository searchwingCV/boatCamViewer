#include "mainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
#include "boxTracker.h"
#include "horizontDetector.h"

Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(boxTracker::trackerInfo)
Q_DECLARE_METATYPE(horizontDetector::horizontInfo)
Q_DECLARE_METATYPE(imgPreprocessing::preProcConf)

int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>();
    qRegisterMetaType<boxTracker::trackerInfo>();
    qRegisterMetaType<horizontDetector::horizontInfo>();
    qRegisterMetaType<imgPreprocessing::preProcConf>();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //std::cout << cv::getBuildInformation() << std::endl;

    return a.exec();
}
