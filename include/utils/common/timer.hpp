//
// Created by lei on 22-11-16.
//

#ifndef GPUPROGRAM_COMMON_HPP
#define GPUPROGRAM_COMMON_HPP

#include <chrono>
#include <iostream>
#include <fstream>
#include <utility>

/**
 * 计时器
 */
class Timer {
public:
    Timer() { _name = ""; }

    explicit Timer(std::string name) : _name(std::move(name)) {
        restart();
    }

    /**
     * 获取当前时间
     * @return
     */
    inline std::chrono::system_clock::time_point get_time() {
        return std::chrono::system_clock::now();
    }

    /**
    * 启动计时
    */
    inline void restart() {
        _start_time = std::chrono::steady_clock::now();
    }

    /**
    * 结束计时
    * @return 返回ms数
    */
    inline double elapsed(bool restart = false) {
        _end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = _end_time - _start_time;
        if (restart) this->restart();
        return diff.count() * 1000;
    }

    /**
    * 打印时间
    * @param reset 输出之后是否重启计时器，true重启，false不重启
    * @param unit_ms true是ms，false是s
    * @param tip 输出提示
    * @param kill 输出之后是否退出程序，true退出，false不退出
    */
    void log(bool reset = false, bool unit_ms = false,
             const std::string &tip = "time: ", bool kill = false) {
        INFO(_name);
        if (unit_ms) {
            if (tip.length() > 0) {
                _DEBUG_P(tip, elapsed());
                DEBUG("ms");
            } else {
                _DEBUG(elapsed());
                DEBUG("ms");
            }
        } else {
            if (tip.length() > 0) {
                _DEBUG_P(tip, elapsed() / 1000.0);
                DEBUG("s");
            } else {
                _DEBUG(elapsed() / 1000.0);
                DEBUG("s");
            }
        }

        if (reset) this->restart();

        if (kill) exit(5);
    }

private:
    std::chrono::steady_clock::time_point _start_time;
    std::chrono::steady_clock::time_point _end_time;
    std::string _name;
};

#endif //GPUPROGRAM_COMMON_HPP
