#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTreeView>
#include <QMessageBox>
#include "global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    lbpTopSettingsWindow = new lbpTopSettings(this);
    exprPathSettingsWindow = new exprPathSettings();
    mainViewScene = new QGraphicsScene;
    ui->mainView->setScene(mainViewScene);
    testing = false;

    this->resize(685,640);
    drawerOpened = false;
    genSVMTrained = false;

    ui->closeDrawerButton->hide();
    ui->playPauseButton->setDisabled(true);
    ui->stopReplayButton->setDisabled(true);
    ui->grayscaleRGBButton->setDisabled(true);
    ui->flipImageButton->setDisabled(true);
    ui->delaySpinBox->setRange(0,10000);
    ui->statusBar->showMessage("No Media");
    ui->delayLabel->setBuddy(ui->delaySpinBox);
    ui->delaySpinBox->setDisabled(true);
    ui->actionFace_Cursor_On_Off->setText("Hide Face Rectangle");

    faceCursor = true;
    cameraTimer = new QTimer();
    videoTimer  = new QTimer();
    imageTimer =  new QTimer();
    trainingTimer = new QTimer();

    ui->trainButton->setDisabled(true);

    connect(ui->openDrawerButton,SIGNAL(clicked()),SLOT(openExtension()));
    connect(ui->closeDrawerButton,SIGNAL(clicked()),SLOT(closeExtension()));
    connect(ui->actionExpression_Settings,SIGNAL(triggered()),SLOT(openExpressionSettings()));
    connect(ui->actionLBP_TOP_Settings,SIGNAL(triggered()),SLOT(openLbpTopSettings()));
    connect(ui->flipImageButton,SIGNAL(pressed()),SLOT(flipImageButtonPressed()));
    connect(ui->grayscaleRGBButton,SIGNAL(pressed()),SLOT(grayScaleRGBButtonPressed()));
    connect(ui->playPauseButton,SIGNAL(pressed()),SLOT(playPauseButtonPressed()));
    connect(ui->actionOpen_Camera,SIGNAL(triggered()),SLOT(openCamera()));
    connect(cameraTimer,SIGNAL(timeout()),SLOT(runCamera()));
    connect(ui->stopReplayButton,SIGNAL(pressed()),SLOT(stopReplayButtonPressed()));
    connect(ui->actionOpen_Video_File,SIGNAL(triggered()),SLOT(openVideo()));
    connect(videoTimer,SIGNAL(timeout()),SLOT(runVideo()));
    connect(ui->actionOpen_Image_Set,SIGNAL(triggered()),SLOT(openImageSeq()));
    connect(imageTimer,SIGNAL(timeout()),SLOT(runImageSeq()));
    connect(ui->delaySpinBox,SIGNAL(valueChanged(int)),SLOT(delayChanged()));
    connect(ui->testButton,SIGNAL(clicked()),SLOT(testButtonPressed()));
    connect(ui->trainButton,SIGNAL(clicked()),SLOT(trainButtonPressed()));
    connect(trainingTimer,SIGNAL(timeout()),SLOT(prepareTrainingFeature()));
    connect(this,SIGNAL(processingFinished()),SLOT(processTrainingFeature()));
    connect(ui->savedFeatureTrain,SIGNAL(clicked()),SLOT(savedFeatureTrain()));
    connect(ui->actionFace_Cursor_On_Off,SIGNAL(triggered()),SLOT(faceCursorOnOff()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete lbpTopSettingsWindow;
    delete exprPathSettingsWindow;
    delete cameraTimer;
    delete videoTimer;
    delete imageTimer;
}

void MainWindow::openExpressionSettings()
{
//    expressionSettingsWindow->show();
    exprPathSettingsWindow->show();
}
void MainWindow::openLbpTopSettings()
{
    lbpTopSettingsWindow->show();
}
void MainWindow::openExtension()
{
    ui->openDrawerButton->hide();
    ui->closeDrawerButton->show();
    resize(1230,640);
}
void MainWindow::closeExtension()
{
    ui->openDrawerButton->show();
    ui->closeDrawerButton->hide();
    resize(685,640);
}

void MainWindow::grayScaleRGBButtonPressed()
{
    grayImage = !grayImage;
    if( grayImage)
        ui->grayscaleRGBButton->setText("RGB");
    else
        ui->grayscaleRGBButton->setText("Gray");
}

void MainWindow::flipImageButtonPressed()
{
    flipImage = !flipImage;
}

void MainWindow::prepareTrainingFeature()
{
    if( trainDirIndex == -1)
    {
        dirList = directory.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
        if( dirList.size()>0)
        {
            trainDirIndex = 0;
            trainFileIndex = -1;
        }
        else
        {
            emit processingFinished();
            return;
        }
    }

    if( trainFileIndex == -1)
    {
        curDir = QDir(dirList.at(trainDirIndex).absoluteFilePath());
        trainDirIndex++;
        imageFileNames = curDir.entryInfoList(QStringList("*.png"));
        if( imageFileNames.size() > 0)
        {
            trainFileIndex = 0;
            trainFaceBuffer.clear();
        }
        else
        {
            trainFileIndex = -1;
            return;
        }
    }
    frame = cv::imread(imageFileNames.at(trainFileIndex).absoluteFilePath().toStdString());
    if( flipImage)
    {
        cv::flip(frame,frame,1);
    }
    cv::Rect roi = fd.detect(frame);
    if( grayImage)
    {
        cv::cvtColor(frame,frame,CV_BGR2GRAY);
        cv::cvtColor(frame,frame,CV_GRAY2BGR);
    }
    if( roi.area()>0)
    {
        frame(roi).copyTo(face);
        cv::cvtColor(face,face,CV_BGR2GRAY);
        cv::resize(face,face,cv::Size(280,280));
        trainFaceBuffer.push_back(face);
        cv::rectangle(frame,roi,cv::Scalar(0,255,0),2);
    }
    trainFileIndex++;
    img = QImage( (const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    img = img.rgbSwapped() ;

    mainViewScene->clear();
    mainViewScene->setSceneRect( 0 , 0 , frame.cols , frame.rows);
    mainViewScene->addPixmap( QPixmap :: fromImage(img));

    if( trainFileIndex == (imageFileNames.size()/2))
    {
        cv::Mat featureData = l.generateSampleData(trainFaceBuffer);
        l.getFullFrames(xy,xt,yt);
        cv::resize(xy,xy,cv::Size(150,150));
        frameXYImg = QImage( (const uchar*)xy.data, xy.cols, xy.rows, xy.step, QImage::Format_Indexed8);
        ui->frameXYView->setPixmap(QPixmap::fromImage(frameXYImg));
        cv::resize(xt,xt,cv::Size(150,150));
        frameXTImg = QImage( (const uchar*)xt.data, xt.cols, xt.rows, xt.step, QImage::Format_Indexed8);
        ui->frameXTView->setPixmap(QPixmap::fromImage(frameXTImg));
        cv::resize(yt,yt,cv::Size(150,150));
        frameYTImg = QImage( (const uchar*)yt.data, yt.cols, yt.rows, yt.step, QImage::Format_Indexed8);
        ui->frameYTView->setPixmap(QPixmap::fromImage(frameYTImg));

        if(prcExprIndex-1 == 1)         expr1Data.push_back(featureData);
        if(prcExprIndex-1 == 2)         expr2Data.push_back(featureData);
        genFeatureIndex++;
        ui->statusBar->showMessage(exprName + " : " + QString::number(genFeatureIndex) + " Feature(s) Generated." );

        histImg = l.getHistImg(featureData);
        cv::resize(histImg,histImg,cv::Size(510,170));
        qhistImg = QImage( (const uchar*)histImg.data, histImg.cols, histImg.rows, histImg.step, QImage::Format_Indexed8);
        ui->histogramView->setPixmap(QPixmap::fromImage(qhistImg));
        trainFaceBuffer.clear();
    }

    if( trainFileIndex == imageFileNames.size())
    {
        cv::Mat featureData = l.generateSampleData(trainFaceBuffer);
        l.getFullFrames(xy,xt,yt);
        cv::resize(xy,xy,cv::Size(150,150));
        frameXYImg = QImage( (const uchar*)xy.data, xy.cols, xy.rows, xy.step, QImage::Format_Indexed8);
        ui->frameXYView->setPixmap(QPixmap::fromImage(frameXYImg));
        cv::resize(xt,xt,cv::Size(150,150));
        frameXTImg = QImage( (const uchar*)xt.data, xt.cols, xt.rows, xt.step, QImage::Format_Indexed8);
        ui->frameXTView->setPixmap(QPixmap::fromImage(frameXTImg));
        cv::resize(yt,yt,cv::Size(150,150));
        frameYTImg = QImage( (const uchar*)yt.data, yt.cols, yt.rows, yt.step, QImage::Format_Indexed8);
        ui->frameYTView->setPixmap(QPixmap::fromImage(frameYTImg));

        if( prcExprIndex-1 == 1)  expr1Data.push_back(featureData);
        if( prcExprIndex-1 == 2)  expr2Data.push_back(featureData);
        genFeatureIndex++;
        ui->statusBar->showMessage(exprName + " : " + QString::number(genFeatureIndex) + " Feature(s) Generated." );
        histImg = l.getHistImg(featureData);
        cv::resize(histImg,histImg,cv::Size(510,170));
        qhistImg = QImage( (const uchar*)histImg.data, histImg.cols, histImg.rows, histImg.step, QImage::Format_Indexed8);
        ui->histogramView->setPixmap(QPixmap::fromImage(qhistImg));
        if( trainDirIndex == searchDepth)
        {
            trainingTimer->stop();
            emit processingFinished();
            return;
        }
        trainFileIndex = -1;
    }
}

void MainWindow::processTrainingFeature()
{

    if( prcExprIndex <= MAX_EXPRESSIONS)
    {
        genFeatureIndex = 0;
        exprPath = exprPathSettingsWindow->getExprPath(prcExprIndex);
        exprName = exprPathSettingsWindow->getExprName(prcExprIndex);
        ui->statusBar->showMessage("Extracting Feature for " + exprName);
        prcExprIndex++;
        directory = QDir(exprPath);
        fdata.clear();
        trainDirIndex = -1;
        trainFileIndex = -1;
        fdata.clear();
        trainingTimer->setInterval(30);
        trainingTimer->start();
    }
    else
    {
        ui->statusBar->showMessage("Feature Extraction Complete");

        int rowSize = expr1Data.size() + expr2Data.size();
        int bins    = (Pxy*(Pxy-1)+3)+(Pxt*(Pxt-1)+3)+(Pyt*(Pyt-1)+3);
        int colSize = bins*xBlocks*yBlocks;
        cv::Mat trainingDataMat ( rowSize , colSize, CV_32FC1,cv::Scalar(0));
        cv::Mat labelsMat       ( rowSize, 1, CV_32FC1,cv::Scalar(0));
        int sizeInc = 0 ;
        for ( unsigned i = 0 ; i< expr1Data.size(); i++ )
        {
            cv::Mat m = expr1Data.at(i);
            m.copyTo(trainingDataMat.row(i+sizeInc));
            labelsMat.at<float>(i+sizeInc) = 1.0 ;
        }
        sizeInc+= expr1Data.size();
        for ( unsigned i = 0 ; i< expr2Data.size(); i++ )
        {
            Mat m = expr2Data.at(i);
            m.copyTo(trainingDataMat.row(i+sizeInc));
            labelsMat.at<float>(i+sizeInc) = -1.0 ;
        }

        // initialize svm parameters
        cv::SVMParams params ;
        params.svm_type = SVM::C_SVC ;
        params.kernel_type = SVM ::LINEAR ;
        params.term_crit = TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,100, 1e-6);

        // training

        genSVM.train(trainingDataMat,labelsMat,Mat(),Mat(),params);
        std::string saveFileName = TRAIN_DIR "expr1-expr2.xml";
        genSVM.save(saveFileName.c_str());
        genSVMTrained = true;
        ui->statusBar->showMessage("Training Comeplete");
    }
}

void MainWindow::trainButtonPressed()
{
    int n = exprPathSettingsWindow->getNumExpr();
    if( n < 2 )
    {
        QMessageBox::information(this,"Expression Name And Path Not Set","Atleast two expression name and path should be set");
        openExpressionSettings();
        return;
    }
    stopReplayButtonPressed();
    prcExprIndex = 1;
    searchDepth = 30;
    flipImage = false;
    grayImage = false;

    emit processingFinished();
}


void MainWindow::savedFeatureTrain()
{
    QFile file(FEATURE_DIR "index.xml");
    if(!file.exists())
    {
        QMessageBox::critical(this,"Error","Can Not Train.\nIndex File Not Found");
        return;
    }
    file.close();
    cv::FileStorage fs(FEATURE_DIR "index.xml", FileStorage::READ);
    int numExpr = (int) fs["expressions"];
    std::string *exprNames = new std::string[numExpr];
    std::string *exprPaths = new std::string[numExpr];
    for( int i=0;i<numExpr;i++)
    {
        string tag = "expression" + QString::number(i+1).toStdString();
        fs[tag] >> exprNames[i];
        exprPaths[i] = FEATURE_DIR + exprNames[i] + "Feature.xml";
    }
    fs.release();
    for( int i=0;i<numExpr;i++)
    {
        QFile file(exprPaths[i].c_str());
        if( !file.exists())
        {
            QString msg = QString(exprPaths[i].c_str()) + "\nnot found" ;
            QMessageBox::critical(this,"Error",msg);
            return;
        }
    }

    c.init(numExpr,exprNames,exprPaths);
    ui->statusBar->showMessage("Features loaded");
    c.learn();

    genSVM.load("./TrainData/expr1-expr2.xml");
    genSVMTrained = true;
//    genSVM.load("C:/Users/cssc/Documents/Barshan/newLBPTOP/TrainData/Anger-Joy.xml");
    QMessageBox::information(this,"Message","Training Complete");
}


void MainWindow::testButtonPressed()
{
    if(!videoTimer->isActive()&& !cameraTimer->isActive() && !imageTimer->isActive())
    {
        QMessageBox::critical(this,"Error", "Can Not Start Expression Recognition.\nNo Media Loaded.");
        return;
    }
    if( genSVMTrained)
    {
        testing = true;
    }
    else
    {
        QMessageBox::critical(this,"Error", "Binary SVM not trained");
    }
}

void MainWindow::openCamera()
{
    ui->statusBar->showMessage("Openeing Camera....");

    if( cameraCap.isOpened())
    {
        cameraTimer->stop();
        cameraCap.release();
    }
    if( videoCap.isOpened())
    {
        videoTimer->stop();
        videoCap.release();
    }
    if( trainingTimer->isActive())
    {
        trainingTimer->stop();
    }
    cameraCap.open(0);
    delay = 10;

    int cols = videoCap.get(CV_CAP_PROP_FRAME_WIDTH);
    int rows = videoCap.get(CV_CAP_PROP_FRAME_HEIGHT);
    mainViewScene->setSceneRect(0,0,cols,rows);
    ui->delaySpinBox->setValue(delay);
    ui->grayscaleRGBButton->setEnabled(true);
    ui->flipImageButton->setEnabled(true);
    ui->playPauseButton->setEnabled(true);
    ui->playPauseButton->setText("Pause");
    ui->grayscaleRGBButton->setText("Gray");
    ui->stopReplayButton->setEnabled(true);
    ui->delaySpinBox->setEnabled(true);
    paused = false;
    grayImage = false;
    flipImage = false;
    mediaType = 'c';
    stopReplay = true;
    ui->statusBar->showMessage("Camera Opened");
    ui->stopReplayButton->setText("Stop");
    fb.init();
    cameraTimer->setInterval(delay);
    cameraTimer->start();

}
void MainWindow::openVideo()
{

    fileName = QFileDialog :: getOpenFileName ( this, tr ("Open Video"), ".","Video files(*.avi *.wmv *.flv *.mp4 *.MOV *.webm *.mpg)");
    if( !fileName.length ( )) return;
    ui->statusBar->showMessage("Opening Video File...");
    if( cameraCap.isOpened())
    {
        cameraTimer->stop();
        cameraCap.release();
    }
    if( videoCap.isOpened())
    {
        videoTimer->stop();
        videoCap.release();
    }
    if ( trainingTimer->isActive())
    {
        trainingTimer->stop();
    }
    videoCap.open (fileName.toStdString());
    fps =  videoCap.get( CV_CAP_PROP_FPS) ;
    delay = int (1000 *1.0 / fps )  ;

    ui->delaySpinBox->setValue(delay);
    ui->grayscaleRGBButton->setEnabled(true);
    ui->flipImageButton->setEnabled(true);
    ui->playPauseButton->setEnabled(true);
    ui->playPauseButton->setText("Pause");
    ui->grayscaleRGBButton->setText("Gray");
    ui->stopReplayButton->setEnabled(true);
    ui->delaySpinBox->setEnabled(true);
    int cols = videoCap.get(CV_CAP_PROP_FRAME_WIDTH);
    int rows = videoCap.get(CV_CAP_PROP_FRAME_HEIGHT);
    mainViewScene->setSceneRect(0,0,cols,rows);
    paused = false;
    grayImage = false;
    flipImage = false;
    stopReplay = true;
    mediaType = 'v';
    fb.init();
    ui->statusBar->showMessage("Video File Opened");
    ui->stopReplayButton->setText("Stop");
    videoTimer->setInterval(delay);
    videoTimer->start();
}

void MainWindow::openImageSeq()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild <QTreeView*>();
    tree->setRootIsDecorated(true);
    tree->setItemsExpandable(true);
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    int result = fd->exec();
    if (!result)
        return;
    directoryPath = fd->selectedFiles()[0];
    QDir dir(directoryPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

    if( fileInfoList.empty())
    {
        QMessageBox::critical(this, "Error","No Image File Found");
        return;
    }
    fileIndex = 0;
    if( cameraCap.isOpened())
    {
        cameraTimer->stop();
        cameraCap.release();
    }
    if( videoCap.isOpened())
    {
        videoTimer->stop();
        videoCap.release();
    }
    if( trainingTimer->isActive())
    {
        trainingTimer->stop();
    }
    delay = 250;

    ui->delaySpinBox->setValue(delay);
    ui->grayscaleRGBButton->setEnabled(true);
    ui->flipImageButton->setEnabled(true);
    ui->playPauseButton->setEnabled(true);
    ui->playPauseButton->setText("Pause");
    ui->grayscaleRGBButton->setText("Gray");
    ui->stopReplayButton->setEnabled(true);
    ui->delaySpinBox->setEnabled(true);
    paused = false;
    grayImage = false;
    flipImage = false;
    stopReplay = true;
    mediaType = 'i';

    ui->stopReplayButton->setText("Stop");

    imageTimer->setInterval(delay);
    imageTimer->start();
}

void MainWindow::runCamera()
{
    cameraCap.read(frame);
    if( frame.empty())
    {
        return;
    }

    if( flipImage)
    {
        cv::flip(frame,frame,1);
    }
    cv::Rect roi = fd.detect(frame);
    if( grayImage)
    {
        cv::cvtColor(frame,frame,CV_BGR2GRAY);
        cv::cvtColor(frame,frame,CV_GRAY2BGR);
    }
    if( roi.area()!= 0)
    {
        frame(roi).copyTo(face);
        cv::resize(face,face,cv::Size(280,280));
        cv::cvtColor(face,face,CV_BGR2GRAY);
        fb.push_back(face);
        if( faceCursor)
            cv::rectangle(frame,roi,cv::Scalar(0,255,0),2);
        std::vector<cv::Mat> volume = fb.peep();
        sampleData= l.generateSampleData(volume);

        // buffer show part
        {
            int s = volume.size();
            int stp = s/5;
            cv::Mat m;
            QImage mImg;

            for( int i=0; i<5; i++)
            {
                volume.at(s-i*stp-1).copyTo(m);
                cv::resize(m,m,cv::Size(100,100));
                mImg= QImage((const uchar*)m.data, m.cols, m.rows, m.step, QImage::Format_Indexed8);
                switch(i)
                {
                case 0: ui->buffer5->setPixmap(QPixmap::fromImage(mImg));    break;
                case 1: ui->buffer4->setPixmap(QPixmap::fromImage(mImg));    break;
                case 2: ui->buffer3->setPixmap(QPixmap::fromImage(mImg));    break;
                case 3: ui->buffer2->setPixmap(QPixmap::fromImage(mImg));    break;
                case 4: ui->buffer1->setPixmap(QPixmap::fromImage(mImg));    break;
                }
            }
            m.release();
        }

        l.getFullFrames(xy,xt,yt);
        cv::resize(xy,xy,cv::Size(150,150));
        frameXYImg = QImage( (const uchar*)xy.data, xy.cols, xy.rows, xy.step, QImage::Format_Indexed8);
        ui->frameXYView->setPixmap(QPixmap::fromImage(frameXYImg));
        cv::resize(xt,xt,cv::Size(150,150));
        frameXTImg = QImage( (const uchar*)xt.data, xt.cols, xt.rows, xt.step, QImage::Format_Indexed8);
        ui->frameXTView->setPixmap(QPixmap::fromImage(frameXTImg));
        cv::resize(yt,yt,cv::Size(150,150));
        frameYTImg = QImage( (const uchar*)yt.data, yt.cols, yt.rows, yt.step, QImage::Format_Indexed8);
        ui->frameYTView->setPixmap(QPixmap::fromImage(frameYTImg));

        histImg = l.getHistImg(sampleData);
        cv::resize(histImg,histImg,cv::Size(510,170));
        qhistImg = QImage( (const uchar*)histImg.data, histImg.cols, histImg.rows, histImg.step, QImage::Format_Indexed8);
        ui->histogramView->setPixmap(QPixmap::fromImage(qhistImg));

        if( testing)
        {
            std::string emo = c.predict_bruteforce(sampleData);
            cv::putText(frame,emo,roi.tl(),CV_FONT_HERSHEY_TRIPLEX,1,Scalar(0,0,255),2);
            std::string emo2 = c.predict(sampleData);
            cv::putText(frame,emo2,roi.br(),CV_FONT_HERSHEY_TRIPLEX,1,Scalar(0,0,255),2);
//            float p = genSVM.predict(sampleData);
//            if ( p == 1.0)
//            {
//                cv::putText(frame,"Anger",roi.tl(),CV_FONT_HERSHEY_TRIPLEX,1,Scalar(0,0,255),2);
//            }
//            else
//            {
//                cv::putText(frame,"Joy",roi.tl(),CV_FONT_HERSHEY_TRIPLEX,1,Scalar(0,0,255),2);
//            }
        }
    }


    img = QImage( (const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    img = img.rgbSwapped() ;

    mainViewScene->clear();
    mainViewScene->addPixmap( QPixmap :: fromImage(img));
    ui->statusBar->showMessage("Camera running");
}

void MainWindow::runVideo()
{
    videoCap.read(frame);
    if( frame.empty())
    {
        videoTimer->stop();
        stopReplayButtonPressed();
        return;
    }

    if( flipImage)
    {
        cv::flip(frame,frame,1);
    }
    cv::Rect roi = fd.detect(frame);
    if( grayImage)
    {
        cv::cvtColor(frame,frame,CV_BGR2GRAY);
        cv::cvtColor(frame,frame,CV_GRAY2BGR);
    }
    if( roi.area() > 0)
    {
        frame(roi).copyTo(face);
        cv::resize(face,face,cv::Size(280,280));
        cv::cvtColor(face,face,CV_BGR2GRAY);
        fb.push_back(face);
        if( faceCursor)
            cv::rectangle(frame,roi,cv::Scalar(0,255,0),2);
        std::vector<cv::Mat> volume = fb.peep();
        sampleData= l.generateSampleData(volume);

        // buffer show part
        {
            int s = volume.size();
            int stp = s/5;
            cv::Mat m;
            QImage mImg;

            for( int i=0; i<5; i++)
            {
                volume.at(s-i*stp-1).copyTo(m);
                cv::resize(m,m,cv::Size(100,100));
                mImg= QImage((const uchar*)m.data, m.cols, m.rows, m.step, QImage::Format_Indexed8);
                switch(i)
                {
                case 0: ui->buffer5->setPixmap(QPixmap::fromImage(mImg));    break;
                case 1: ui->buffer4->setPixmap(QPixmap::fromImage(mImg));    break;
                case 2: ui->buffer3->setPixmap(QPixmap::fromImage(mImg));    break;
                case 3: ui->buffer2->setPixmap(QPixmap::fromImage(mImg));    break;
                case 4: ui->buffer1->setPixmap(QPixmap::fromImage(mImg));    break;
                }
            }
            m.release();
        }


        l.getFullFrames(xy,xt,yt);
        cv::resize(xy,xy,cv::Size(150,150));
        frameXYImg = QImage( (const uchar*)xy.data, xy.cols, xy.rows, xy.step, QImage::Format_Indexed8);
        ui->frameXYView->setPixmap(QPixmap::fromImage(frameXYImg));
        cv::resize(xt,xt,cv::Size(150,150));
        frameXTImg = QImage( (const uchar*)xt.data, xt.cols, xt.rows, xt.step, QImage::Format_Indexed8);
        ui->frameXTView->setPixmap(QPixmap::fromImage(frameXTImg));
        cv::resize(yt,yt,cv::Size(150,150));
        frameYTImg = QImage( (const uchar*)yt.data, yt.cols, yt.rows, yt.step, QImage::Format_Indexed8);
        ui->frameYTView->setPixmap(QPixmap::fromImage(frameYTImg));

        histImg = l.getHistImg(sampleData);
        cv::resize(histImg,histImg,cv::Size(510,170));
        qhistImg = QImage( (const uchar*)histImg.data, histImg.cols, histImg.rows, histImg.step, QImage::Format_Indexed8);
        ui->histogramView->setPixmap(QPixmap::fromImage(qhistImg));

        if( testing)
        {
            std::string emo = c.predict_bruteforce(sampleData);
            cv::putText(frame,emo,roi.tl(),CV_FONT_HERSHEY_TRIPLEX,1,Scalar(0,0,255),2);
            std::string emo2 = c.predict(sampleData);
            cv::putText(frame,emo2,roi.br(),CV_FONT_HERSHEY_TRIPLEX,1,Scalar(0,0,255),2);
        }
    }

    img = QImage( (const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    img = img.rgbSwapped() ;

    mainViewScene->clear();
    mainViewScene->addPixmap( QPixmap :: fromImage(img));
    ui->statusBar->showMessage("Video playing");
}

void MainWindow::runImageSeq()
{
    frame = cv::imread(fileInfoList.at(fileIndex).absoluteFilePath().toStdString());
    if( flipImage)
    {
        cv::flip(frame,frame,1);
    }
    cv::Rect roi = fd.detect(frame);
    if( grayImage)
    {
        cv::cvtColor(frame,frame,CV_BGR2GRAY);
        cv::cvtColor(frame,frame,CV_GRAY2BGR);
    }
    if( roi.area()>0)
    {
        frame(roi).copyTo(face);
        cv::resize(face,face,cv::Size(280,280));
        cv::cvtColor(face,face,CV_BGR2GRAY);
        fb.push_back(face);
        if( faceCursor)
            cv::rectangle(frame,roi,cv::Scalar(0,255,0),2);

        std::vector<cv::Mat> volume = fb.peep();
        cv::Mat sampleData= l.generateSampleData(volume);

        // buffer show part
        {
            int s = volume.size();
            int stp = s/5;
            cv::Mat m;
            QImage mImg;

            for( int i=0; i<5; i++)
            {
                volume.at(s-i*stp-1).copyTo(m);
                cv::resize(m,m,cv::Size(100,100));
                mImg= QImage((const uchar*)m.data, m.cols, m.rows, m.step, QImage::Format_Indexed8);
                switch(i)
                {
                case 0: ui->buffer5->setPixmap(QPixmap::fromImage(mImg));    break;
                case 1: ui->buffer4->setPixmap(QPixmap::fromImage(mImg));    break;
                case 2: ui->buffer3->setPixmap(QPixmap::fromImage(mImg));    break;
                case 3: ui->buffer2->setPixmap(QPixmap::fromImage(mImg));    break;
                case 4: ui->buffer1->setPixmap(QPixmap::fromImage(mImg));    break;
                }
            }
            m.release();
        }

        l.getFullFrames(xy,xt,yt);
        cv::resize(xy,xy,cv::Size(150,150));
        frameXYImg = QImage( (const uchar*)xy.data, xy.cols, xy.rows, xy.step, QImage::Format_Indexed8);
        ui->frameXYView->setPixmap(QPixmap::fromImage(frameXYImg));
        cv::resize(xt,xt,cv::Size(150,150));
        frameXTImg = QImage( (const uchar*)xt.data, xt.cols, xt.rows, xt.step, QImage::Format_Indexed8);
        ui->frameXTView->setPixmap(QPixmap::fromImage(frameXTImg));
        cv::resize(yt,yt,cv::Size(150,150));
        frameYTImg = QImage( (const uchar*)yt.data, yt.cols, yt.rows, yt.step, QImage::Format_Indexed8);
        ui->frameYTView->setPixmap(QPixmap::fromImage(frameYTImg));

        histImg = l.getHistImg(sampleData);
        cv::resize(histImg,histImg,cv::Size(510,170));
        qhistImg = QImage( (const uchar*)histImg.data, histImg.cols, histImg.rows, histImg.step, QImage::Format_Indexed8);
        ui->histogramView->setPixmap(QPixmap::fromImage(qhistImg));

        if( testing)
        {
            std::string emo = c.predict_bruteforce(sampleData);
            cv::putText(frame,emo,roi.tl(),CV_FONT_HERSHEY_TRIPLEX,1,Scalar(0,0,255),2);
            std::string emo2 = c.predict(sampleData);
            cv::putText(frame,emo2,roi.br(),CV_FONT_HERSHEY_TRIPLEX,1,Scalar(0,0,255),2);
        }


    }
    img = QImage( (const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    img = img.rgbSwapped() ;

    mainViewScene->clear();
    mainViewScene->setSceneRect( 0 , 0 , frame.cols , frame.rows);
    mainViewScene->addPixmap( QPixmap :: fromImage(img));

    if( fileIndex == fileInfoList.size()-1)
    {
        imageTimer->stop();
        stopReplayButtonPressed();
    }
    else
    {
        fileIndex++;
        ui->statusBar->showMessage("Showing Image Sequence");
    }
}

void MainWindow::playPauseButtonPressed()
{
    paused = !paused;
    if( paused )
    {
        ui->flipImageButton->setDisabled(true);
        ui->grayscaleRGBButton->setDisabled(true);
        if (mediaType == 'c')
        {
            cameraTimer->stop();
        }
        else if (mediaType == 'v')
        {
            videoTimer->stop();
        }
        else if( mediaType == 'i')
        {
            imageTimer->stop();
        }
        ui->playPauseButton->setText("Play");
        ui->statusBar->showMessage("Paused");
    }
    else
    {
        ui->flipImageButton->setEnabled(true);
        ui->grayscaleRGBButton->setEnabled(true);
        if( mediaType == 'c')
            cameraTimer->start();
        else if ( mediaType == 'v')
            videoTimer->start();
        else if ( mediaType == 'i')
            imageTimer->start();
        ui->playPauseButton->setText("Pause");
        ui->statusBar->showMessage("Playing");
    }
}

void MainWindow::stopReplayButtonPressed()
{
    stopReplay = !stopReplay;
    if ( stopReplay)
    {
        if(mediaType == 'v')
        {
            videoCap.release();
            videoCap.open(fileName.toStdString());
            int cols = videoCap.get(CV_CAP_PROP_FRAME_WIDTH);
            int rows = videoCap.get(CV_CAP_PROP_FRAME_HEIGHT);
            mainViewScene->setSceneRect(0,0,cols,rows);
            ui->stopReplayButton->setText("Stop");
            videoTimer->start();


        }
        else if ( mediaType == 'i')
        {
            fileIndex = 0;
            imageTimer->start();
            ui->stopReplayButton->setText("Stop");
        }
        ui->grayscaleRGBButton->setEnabled(true);
        ui->flipImageButton->setEnabled(true);
        ui->playPauseButton->setEnabled(true);
        ui->playPauseButton->setText("Pause");
        ui->grayscaleRGBButton->setText("Gray");
        paused = false;
        grayImage = false;
        flipImage = false;
        ui->statusBar->showMessage("Replay");
    }
    else
    {

        if( cameraCap.isOpened())
        {
            cameraTimer->stop();
            cameraCap.release();
            ui->stopReplayButton->setText("Stop / Replay");
            ui->stopReplayButton->setDisabled(true);

        }
        else if( videoCap.isOpened())
        {
            videoTimer->stop();
            videoCap.release();
            ui->stopReplayButton->setText("Replay");
        }
        else if ( mediaType == 'i')
        {
            imageTimer->stop();
            ui->stopReplayButton->setText("Replay");

        }
//        mainViewScene->setSceneRect(0,0,640,480);
//        mainViewScene->clear();
//        ui->histogramView->clear();

        ui->playPauseButton->setText("Play / Pause");
        ui->grayscaleRGBButton->setText("Gray / RGB");
        ui->playPauseButton->setDisabled(true);
        ui->grayscaleRGBButton->setDisabled(true);
        ui->flipImageButton->setDisabled(true);
        ui->delaySpinBox->setDisabled(true);
        ui->statusBar->showMessage("Stopped");
    }
}

void MainWindow::delayChanged()
{
    delay = ui->delaySpinBox->value();
    if ( mediaType == 'v')
        videoTimer->setInterval(delay);
    else if ( mediaType == 'c')
        cameraTimer->setInterval(delay);
    else if ( mediaType == 'i')
        imageTimer->setInterval(delay);
}

void MainWindow::faceCursorOnOff()
{
    faceCursor = !faceCursor;
    if( faceCursor)
    {
        ui->actionFace_Cursor_On_Off->setText("Hide Face Rectangle");
    }
    else
    {
        ui->actionFace_Cursor_On_Off->setText("Show Face Rectangle");
    }
}
