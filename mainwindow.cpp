#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QtWidgets>
#include "boxTracker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_imageIO(),
    m_tracker(boxTracker::trackertype::MEDIANFLOW),
    m_imgPreprocessor()
{
    ui->setupUi(this);
    ui->trackerimageviewer->setScene(new QGraphicsScene(this));
    ui->trackerimageviewer->scene()->addItem(&m_pixmap);

    QList<QString> names=m_tracker.getTrackerTypes();
    for (auto iter = names.cbegin(); iter != names.cend();iter++){
        ui->trackerComboBox->addItem(*iter);
    }

    sendPreProcConf();
    ui->horizontDetectorTreshSliderVal->setText(QString::number(ui->horizontDetectorTreshSlider->value()));

    connect(&m_imageIO, SIGNAL(sendImage(cv::Mat)), &m_imgPreprocessor, SLOT(receiveImage(cv::Mat)));
    connect(&m_imgPreprocessor, SIGNAL(sendImage(cv::Mat)), this, SLOT(receiveImage(cv::Mat)));
    connect(&m_imgPreprocessor, SIGNAL(sendImage(cv::Mat)), &m_tracker, SLOT(receiveImage(cv::Mat)));

    connect(ui->trackerimageviewer, SIGNAL(sendRoi(QRect)), &m_tracker, SLOT(receiveRoi(QRect)));
    connect(&m_tracker, SIGNAL(sendTrackerInfo(boxTracker::trackerInfo)), ui->trackerimageviewer, SLOT(receiveTrackerInfo(boxTracker::trackerInfo)));
    connect(&m_imgPreprocessor, SIGNAL(sendHorizontInfo(horizontDetector::horizontInfo )), ui->trackerimageviewer, SLOT(receiveHorizontInfo(horizontDetector::horizontInfo)));
    connect(&m_tracker, SIGNAL(sendControlMsg(ImageIO::ctrlMsg)), &m_imageIO, SLOT(receiveControlMsg(ImageIO::ctrlMsg)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadButton_clicked()
{
    m_tracker.resetTracker();
    QString fileName = this->ui->pathInput->text();
    if(fileName == ""){
        fileName = QFileDialog::getOpenFileName(this,
                tr("Open Image"), "/home/julle/ControlerProjekte/SearchWing/Data/video/SchwedenFaehre/");
    }
    int ret;
    ret = m_imageIO.loadVideo(fileName);

    if(ret < 0){
        QMessageBox::critical(this,
                              "Video Error",
                              "Make sure you entered a correct and supported video file path,"
                              "<br>or a correct RTSP feed URL!");
    }
    QString strFrameRate = this->ui->framerate->text();
    m_imageIO.setFrameRate(strFrameRate.toInt());
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
    m_pixmap.setPixmap( QPixmap::fromImage(qnewImage) );
    ui->trackerimageviewer->fitInView(&m_pixmap, Qt::KeepAspectRatioByExpanding); //Qt::KeepAspectRatio

}

void MainWindow::on_pauseButton_clicked()
{
    m_imageIO.Pause();
}

void MainWindow::on_playButton_clicked()
{
    m_imageIO.Play();
}

void MainWindow::on_trackerComboBox_currentIndexChanged(int index)
{
    boxTracker::trackertype type=static_cast<boxTracker::trackertype>(index);
    m_tracker.createNewTracker(type);
}


void MainWindow::on_framerate_editingFinished()
{
    QString strFrameRate = this->ui->framerate->text();
    m_imageIO.setFrameRate(strFrameRate.toInt());
}

void MainWindow::on_horizontCorrectionCheckBox_toggled(bool checked)
{
    if(ui->horizontDetectionCheckBox->isChecked() == false)
        ui->horizontCorrectionCheckBox->setChecked(false);
    sendPreProcConf();
}

void MainWindow::on_horizontDetectorTreshSlider_sliderMoved(int position)
{
    ui->horizontDetectorTreshSliderVal->setText(QString::number(ui->horizontDetectorTreshSlider->value()));
    sendPreProcConf();
}

void MainWindow::on_horizontDetectionCheckBox_toggled(bool checked)
{
    if(ui->horizontDetectionCheckBox->isChecked() == false)
        ui->horizontCorrectionCheckBox->setChecked(false);
    sendPreProcConf();
}

void MainWindow::sendPreProcConf()
{
    imgPreprocessing::preProcConf newConf;
    newConf.horizontGradientTresh = ui->horizontDetectorTreshSlider->value();
    newConf.useHorizontStabilitator = ui->horizontCorrectionCheckBox->isChecked();
    newConf.useHorizontDetector = ui->horizontDetectionCheckBox->isChecked();
    m_imgPreprocessor.setPreProcConf(newConf);
}
