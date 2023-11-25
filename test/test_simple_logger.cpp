//
// Created by Joaquin Bejar Garcia on 1/2/23.
//

#include <simple_logger/logger.h>
#include <simple_config/config.h>
#include <catch2/catch_test_macros.hpp>

using namespace simple_logger;

class LoggerConfig : public simple_config::Config {
public:
    [[nodiscard]] bool validate() override {
        return true;
    }

    [[nodiscard]] std::string to_string() const override {
        return R"({ "loglevel": ")" + *loglevel + R"(", "test1": ")" R"(", "test2": )" +
               " }";
    }

    [[nodiscard]] json to_json() const override;

    void from_json(const json &j) override;
};

json LoggerConfig::to_json() const {
    return {};
}

void LoggerConfig::from_json(const json &j) {

}

// ---------------------------------------------------------------------------------------------------

TEST_CASE("Logger rule of 5 ", "[Logger]") {
    simple_logger::Logger obj1("debug");
    std::shared_ptr<simple_logger::Logger> obj2 = std::make_shared<simple_logger::Logger>("debug");
    const std::shared_ptr<simple_logger::Logger>& obj3 = obj2;
    simple_logger::Logger obj4(std::move(obj1));
    simple_logger::Logger obj5 = std::move(*obj2);
    REQUIRE(obj3->getLevel() == LogLevel::DEBUG);
    REQUIRE(obj4.getLevel() ==LogLevel::DEBUG);
    REQUIRE(obj5.getLevel() == LogLevel::DEBUG);
}

TEST_CASE("Declare logger", "[Logger]") {
    simple_logger::Logger logger = simple_logger::Logger((std::string) "debug");
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
    logger.setLevel("info");
    REQUIRE(logger.getLevel() == LogLevel::INFORMATIONAL);
    REQUIRE(!std::string("info").empty());
}

TEST_CASE("Declare logger on file", "[Logger]") {
    simple_logger::Logger logger("debug", "test.log");
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
    logger.setLevel("info");
    REQUIRE(logger.getLevel() == LogLevel::INFORMATIONAL);

    simple_logger::Logger logger1(LogLevel::DEBUG, "test.log");
    REQUIRE(logger1.getLevel() == LogLevel::DEBUG);
    logger1.setLevel("info");
    REQUIRE(logger1.getLevel() == LogLevel::INFORMATIONAL);
}

TEST_CASE("Declare logger Config wrong", "[Logger]") {
    setenv("LOGLEVEL", "caca", 1);
    LoggerConfig config;
    simple_logger::Logger logger(config);
    REQUIRE(logger.getLevel() == LogLevel::INFORMATIONAL);
    REQUIRE(*config.loglevel == "caca");
    logger.setLevel("debug");
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
}

TEST_CASE("Declare logger Config", "[Logger]") {
    setenv("LOGLEVEL", "error", 1);
    LoggerConfig config;
    simple_logger::Logger logger(config);
    REQUIRE(logger.getLevel() == LogLevel::ERROR);
}

TEST_CASE("Change logger Config", "[Logger]") {
    setenv("LOGLEVEL", "error", 1);
    LoggerConfig config;
    simple_logger::Logger logger(config);
    REQUIRE(logger.getLevel() == LogLevel::ERROR);
    logger.setLevel("debug");
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
}

TEST_CASE("Change logger string pointer", "[Logger]") {
    auto loglevel = LogLevel::ERROR;
    simple_logger::Logger logger(loglevel);
    REQUIRE(logger.getLevel() == LogLevel::ERROR);
    logger.setLevel(LogLevel::DEBUG);
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
}

TEST_CASE("Change logger string pointer error", "[Logger]") {
    auto loglevel = LogLevel::ERROR;
    simple_logger::Logger logger(loglevel);
    REQUIRE(logger.getLevel() == LogLevel::ERROR);
    logger.send<LogLevel::ERROR>("level error print error");
    logger.send<LogLevel::INFORMATIONAL>("level error print info");
    logger.send<LogLevel::DEBUG>("level error print debug");
}

TEST_CASE("Change logger string pointer info", "[Logger]") {
    auto loglevel = LogLevel::INFORMATIONAL;
    simple_logger::Logger logger(loglevel);
    REQUIRE(logger.getLevel() == LogLevel::INFORMATIONAL);
    logger.send<LogLevel::ERROR>("level info print error");
    logger.send<LogLevel::INFORMATIONAL>("level info print info");
    logger.send<LogLevel::DEBUG>("level info print debug");
}

