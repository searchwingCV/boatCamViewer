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
    m_curTrackertype=tracker;
    resetTracker();
}

boxTracker::~boxTracker()
{
    m_isInitalized=false;
    quit();
    wait();
    trackerMutex.lock();
    m_openCvTracker.release();
}
void boxTracker::createNewTracker(boxTracker::trackertype tracker)
{
    resetTracker();
    m_curTrackertype=tracker;
    cv::String m_trackerString=QtEnumToString(m_curTrackertype);

    //emit sendControlMsg(ImageIO::ctrlMsg::pause);
    m_openCvTracker = createOpenCvTrackerByName(m_trackerString);
    //emit sendControlMsg(ImageIO::ctrlMsg::play);
}
void boxTracker::resetTracker()
{
    m_isInitalized=false;

    quit();
    wait();
    trackerMutex.lock();
    m_openCvTracker.release();

    m_frameCounter=0;
    m_curFps=0;
    m_currImageProcessed=false;
    m_curTrackedRoi = QRect(0,0,0,0);
    m_curTrackerStatus=trackerstatus::NO_TRACK;

    trackerMutex.unlock();
    emit sendTrackerInfo(createTrackerInfo());

}

bool boxTracker::init(cv::Mat image, cv::Rect2d boundingbox)
{

    if(!m_isInitalized){
        //m_isInitalized=false;
        createNewTracker(m_curTrackertype);
    }
    bool ok=false;

    if (m_openCvTracker != NULL){
        trackerMutex.lock();
        ok = m_openCvTracker->init(image,boundingbox);
        m_currImageProcessed = true;
        trackerMutex.unlock();
    }

    if(ok){
        m_isInitalized=true;
        start(QThread::NormalPriority);
    }
    return ok;
}

bool boxTracker::update(cv::Mat image, cv::Rect2d &boundingbox)
{
    trackerMutex.lock();
    int ok=false;
    ok = m_openCvTracker->update(image,boundingbox);
    trackerMutex.unlock();
    return ok;
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
            m_currImageProcessed=true;
            std::cout <<"roi.x: " << trackedRoi.x <<"roi.y: " << trackedRoi.y<<"roi.width: " << trackedRoi.width<<"roi.height: " << trackedRoi.height <<std::endl;
            if( !ok ||
                trackedRoi.x < 0 || trackedRoi.x > m_currImage.cols || trackedRoi.width > m_currImage.cols ||
                trackedRoi.y < 0 || trackedRoi.y > m_currImage.rows || trackedRoi.height > m_currImage.rows )
//                  || trackedRoi.size().area()==0)
            {
                m_curFps = 0;
                m_curTrackerStatus=trackerstatus::NO_TRACK;
                emit sendTrackerInfo(createTrackerInfo());
                resetTracker();
                break;
            }
            if(ok){
                QRect qtrackedRoi = QRect(trackedRoi.x,trackedRoi.y,trackedRoi.width,trackedRoi.height);
                m_curTrackedRoi = qtrackedRoi;
                m_curTrackerStatus=trackerstatus::TRACKING;

                m_frameCounter++;
                float elapsedTimeSec = timeMeasurement.elapsed() / 1000.0;
                if(elapsedTimeSec > 1.0){
                    m_curFps = m_frameCounter/elapsedTimeSec;
                    m_frameCounter=0;
                    timeMeasurement.restart();
                }
                emit sendTrackerInfo(createTrackerInfo());
            }
        }
        else {
            this->msleep(10);
        }
    }
    return;
}

void boxTracker::receiveRoi(const QRect &roi)
{
    emit sendControlMsg(ImageIO::ctrlMsg::pause);
    cv::Point p1(roi.topLeft().x(),roi.topLeft().y());
    cv::Point p2(roi.bottomRight().x(),roi.bottomRight().y());
    m_initRoi=cv::Rect(p1,p2);
    m_isInitalized=false;
    init(m_currImage,m_initRoi);
    emit sendControlMsg(ImageIO::ctrlMsg::play);
}

void boxTracker::receiveImage(cv::Mat newImage)
{
    m_currImage = newImage;
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

cv::Ptr<cv::Tracker> boxTracker::createOpenCvTrackerByName(cv::String name)
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
