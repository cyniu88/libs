#ifndef ANDROID_INTERFACE_H
#define ANDROID_INTERFACE_H

#include <QObject>
#ifdef Q_OS_ANDROID
//#include "../libs/android_functions/androidhelper_cyniu.h"
#include "../../android_functions/androidhelper_cyniu.h"
#endif

class android_interface
{
public:
    android_interface();
    void vibrate(int msec);
    double getProximity();
    QString getAccelerometer();
    void makeToast(const QString &s);
    void updateAndroidNotification (const QString& s);
    void keepScreenOn(bool on);
    void sendSMS(const QString &nr, const QString &msg);
    bool share(const QString& text);
    bool hasPermission(const QString &text);
private:
#ifdef Q_OS_ANDROID
    AndroidHelper_cyniu droid;
#endif
};

#endif // ANDROID_INTERFACE_H
