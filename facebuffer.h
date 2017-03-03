#ifndef FACEBUFFER_H
#define FACEBUFFER_H
#include <opencv2/opencv.hpp>
#include <vector>

class faceBuffer
{
private:
    int size ;
    std::vector<cv::Mat> frames;

public:
    faceBuffer();
    void init();
    void push_back(cv::Mat &f);
    std::vector<cv::Mat> peep();
    void clear();

};

#endif // FACEBUFFER_H
