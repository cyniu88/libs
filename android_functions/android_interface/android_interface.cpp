#include "android_interface.h"

android_interface::android_interface()
{

}

void android_interface::vibrate(int msec)
{
#ifdef Q_OS_ANDROID

    droid.vibrate(msec);

#endif
    msec++;
}

double android_interface::getProximity()
{
    double value = -1.1;
#ifdef Q_OS_ANDROID

    value = droid.getProximity();

#endif
    return value;
}

QString android_interface::getAccelerometer()
{
    QString value =" ";
#ifdef Q_OS_ANDROID
    value = droid.getAccelerometer();
#endif
    return value;
}

void android_interface::makeToast(QString s)
{
#ifdef Q_OS_ANDROID
    droid.makeToast(s);
#endif
}

void android_interface::updateAndroidNotification(QString s)
{
#ifdef Q_OS_ANDROID
    droid.updateAndroidNotification(s);
#endif
}

void android_interface::keepScreenOn(bool on)
{
#ifdef Q_OS_ANDROID
    droid.keep_screen_on(on);
#endif
}

void android_interface::sendSMS(QString nr, QString msg)
{
#ifdef Q_OS_ANDROID
    droid.sendSMS(nr,msg);
#endif
}
