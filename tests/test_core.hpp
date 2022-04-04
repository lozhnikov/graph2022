/**
 * @file tests/test_core.hpp
 * @author Mikhail Lozhnikov
 *
 * Реализация тестового фреймворка.
 */

#ifndef TESTS_TEST_CORE_HPP_
#define TESTS_TEST_CORE_HPP_

#include <httplib.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <string>
#include <algorithm>
#include <functional>
#include <mutex>
#include <cmath>
#include "io.hpp"

namespace ascii_codes {
  static const char esc = 27;
  static const char* defaultColor = "[39m";
  //  const char* blackColor = "[30m";
  static const char* greenColor = "[32m";
  static const char* redColor = "[31m";
//  static const char* yellowColor = "[33m";
//  static const char* magentaColor = "[35m";
//  static const char* cyanColor = "[36m";
  static const char* bold = "[1m";
  static const char* normal = "[22m";
}  // namespace ascii_codes

class TestSuite {
 public:
  explicit TestSuite(const std::string& testSuiteName) :
    testSuiteName(testSuiteName) {
    std::cerr << "Starting test suite " << testSuiteName << "..." << std::endl;
  }

  static int Status() {
    using ascii_codes::esc;
    using ascii_codes::greenColor;
    using ascii_codes::redColor;
    using ascii_codes::defaultColor;
    using ascii_codes::bold;
    using ascii_codes::normal;

    std::lock_guard<std::mutex> locker(statsMutex);

    std::cerr << std::endl << "Summary:" << std::endl;

    if (numFailedTests == 0) {
      std::cerr << esc << greenColor
                << "100% tests succeeded!"
                << esc << defaultColor << std::endl;

      return 0;
    } else {
      std::streamsize prevPrecision = std::cerr.precision(2);

      std::cerr << 100.0 * static_cast<double>(totalNumTests - numFailedTests) /
                   static_cast<double>(totalNumTests)
                << "% tests succeeded!" << std::endl
                << esc << redColor << esc << bold
                << numFailedTests << " test(s) failed!"
                << esc << defaultColor << esc << normal << std::endl;

      std::cerr.precision(prevPrecision);

      return 1;
    }
  }

  ~TestSuite() {
    std::cerr << "Test suite " << testSuiteName << " finished." << std::endl;
  }

  template<typename Test>
  void RunTest(Test test, const std::string& testName) {
    using ascii_codes::esc;
    using ascii_codes::greenColor;
    using ascii_codes::redColor;
    using ascii_codes::defaultColor;
    using ascii_codes::bold;
    using ascii_codes::normal;

    try {
      std::chrono::time_point<std::chrono::steady_clock> start =
          std::chrono::steady_clock::now();

      test();

      std::chrono::time_point<std::chrono::steady_clock> finish =
          std::chrono::steady_clock::now();

      std::cerr << esc << greenColor << "[OK]" << esc << defaultColor
                << "   Test " << testName << " passed! ("
                << static_cast<double>(
                     std::chrono::duration_cast<std::chrono::milliseconds>(
                     finish - start).count()) / 1000.0 << "s)" << std::endl;

      std::lock_guard<std::mutex> locker(statsMutex);

      totalNumTests++;
    }  catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
      std::cerr << esc << redColor << esc << bold << "[FAIL]"
                << esc << defaultColor << esc << normal
                << " Test " << testName << " failed!" << std::endl;

      std::lock_guard<std::mutex> locker(statsMutex);

      numFailedTests++;
      totalNumTests++;
    }
  }

  template<typename Test>
  void RunTest(httplib::Client* cli, Test test, const std::string& testName) {
    using ascii_codes::esc;
    using ascii_codes::greenColor;
    using ascii_codes::redColor;
    using ascii_codes::defaultColor;
    using ascii_codes::bold;
    using ascii_codes::normal;

    try {
      std::chrono::time_point<std::chrono::steady_clock> start =
          std::chrono::steady_clock::now();

      test(cli);

      std::chrono::time_point<std::chrono::steady_clock> finish =
          std::chrono::steady_clock::now();

      std::cerr << esc << greenColor << "[OK]" << esc << defaultColor
                << "   Test " << testName << " passed! ("
                << static_cast<double>(
                     std::chrono::duration_cast<std::chrono::milliseconds>(
                     finish - start).count()) / 1000.0 << "s)" << std::endl;

      std::lock_guard<std::mutex> locker(statsMutex);

      totalNumTests++;
    }  catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
      std::cerr << esc << redColor << esc << bold << "[FAIL]"
                << esc << defaultColor << esc << normal
                << " Test " << testName << " failed!" << std::endl;

      std::lock_guard<std::mutex> locker(statsMutex);

      numFailedTests++;
      totalNumTests++;
    }
  }

 private:
  std::string testSuiteName;
  static size_t numFailedTests;
  static size_t totalNumTests;
  static std::mutex statsMutex;
};

