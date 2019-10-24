#ifndef ANDROIDHELPER_CYNIU_H
#define ANDROIDHELPER_CYNIU_H
#include <memory>
#include <QDebug>
#include <QtAndroidExtras/QAndroidJniObject> // For JNI, need to select Android Build
#include <QAndroidJniObject>
#include <QAccelerometer>
#include <QProximitySensor>
#include <QProximityReading>

class AndroidHelper_cyniu
{
private:
     QProximitySensor *proximitySensor;
     QAccelerometer *accSensor;

public:
    AndroidHelper_cyniu();
    AndroidHelper_cyniu(const AndroidHelper_cyniu& androidHelper_cyniu);
    ~AndroidHelper_cyniu();
    AndroidHelper_cyniu& operator=(const AndroidHelper_cyniu& androidHelper_cyniu);
    void vibrate(int msec);
    double getProximity();
    QString getAccelerometer();
    int fibo(int n);
    void makeToast(QString text);
    int updateAndroidNotification(QString msg);
    void keep_screen_on(bool on);
    void sendSMS(QString nr, QString msg);
    bool share(QString text);
};

#endif // ANDROIDHELPER_CYNIU_H
