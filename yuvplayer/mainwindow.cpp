#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_palyer = nullptr;
    ui->setupUi(this);
    m_pDlgParamSetting = new DlgParamSetting(this);
    if(m_pDlgParamSetting)
    {
        m_pDlgParamSetting->hide();
    }
    connect(m_pDlgParamSetting,SIGNAL(sigParame(PlayParam)) , this , SLOT(on_sigParame(PlayParam)) );
    m_palyer = new CYUVPlay(this);

}

MainWindow::~MainWindow()
{
    if(m_pDlgParamSetting)
    {
        delete m_pDlgParamSetting;
        m_pDlgParamSetting = NULL;
    }
    delete ui;
}
void MainWindow::resizeEvent(QResizeEvent *)
{
    ui->mainWidget->resize(ui->centralwidget->size());
}

void MainWindow::on_btSelectFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,QStringLiteral("select file！"),"/home/",QStringLiteral("yuvfile(*yuv);;""allfile(*.*)"));
    if(filePath.isEmpty())
        return;

    ui->edFilePath->setText(filePath);
    if(m_palyer)
        m_palyer->SetFilePath(filePath);
}


void MainWindow::on_btPlay_clicked()
{
    if(m_param.frameRate < 0 || m_param.widht < 0 || m_param.height < 0 || m_palyer == nullptr)
    {
        on_btSetParam_clicked();
    }
    else
    {
        QString filePath = ui->edFilePath->text();
        m_palyer->SetFilePath(filePath);
        m_palyer->SetRenderWidget(ui->videoRender);
        m_palyer->Play();
    }
}


void MainWindow::on_btPause_clicked()
{
    if(m_palyer)
        m_palyer->Pause(!m_palyer->isPause());
}


void MainWindow::on_btStop_clicked()
{
    if(m_palyer)
        m_palyer->Stop();
}


void MainWindow::on_btSetParam_clicked()
{
    if(m_pDlgParamSetting)
    {
        m_pDlgParamSetting->SetFilePath(ui->edFilePath->text());
        m_pDlgParamSetting->show();
    }
}

void MainWindow::on_sigParame(PlayParam param)
{
    if(param.height > 0 && param.widht > 0 && param.frameRate && m_palyer)
    {
        m_param = param;
        m_palyer->SetParmam(param);
        QString filePath = ui->edFilePath->text();
        m_palyer->SetFilePath(filePath);
        m_palyer->SetRenderWidget(ui->videoRender);
        m_palyer->Play();
    }
}


