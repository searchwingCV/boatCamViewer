#include "imageio.h"
#include <QMessageBox>

ImageIO::ImageIO() : QThread()
{
    stopVideo = false;
    pauseVideo = true;
}


int ImageIO::loadVideo(QString path)
{
    stopVideo = true;
    quit();
    pauseVideo = true;
    const std::string stdpath = path.toStdString();
    if(!video.open(stdpath))
    {
        return -1;
    }
    else{
        cv::Mat frame;
        if (!video.read(frame))
        {
            stopVideo = true;
        }
        if(!frame.empty())
        {
            cv::cvtColor(frame, frame, CV_BGR2RGB);
            //QImage qimg(frame.data,
            //            frame.cols,
            //            frame.rows,
            //            frame.step,
            //            QImage::Format_RGB888);
            emit sendImage(frame);
        }
        frameRate = (int) video.get(CV_CAP_PROP_FPS);
        return 0;
    }

}
void ImageIO::run()
{
    cv::Mat frame;
    while(video.isOpened()){
        if (!video.read(frame))
        {
            stopVideo = true;
        }
        if(!frame.empty())
        {
            cv::cvtColor(frame, frame, CV_BGR2RGB);
            //QImage qimg(frame.data,
            //            frame.cols,
            //            frame.rows,
            //            frame.step,
            //            QImage::Format_RGB888);
            emit sendImage(frame);
        }
        if (stopVideo){
            return;
        }
        while(pauseVideo){
            msleep(10);
        }
        msleep((1000/frameRate));
    }
}
void ImageIO::Play()
{
    if (video.isOpened()) {
        int currFramePos =video.get(CV_CAP_PROP_POS_FRAMES);
        int frameCount = video.get(CV_CAP_PROP_FRAME_COUNT);
        if (currFramePos == frameCount/2)
        {
            video.set(CV_CAP_PROP_POS_FRAMES,0);
        }
        stopVideo = false;
        pauseVideo = false;

    }
    else{

        stopVideo = false;
        pauseVideo = false;
    }
    if(!isRunning())
        start();
}
void ImageIO::Pause()
{
    pauseVideo = true;
}

void ImageIO::setFrameRate(int i_frameRate)
{
    if(i_frameRate < 0 )
        frameRate=(int) video.get(CV_CAP_PROP_FPS);
    frameRate = i_frameRate;
}


void ImageIO::receiveControlMsg(ImageIO::ctrlMsg ctrlMsgIn)
{
    switch (ctrlMsgIn) {
    case ImageIO::ctrlMsg::pause:
            pauseVideo = true;
        break;
    case ImageIO::ctrlMsg::play:
            pauseVideo = false;
        break;
    default:
        break;
    }
}

ImageIO::~ImageIO()
{
    mutex.lock();
    stopVideo = true;
    quit();
    mutex.unlock();

    wait();
}

