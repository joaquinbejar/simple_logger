//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#include "simple_logger/logger.h"

namespace simple_logger {

    std::string log_level_to_string(LogLevel level) {
        auto it = log_level_strings.find(level);
        if (it != log_level_strings.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Unknown log level");
        }
    }

    std::string get_colored_level(LogLevel level) {
        switch (level) {
            case LogLevel::INFORMATIONAL:
                return simple_color::give_color(simple_color::Colors::WHITE, "[INFO]: ");
            case LogLevel::DEBUG:
                return simple_color::give_color(simple_color::Colors::SKYBLUE1, "[DEBUG]: ");
            case LogLevel::ERROR:
                return simple_color::give_color(simple_color::Colors::RED, "[ERROR]: ");
            case LogLevel::EMERGENCY:
                return simple_color::give_color(simple_color::Colors::MAGENTA, "[EMERGENCY]: ");
            case LogLevel::ALERT:
                return simple_color::give_color(simple_color::Colors::DEEPPINK1A, "[ALERT]: ");
            case LogLevel::CRITICAL:
                return simple_color::give_color(simple_color::Colors::LIGHTCORAL, "[CRITICAL]: ", true);
            case LogLevel::WARNING:
                return simple_color::give_color(simple_color::Colors::YELLOW, "[WARNING]: ");
            case LogLevel::NOTICE:
                return simple_color::give_color(simple_color::Colors::LIGHTGREEN, "[NOTICE]: ");
            default:
                return simple_color::give_color(simple_color::Colors::GREY54, "[UNKNOWN]: ");
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

    bool Logger::operator==(const Logger &rhs) const {
        if (this->m_level == rhs.m_level && this->write_to_file == rhs.write_to_file) {
            return true;
        }
        return false;
    }

    bool Logger::operator!=(const Logger &rhs) const {
        return !(rhs == *this);
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

    void Logger::safe_cout(const std::string &s, bool flush) {
        std::unique_lock<std::mutex> lock(m_mtx);
        std::string ss = s;
        if (flush) {
            ss = "\r" + s;
        } else if (last_was_flush.load(std::memory_order_seq_cst)) {
            ss = "\n" + s;
        }

        if (!flush) {
            ss = ss + "\n";
        }

        std::cout << ss;
        last_was_flush.store(flush, std::memory_order_seq_cst);
        std::cout.flush();
        lock.unlock();

        if (write_to_file) {
            std::lock_guard<std::mutex> file_lock(m_mtx_file);
            m_log_file << s;
        }
    }

    void Logger::safe_cerr(const std::string &s, bool flush) {
        std::unique_lock<std::mutex> lock(m_mtx);
        std::string ss = s;
        if (flush) {
            ss = "\r" + s;
        } else if (last_was_flush.load(std::memory_order_seq_cst)) {
            ss = "\n" + s;
        }

        if (!flush) {
            ss = ss + "\n";
        }

        std::cerr << ss;
        last_was_flush.store(flush, std::memory_order_seq_cst);
        std::cerr.flush();
        lock.unlock();


        if (write_to_file) {
            std::lock_guard<std::mutex> file_lock(m_mtx_file);
            m_log_file << s;
        }
    }

    void Logger::m_log(const std::string &s, bool flush) {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::ostringstream time_prefix;
        time_prefix << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << " " << s;
        safe_cout(time_prefix.str(), flush);
    }

    void Logger::m_log_error(const std::string &s, bool flush) {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::ostringstream time_prefix;
        time_prefix << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << " " << s;
        safe_cerr(time_prefix.str(), flush);
    }

    Logger::operator std::string() const {
        return log_level_to_string(m_level);
    }
}
