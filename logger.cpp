#include <iostream>
#include "logger.hpp"

using namespace std;

Logger::Logger() {

};

/**
 * @brief Logges text 
 * @todo Add time 
 * 
 * @param logText 
 */
void Logger::log(string logText) {
    cout << "[+]" << logText << "..." << endl;
}