/**
 * @file logginh.hpp
 * @brief Logger header
 * @author Nikita Zhukov
 * @date 15.04.2022
 */

#ifndef _LOGGER_H
#define _LOGGER_H

using namespace std;

class Logger {
    public:
        Logger();
        void log(string logText);
};

#endif