#ifndef IMAGEIO_H
#define IMAGEIO_H

#include <QObject>
#include <QString>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QMutex>
//#include <opencv2/imgproc/imgproc.hpp>

class ImageIO : public QThread
{
    Q_OBJECT
public:
    ImageIO();
    ~ImageIO();
    int loadVideo(QString path);
    void Play();
    void Pause();
    void setFrameRate(int i_frameRate);
    enum ctrlMsg{
        play,pause,stop,restart
    };

signals:
    void sendImage(cv::Mat newImage);
public slots:
    void receiveControlMsg(ImageIO::ctrlMsg ctrlMsgIn);
private:
    bool stopVideo;
    bool pauseVideo;
    QImage m_image;
    QMutex mutex;
    cv::VideoCapture video;
    int frameRate;
protected:
    void run();


};

#endif // IMAGEIO_H
