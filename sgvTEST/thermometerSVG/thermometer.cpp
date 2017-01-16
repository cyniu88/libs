#include "thermometer.h"
#include <QDebug>
Thermometer::Thermometer(QWidget *parent)
{
    QSvgWidget::load(getByteArraySVG());
}

void Thermometer::setColor(QColor c)
{
    color = c.name();

    QSvgWidget::load(getByteArraySVG());
}

void Thermometer::setBackgroundColor(QColor c)
{
    backgroundColor = c.name();
     QSvgWidget::load(getByteArraySVG());
}

void Thermometer::setTemperature(double t)
{
    degr = map_f(t,-30.0,50.0,-30.0,-590.0);
    qDebug() << "temp " << t << " degr " << degr;
    if (t > 0 ){
        color = "#ff0000";
    }
    else{
        color = "#0000ff";
    }

   QSvgWidget::load(getByteArraySVG());
}


auto Thermometer::map_f(auto value, auto from_min, auto from_max, auto to_min, auto to_max)
{
    return (value - from_min) * (to_max - to_min)/(from_max - from_min) + to_min;
}

QByteArray Thermometer::getByteArraySVG()
{  QString temp  = firstSVG+ backgroundColor+ firstSVG_A + QString::number(degr) + secSVG + color +thirdSVG + color + fourthSVG;
  return   temp.toUtf8();
}


