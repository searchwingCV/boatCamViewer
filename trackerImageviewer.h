
#include <QtGui>
#include <QGraphicsView>
#include <QRect>
#include <QGraphicsTextItem>
#include "boxTracker.h"
#include "horizontDetector.h"

class trackerImageviewer : public QGraphicsView
{
    Q_OBJECT
public:
    trackerImageviewer(QWidget *parent);
    ~trackerImageviewer();
protected:
    //void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent ( QMouseEvent * mouseEvent );
    void mousePressEvent ( QMouseEvent * mouseEvent );
    void mouseReleaseEvent ( QMouseEvent * mouseEvent );
    void paintEvent(QPaintEvent *event) override;
private:
    int m_clickedX,m_clickedY;
    bool m_roiSelect;
    QRect m_roi;
    QGraphicsRectItem* m_initRoiRect;
    QGraphicsRectItem* m_trackedRoiRect;
    QPen m_roiSelectPen;
    QPen m_trackroiPen;
    QPen m_horizontLinePen;
    QPen m_horizontLinePtPen;
    QGraphicsEllipseItem *m_roiCenterCircle;
    QGraphicsTextItem* m_trackerText;
    QGraphicsLineItem* m_horizontLine;
    QList<QGraphicsEllipseItem*> m_horizontLinePtCircles;

signals:
    void sendRoi(const QRect &roi);
public slots:
    void receiveTrackerInfo(boxTracker::trackerInfo trackerInfo);
    void receiveHorizontInfo(horizontDetector::horizontInfo horizontInfo);

};

