#include "pattern.h"
#include "global.h"
#include <QString>
#define ELEM(start,xpos) *((int*)(start+(xpos)*sizeof(int)))
pattern::pattern()
{
}

void pattern::load()
{
    FileStorage fs;
    QString patternFile= PATTERN_FILE_DIR "Uniform Pattern " + QString::number(Pxy)+" bits.xml";
    fs.open(patternFile.toStdString(),FileStorage::READ);
    if ( !fs.isOpened())
    {
        std::cout << "Error: Can not load pattern" << std::endl;
        return;
    }
    bits = (int)fs["bits"];
    fs["uniformPattern"] >> u2Pattern;
    fs.release();
}

int pattern::indexOf(unsigned s)
{
    // binary search
    int begin= 0 , end = u2Pattern.cols-1 ;
    while( begin <= end )
    {
        int mid = (begin + end)/2;
        int x = ELEM(u2Pattern.data,mid);
        if ( x == int(s))           return mid;
        else if ( x > int(s) )      end   = mid-1;
        else                        begin = mid+1;
    }
    return -1;
}

