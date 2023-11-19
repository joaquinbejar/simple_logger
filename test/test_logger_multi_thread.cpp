//
// Created by Joaquin Bejar Garcia on 1/3/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_logger/logger.h>
#include <thread>

using namespace simple_logger;

TEST_CASE("Logger can print in multi-thread environment", "[Cache]") {
    simple_logger::Logger logger("debug", "test.log");
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
    std::vector<std::thread> threads;
    int number_of_threads = 30;
    threads.reserve(number_of_threads);
    for (int i = 0; i < number_of_threads; ++i) {
        threads.emplace_back([&logger,  i] {
            for (int j = 0; j < 1000; ++j) {
                logger.send<LogLevel::DEBUG>("This is a debug message from thread " + std::to_string(i));
                logger.send<LogLevel::INFORMATIONAL>("This is a info message from thread " + std::to_string(i));
                logger.send<LogLevel::ERROR>("This is a error message from thread " + std::to_string(i));
            }
        });
    }
    for (auto &thread : threads) {
        thread.join();
    }
    REQUIRE(true);
}