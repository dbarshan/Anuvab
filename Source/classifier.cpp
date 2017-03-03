#include "classifier.h"
#include "global.h"
#include <QString>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QDialog>
#include "ui_loadingdialog.h"
#include <QDir>

classifier::classifier()
{

}

void classifier::init(int n, std::string *names, std::string *paths)
{
    numExpr = n;
    svmCount = 0;
    for( int i=0; i<n ; i++)
        for ( int j = i+1 ; j<n; j++ )
            svmCount ++;
    svm = new cv::SVM[svmCount];
    exprNames = new std::string[numExpr];
    exprPaths = new std::string[numExpr];
    exprData = new std::vector<cv::Mat>[numExpr];
    for(int i=0; i<numExpr; i++)
    {
        exprNames[i] = names[i];
        exprPaths[i] = paths[i];
        loadFeature(exprData[i], exprPaths[i]);
    }
}

void classifier::loadFeature(std::vector<cv::Mat> &sample, std::string fileName)
{
    cv::FileStorage fs(fileName, cv::FileStorage::READ);
    int size;
    size = (int) fs["size"];
    for( int i=0;i<size;i++)
    {
        cv::Mat m;
        std::string name = "mat" + QString::number(i).toStdString();
        fs[name] >> m;
        sample.push_back(m);
    }
    fs.release();
}

void classifier::learn()
{
    QDir dir(TRAIN_DIR);
    if( !dir.exists())
    {
        QDir().mkdir(TRAIN_DIR);
    }
    for( int i=0, c = 0; i<numExpr; i++)
    {
        for( int j=i+1; j<numExpr; j++)
        {
            std::string saveFileName = TRAIN_DIR + exprNames[i]+"-"+exprNames[j]+".xml";
            train(exprData[i], exprData[j], svm[c], saveFileName);
            c++;
        }
    }
}

void classifier::train(const std::vector<cv::Mat> &posSample, const std::vector<cv::Mat> &negSample, cv::SVM &svm, const std::string &saveFileName)
{
    // Generalized train function
    int rowSize = posSample.size() + negSample.size();
    int bins    = (Pxy*(Pxy-1)+3)+(Pxt*(Pxt-1)+3)+(Pyt*(Pyt-1)+3);
    int colSize = bins*xBlocks*yBlocks;
    cv::Mat trainingDataMat ( rowSize , colSize, CV_32FC1,cv::Scalar(0));
    cv::Mat labelsMat       ( rowSize, 1, CV_32FC1);
    int sizeInc = 0 ;
    for ( unsigned i = 0 ; i< posSample.size(); i++ )
    {
        cv::Mat m = posSample.at(i);
        m.copyTo(trainingDataMat.row(i+sizeInc));
        labelsMat.at<float>(i+sizeInc) = 1.0 ;
    }
    sizeInc+= posSample.size();
    for ( unsigned i = 0 ; i< negSample.size(); i++ )
    {
        cv::Mat m = negSample.at(i);
        m.copyTo(trainingDataMat.row(i+sizeInc));
        labelsMat.at<float>(i+sizeInc) = -1.0 ;
    }
    // initialize svm parameters
    SVMParams params ;
    params.svm_type = SVM::C_SVC ;
    params.kernel_type = SVM ::LINEAR ;
    params.term_crit = TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,100, 1e-6);

    // training
    svm.train(trainingDataMat,labelsMat,Mat(),Mat(),params);
    svm.save(saveFileName.c_str()) ;
}

std::string classifier::predict(const cv::Mat &sampleData)
{
    int c= 0;
    int *vote = new int[numExpr];
    for( int i=0; i<numExpr; i++)
        vote[i] = 0;
    for( int i=0; i<numExpr; i++)
    {
        for(int j=i+1; j<numExpr; j++)
        {
            float p = svm[c].predict(sampleData);
            c++;
            if( p== 1.0 )       vote[i] ++;
            else                vote[j] ++;
        }
    }

    int max = 0;
    for( int i=1; i<numExpr; i++)
    {
        if( vote[i] > vote[max])
            max = i;
    }
    return(exprNames[max]);
}


std::string classifier::predict_bruteforce(const cv::Mat &s)
{
    // For expression =2
    if( numExpr == 2)
    {
        if( svm[0].predict(s) == 1.0)   return exprNames[0];
        else                            return exprNames[1];
    }
    // For expression = 3
    if( numExpr == 3)
    {
        if(svm[0].predict(s) == 1.0)
        {
            if( svm[1].predict(s) == 1.0)
            {
                return exprNames[0];
            }
            else
            {
                if( svm[2].predict(s)== 1.0)
                {
                    return expr[0];
                }
                else
                {
                    return exprNames[2];
                }
            }
        }
        else
        {
            if( svm[1].predict(s) == 1.0)
            {
                if( svm[2].predict(s) == 1.0)
                {
                    return exprNames[1];
                }
                else
                {
                    return "No Decession";
                }
            }
            else
            {
                if( svm[2].predict(s)==1.0)
                {
                    return exprNames[1];
                }
                else
                {
                    return exprNames[2];
                }
            }
        }
    }
    // For expression 4
    return " ";
}
