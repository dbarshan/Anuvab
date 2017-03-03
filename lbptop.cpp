#include "lbptop.h"
#include "global.h"
#define ELEM(start,step,xpos,ypos) (*((uchar*)(start+step*(ypos)+(xpos)*sizeof(uchar))))

lbpTop::lbpTop()
{
    nbrTable = new Point[Pxy];
}

void lbpTop::init()
{
    nbrTable[0] = Point(1,0);
    nbrTable[1] = Point(1,-1);
    nbrTable[2] = Point(0,-1);
    nbrTable[3] = Point(-1,-1);
    nbrTable[4] = Point(-1,0);
    nbrTable[5] = Point(-1,1);
    nbrTable[6] = Point(0,1);
    nbrTable[7] = Point(1,1);
    p.load();
}

lbpTop::~lbpTop()
{
    delete nbrTable;
}

cv::Mat lbpTop :: getLBPu2Hist ( cv::Mat &frame, int P , int R1, int R2 )
{
    unsigned bins = unsigned ( P*( P - 1 ) + 3 );
    cv::Mat LBPu2Hist(1, bins, CV_32FC1,Scalar(0) );
    for ( int yc = R2 ; yc < frame.rows -R2 ; yc ++ )
    {
        for( int xc = R1 ; xc < frame.cols -R1 ; xc++ )
        {
            unsigned char thresh = ELEM(frame.data, frame.step, xc , yc);
            unsigned V = 0;
            for ( int i = P- 1 ; i >=0 ; i-- )
            {
                int xp = xc +  int ( R1 * nbrTable[i].x);
                int yp = yc +  int ( R2 * nbrTable[i].y) ;
                bool vp = (ELEM(frame.data, frame.step, xp , yp) >= thresh ? 1 : 0 );
                V = (V <<  1 ) | int(vp) ;
            }
            int i  = p.indexOf (V);
            if ( i == -1 )
                 (*((float*)(LBPu2Hist.data+(bins-1)*sizeof(float))))++;
            else
                (*((float*)(LBPu2Hist.data+(i)*sizeof(float))))++;
        }
    }
    return LBPu2Hist ;
}

void lbpTop::normalizeHist(cv::Mat &matG)
{
    float sum = 0;
    for(int i=0; i<matG.cols; i++ )  sum+= (*((float*)(matG.data+(i)*sizeof(float))));
    if ( !sum) return;
    for( int i=0;i<matG.cols; i++ )  (*((float*)(matG.data+(i)*sizeof(float)))) = (*((float*)(matG.data+(i)*sizeof(float))))/ sum ;
}

cv::Mat lbpTop::generateSampleData(std::vector<cv::Mat> &volume)
{

    int binsXY = (Pxy*(Pxy-1)+3);
    int binsXT = (Pxt*(Pxt-1)+3);
    int binsYT = (Pyt*(Pyt-1)+3);
    int bins   = binsXY+binsXT+ binsYT;

    int blockCount  = xBlocks*yBlocks;
    cv::Mat histTot(1,bins*blockCount,CV_32FC1,cv::Scalar(0));
    cv::Mat histV(1,bins,CV_32FC1,cv::Scalar(0));
    cv::Mat matG;
    int size = volume.size();

    fullFrameXY = volume.at(size-1);
    fullFrameYT = cv::Mat(ySize,size*xBlocks,CV_8UC1,cv::Scalar(255));
    fullFrameXT = cv::Mat(size*yBlocks,xSize,CV_8UC1,cv::Scalar(255));

    int xWidth = xSize/xBlocks;
    int yWidth = ySize/yBlocks;

    for( int k = 0 ; k<blockCount; k++)
    {
        cv::Mat frameXY(yWidth, xWidth,CV_8UC1);
        cv::Mat frameYT(yWidth,size,CV_8UC1);
        cv::Mat frameXT(size, xWidth,CV_8UC1);

        int i = k%xBlocks ;         // blocks horizontally alligned
        int j = k/xBlocks ;         // blocks vertically alligned
        int p = xWidth*i ;          // kth block top left pixel x val
        int q = yWidth*j ;          // kth block top left pixel y val
        cv::Rect r( p , q, xWidth , yWidth);

        for( int l = 0; l< size; l++)
        {
            cv::Mat f = volume.at(l)(r);
            f.col(f.cols/2).copyTo(frameYT.col(l));
            f.row(f.rows/2).copyTo(frameXT.row(l));
        }
        volume.at(size-1)(r).copyTo(frameXY);
        r = cv::Rect(p,size*j,xWidth , size);
        frameXT.copyTo(fullFrameXT(r));
        r = cv::Rect(size*i,q,size,yWidth);
        frameYT.copyTo(fullFrameYT(r));
        matG = getLBPu2Hist(frameXY,Pxy,Rx,Ry);
        normalizeHist(matG);
        matG.colRange(0,binsXY).copyTo(histV.colRange(0,binsXY));

        matG = getLBPu2Hist(frameXT,Pxt,Rx,Rt);
        normalizeHist(matG);
        matG.colRange(0,binsXT).copyTo(histV.colRange(binsXY,(binsXY+binsXT)));

        matG = getLBPu2Hist(frameYT,Pyt,Rt,Ry);
        normalizeHist(matG);
        matG.colRange(0,binsYT).copyTo(histV.colRange((binsXY+binsXT),(binsXY+binsXT+binsYT)));

        histV.colRange(0,bins).copyTo(histTot.colRange(bins*k,bins*(k+1)));

        frameXY.release();
        frameXT.release();
        frameYT.release();
    }
    return histTot;
}

cv::Mat lbpTop::getHistImg(const cv::Mat &hist)
{
    cv::Mat histImg(256,hist.cols,CV_8UC1, cv::Scalar(255));
    for( int i=0; i< hist.cols; i++)
    {
        float binVal = hist.at<float>(i);
        int intensity = static_cast<int>(binVal*300);
        cv::line(histImg,cv::Point(i,255),cv::Point(i,255-intensity),cv::Scalar(0));
    }
    return (histImg);
}

void lbpTop::getFullFrames(Mat &xy, Mat &xt, Mat &yt)
{
    xy = fullFrameXY;
    xt = fullFrameXT;
    yt = fullFrameYT;
//    fullFrameXT.release();
//    fullFrameYT.release();
}
