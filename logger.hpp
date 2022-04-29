/**
 * @file logger.hpp
 * @brief Logger header
 * @author Nikita Zhukov
 * @date 29.04.2022
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
        void logError(string errText);
};

#endif