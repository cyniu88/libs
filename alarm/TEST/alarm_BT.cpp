#include <gtest/gtest.h>
#include "../alarm.h"

class alarm_BT: public ::testing::Test
{
public:
    void SetUp()
    {

    }
    void TearDown()
    {

    }
};

TEST_F(alarm_BT, main)
{
    alarm_C  a;
    EXPECT_EQ(a.alarmSize(), 0);

    a.raiseAlarm(22, "TEST Alarm1");
    std::cout << "alarm: " << a.showAlarm() << std::endl;
    EXPECT_EQ(a.alarmSize(), 1);

    a.raiseAlarm(33, "TEST Alarm2");
    std::cout << "alarm: " << a.showAlarm() << std::endl;
    EXPECT_EQ(a.alarmSize(), 2);

    a.clearAlarm(33);
    std::cout << "alarm: " << a.showAlarm() << std::endl;
    EXPECT_EQ(a.alarmSize(), 1);

    a.clearAlarm(3003);
    std::cout << "alarm: " << a.showAlarm() << std::endl;
    EXPECT_EQ(a.alarmSize(), 1);
}
