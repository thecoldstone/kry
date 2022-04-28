/**
 * @file logginh.hpp
 * @brief Logger header
 * @author Nikita Zhukov
 * @date 15.04.2022
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#include <gmpxx.h>

using namespace std;

class Logger {
    public:
        bool LOGGING;

        Logger();
        void log(string logText, bool newl);
        void logGMPVariable(mpz_class var, bool newl);
};

#endif