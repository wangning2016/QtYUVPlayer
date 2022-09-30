#ifndef COMDEFINE_H
#define COMDEFINE_H
#include<QString>
#define SHOW_YUV 0
#define SHOW_Y 1
#define SHOW_U 2
#define SHOW_V 3



struct YUVItem
{
    int avPixFormat;
    QString friendName;

    YUVItem(int avformat,QString fname)
    {
        avPixFormat = avformat;
        friendName = fname;
    }
};

struct PlayParam
{
    int avPixformat;
    int widht;
    int height;
    int frameRate;

    PlayParam()
    {
        avPixformat = -1;
        widht = -1;
        height = -1;
        frameRate = 25;
    }
};


#endif // COMDEFINE_H
