#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imageio.h>
#include <QGraphicsPixmapItem>
#include "boxTracker.h"

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

private slots:
    void on_pauseButton_clicked();
    void on_playButton_clicked();
    void on_trackerComboBox_currentIndexChanged(int index);

    void on_framerate_editingFinished();

private:
    Ui::MainWindow *ui;
    ImageIO imageIO;
    boxTracker tracker;
    QGraphicsPixmapItem pixmap;
};

#endif // MAINWINDOW_H
