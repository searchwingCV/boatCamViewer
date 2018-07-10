
#include <QtGui>
#include <QGraphicsView>
#include <QRect>
#include <QGraphicsTextItem>
#include "boxTracker.h"

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
    QGraphicsEllipseItem *m_roiCenterCircle;
    QGraphicsTextItem* m_trackerText;

signals:
    void sendRoi(const QRect &roi);
public slots:
    void receiveTrackerInfo(boxTracker::trackerInfo trackerInfo);

};

