#pragma once

#include <QMainWindow>
#include <QSvgWidget>

#include <thermometerSVG/thermometer.h>
namespace Ui {
class eye;
}

class eye : public QMainWindow
{
    Q_OBJECT

public:
    explicit eye(QWidget *parent = 0);
    ~eye();
 Thermometer ter;

private slots:
    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::eye *ui;
};

