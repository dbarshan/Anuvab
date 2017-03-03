#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;
class classifier
{
private:
    std::vector<cv::Mat> *exprData;
    std::string *exprNames;
    std::string *exprPaths;
    cv::SVM *svm;
    int numExpr;
    int svmCount;

public:
    classifier();
    void init(int n,std::string *names, std::string *paths);
    void loadFeature(std::vector<cv::Mat> &sample, std::string fileName);
    void train(const std::vector<cv::Mat> &posSample, const std::vector<cv::Mat> &negSample, cv::SVM &svm,const std::string &saveFileName);
    void learn();
    std::string predict(const cv::Mat &sampleData);
    std::string predict_bruteforce(const cv::Mat &s);
};

#endif // CLASSIFIER_H
