#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dlgparamsetting.h"
#include "cyuvplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    virtual void resizeEvent(QResizeEvent *event);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btSelectFile_clicked();

    void on_btPlay_clicked();

    void on_btPause_clicked();

    void on_btStop_clicked();

    void on_btSetParam_clicked();

    void on_sigParame(PlayParam);


private:
    Ui::MainWindow *ui;
    DlgParamSetting* m_pDlgParamSetting;
    CYUVPlay* m_palyer;
    PlayParam m_param;
};
#endif // MAINWINDOW_H
