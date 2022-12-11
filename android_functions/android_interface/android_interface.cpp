#include "android_interface.h"
#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif

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

void android_interface::makeToast(const QString& s)
{
#ifdef Q_OS_ANDROID
    droid.makeToast(s);
#endif
}

void android_interface::updateAndroidNotification(const QString &s)
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

void android_interface::sendSMS(const QString& nr, const QString& msg)
{
#ifdef Q_OS_ANDROID
    droid.sendSMS(nr,msg);
#endif
}

bool android_interface::share(const QString &text)
{
    bool ret(false);
#ifdef Q_OS_ANDROID
   ret = droid.share(text);
#endif
   return ret;
}

bool android_interface::hasPermission(const QString& text)
{
    bool ret(false);
#ifdef Q_OS_ANDROID
   // ret = droid.hasPermission(text);
    auto r = QtAndroidPrivate::checkPermission(QtAndroidPrivate::Storage).result();
    if (r == QtAndroidPrivate::Denied)
    {
        r = QtAndroidPrivate::requestPermission(QtAndroidPrivate::Storage).result();
        if (r == QtAndroidPrivate::Denied)
            return false;
    }
    return true;
#endif
    return ret;
}
