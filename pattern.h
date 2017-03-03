#ifndef PATTERN_H
#define PATTERN_H
#include <opencv2/opencv.hpp>
using namespace cv;
class pattern
{
private:
    cv::Mat u2Pattern;
    int bits;
public:
    pattern();
    void load();
    int indexOf(unsigned V);
};
#endif // PATTERN_HPP
