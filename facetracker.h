#ifndef FACETRACKER_H
#define FACETRACKER_H
#include <opencv2/opencv.hpp>

class faceTracker
{
private:
    cv::CascadeClassifier cad;
    cv::Mat frameGray;
    std::vector<cv::Rect> faceRegions;

public:
    faceTracker();
    void init();
    cv::Rect detect(const cv::Mat &frame);

};

#endif // FACETRACKER_H
