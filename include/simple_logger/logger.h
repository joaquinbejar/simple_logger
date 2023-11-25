//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#ifndef SIMPLE_LOGGER_LOGGER_H
#define SIMPLE_LOGGER_LOGGER_H

#include <iostream>
#include <mutex>
#include <simple_color/color.h>
#include <simple_config/config.h>
#include <fstream>

namespace simple_logger {

    enum class LogLevel {
        EMERGENCY = 0,
        ALERT = 1,
        CRITICAL = 2,
        ERROR = 3,
        WARNING = 4,
        NOTICE = 5,
        INFORMATIONAL = 6,
        DEBUG = 7
    };

    // create a map of log level strings
    static const std::map<LogLevel, std::string> log_level_strings{
            {LogLevel::EMERGENCY, "emergency"},
            {LogLevel::ALERT, "alert"},
            {LogLevel::CRITICAL, "critical"},
            {LogLevel::ERROR, "error"},
            {LogLevel::WARNING, "warning"},
            {LogLevel::NOTICE, "notice"},
            {LogLevel::INFORMATIONAL, "info"},
            {LogLevel::DEBUG, "debug"}
    };

    std::string log_level_to_string(LogLevel level);
    std::string get_colored_level(LogLevel level) ;


    class Logger {
    public:

        explicit Logger(simple_config::Config &config);

        explicit Logger(const std::string &level);

        explicit Logger(const std::shared_ptr<std::string> &level);

        explicit Logger(const std::string &level, const std::string &filename);

        explicit Logger(const LogLevel &level, const std::string &filename);

        explicit Logger(const LogLevel &level);

        Logger() = delete;

        Logger(const Logger &) = delete;

        //copy assignment
        Logger &operator=(const Logger &rhs) = delete;

        //move constructor
        Logger(Logger &&rhs) noexcept;

        //move assignment
        Logger &operator=(Logger &&rhs) noexcept;

        bool operator==(const Logger &rhs) const;

        bool operator!=(const Logger &rhs) const;

        explicit operator std::string() const;

        void setLevel(const std::string &s);

        void setLevel(const LogLevel &level);

        LogLevel getLevel() const;

        void setFile(const std::string &s);

        template<LogLevel logLevel>
        void send(const std::string &s, bool flush = false) {
            if (m_level >= logLevel) {
                if (logLevel <= LogLevel::ERROR) {
                    m_log_error(get_colored_level(logLevel) + s, flush);
                } else {
                    m_log(get_colored_level(logLevel) + s, flush);
                }
            }
        }

    private:
        LogLevel m_level = LogLevel::INFORMATIONAL;
        std::ofstream m_log_file;
        bool write_to_file = false;

        void safe_cout(const std::string &s, bool flush = false);

        void safe_cerr(const std::string &s, bool flush = false);

        void m_log(const std::string &s, bool flush = false);

        void m_log_error(const std::string &s, bool flush = false);

        std::mutex m_mtx;
    };
    
}

#endif //SIMPLE_LOGGER_LOGGER_H
