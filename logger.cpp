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
 * @param varName 
 * @param value 
 */
void Logger::logVariable(string varName, string value)
{
    string output = varName.append(" ");
    output = output.append("0x");
    output = output.append(value);
    output = output.append(" ");

    this->log(output, false);
}