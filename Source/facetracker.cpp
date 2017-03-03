#include "facetracker.h"
#include "global.h"

faceTracker::faceTracker()
{
}

void faceTracker::init()
{
    cad.load(CASCADE_FILENAME);
}

cv::Rect faceTracker::detect(const cv::Mat &frame)
{
    cv::resize(frame,frameGray,cv::Size(frame.cols/2, frame.rows/2));
    cv::cvtColor(frameGray,frameGray,CV_BGR2GRAY);
    cad.detectMultiScale(frameGray,faceRegions,1.1,3,CV_HAAR_FIND_BIGGEST_OBJECT,cv::Size(30,30));
    if( faceRegions.empty())
    {
        return (cv::Rect(0,0,0,0));
    }
    cv::Rect roi= faceRegions.at(faceRegions.size()-1);
    roi &= cv::Rect(0,0,frameGray.cols-1,frameGray.rows-1);
    roi.x <<= 1; roi.y <<= 1;
    roi.width<<=1; roi.height<<=1;
    return (roi);
}
