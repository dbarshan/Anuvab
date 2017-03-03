#include "mainwindow.h"
#include <QApplication>
#include "global.h"

faceTracker fd;
faceBuffer fb;
lbpTop l;
classifier c;

std::vector<cv::Mat> expr1Data;
std::vector<cv::Mat> expr2Data;
std::vector<cv::Mat> expr3Data;
std::vector<cv::Mat> expr4Data;
std::vector<cv::Mat> expr5Data;
std::vector<cv::Mat> expr6Data;
std::vector<cv::Mat> expr7Data;


int Pxy = default_Pxy ;
int Pxt = default_Pxt ;
int Pyt = default_Pyt ;

int Rx = default_Rx ;
int Ry = default_Ry ;
int Rt = default_Rt ;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    fd.init();
    fb.init();
    l.init();

    MainWindow w;
    w.show();

    return a.exec();
}
