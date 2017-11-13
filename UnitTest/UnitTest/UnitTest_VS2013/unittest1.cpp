#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

int Add(int a, int b) {
  return a + b;
}
namespace UnitTest_VS2013 {
TEST_CLASS(UnitTest1) {
public:

  TEST_METHOD(TestMethod1) {
    // TODO: Your test code here
    int ret = Add(3, 1);
    Assert::AreEqual(41, ret, L"not 4", LINE_INFO());
  }

};
}