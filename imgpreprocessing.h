#ifndef IMGPREPROCESSING_H
#define IMGPREPROCESSING_H
#include <opencv2/opencv.hpp>
#include <QObject>
#include "horizontDetector.h"
#include "horizontCorrector.h"

class imgPreprocessing : public QObject
{
    Q_OBJECT
public:
    explicit imgPreprocessing(QObject *parent = 0);
    struct preProcConf{
        int horizontGradientTresh;
        bool useHorizontStabilitator;
        bool useHorizontDetector;
    };
    void setPreProcConf(preProcConf newPreProcConf);

private:
    horizontDetector m_horizontDector;
    bool useHorizontStabilisator;
    bool useHorizontDetector;

signals:
    void sendImage(cv::Mat newImage);
    void sendHorizontInfo(horizontDetector::horizontInfo newHorizont);

public slots:
    void receiveImage(cv::Mat newImage);
    //void receivePreProcConf(preProcConf newPreProcConf);

};

#endif // IMGPREPROCESSING_H
