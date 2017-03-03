#ifndef LBPTOP_H
#define LBPTOP_H
#include <vector>
#include <opencv2/opencv.hpp>
#include "pattern.h"
class lbpTop
{
private:
    pattern p;
    cv::Point *nbrTable;
    cv::Mat fullFrameXY;
    cv::Mat fullFrameXT;
    cv::Mat fullFrameYT;

public:
    lbpTop();
    ~lbpTop();
    void init();
    cv::Mat getLBPu2Hist ( cv::Mat &frame, int P , int R1, int R2 );
    cv::Mat generateSampleData(std::vector<cv::Mat> &volume);
    void normalizeHist(cv::Mat &matG);
    cv::Mat getHistImg(const cv::Mat &hist);
    void getFullFrames(cv::Mat &xy, cv::Mat &xt, cv::Mat &yt);
};

#endif // LBPTOP_H
