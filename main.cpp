/**
 * @file main.cpp
 * @brief Main file
 * @author Nikita Zhukov
 * @date 29.04.2022
 */

#include <iostream>
#include "rsa.hpp"

using namespace std;

int main(int argc, char **argv) {

    int rc = EXIT_SUCCESS;
    RSA rsa = RSA();
    
    // Parse arguments 
    if((rc = rsa.parse(argc, argv)) == 0) {
        // Run RSA based on passed arguments
        rsa.run();
    };

    return rc;
}