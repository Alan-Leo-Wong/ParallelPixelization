//
// Created by lei on 22-11-17.
//

#ifndef GPUPROGRAM_LOG_HPP
#define GPUPROGRAM_LOG_HPP

#include <fstream>
#include <ctime>
#include <utility>
#include "infoPrint.hpp"
#include "../utils/common/timer.hpp"

typedef enum logLevel {
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG
} log_level;

class Log {
public:
    Log() {
        to_console = true;
        print_time = false;
    }

    explicit Log(bool pt) {
        to_console = true;
        print_time = pt;
    }

    explicit Log(std::string path) : log_path(std::move(path)) {
        to_console = true;
        print_time = false;
    }

    Log(std::string path, bool pt) : log_path(std::move(path)) {
        to_console = true;
        print_time = pt;
    }

    ~Log() = default;

    void log_out(const std::string &line, const std::string &pre, const std::string &info, log_level level);

protected:
    std::string get_current_time();

    void
    flush_to_console(const std::string &line, const std::string &pre, const std::string &info,
                     log_level level); // print log info to console
    void
    flush_to_file(const std::string &line, const std::string &pre, const std::string &info,
                  log_level level); // print log info to ".txt" file

private:
    bool to_console;
    bool print_time;
    std::string log_path; // path to save logs
    Timer t;
};

void Log::log_out(const std::string &line, const std::string &pre, const std::string &info, log_level level) {
    if (to_console) flush_to_console(line, pre, info, level);
    else flush_to_file(line, pre, info, level);
}

std::string Log::get_current_time() {
    time_t now = std::chrono::system_clock::to_time_t(t.get_time());
    struct tm *local = localtime(&now);
    std::string current_time;
    char temp[50] = {0};
    sprintf(temp, "[%d-%d-%d %d:%d:%d]", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
            local->tm_hour, local->tm_min, local->tm_sec);
    current_time = temp;
    return current_time;
}

void Log::flush_to_console(const std::string &line, const std::string &pre, const std::string &info, log_level level) {
    std::string pre_msg;
    pre_msg = print_time ? get_current_time() + line : line;
    DIG(pre_msg);

    std::string content;
    switch (level) {
        case FATAL:
            _FATAL("[FATAL]::");
            FATAL_P(pre, info);
            break;
        case ERROR:
            _ERROR("[ERROR]::");
            ERROR_P(pre, info);
            break;
        case WARN:
            _WARN("[WARN]::");
            WARN_P(pre, info);
            break;
        case INFO:
            _INFO("[INFO]::");
            INFO_P(pre, info);
            break;
        case DEBUG:
            _DEBUG("[DEBUG]::");
            DEBUG_P(pre, info);
            break;
        default:
            WARN_P("[WARN]::(Invalid level)\n", info);
            break;
    }
}

void Log::flush_to_file(const std::string &line, const std::string &pre, const std::string &info, log_level level) {
    std::ofstream outfile(log_path.c_str(), std::ios::app);
    if (!outfile) {
        std::string content = "INVALID LOG PATH: " + log_path + ", log will be flushed to the console.";
        WARN(content);
        flush_to_console(line, pre, info, level);
    } else {
        INFO_P("LOG WILL BE SAVED: ", log_path);

        std::string pre_msg;
        pre_msg = print_time ? get_current_time() + line : line;

        switch (level) {
            case FATAL:
                outfile << pre_msg << "[FATAL]::" << pre << info << std::endl;
                break;
            case ERROR:
                outfile << pre_msg << "[ERROR]::" << pre << info << std::endl;
                break;
            case WARN:
                outfile << pre_msg << "[WARN]::" << pre << info << std::endl;
                break;
            case INFO:
                outfile << pre_msg << "[INFO]::" << pre << info << std::endl;
                break;
            case DEBUG:
                outfile << pre_msg << "[DEBUG]::" << pre << info << std::endl;
                break;
            default:
                outfile << pre_msg << "[WARN]::(Invalid level) " << std::endl << pre << info << std::endl;
                break;
        }
    }
    outfile.close();
}

#define LOG(pre, info, level)                                    \
do{                                                         \
    char temp[256] = {0};                                   \
    std::string line;                                       \
    sprintf(temp, "[File:%s Line:%04d]", __FILE__, __LINE__);\
    line = temp;                                            \
    Log log;                                                \
    log.log_out(line, pre, info, level);                             \
}while(0)

#endif //GPUPROGRAM_LOG_HPP
