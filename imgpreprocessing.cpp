#include "imgpreprocessing.h"

imgPreprocessing::imgPreprocessing(QObject *parent) :
    QObject(parent),
    m_horizontDector()
{

}

void imgPreprocessing::setPreProcConf(imgPreprocessing::preProcConf newPreProcConf)
{
    m_horizontDector.setGradientTreshold(newPreProcConf.horizontGradientTresh);
    useHorizontStabilisator = newPreProcConf.useHorizontStabilitator;
    useHorizontDetector = newPreProcConf.useHorizontDetector;
}


void imgPreprocessing::receiveImage(cv::Mat newImage)
{
    horizontDetector::horizontInfo horizont;
    if(useHorizontDetector){
        float horizontDetectorScaling = 1.0/4.0;
        horizont = m_horizontDector.calcHorizont(newImage,horizontDetectorScaling);
    }
    if(useHorizontStabilisator){
        cv::Mat horizontStabTransform = calcHorizontCorrectionTransform(newImage,horizont.centerPt,horizont.angle);
        correctHorizont(newImage,horizontStabTransform);
        horizont.transformation=horizontStabTransform;
    }
    emit sendHorizontInfo(horizont);
    emit sendImage(newImage);

}
