#include <iostream>
#include "../client.h"

int main()
{
  std::cout << "Hello World!" << std::endl;
  try {
    char points[4096];
    int pointsSize = 4096;

    influx_client::flux::Client client(
          "10.9.0.34", /* port */ 8086, /* token */
          " "
          "-aaaapov11112lj2-ovr5bbbbso6q==",
          "organization", "testBT");


    auto code = client.writes(
          {
            {
              "temperatura",
              {},
              {{"outdoor",99},
               {"inside", 100},
               {"floor", 101},
               {"bojler", 102},
               {"domek", 103},
               {"flow",104},
               {"shedTemp", 105}},
              0,
            },
            {
              "wilgoc",
              {},
              {{"humi", 22.2}},
              0,
            },
            {
              "bateria",
              {},
              {{"volt",12.4}},
              0,
            },
            {
              "smog",
              {},
              {{"smog",555.5555}},
              0,
            },
            {
              "cisnienie",
              {},
              {{"dom", 998.8}},
              0,
            },
            {
              "piec",
              {},
              {{"praca", true}},
              0,
            },
          },
          points, pointsSize);
    std::cout << "status code: " << code << std::endl;
    if (code != 204)
      {

        throw 55;
      }

  }
  catch (...)
  {
    std::cout << "wyjatek  "  << std::endl;
  }
  return 0;
}


