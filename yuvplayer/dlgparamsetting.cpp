#include "dlgparamsetting.h"
#include "ui_dlgparamsetting.h"
#include "comdefine.h"
#include <SDL2/SDL.h>


QString g_frameRateArray[] = {"1","2","4","8","16","25","30","60"};
YUVItem g_YUVItemArray[] = {YUVItem(SDL_PIXELFORMAT_RGB24,"RGB24"),
                            YUVItem(SDL_PIXELFORMAT_BGR24,"BGR24"),
                            YUVItem(SDL_PIXELFORMAT_RGBA32,"RGBA32"),
                            YUVItem(SDL_PIXELFORMAT_ARGB32,"ARGB32"),
                            YUVItem(SDL_PIXELFORMAT_BGRA32,"BGRA32"),
                            YUVItem(SDL_PIXELFORMAT_ABGR32,"ABGR32"),
                            YUVItem(SDL_PIXELFORMAT_IYUV,"IYUV"),
                            YUVItem(SDL_PIXELFORMAT_YV12,"YV12"),
                            YUVItem(SDL_PIXELFORMAT_YUY2,"YUY2"),
                            YUVItem(SDL_PIXELFORMAT_UYVY,"UYVY"),
                            YUVItem(SDL_PIXELFORMAT_YVYU,"YVYU"),
                            YUVItem(SDL_PIXELFORMAT_NV12,"NV12"),
                            YUVItem(SDL_PIXELFORMAT_NV21,"NV21")};

DlgParamSetting::DlgParamSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParamSetting)
{
    m_palyer = nullptr;
    ui->setupUi(this);
    for(int i = 0; i < sizeof(g_frameRateArray)/sizeof(g_frameRateArray[0]);i++)
    {
        ui->cmbFrameRate->addItem(g_frameRateArray[i]);
    }
    for(int i = 0; i < sizeof(g_YUVItemArray)/sizeof(g_YUVItemArray[0]); i++)
    {
        QVariant var=g_YUVItemArray[i].avPixFormat;
        ui->cmbYUVTYPE->addItem(g_YUVItemArray[i].friendName,var);
    }
    ui->cmbFrameRate->setCurrentText("25");
    QRegExp regExp("^(\\d+)(\\d+)?$");
    QRegExpValidator *pattern= new QRegExpValidator(regExp, this);
    ui->edVideoHeight->setValidator(pattern);
    ui->edVideoWidth->setValidator(pattern);
    ui->edVideoHeight->setEnabled(false);
    ui->edVideoWidth->setEnabled(false);
    ui->btSetPix->setEnabled(false);
    m_palyer = new CYUVPlay(this);
    DrawPix();
}

DlgParamSetting::~DlgParamSetting()
{
    delete ui;
    if(m_palyer)
    {
        delete m_palyer;
        m_palyer = NULL;
    }
}

void DlgParamSetting::SetFilePath(QString filePath)
{
    m_filePath = filePath;
    DrawPix();
}

void DlgParamSetting::DrawPix()
{
    if(m_palyer == NULL)
        return;

    m_palyer->SetRenderWidget(ui->widgetVideoPreview);
    m_param.avPixformat = ui->cmbYUVTYPE->currentData().toInt();
    m_param.frameRate = ui->cmbFrameRate->currentText().toInt();
    if(m_param.avPixformat == SDL_PIXELFORMAT_IYUV)
    {
        int i = 0;
        i++;
    }
    QString strVideoHeight,strVideoWidth;
    if(ui->cmbPix->currentIndex() == 0)
    {
        strVideoHeight = ui->edVideoHeight->text();
        strVideoWidth = ui->edVideoWidth->text();
    }
    else
    {
        QString strPix = ui->cmbPix->currentText();
        QStringList splits  = strPix.split("x");
        strVideoHeight = splits.at(1);
        strVideoWidth = splits.at(0);
    }
    int videoW = strVideoWidth.toInt();
    int videoH = strVideoHeight.toInt();
    if(videoW < 100)videoW = 176;
    if(videoH < 100)videoH = 120;
    m_param.height = videoH;
    m_param.widht = videoW;
    if(m_filePath.isEmpty())
    {
        return;
    }
    m_palyer->SetFilePath(m_filePath);
    m_palyer->SetParmam(m_param);

    m_palyer->Preview(ui->widgetVideoPreview);
}

void DlgParamSetting::on_btPlay_clicked()
{
    emit sigParame(m_param);
    hide();
}


void DlgParamSetting::on_btCancel_clicked()
{
    hide();
}


void DlgParamSetting::on_cmbPix_currentIndexChanged(const QString &)
{
    int index = ui->cmbPix->currentIndex();
    ui->edVideoHeight->setEnabled(index == 0);
    ui->edVideoWidth->setEnabled(index == 0);
    ui->btSetPix->setEnabled(index == 0);
    if(index != 0)
    {
        DrawPix();
    }
}


void DlgParamSetting::on_btSetPix_clicked()
{
    DrawPix();
}


void DlgParamSetting::on_cmbYUVTYPE_currentIndexChanged(const QString &)
{
    DrawPix();
}

