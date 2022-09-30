#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cmbYUV->addItem("YUV");
    ui->cmbYUV->addItem("Y");
    ui->cmbYUV->addItem("U");
    ui->cmbYUV->addItem("V");
    m_pDlgParamSetting = new DlgParamSetting(this);
    if(m_pDlgParamSetting)
    {
        m_pDlgParamSetting->hide();
    }
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
void MainWindow::resizeEvent(QResizeEvent *event)
{

    ui->mainWidget->resize(ui->centralwidget->size());
}

void MainWindow::on_btSelectFile_clicked()
{

}


void MainWindow::on_btPlay_clicked()
{

}


void MainWindow::on_btPause_clicked()
{

}


void MainWindow::on_btStop_clicked()
{

}


void MainWindow::on_btSetParam_clicked()
{
    if(m_pDlgParamSetting)
    {
        m_pDlgParamSetting->show();
    }
}

