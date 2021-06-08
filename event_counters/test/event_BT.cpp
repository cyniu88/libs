#include <gtest/gtest.h>
#include "test_data.h"
#include "../../../iDom_server_OOP/src/iDomTools/test/iDomTools_fixture.h"
#include "../event_counters_handler.h"

class event_counter_fixture : public iDomTOOLS_ClassTest
{
public:
     event_counters_handler mainEvent;
     std::string msg = "info";
     std::string testEvent = "testEvent";
     void preper9000Event(){
         int counter = 9000;
         EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),0);
         mainEvent.run(testEvent)->addEvent(msg);
         EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1);
         std::string returnedString = mainEvent.run(testEvent)->getEvent();
         EXPECT_THAT(returnedString, testing::HasSubstr(msg));
         EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1);

         for (int i = 1; i not_eq counter; counter--)
         {
             mainEvent.run(testEvent)->addEvent(msg);
             if(counter == 500){}
         }
         mainEvent.run(testEvent)->addEvent("cyniu");
         EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),9001);
         returnedString = mainEvent.run(testEvent)->getEvent();
         EXPECT_THAT(returnedString, testing::HasSubstr("cyniu"));
     }

     void preper1001Event(){
         int counter = 1000;
         EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),0);
         mainEvent.run(testEvent)->addEvent(msg);
         EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1);
         std::string returnedString = mainEvent.run(testEvent)->getEvent();
         EXPECT_THAT(returnedString, testing::HasSubstr(msg));
         EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1);

         for (int i = 1; i not_eq counter; counter--)
         {
             mainEvent.run(testEvent)->addEvent(msg);
             if(counter == 500)
                 mainEvent.run(testEvent)->addEvent("cyniu");
         }
         EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1001);
         returnedString = mainEvent.run(testEvent)->getEvent();
         EXPECT_THAT(returnedString, testing::HasSubstr("cyniu"));
     }
};

TEST_F(event_counter_fixture, add_get_clear_event)
{
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),0);
    mainEvent.run(testEvent)->addEvent(msg);
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1);
    std::string returnedString = mainEvent.run(testEvent)->getEvent();
    EXPECT_THAT(returnedString, testing::HasSubstr(msg));
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1);
    mainEvent.run(testEvent)->clearEvent();
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),0);
}

TEST_F(event_counter_fixture, clear_middle_part_of_event_small_big)
{
    preper1001Event();
    mainEvent.run(testEvent)->clearEvent(400,600);
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),801);
    std::string returnedString = mainEvent.run(testEvent)->getEvent();
    EXPECT_THAT(returnedString, testing::Not(testing::HasSubstr("cyniu")));
}

TEST_F(event_counter_fixture, clear_middle_part_of_event_big_small)
{
    preper1001Event();
    mainEvent.run(testEvent)->clearEvent(600,400);
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1001);
    std::string returnedString = mainEvent.run(testEvent)->getEvent();
    EXPECT_THAT(returnedString,testing::HasSubstr("cyniu"));
}

TEST_F(event_counter_fixture, clear_middle_part_of_event_to_max)
{
    preper1001Event();
    mainEvent.run(testEvent)->clearEvent(400,1600);
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),400);
    std::string returnedString = mainEvent.run(testEvent)->getEvent();
    EXPECT_THAT(returnedString, testing::Not(testing::HasSubstr("cyniu")));
}

TEST_F(event_counter_fixture, clear_middle_part_of_event_from_max)
{
    preper1001Event();
    mainEvent.run(testEvent)->clearEvent(1400,1600);
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1001);
    std::string returnedString = mainEvent.run(testEvent)->getEvent();
    EXPECT_THAT(returnedString, testing::HasSubstr("cyniu") );
}

TEST_F(event_counter_fixture, getListPossibleEvents)
{
    mainEvent.run("INFO")->addEvent("kokolino");
   std::string returnedString = mainEvent.getListPossibleEvents();
   std::cout << "wynik: " << returnedString << std::endl;
   EXPECT_THAT(returnedString, testing::HasSubstr("INFO") );
}

TEST_F(event_counter_fixture, getHelp)
{
   mainEvent.run("INFO")->addEvent("kokolino");
   std::string returnedString = mainEvent.help("connections");
   std::cout << "wynik: " << returnedString << std::endl;
   EXPECT_THAT(returnedString, testing::HasSubstr("start") );
   returnedString = mainEvent.help("");
   std::cout << "wynik: " << returnedString << std::endl;
   EXPECT_THAT(returnedString, testing::HasSubstr("pilot") );
}

TEST_F(event_counter_fixture, getLast1minNumberEvent)
{
    preper1001Event();
    mainEvent.run(testEvent)->eventList.at(500).posixTime -= 65;
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1001);
    EXPECT_EQ(mainEvent.run(testEvent)->getLast1minNumberEvent(),500);
}

TEST_F(event_counter_fixture, getLast1minNumberEventWhenEmpty)
{
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),0);
    EXPECT_EQ(mainEvent.run(testEvent)->getLast1minNumberEvent(),0);
}


TEST_F(event_counter_fixture, clearOldEvent)
{
    preper9000Event();
    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),9001);

    mainEvent.clearOld(8000, 1000, [](const std::string& name){
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "skasowanao nadmarowe eventy w: "<< name << std::endl;
        log_file_mutex.mutex_unlock();
    });

    EXPECT_EQ(mainEvent.run(testEvent)->howManyEvent(),1000);
    auto returnedString = mainEvent.run(testEvent)->getEvent();
    EXPECT_THAT(returnedString, testing::HasSubstr("cyniu"));
}
