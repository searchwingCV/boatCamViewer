#ifndef BOXTRACKER_H
#define BOXTRACKER_H
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <QObject>
#include <QMetaEnum>
#include <QRect>
#include <QMutex>
#include <QThread>

class boxTracker : public QThread {
    Q_OBJECT
public:
    enum trackertype{
        MEDIANFLOW,GOTURN, MIL, BOOSTING, TLD, KCF, MOSSE,CSRT
    };
    Q_ENUM(trackertype)
    enum trackerstatus{
        NO_TRACK,TRACKING
    };
    struct trackerInfo{
        trackerstatus status;
        QRect roi;
        QPoint roiCenter;
        QPoint deltasCenter;
        uint fps;
    };
    struct circularBuf{
        QVector<QRect> roiCircularBuf();
        int pos=0;
        int validCount=0;
    };


    boxTracker(trackertype tracker);
    ~boxTracker();
    bool init(cv::Mat image, cv::Rect2d boundingbox);
    qint8 update(cv::Mat image,cv::Rect2d &boundingbox);
    QList<QString> getTrackerTypes();
    bool m_isInitalized;

    void setTrackerType(boxTracker::trackertype tracker);
    void createNewTracker(boxTracker::trackertype tracker);
    void resetTracker();
private:
    cv::Ptr<cv::Tracker> m_tracker;
    QMetaEnum m_metaEnumTrackers;
    cv::Ptr<cv::Tracker> createTrackerByName(cv::String name);
    cv::Rect2d m_initRoi;
    cv::Mat m_currImage;
    QMutex trackerMutex;
    bool m_initRoiReceived;
    trackertype m_curTrackertype;
    trackerstatus m_curTrackerStatus;
    QRect m_curTrackedRoi;
    QRect m_curTrackedRoiFiltered;
    float m_curFps;
    bool m_newImgProcessed;
    uint m_frameCounter;
    bool m_currImageProcessed;

    boxTracker::trackerInfo createTrackerInfo();
    QRect filterRoi(const QRect &roi);
    circularBuf m_roiCircularBuf;

public slots:
    void receiveImage(cv::Mat newImage);
    void receiveRoi(const QRect &roi);
signals:
    void sendTrackerInfo(boxTracker::trackerInfo trackerInfo);
protected:
    void run();
};

#endif // TRACKER_H
