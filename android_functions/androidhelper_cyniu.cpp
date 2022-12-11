#include "androidhelper_cyniu.h"
#include <QtCore/qjniobject.h>
#include <QtCore/qcoreapplication.h>

AndroidHelper_cyniu::AndroidHelper_cyniu()
{
//    proximitySensor = new QProximitySensor();
//    proximitySensor->start();
//    accSensor = new QAccelerometer();
//    accSensor->start();
    qDebug("konstruktor androidhelper !!!!!!!!!!!!!!\n");
}
AndroidHelper_cyniu::AndroidHelper_cyniu(const AndroidHelper_cyniu &androidHelper_cyniu)
{
//    proximitySensor = new QProximitySensor();
//    proximitySensor->start();
//    accSensor = new QAccelerometer();
//    accSensor->start();
    qDebug("konstruktor kopiujący androidhelper !!!!!!!!!!!!!!\n");
}

AndroidHelper_cyniu::~AndroidHelper_cyniu()
{
//    proximitySensor->stop();
//    delete proximitySensor;
//    accSensor->stop();
//    delete accSensor;
    qDebug("\n\n\n destruktor androidhelper\n\n\n");
}

AndroidHelper_cyniu &AndroidHelper_cyniu::operator=(const AndroidHelper_cyniu &androidHelper_cyniu)
{
//    assert(this != &androidHelper_cyniu);
//    proximitySensor = new QProximitySensor();
//    proximitySensor->start();
//    accSensor = new QAccelerometer();
//    accSensor->start();
//    qDebug("operator = androidhelper !!!!!!!!!!!!!!\n");
    return *this;
}

void AndroidHelper_cyniu::vibrate(int msec)
{
    QJniObject::callStaticMethod<void>("org/qtproject/example/Chronometer/AndroidHelper", "vibrate", "(I)V", msec);
}

double AndroidHelper_cyniu::getProximity()
{

    QProximityReading *reading = proximitySensor->reading();
    qreal value = reading->value(0).value<qreal>();
    // qreal value = reading->property("Distance").value<qreal>();
    return value ;
}

QString AndroidHelper_cyniu::getAccelerometer()
{
    QSensorReading *reading = accSensor->reading();
    qreal x = reading->property("x").value<qreal>();
    qreal y = reading->value(1).value<qreal>();
    qreal z = reading->property("z").value<qreal>();

    return QString::number(x)+" "+QString::number(y)+" "+QString::number(z);
}

void AndroidHelper_cyniu::makeToast(QString text)
{
    QJniObject::callStaticMethod<void>("org/qtproject/example/Chronometer/AndroidHelper", "makeToast",  "(Ljava/lang/String;)V",QJniObject::fromString(text).object<jstring>());
}
int AndroidHelper_cyniu::updateAndroidNotification(QString msg)
{
   // QJniObject::callStaticMethod<void>("org/qtproject/example/Chronometer/AndroidHelper", "notify",  "(Ljava/lang/String;)V",
   //                                           QNativeInterface::QAndroidApplication::context() ,
   //                                           QJniObject::fromString(msg).object<jstring>());
    return 0;
}

void AndroidHelper_cyniu::keep_screen_on(bool on)
{

        // get the Qt android activity
        QJniObject activity = QJniObject::callStaticObjectMethod("org/qtproject/qt/android/QtNative",
                                                                               "activity",
                                                                               "()Landroid/app/Activity;");
        if (activity.isValid()) {
            QJniObject window =
                    activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

            if (window.isValid()) {
                const int FLAG_KEEP_SCREEN_ON = 128;
                if (on) {
                    window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                } else {
                    window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                }
            }
        }
        QJniEnvironment env;
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }


}

void AndroidHelper_cyniu::sendSMS(QString nr, QString msg)
{
    // get the Qt android activity
    QJniObject activity = QJniObject::callStaticObjectMethod("org/qtproject/qt/android/QtNative",
                                                                           "activity",
                                                                           "()Landroid/app/Activity;");
    if (activity.isValid()){

        //get the default SmsManager
        QJniObject mySmsManager = QJniObject::callStaticObjectMethod("android/telephony/SmsManager",
                                                                                   "getDefault",
                                                                                   "()Landroid/telephony/SmsManager;");
        if (!mySmsManager.isValid()) {
            qDebug() << "Something wrong with SMS manager...";
        } else {

            // get phone number & text from UI and convert to Java String
            QJniObject myPhoneNumber = QJniObject::fromString(nr);
            QJniObject myTextMessage = QJniObject::fromString(msg);
            QJniObject scAddress = NULL;
            QJniObject sentIntent = NULL;
            QJniObject deliveryIntent = NULL;

            // call the java function:
            // public void SmsManager.sendTextMessage(String destinationAddress,
            //                                        String scAddress, String text,
            //                                        PendingIntent sentIntent, PendingIntent deliveryIntent)
            // see: http://developer.android.com/reference/android/telephony/SmsManager.html

            mySmsManager.callMethod<void>("sendTextMessage",
                                          "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Landroid/app/PendingIntent;Landroid/app/PendingIntent;)V",
                                          myPhoneNumber.object<jstring>(),
                                          scAddress.object<jstring>(),
                                          myTextMessage.object<jstring>(), NULL, NULL );
        }
    }

}

bool AndroidHelper_cyniu::share(QString text)
{
   QJniObject::callStaticMethod<void>("org/qtproject/example/Chronometer/AndroidHelper", "share",  "(Ljava/lang/String;)V",QJniObject::fromString(text).object<jstring>());
   return true;
}

bool AndroidHelper_cyniu::hasPermission(QString text)
{
    jboolean ret = QJniObject::callStaticMethod<jboolean>("org/qtproject/example/Chronometer/AndroidHelper", "hasPermission",
                                       "(Landroid/content/Context;Ljava/lang/String;)V",
                                       QNativeInterface::QAndroidApplication::context(),
                                       QJniObject::fromString(text).object<jstring>());

    return ret;
}
