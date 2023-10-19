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

    class Logger {
    public:

        explicit Logger(simple_config::Config &config);

        explicit Logger(const std::string &level);

        explicit Logger(const std::shared_ptr<std::string> &level);

        explicit Logger(const std::string &level, const std::string &filename);

        //copy constructor
        Logger(const Logger &rhs);

        //copy assignment
        Logger &operator=(const Logger &rhs);

        //move constructor
        Logger(Logger &&rhs) noexcept;

        //move assignment
        Logger &operator=(Logger &&rhs) noexcept;

        bool operator==(const Logger &rhs) const;

        bool operator!=(const Logger &rhs) const;

        void setLevel(const std::string &s);

        [[nodiscard]] std::string getLevel() const;

        void setFile(const std::string &s);

        void log_info(const std::string &s, bool flush = false);

        void log_debug(const std::string &s, bool flush = false);

        void log_error(const std::string &s, bool flush = false);

    private:
        std::shared_ptr<std::string> m_level = std::make_shared<std::string>("info");
        std::ofstream m_log_file;
        bool write_to_file = false;
        std::string INFO_ = simple_color::give_color("bwhite", "[INFO]: ");
        std::string DEBUG_ = simple_color::give_color("byellow", "[DEBUG]: ");
        std::string ERROR_ = simple_color::give_color("bred", "[ERROR]: ");

        void safe_cout(const std::string &s, bool flush = false);

        void safe_cerr(const std::string &s, bool flush = false);

        void m_log(const std::string &s, bool flush = false);

        void m_log_error(const std::string &s, bool flush = false);

        std::mutex m_mtx;
    };
    
}

#endif //SIMPLE_LOGGER_LOGGER_H
