#ifndef GLOBAL_H
#define GLOBAL_H
#include "facetracker.h"
#include "facebuffer.h"
#include "lbptop.h"
#include "classifier.h"

extern faceTracker fd;
extern faceBuffer fb;
extern lbpTop l;
extern classifier c;

extern std::vector<cv::Mat> expr1Data;
extern std::vector<cv::Mat> expr2Data;
extern std::vector<cv::Mat> expr3Data;
extern std::vector<cv::Mat> expr4Data;
extern std::vector<cv::Mat> expr5Data;
extern std::vector<cv::Mat> expr6Data;
extern std::vector<cv::Mat> expr7Data;

extern int Pxy;
extern int Pxt;
extern int Pyt;
extern int Rx;
extern int Ry;
extern int Rt;

#define roiXinc 50
#define roiYinc 50
#define xSize 280
#define ySize 280
#define xBlocks 14
#define yBlocks 14
#define hInc 0
#define vInc 0
#define BUFFER_MAX_SIZE 10
#define MAX_EXPRESSIONS 2

#define default_Pxy 8
#define default_Pxt 8
#define default_Pyt 8
#define default_Rx 3
#define default_Ry 3
#define default_Rt 3

#define PATTERN_FILE_DIR "./patterns/"
#define CASCADE_FILENAME "./cascade/lbpcascade_frontalface.xml"
#define FEATURE_DIR   "./FeatureData/"
#define TRAIN_DIR       "./TrainData/"

#endif // GLOBAL_H
