/**
 * @file logger.cpp
 * @brief Logger implementation
 * @author Nikita Zhukov
 * @date 29.04.2022
 */

#include <iostream>
#include <cctype>
#include "logger.hpp"

using namespace std;

Logger::Logger(){};

/**
 * @brief Prints out text
 * @todo Add time
 *
 * @param logText
 */
void Logger::log(string logText, bool newl)
{
    if (LOGGING)
    {
        cout << "[+]";
    }
    cout << logText;
    if (newl)
        cout << endl;
}

/**
 * @brief Prints out gmp variables 
 *
 * @param var
 */
void Logger::logGMPVariable(mpz_class var, bool newl)
{
    gmp_printf("%#Zx ", var.get_mpz_t());

    if (newl)
        cout << endl;
}

/**
 * @brief Prints out error text
 * 
 * @param errText 
 */
void Logger::logError(string errText)
{
    cout << "[!]" << errText << endl;
}