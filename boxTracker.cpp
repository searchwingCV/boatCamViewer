#include "boxTracker.h"
#include "helpingfunctions.h"
#include <QElapsedTimer>

template<typename QEnum>
std::string QtEnumToString (const QEnum value)
{
  return std::string(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

boxTracker::boxTracker(boxTracker::trackertype tracker)
{
    /*
    m_curTrackertype=tracker;
    cv::String m_trackerString=QtEnumToString(m_curTrackertype);
    m_tracker = createTrackerByName(m_trackerString);
    m_isInitalized=false;
    m_initRoiReceived=false;
    m_currImage = cv::Mat();
    m_frameCounter=0;
    m_curFps=0;
    m_currImageProcessed=false;
    */
    m_curTrackertype=tracker;
    resetTracker();
}

boxTracker::~boxTracker()
{
    m_isInitalized=false;
    quit();
    wait();
    trackerMutex.lock();
    m_tracker.release();
}
void boxTracker::createNewTracker(boxTracker::trackertype tracker)
{
    m_curTrackertype=tracker;
    resetTracker();
}
void boxTracker::resetTracker()
{
    m_isInitalized=false;
    quit();
    wait();
    trackerMutex.lock();
    m_tracker.release();
    //m_curTrackertype=tracker;
    cv::String m_trackerString=QtEnumToString(m_curTrackertype);
    m_tracker = createTrackerByName(m_trackerString);
    m_initRoiReceived = false;
    m_frameCounter=0;
    m_curFps=0;
    m_currImageProcessed=false;
    m_curTrackedRoi = QRect(0,0,0,0);
    m_curTrackerStatus=trackerstatus::NO_TRACK;
    m_roiCircularBuf = circularBuf();
    //m_roiCircularBuf.roiCircularBuf.resize(10);
    trackerMutex.unlock();
    emit sendTrackerInfo(createTrackerInfo());
}

bool boxTracker::init(cv::Mat image, cv::Rect2d boundingbox)
{
    if(!m_isInitalized){
        m_isInitalized=false;
        createNewTracker(m_curTrackertype);
    }
    if (m_tracker != NULL){
        trackerMutex.lock();
        bool ok = m_tracker->init(image,boundingbox);
        trackerMutex.unlock();
        if(ok){
            m_isInitalized=true;
            start(QThread::LowPriority);
        }
    }
}

qint8 boxTracker::update(cv::Mat image, cv::Rect2d &boundingbox)
{

    trackerMutex.lock();
    int ok;
    try {
    ok = m_tracker->update(image,boundingbox);
    }
    catch(cv::Exception e){

    }
    trackerMutex.unlock();
    qint8 qok = static_cast<qint8>(ok);
    return qok;
}

QList<QString> boxTracker::getTrackerTypes()
{
    QList<QString> trackerNameList;
    for ( uint i = 0; i < 8; i++ )
    {
        QString trackerName = QString::fromStdString(QtEnumToString(static_cast<trackertype>(i)));
        trackerNameList.append(trackerName);
    }
    return trackerNameList;
}

void boxTracker::run()
{
    QElapsedTimer timeMeasurement;
    timeMeasurement.start();
    while(m_isInitalized){
        if(!m_currImageProcessed)
        {
            cv::Rect2d trackedRoi;

            bool ok = update(m_currImage,trackedRoi);
            if( trackedRoi.x < 0 || trackedRoi.x > m_currImage.cols || trackedRoi.width > m_currImage.cols ||
                trackedRoi.y < 0 || trackedRoi.y > m_currImage.rows || trackedRoi.height > m_currImage.rows )
            {
                resetTracker();
                ok = false;
            }
            if(ok){
                QRect qtrackedRoi = QRect(trackedRoi.x,trackedRoi.y,trackedRoi.width,trackedRoi.height);
                m_curTrackedRoi = qtrackedRoi;
                m_curTrackerStatus=trackerstatus::TRACKING;

            }
            else{
                m_curTrackerStatus=trackerstatus::NO_TRACK;
            }

            m_frameCounter++;
            float elapsedTimeSec = timeMeasurement.elapsed() / 1000.0;
            if(elapsedTimeSec > 1.0){
                m_curFps = m_frameCounter/elapsedTimeSec;
                m_frameCounter=0;
                timeMeasurement.restart();
            }
            emit sendTrackerInfo(createTrackerInfo());
            m_currImageProcessed=true;
        }

        msleep(5);
    }
    return;
}

//QRect boxTracker::filterRoi(const QRect &roi){
//    m_roiCircularBuf.roiCircularBuf().at(m_roiCircularBuf.pos) = roi;
//    m_roiCircularBuf.pos++;
//    m_roiCircularBuf.validCount++;
//
//}

void boxTracker::receiveRoi(const QRect &roi)
{
    cv::Point p1(roi.topLeft().x(),roi.topLeft().y());
    cv::Point p2(roi.bottomRight().x(),roi.bottomRight().y());
    m_initRoi=cv::Rect(p1,p2);
    if(!m_isInitalized){
        m_initRoiReceived = true;
    }
    else{
        m_initRoiReceived = true;
        m_isInitalized = false;
    }
}

void boxTracker::receiveImage(cv::Mat newImage)
{
    if(!m_isInitalized && m_initRoiReceived){
        if( init(newImage,m_initRoi)){
            m_isInitalized=true;
            m_initRoiReceived = false;
        }
    }
    else if(m_isInitalized){
        m_currImage = newImage;
    }
    m_currImageProcessed=false;
}

boxTracker::trackerInfo boxTracker::createTrackerInfo(){
    boxTracker::trackerInfo info;
    info.roi = m_curTrackedRoi;
    info.roiCenter = QPoint(m_curTrackedRoi.topLeft().x() + m_curTrackedRoi.width()/2,m_curTrackedRoi.topLeft().y() + m_curTrackedRoi.height()/2);
    cv::Size imgSize = m_currImage.size();
    info.deltasCenter = QPoint(info.roiCenter.x() - imgSize.width/2,info.roiCenter.y() - imgSize.height/2);
    info.status = m_curTrackerStatus;
    info.fps = static_cast<int>(m_curFps);
    return info;
}

cv::Ptr<cv::Tracker> boxTracker::createTrackerByName(cv::String name)
{
    cv::Ptr<cv::Tracker> tracker;

    if (name == "KCF")
        tracker = cv::TrackerKCF::create();
    else if (name == "TLD")
        tracker = cv::TrackerTLD::create();
    else if (name == "BOOSTING")
        tracker = cv::TrackerBoosting::create();
    else if (name == "MEDIANFLOW")
        tracker = cv::TrackerMedianFlow::create();
    else if (name == "MIL")
        tracker = cv::TrackerMIL::create();
    else if (name == "GOTURN")
        tracker = cv::TrackerGOTURN::create();
    else if (name == "MOSSE")
        tracker = cv::TrackerMOSSE::create();
    else if (name == "CSRT")
        tracker = cv::TrackerCSRT::create();
    else
        CV_Error(cv::Error::StsBadArg, "Invalid tracking algorithm name\n");

    return tracker;
}

