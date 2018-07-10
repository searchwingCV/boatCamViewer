#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QtWidgets>
#include "boxTracker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    imageIO(),
    tracker(boxTracker::trackertype::MEDIANFLOW)
{
    ui->setupUi(this);
    ui->trackerimageviewer->setScene(new QGraphicsScene(this));
    ui->trackerimageviewer->scene()->addItem(&pixmap);

    QList<QString> names=tracker.getTrackerTypes();
    for (auto iter = names.cbegin(); iter != names.cend();iter++){
        ui->trackerComboBox->addItem(*iter);
    }

    ui->trackerStatusLabel->setStyleSheet("QLabel { background-color : red; color : blue; }");

    connect(&imageIO, SIGNAL(sendImage(cv::Mat)), this, SLOT(receiveImage(cv::Mat)));
    connect(&imageIO, SIGNAL(sendImage(cv::Mat)), &tracker, SLOT(receiveImage(cv::Mat)));

    connect(ui->trackerimageviewer, SIGNAL(sendRoi(QRect)), &tracker, SLOT(receiveRoi(QRect)));
    connect(&tracker, SIGNAL(sendTrackerInfo(boxTracker::trackerInfo)), ui->trackerimageviewer, SLOT(receiveTrackerInfo(boxTracker::trackerInfo)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadButton_clicked()
{
    tracker.resetTracker();
    QString fileName = this->ui->pathInput->text();
    if(fileName == ""){
        fileName = QFileDialog::getOpenFileName(this,
                tr("Open Image"), "/home/julle/");
    }
    int ret;
    ret = imageIO.loadVideo(fileName);

    if(ret < 0){
        QMessageBox::critical(this,
                              "Video Error",
                              "Make sure you entered a correct and supported video file path,"
                              "<br>or a correct RTSP feed URL!");
    }
}

void MainWindow::receiveImage(cv::Mat newImage)
{
    QImage qnewImage(newImage.data,
                newImage.cols,
                newImage.rows,
                newImage.step,
                QImage::Format_RGB888);
    int width=qnewImage.width();
    int height=qnewImage.height();
    ui->trackerimageviewer->setFixedSize(width, height);
    ui->trackerimageviewer->setSceneRect(0, 0, width, height);
    ui->trackerimageviewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->trackerimageviewer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pixmap.setPixmap( QPixmap::fromImage(qnewImage) );
    ui->trackerimageviewer->fitInView(&pixmap, Qt::KeepAspectRatioByExpanding); //Qt::KeepAspectRatio

}

void MainWindow::on_pauseButton_clicked()
{
    imageIO.Pause();
}

void MainWindow::on_playButton_clicked()
{
    imageIO.Play();
}

void MainWindow::on_trackerComboBox_currentIndexChanged(int index)
{
    boxTracker::trackertype type=static_cast<boxTracker::trackertype>(index);
    tracker.createNewTracker(type);
}

