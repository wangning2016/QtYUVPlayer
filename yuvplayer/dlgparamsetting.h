#ifndef DLGPARAMSETTING_H
#define DLGPARAMSETTING_H

#include <QDialog>
#include "comdefine.h"
#include "cyuvplay.h"

namespace Ui {
class DlgParamSetting;
}

class DlgParamSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParamSetting(QWidget *parent = nullptr);
    ~DlgParamSetting();
    void SetFilePath(QString filePath);
    void DrawPix();
public:
signals:
    void sigParame(PlayParam);
private slots:
    void on_btPlay_clicked();

    void on_btCancel_clicked();

    void on_cmbPix_currentIndexChanged(const QString &arg1);

    void on_btSetPix_clicked();

    void on_cmbYUVTYPE_currentIndexChanged(const QString &arg1);

private:
    PlayParam m_param;
    Ui::DlgParamSetting *ui;
    QString m_filePath;
    CYUVPlay* m_palyer;
};

#endif // DLGPARAMSETTING_H
