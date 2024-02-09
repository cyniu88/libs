#include <iostream>
#include <chrono>

#include "../client.h"

int main()
{
  std::cout << "Hello World!" << std::endl;
  try
  {
    char points[4096];
    int pointsSize = 4096;

    influx_client::flux::Client client(
        "10.9.0.34", /* port */ 8086, /* token */
        " "
        "-aaaapov11112lj2-ovr5bbbbso6q==",
        "organization", "testBT");

    std::vector<influx_client::kv_t> tags;
    std::vector<influx_client::kv_t> fields;
    fields.emplace_back("smog", 0.555);
    auto timestamp = duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto code = client.write("smog", tags, fields, timestamp);

    std::cout << timestamp << " response: " << code << std::endl;
  }
  catch (...)
  {
    std::cout << "wyjatek  " << std::endl;
  }
  return 0;
}
