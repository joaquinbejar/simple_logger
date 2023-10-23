//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#include "simple_logger/logger.h"

namespace simple_logger {

    std::string get_colored_level(LogLevel level) {
        switch (level) {
            case LogLevel::INFORMATIONAL:
                return simple_color::give_color(simple_color::Colors::WHITE, "[INFO]: ");
            case LogLevel::DEBUG:
                return simple_color::give_color(simple_color::Colors::LIGHTGOLDENROD1, "[DEBUG]: ");
            case LogLevel::ERROR:
                return simple_color::give_color(simple_color::Colors::RED, "[ERROR]: ");
            case LogLevel::EMERGENCY:
                return simple_color::give_color(simple_color::Colors::MAGENTA, "[EMERGENCY]: ");
            case LogLevel::ALERT:
                return simple_color::give_color(simple_color::Colors::ORANGE, "[ALERT]: ");
            case LogLevel::CRITICAL:
                return simple_color::give_color(simple_color::Colors::ORANGE, "[CRITICAL]: ", true);
            case LogLevel::WARNING:
                return simple_color::give_color(simple_color::Colors::YELLOW, "[WARNING]: ");
            case LogLevel::NOTICE:
                return simple_color::give_color(simple_color::Colors::LIGHTGREEN, "[NOTICE]: ");
            default:
                return "[UNKNOWN]: ";
        }
    }

    Logger::Logger(simple_config::Config &config) {
        this->setLevel(*config.loglevel);
    }

    Logger::Logger(const std::string &level) {
        this->setLevel(level);
    }

    Logger::Logger(const std::string &level, const std::string &filename) {
        setFile(filename);
        this->setLevel(level);
    }

    Logger::Logger(const std::shared_ptr<std::string> &level) {
        this->setLevel(*level);
    }

    Logger::Logger(const LogLevel &level, const std::string &filename) {
        setFile(filename);
        this->m_level = level;
    }

    Logger::Logger(const LogLevel &level) {
        this->m_level = level;
    }

    void Logger::setLevel(const std::string &s) {
        if (s == "debug") {
            this->m_level = LogLevel::DEBUG;
        } else if (s == "error") {
            this->m_level = LogLevel::ERROR;
        } else if (s == "warning") {
            this->m_level = LogLevel::WARNING;
        } else if (s == "notice") {
            this->m_level = LogLevel::NOTICE;
        } else if (s == "alert") {
            this->m_level = LogLevel::ALERT;
        } else if (s == "emergency") {
            this->m_level = LogLevel::EMERGENCY;
        } else if (s == "critical") {
            this->m_level = LogLevel::CRITICAL;
        } else {
            this->m_level = LogLevel::INFORMATIONAL;
        }
    }

    void Logger::setLevel(const LogLevel &level) {
        this->m_level = level;
    }

    LogLevel Logger::getLevel() const {
        return m_level;
    }

    void Logger::setFile(const std::string &s) {
        m_log_file = std::ofstream(s, std::ios_base::app | std::ios_base::out);
        write_to_file = true;
    }

    void Logger::debug(const std::string &s, bool flush) {
        if (this->m_level == LogLevel::DEBUG) {
            if (s.empty()) {
                std::lock_guard<std::mutex> lock(m_mtx);
                std::cout << std::endl;
                return;
            }
            this->m_log(get_colored_level(LogLevel::DEBUG) + s, flush);
        }
    }

    void Logger::info(const std::string &s, bool flush) {
        if (this->m_level == LogLevel::DEBUG || this->m_level == LogLevel::INFORMATIONAL) {
            if (s.empty()) {
                std::lock_guard<std::mutex> lock(m_mtx);
                std::cout << std::endl;
                return;
            }
            this->m_log(get_colored_level(LogLevel::INFORMATIONAL) + s, flush);
        }
    }

    void Logger::error(const std::string &s, bool flush) {
        if (this->m_level == LogLevel::ERROR || this->m_level == LogLevel::DEBUG ||
            this->m_level == LogLevel::INFORMATIONAL) {
            if (s.empty()) {
                std::lock_guard<std::mutex> lock(m_mtx);
                std::cerr << std::endl;
                return;
            }
            this->m_log(get_colored_level(LogLevel::ERROR) + s, flush);
        }
    }

    void Logger::safe_cout(const std::string &s, bool flush) {
        std::lock_guard<std::mutex> lock(m_mtx);
        std::cout << s;
        if (flush) {
            std::cout.flush();
        }
    }

    void Logger::safe_cerr(const std::string &s, bool flush) {
        std::lock_guard<std::mutex> lock(m_mtx);
        std::cerr << s;
        if (flush) {
            std::cerr.flush();
        }
    }

    void Logger::m_log(const std::string &s, bool flush) {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::ostringstream time_prefix;
        time_prefix << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << " " << s;
        if (flush) {
            safe_cout("\r" + time_prefix.str(), flush);
        } else {
            safe_cout(time_prefix.str() + "\n");
        }

        if (write_to_file) {
            m_log_file << time_prefix.str() << std::endl;
        }
    }

    void Logger::m_log_error(const std::string &s, bool flush) {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::ostringstream time_prefix;
        time_prefix << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << " " << s;
        if (flush) {
            safe_cerr("\r" + time_prefix.str(), flush);
        } else {
            safe_cerr(time_prefix.str() + "\n");
        }

        if (write_to_file) {
            m_log_file << time_prefix.str() << std::endl;
        }
    }

    bool Logger::operator==(const Logger &rhs) const {
        if (this->m_level == rhs.m_level && this->write_to_file == rhs.write_to_file) {
            return true;
        }
        return false;
    }

    Logger::Logger(const Logger &rhs) {
        this->m_level = rhs.m_level;
        this->write_to_file = rhs.write_to_file;
    }

    Logger &Logger::operator=(const Logger &rhs) {
        if (this == &rhs) {
            return *this;
        }
        this->m_level = rhs.m_level;
        this->write_to_file = rhs.write_to_file;
        return *this;
    }

    Logger::Logger(Logger &&rhs) noexcept {
        this->m_level = rhs.m_level;
        this->write_to_file = rhs.write_to_file;
    }

    Logger &Logger::operator=(Logger &&rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }
        this->m_level = rhs.m_level;
        this->write_to_file = rhs.write_to_file;
        return *this;
    }

    bool Logger::operator!=(const Logger &rhs) const {
        return !(rhs == *this);
    }
}
