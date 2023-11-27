//
// Created by Joaquin Bejar Garcia on 1/3/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_logger/logger.h>
#include <thread>

using namespace simple_logger;

bool generateRandomBool() {
    // Inicializa el generador de números aleatorios
    std::random_device rd;
    std::mt19937 gen(rd()); // Generador de números aleatorios (Mersenne Twister)

    // Crea una distribución booleana (50% de probabilidad para true y false)
    std::bernoulli_distribution d(0.5);

    return d(gen); // Devuelve true o false de manera aleatoria
}


TEST_CASE("Logger can print in multi-thread environment", "[Cache]") {
    simple_logger::Logger logger("debug", "test.log");
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
    std::vector<std::thread> threads;
    int number_of_threads = 30;
    threads.reserve(number_of_threads);
    for (int i = 0; i < number_of_threads; ++i) {
        threads.emplace_back([&logger, i] {
            for (int j = 0; j < 1000; ++j) {
                logger.send<LogLevel::DEBUG>("This is a debug message from thread " + std::to_string(i));
                logger.send<LogLevel::INFORMATIONAL>("This is a info message from thread " + std::to_string(i));
                logger.send<LogLevel::ERROR>("This is a error message from thread " + std::to_string(i));
            }
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
    logger.send<LogLevel::INFORMATIONAL>("END");
    REQUIRE(true);
}

TEST_CASE("Logger can print in multi-thread environment and flush", "[Cache]") {
    simple_logger::Logger logger("debug", "test.log");
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
    std::vector<std::thread> threads;
    int number_of_threads = 30;
    threads.reserve(number_of_threads);
    for (int i = 0; i < number_of_threads; ++i) {
        threads.emplace_back([&logger, i] {
            for (int j = 0; j < 10; ++j) {
                auto random_bool = generateRandomBool();
                logger.send<LogLevel::DEBUG>("Last was " + std::to_string(logger.last_was_flush) +
                        " is debug flush: " + std::to_string(random_bool) + " | Thread: " +
                        std::to_string(i), random_bool);
                random_bool = generateRandomBool();
                logger.send<LogLevel::INFORMATIONAL>("Last was " + std::to_string(logger.last_was_flush) +
                        " is info flush: " + std::to_string(random_bool) + " | Thread: " +
                        std::to_string(i), random_bool);
                random_bool = generateRandomBool();
                logger.send<LogLevel::ERROR>(
                        "This is a error message from thread " + std::to_string(random_bool) + " | " +
                        std::to_string(i), random_bool);
            }
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
    REQUIRE(true);
}