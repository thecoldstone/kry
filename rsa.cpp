#include <iostream>
#include <cstddef>
#include <stddef.h>
#include "rsa.hpp"

using namespace std;

unsigned long int seed = time(NULL);
gmp_randclass randomizer = gmp_randclass(gmp_randinit_mt);

RSA::RSA() {
}

int getRandNumInBitRange(int bitRange) {
}

mpz_class RSA::generatePrime() {
    
    // TODO Do we need exception for checking the number of bits
    mpz_class randPrime = randomizer.get_z_bits(((this->B/2)));

    cout << "Random number: " << randPrime << endl;

    return randPrime;
}

int RSA::rsa() {
    randomizer.seed(seed);

    this->P = this->generatePrime();
    // this->Q = this->generatePrime();

    // this->N = this->P * this->Q;

    // TODO Compute public exponent e
    // TODO Compute private exponent d
}

int RSA::run() {
    switch (this->op)
    {
    case GENERATE:
        logger.log("Generating keys");
        this->rsa();
        break;
    case ENCRYPT:
        logger.log("Encrypting");
        break;
    case DECRYPT:
        logger.log("Decrypting");
        break;
    case CRACK:
        logger.log("Cracking");
        break;
    default:
        break;
    }
}