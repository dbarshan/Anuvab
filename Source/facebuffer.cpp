#include "facebuffer.h"
#include <QString>
#include "global.h"

faceBuffer::faceBuffer()
{
}

void faceBuffer::init()
{
    frames.clear();
    cv::Mat t(xSize,ySize,CV_8UC1,cv::Scalar(0));
    for ( int i = 0 ; i<BUFFER_MAX_SIZE; i++)
        frames.push_back(t);

    size = BUFFER_MAX_SIZE;

}

void faceBuffer::push_back(cv::Mat &f)
{
    frames.erase(frames.begin());
    frames.push_back(f);
}

std::vector<cv::Mat> faceBuffer::peep()
{
    for ( int i=0; i< size; i++)
    {
        std::string name = QString::number(i).toStdString();
//        cv::imshow(name,frames.at(i));
    }
    return (frames);
}
void faceBuffer::clear()
{
    frames.clear();
}