template<typename Arg>
void Require(const Arg& arg, const char* argName,
             const char* fileName, int line) {
  using ascii_codes::esc;
  using ascii_codes::redColor;
  using ascii_codes::defaultColor;
  using ascii_codes::bold;
  using ascii_codes::normal;

  if (!arg) {
    std::ostringstream reason;

    reason << esc << redColor << esc << bold
           << "ERROR: "
           << esc << defaultColor << esc << normal
           << fileName << ":" << line << " "
           << argName << "(" << arg << ") equals to false!";

    throw std::runtime_error(reason.str());
  }
}

template<typename Left, typename Right>
void RequireEqual(const Left& left, const Right& right,
                  const char* leftName, const char* rightName,
                  const char* fileName, int line) {
  using ascii_codes::esc;
  using ascii_codes::redColor;
  using ascii_codes::defaultColor;
  using ascii_codes::bold;
  using ascii_codes::normal;

  if (left != right) {
    std::ostringstream reason;

    reason << esc << redColor << esc << bold
           << "ERROR: "
           << esc << defaultColor << esc << normal
           << fileName << ":" << line << " "
           << leftName << " != " << rightName
           << "  (" << left << " != " << right << ")!";

    throw std::runtime_error(reason.str());
  }
}

template<typename FloatingPointType>
void RequireClose(FloatingPointType left, FloatingPointType right,
                  FloatingPointType tolerance,
                  const char* leftName, const char* rightName,
                  const char* fileName, int line) {
  using ascii_codes::esc;
  using ascii_codes::redColor;
  using ascii_codes::defaultColor;
  using ascii_codes::bold;
  using ascii_codes::normal;

  if (std::fabs(left - right) >
      std::max(std::fabs(left), std::fabs(right)) * tolerance) {
    std::ostringstream reason;

    reason << esc << redColor << esc << bold
           << "ERROR: "
           << esc << defaultColor << esc << normal
           << fileName << ":" << line << " "
           << leftName << " != " << rightName
           << "  (" << left << " != " << right << ")!";

    throw std::runtime_error(reason.str());
  }
}

template<typename ActionType, typename ExceptionType>
void RequireThrow(ActionType action,
                  const char* actionName,
                  const char* exceptionName,
                  const char* fileName,
                  int line) {
  using ascii_codes::esc;
  using ascii_codes::redColor;
  using ascii_codes::defaultColor;
  using ascii_codes::bold;
  using ascii_codes::normal;

  bool exceptionThrown = false;

  try {
    action();
  } catch (ExceptionType&) {
    exceptionThrown = true;
  }

  if (exceptionThrown == false) {
    std::ostringstream reason;

    reason << esc << redColor << esc << bold
           << "ERROR: "
           << esc << defaultColor << esc << normal
           << fileName << ":" << line << " "
           << "Action " << actionName
           << " didn't throw " << exceptionName << "!";

    throw std::runtime_error(reason.str());
  }
}

#define RUN_TEST(suite, test) suite.RunTest(test, #test)

#define RUN_TEST_REMOTE(suite, cli, test) suite.RunTest(cli, test, #test)

#define REQUIRE(arg) Require(arg, #arg, __FILE__, __LINE__)

#define REQUIRE_EQUAL(left, right) \
  RequireEqual(left, right, #left, #right, __FILE__, __LINE__)

#define REQUIRE_CLOSE(left, right, tolerance) \
  RequireClose(left, right, tolerance, #left, #right, __FILE__, __LINE__)

#define REQUIRE_THROW(action, ExceptionType)                                  \
  RequireThrow<std::function<void(void)>, ExceptionType>(                     \
    [&]() {                                                                   \
      action;                                                                 \
    },                                                                        \
    #action,                                                                  \
    #ExceptionType,                                                           \
    __FILE__,                                                                 \
    __LINE__                                                                  \
  )

#endif  // TESTS_TEST_CORE_HPP_
