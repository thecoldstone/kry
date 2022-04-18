#include <iostream>
#include "rsa.hpp"

using namespace std;

int main(int argc, char **argv) {

    int rc = EXIT_SUCCESS;
    RSA rsa = RSA();
    
    // Parse arguments 
    rc = rsa.parse(argc, argv);

    // Run RSA based on passed arguments
    rc = rsa.run();

    return rc;
}