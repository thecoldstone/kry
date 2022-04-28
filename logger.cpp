#include <iostream>
#include <cctype>
#include "logger.hpp"

using namespace std;

Logger::Logger(){

};

/**
 * @brief Logges text
 * @todo Add time
 *
 * @param logText
 */
void Logger::log(string logText, bool newl)
{
    if (LOGGING) {
        cout << "[+]";
    }
    cout << logText;
    if (newl)
        cout << endl;
}

/**
 * @brief 
 * 
 * @param var 
 */
void Logger::logGMPVariable(mpz_class var, bool newl) 
{
    gmp_printf("%#Zx ", var.get_mpz_t());

    if (newl)
        cout << endl;
}