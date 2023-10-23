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
    simple_logger::Logger obj2(obj1);
    simple_logger::Logger obj3 = obj1;
    simple_logger::Logger obj4(std::move(obj1));
    simple_logger::Logger obj5 = std::move(obj2);
    REQUIRE(obj3.getLevel() == LogLevel::DEBUG);
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
}

TEST_CASE("Declare logger Config wrong", "[Logger]") {
    setenv("LOGLEVEL", "caca", 1);
    LoggerConfig config;
    simple_logger::Logger logger(config);
    REQUIRE(logger.getLevel() == LogLevel::INFORMATIONAL);
    REQUIRE(*config.loglevel == "info");
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
    *config.loglevel = "debug";
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
}

TEST_CASE("Change logger string pointer", "[Logger]") {
    auto loglevel = LogLevel::ERROR;
    simple_logger::Logger logger(loglevel);
    REQUIRE(logger.getLevel() == LogLevel::ERROR);
    loglevel = LogLevel::DEBUG;
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
}

TEST_CASE("Change logger string pointer error", "[Logger]") {
    auto loglevel = LogLevel::ERROR;
    simple_logger::Logger logger(loglevel);
    REQUIRE(logger.getLevel() == LogLevel::ERROR);
    logger.error("level error print error");
    logger.info("level error print info");
    logger.debug("level error print debug");
}

TEST_CASE("Change logger string pointer info", "[Logger]") {
    auto loglevel = LogLevel::INFORMATIONAL;
    simple_logger::Logger logger(loglevel);
    REQUIRE(logger.getLevel() == LogLevel::INFORMATIONAL);
    logger.error("level info print error");
    logger.info("level info print info");
    logger.debug("level info print debug");
}

TEST_CASE("Change logger string pointer debug", "[Logger]") {
    std::shared_ptr<std::string> loglevel = std::make_shared<std::string>("debug");
    simple_logger::Logger logger(loglevel);
    REQUIRE(logger.getLevel() == LogLevel::DEBUG);
    logger.error("level debug print error");
    logger.info("level debug print info");
    logger.debug("level debug print debug");
}