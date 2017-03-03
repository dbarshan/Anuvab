#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QFileInfoList>
#include <QDir>

#include "lbptopsettings.h"
#include "exprpathsettings.h"
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void openExpressionSettings();
    void openLbpTopSettings();
    void openExtension();
    void closeExtension();
    void playPauseButtonPressed();
    void stopReplayButtonPressed();
    void grayScaleRGBButtonPressed();
    void flipImageButtonPressed();
    void trainButtonPressed();
    void testButtonPressed();
    void openCamera();
    void runCamera();
    void openVideo();
    void runVideo();
    void openImageSeq();
    void runImageSeq();
    void delayChanged();
    void prepareTrainingFeature();
    void processTrainingFeature();
    void savedFeatureTrain();
    void faceCursorOnOff();

signals:
    void processingFinished();

private:
    Ui::MainWindow *ui;
    lbpTopSettings *lbpTopSettingsWindow;
    exprPathSettings *exprPathSettingsWindow;
    bool drawerOpened;
    bool paused;
    bool grayImage;
    bool flipImage;
    bool stopReplay;
    bool testing;
    bool genSVMTrained;
    bool faceCursor;

    cv::VideoCapture cameraCap;
    cv::VideoCapture videoCap;
    cv::Mat frame;
    cv::Mat face;
    cv::Mat histImg;
    cv::Mat xy,xt,yt;
    cv::Mat sampleData;

    cv::SVM genSVM;
    std::vector<cv::Mat> fdata;
    std::vector<cv::Mat> trainFaceBuffer;

    QTimer *cameraTimer;
    QTimer *videoTimer;
    QTimer *imageTimer;
    QTimer *trainingTimer;

    QImage img;
    QImage frameXYImg;
    QImage frameXTImg;
    QImage frameYTImg;
    QImage qhistImg;

    QGraphicsScene *mainViewScene;
    QGraphicsScene *histogramViewScene;
    QString fileName;
    QString directoryPath;
    QFileInfoList fileInfoList;
    QDir directory, curDir;
    QFileInfoList dirList, imageFileNames;

    QString exprName;
    QString exprPath;


    int fileIndex;
    int fps;
    int delay;
    char mediaType;
    int trainFileIndex;
    int trainDirIndex;
    int searchDepth;
    int prcExprIndex;
    int genFeatureIndex;
};

#endif // MAINWINDOW_H
