#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imageio.h>
#include <QGraphicsPixmapItem>
#include "boxTracker.h"
#include "imgpreprocessing.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_loadButton_clicked();
    void receiveImage(cv::Mat newImage);
signals:


private slots:
    void on_pauseButton_clicked();
    void on_playButton_clicked();
    void on_trackerComboBox_currentIndexChanged(int index);
    void on_framerate_editingFinished();
    void on_horizontCorrectionCheckBox_toggled(bool checked);
    void on_horizontDetectorTreshSlider_sliderMoved(int position);
    void on_horizontDetectionCheckBox_toggled(bool checked);


private:
    Ui::MainWindow *ui;
    ImageIO m_imageIO;
    boxTracker m_tracker;
    QGraphicsPixmapItem m_pixmap;
    imgPreprocessing m_imgPreprocessor;

    void sendPreProcConf();
};

#endif // MAINWINDOW_H