TEST_CASE("Change logger string pointer debug", "[Logger]") {
    std::shared_ptr<std::string> loglevel = std::make_shared<std::string>("debug");
    simple_logger::Logger logger(loglevel);
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
    logger.send<LogLevel::ERROR>("level debug print error");
    logger.send<LogLevel::INFORMATIONAL>("level debug print info");
    logger.send<LogLevel::DEBUG>("level debug print debug");
}

TEST_CASE("Logger send template function") {

    SECTION("logger to string") {
        simple_logger::Logger logger("debug"); // must log everything
        REQUIRE("debug" == (std::string) logger);
    }

    SECTION("Log level Debug") {
        simple_logger::Logger logger("debug"); // must log everything

        logger.send<LogLevel::INFORMATIONAL>("I'm DEBUG and this is a Info message");
        logger.send<LogLevel::ERROR>("I'm DEBUG and this is a Error message");
        logger.send<LogLevel::DEBUG>("I'm DEBUG and this is a Debug message");
        logger.send<LogLevel::CRITICAL>("I'm DEBUG and this is a Critical message");
    }

    SECTION("Log level Info") {
        simple_logger::Logger logger(LogLevel::INFORMATIONAL);

        logger.send<LogLevel::INFORMATIONAL>("I'm INFORMATIONAL and this is a Info message");
        logger.send<LogLevel::ERROR>("I'm INFORMATIONAL and this is a Error message");
        logger.send<LogLevel::DEBUG>("I'm INFORMATIONAL and this is a Debug message"); // not logged
        logger.send<LogLevel::CRITICAL>("I'm INFORMATIONAL and this is a Critical message");
    }

    SECTION("Log level Notice") {
        simple_logger::Logger logger(LogLevel::NOTICE);

        logger.send<LogLevel::INFORMATIONAL>("I'm NOTICE and this is a Info message"); // not logged
        logger.send<LogLevel::NOTICE>("I'm NOTICE and this is a Notice message");
        logger.send<LogLevel::ERROR>("I'm NOTICE and this is a Error message");
        logger.send<LogLevel::DEBUG>("I'm NOTICE and this is a Debug message"); // not logged
        logger.send<LogLevel::CRITICAL>("I'm NOTICE and this is a Critical message");
    }

    SECTION("Log level Error") {
        simple_logger::Logger logger(LogLevel::ERROR);

        logger.send<LogLevel::INFORMATIONAL>("I'm ERROR and this is a Info message"); // not logged
        logger.send<LogLevel::ERROR>("I'm ERROR and this is a Error message");
        logger.send<LogLevel::DEBUG>("I'm ERROR and this is a Debug message"); // not logged
        logger.send<LogLevel::CRITICAL>("I'm ERROR and this is a Critical message");
    }

    SECTION("Log level Alert") {
        simple_logger::Logger logger(LogLevel::ALERT);

        logger.send<LogLevel::INFORMATIONAL>("I'm ALERT and this is a Info message"); // not logged
        logger.send<LogLevel::ERROR>("I'm ALERT and this is a Error message"); // not logged
        logger.send<LogLevel::DEBUG>("I'm ALERT and this is a Debug message"); // not logged
        logger.send<LogLevel::CRITICAL>("I'm ALERT and this is a Critical message"); // not logged
        logger.send<LogLevel::ALERT>("I'm ALERT and this is a ALERT message");
        logger.send<LogLevel::EMERGENCY>("I'm ALERT and this is a EMERGENCY message");
    }

    SECTION("Log level EMERGENCY") {
        simple_logger::Logger logger(LogLevel::EMERGENCY);

        logger.send<LogLevel::INFORMATIONAL>("I'm EMERGENCY and this is a Info message"); // not logged
        logger.send<LogLevel::ERROR>("I'm EMERGENCY and this is a Error message"); // not logged
        logger.send<LogLevel::DEBUG>("I'm EMERGENCY and this is a Debug message"); // not logged
        logger.send<LogLevel::CRITICAL>("I'm EMERGENCY and this is a Critical message"); // not logged
        logger.send<LogLevel::ALERT>("I'm EMERGENCY and this is a ALERT message"); // not logged
        logger.send<LogLevel::EMERGENCY>("I'm EMERGENCY and this is a EMERGENCY message");
    }

}