//
// Created by Joaquin Bejar Garcia on 19/10/23.
//

#include "simple_logger/logger.h"

namespace simple_logger {
    Logger::Logger(simple_config::Config &config) {
        if (*config.loglevel != "debug" && *config.loglevel != "info" && *config.loglevel != "error") {
            *config.loglevel = "info";
        }
        this->m_level = std::shared_ptr<std::string>(config.loglevel);
    }

    Logger::Logger(const std::string &level) {
        this->setLevel(level);
    }

    Logger::Logger(const std::string &level, const std::string &filename) {
        setFile(filename);
        *this->m_level = level;
    }

    Logger::Logger(const std::shared_ptr<std::string> &level) {
        if (*level != "debug" && *level != "info" && *level != "error") {
            *level = "info";
        }
        this->m_level = std::shared_ptr<std::string>(level);
    }

    void Logger::setLevel(const std::string &s) {
        if (s != "debug" && s != "info" && s != "error") {
            m_level = std::make_shared<std::string>("info");
        } else if (m_level == nullptr) {
            m_level = std::make_shared<std::string>(s);
        } else {
            *m_level = s;
        }
    }

    std::string Logger::getLevel() const {
        return *m_level;
    }

    void Logger::setFile(const std::string &s) {
        m_log_file = std::ofstream(s, std::ios_base::app | std::ios_base::out);
        write_to_file = true;
    }

    void Logger::log_debug(const std::string &s, bool flush) {
        if (*this->m_level == "debug") {
            if (s.empty()) {
                std::lock_guard<std::mutex> lock(m_mtx);
                std::cout << std::endl;
                return;
            }
            this->m_log(DEBUG_ + s, flush);
        }
    }

    void Logger::log_info(const std::string &s, bool flush) {
        if (*this->m_level == "debug" || *this->m_level == "info") {
            if (s.empty()) {
                std::lock_guard<std::mutex> lock(m_mtx);
                std::cout << std::endl;
                return;
            }
            this->m_log(INFO_ + s, flush);
        }
    }

    void Logger::log_error(const std::string &s, bool flush) {
        if (*this->m_level == "error" || *this->m_level == "info" || *this->m_level == "debug") {
            if (s.empty()) {
                std::lock_guard<std::mutex> lock(m_mtx);
                std::cerr << std::endl;
                return;
            }
            this->m_log_error(ERROR_ + s, flush);
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
        if (*this->m_level == *rhs.m_level && this->write_to_file == rhs.write_to_file) {
            return true;
        }
        return false;
    }

    Logger::Logger(const Logger &rhs) {
        this->m_level = std::make_shared<std::string>(*rhs.m_level);
        this->write_to_file = rhs.write_to_file;
    }

    Logger &Logger::operator=(const Logger &rhs) {
        if (this == &rhs) {
            return *this;
        }
        this->m_level = std::make_shared<std::string>(*rhs.m_level);
        this->write_to_file = rhs.write_to_file;
        return *this;
    }

    Logger::Logger(Logger &&rhs) noexcept {
        this->m_level = std::move(rhs.m_level);
        this->write_to_file = rhs.write_to_file;
    }

    Logger &Logger::operator=(Logger &&rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }
        this->m_level = std::move(rhs.m_level);
        this->write_to_file = rhs.write_to_file;
        return *this;
    }

    bool Logger::operator!=(const Logger &rhs) const {
        return !(rhs == *this);
    }
}
