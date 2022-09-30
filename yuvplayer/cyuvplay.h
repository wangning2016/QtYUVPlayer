#ifndef CYUVPLAY_H
#define CYUVPLAY_H
#include <QPixmap>
#include "comdefine.h"

class CYUVPlay
{
public:
    CYUVPlay();


    QPixmap GetYUVFrame(int frameIndex);
    void SetFilePath(QString filePath);
    void SetParmam(PlayParam param);
    void SetPlayMode(int mode = SHOW_YUV);
    bool setWidgetBkImg(QWidget *pWidget, QPixmap pix);
private:
    PlayParam m_param;
    QString m_filePath;
    int m_mode;
};

#endif // CYUVPLAY_H
