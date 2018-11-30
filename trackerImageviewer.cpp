#include "trackerImageviewer.h"
#include <qgraphicsscene.h>
#include <QGraphicsItem>

trackerImageviewer::trackerImageviewer(QWidget *parent) : QGraphicsView (parent)
{
    m_roiSelect=false;
    m_initRoiRect = nullptr;
    m_trackedRoiRect = nullptr;
    m_trackerText = nullptr;
    m_roiCenterCircle = nullptr;
    m_horizontLine = nullptr;
    m_roiSelectPen.setColor(QColor(255,0,0,255));//
    m_roiSelectPen.setWidth(2);
    m_trackroiPen.setColor(QColor(0,255,0,255));
    m_trackroiPen.setWidth(3);
    m_horizontLinePen.setColor(QColor(255,255,0,255));
    m_horizontLinePen.setWidth(2);
    m_horizontLinePtPen.setColor(QColor(255,255,0,255));
    m_horizontLinePtPen.setWidth(2);
}

trackerImageviewer::~trackerImageviewer()
{

}

void trackerImageviewer::mousePressEvent(QMouseEvent *event)
{
    m_clickedX=event->x();
    m_clickedY=event->y();
    m_roiSelect = true;
}
void trackerImageviewer::mouseMoveEvent(QMouseEvent *event)
{
    if(m_roiSelect){
        int movedX=event->x();
        int movedY=event->y();
        m_roi=QRect(QPoint(m_clickedX,m_clickedY),QPoint(movedX,movedY));
    }
}
void trackerImageviewer::mouseReleaseEvent(QMouseEvent *event)
{

    m_roiSelect=false;
    emit sendRoi(m_roi);

}
void trackerImageviewer::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

    if(m_initRoiRect == nullptr ){
        m_initRoiRect=this->scene()->addRect(m_roi,m_roiSelectPen);
    }
    if(m_roiSelect == true )
    {
        m_initRoiRect->setRect(m_roi);
    }
    else
    {
        m_initRoiRect->setRect(QRect(0,0,0,0));
    }
}

void trackerImageviewer::receiveTrackerInfo(boxTracker::trackerInfo trackerInfo)
{
    if(m_trackerText == nullptr){
        m_trackerText=this->scene()->addText("");
        m_trackerText->setPos(10,10);
        m_trackerText->setDefaultTextColor(QColor(255,0,0,255));
        m_trackerText->setFont(QFont("Courier New"));
        m_trackerText->setScale(1);
        m_trackerText->setFlags(    QGraphicsItem::ItemIsMovable   |
                                    QGraphicsItem::ItemIsFocusable |
                                    QGraphicsItem::ItemIsSelectable);
        m_trackerText->setEnabled (true);
        m_trackerText->setVisible (true);
    }
    QString trackerStatus;
    if (trackerInfo.status == boxTracker::trackerstatus::TRACKING){
        m_trackerText->setDefaultTextColor(QColor(0,255,0,255));
        trackerStatus = "Active Tracking";
    }
    else if (trackerInfo.status == boxTracker::trackerstatus::NO_TRACK){
        m_trackerText->setDefaultTextColor(QColor(255,0,0,255));
        trackerStatus = "No Tracking";
    }
    QString statusString =
            trackerStatus + "\n" +
            QString::number(trackerInfo.fps)+" fps" +"\n" +
            "dx " +QString::number(trackerInfo.deltasCenter.x())+" dy " +QString::number(trackerInfo.deltasCenter.y());
    m_trackerText->setPlainText(statusString);

    if(m_trackedRoiRect == nullptr){

        m_trackedRoiRect=this->scene()->addRect(trackerInfo.roi,m_trackroiPen);
    }
    m_trackedRoiRect->setRect(trackerInfo.roi);

    int ellipseSize=20;
    QRectF ellipseRect(trackerInfo.roiCenter-QPoint(ellipseSize,ellipseSize)/2,QSize(ellipseSize,ellipseSize));
    if(m_roiCenterCircle == nullptr){
        m_roiCenterCircle=this->scene()->addEllipse(ellipseRect,m_trackroiPen);
    }
    m_roiCenterCircle->setRect(ellipseRect);

}

void trackerImageviewer::receiveHorizontInfo(horizontDetector::horizontInfo horizontInfo)
{
    //Calc horizont line
    float horizontAngleRad = (horizontInfo.angle*M_PI)/180.0;
    float slopeX = 1.0;
    float slopeY = slopeX * tan(horizontAngleRad);
    cv::Vec2f lineBasePtVec(horizontInfo.centerPt.x,horizontInfo.centerPt.y);
    cv::Vec2f lineSlopeVec(slopeX,slopeY);
    cv::Vec2f horizontPtVec0 = lineBasePtVec-500*lineSlopeVec;
    cv::Vec2f horizontPtVec1 = lineBasePtVec+500*lineSlopeVec;

    //Transform horizont to current image by applying correction transformation
    cv::Point2f p0(horizontPtVec0[0],horizontPtVec0[1]);
    cv::Point2f p1(horizontPtVec1[0],horizontPtVec1[1]);
    std::vector<cv::Point2f> ptVec;
    ptVec.push_back(p0);
    ptVec.push_back(p1);
    cv::transform(ptVec,ptVec,horizontInfo.transformation);
    //Draw
    QPoint horizontPt0;
    horizontPt0.setX(ptVec[0].x);
    horizontPt0.setY(ptVec[0].y);
    QPoint horizontPt1;
    horizontPt1.setX(ptVec[1].x);
    horizontPt1.setY(ptVec[1].y);
    QLineF horizontLine(horizontPt0,horizontPt1);
    if(m_horizontLine == nullptr){
        m_horizontLine=this->scene()->addLine(horizontLine,m_horizontLinePen);
    }
    m_horizontLine->setLine(horizontLine);

    //Transform horizontpts to current image by applying correction transformation
    if(horizontInfo.detectedHorizPts.size() == 0)
        return;
    cv::transform(horizontInfo.detectedHorizPts,horizontInfo.detectedHorizPts,horizontInfo.transformation);
    //Draw
    if(m_horizontLinePtCircles.size()==0){
        for (const auto ptIt : horizontInfo.detectedHorizPts){
            m_horizontLinePtCircles.append(this->scene()->addEllipse(ptIt.x,ptIt.y,3,3,m_horizontLinePtPen));
        }
    }
    int i = 0;
    for (const auto ptIt : horizontInfo.detectedHorizPts){
        m_horizontLinePtCircles.at(i)->setRect(ptIt.x,ptIt.y,3,3);
        i++;
    }


}
