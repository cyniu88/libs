#include "eye.h"
#include "ui_eye.h"
#include <QDebug>
eye::eye(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::eye)
{
    ui->setupUi(this);
    QString path = "C:/cyniu/PROGRAMOWANIE/QT/QT_kurs/iDom_Client/files/svg/test.svg";

//    obraz = stringSVG.toUtf8();
//    qDebug() << "obraz ma: " <<obraz.size() <<  stringSVG;
//    k.load(obraz);
  ui->gridLayout->addWidget(&ter);
//   ui->doubleSpinBox->setValue(stringSVG2);
}

eye::~eye()
{
    delete ui;
}

void eye::on_doubleSpinBox_valueChanged(double arg1)
{
//    stringSVG2 = ui->doubleSpinBox->value();
//    stringSVG = stringSVG1 + QString::number(stringSVG2) + stringSVG3 + kolor +stringSVG4 + kolor + stringSVG5;
//    obraz = stringSVG.toUtf8();
//    qDebug() << "obraz ma: " <<obraz.size() <<  stringSVG;
//    k.load(obraz);
//   //ui->gridLayout->addWidget(&k);
//   ui->progressBar->setValue(stringSVG2);
    ter.setTemperature(ui->doubleSpinBox->value());
    ter.setBackgroundColor(Qt::yellow);

}
