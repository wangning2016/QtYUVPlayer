#include "dlgparamsetting.h"
#include "ui_dlgparamsetting.h"
#include "comdefine.h"

QString g_frameRateArray[] = {"1","2","4","8","16","25","30","60"};
YUVItem g_YUVItemArray[] = {YUVItem(0,"IUYV"),YUVItem(1,"IYU2"),YUVItem(2,"IYUV(I420)"),YUVItem(3,"NV12"),
                            YUVItem(4,"NV16"),YUVItem(5,"UYVY"),YUVItem(6,"YUYV"),YUVItem(7,"YV12"),
                            YUVItem(8,"YV16"),YUVItem(8,"YVYU"),YUVItem(10,"RGB565"),YUVItem(11,"RGB24"),YUVItem(12,"RGB32")};

DlgParamSetting::DlgParamSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParamSetting)
{
    ui->setupUi(this);
    for(int i = 0; i < sizeof(g_frameRateArray)/sizeof(g_frameRateArray[0]);i++)
    {
        ui->cmbFrameRate->addItem(g_frameRateArray[i]);
    }
    for(int i = 0; i < sizeof(g_YUVItemArray)/sizeof(g_YUVItemArray[0]); i++)
    {
        ui->cmbYUVTYPE->addItem(g_YUVItemArray[i].friendName,g_YUVItemArray[i].avPixFormat);
    }
    ui->cmbFrameRate->setCurrentText("25");
    QRegExp regExp("^(\\d+)(\\d+)?$");
    QRegExpValidator *pattern= new QRegExpValidator(regExp, this);
    ui->edVideoHeight->setValidator(pattern);
    ui->edVideoWidth->setValidator(pattern);
    ui->edVideoHeight->setEnabled(false);
    ui->edVideoWidth->setEnabled(false);
    ui->btSetPix->setEnabled(false);
    DrawPix();
}

DlgParamSetting::~DlgParamSetting()
{
    delete ui;
}

void DlgParamSetting::SetFilePath(QString filePath)
{
    m_filePath = filePath;
    DrawPix();
}

void DlgParamSetting::DrawPix()
{
    m_param.avPixformat = ui->cmbPix->currentData().toInt();
    m_param.frameRate = ui->cmbFrameRate->currentText().toInt();
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
    m_palyer.SetFilePath(m_filePath);
    m_palyer.SetParmam(m_param);
    QPixmap pix = m_palyer.GetYUVFrame(0);
    m_palyer.setWidgetBkImg(ui->widgetVideoPreview,pix);
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

