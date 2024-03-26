#include <iostream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

class TestIf
{
public:
  virtual ~TestIf() = default;
  virtual void method(int i) = 0;
};

class test : public TestIf
{
public:
  test() = default;
  ~test() = default;
  void method(int i) override{
    std::cout << "cyniu " << std::to_string(i)<< std::endl;
  }
};

class mockTest : public TestIf , public testing::Mock
{
public:
  ~mockTest() = default;
  MOCK_METHOD1(method, void(int i));

};

class testHandler
{
public:
  ~testHandler() = default;
  static std::shared_ptr<TestIf> tt;
  void print(int i){
    tt->method(i);
  }
};
 std::shared_ptr<TestIf> testHandler::tt;

class testTest : public testing::Test
{
public:
  testTest(){
    ptr = std::make_shared<mockTest>();
    t.tt = ptr;
  }
  testHandler t;
  std::shared_ptr<TestIf> ptr;

};

TEST_F(testTest, main)
{
  //testHandler::tt = ptr;
  //EXPECT_CALL(*testHandler::tt.get(), method(testing::_));
  t.print(67);

}
