#include "cyuvplay.h"
#include <QPalette>
#include <QWidget>

CYUVPlay::CYUVPlay()
{
    m_mode = SHOW_YUV;
}

QPixmap CYUVPlay::GetYUVFrame(int frameIndex)
{
    return QPixmap();
}

void CYUVPlay::SetFilePath(QString filePath)
{
    m_filePath = filePath;
}

void CYUVPlay::SetParmam(PlayParam param)
{
    m_param = param;
}

void CYUVPlay::SetPlayMode(int mode)
{

}
bool CYUVPlay::setWidgetBkImg(QWidget *pWidget, QPixmap pix)
{
    if(pWidget == nullptr)
        return false;


    QPalette pal = pWidget->palette();
    if(pix.isNull())
    {
        return false;
    }

    QBrush brush = QBrush(pix);
    pal.setBrush(QPalette::Background,brush);
    pWidget->setPalette(pal);
    return true;
}
