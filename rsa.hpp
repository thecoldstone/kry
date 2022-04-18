/**
 * @file rsa.hpp
 * @brief RSA header
 * @author Nikita Zhukov
 * @date 09.04.2022
 */

#ifndef _RSA_H
#define _RSA_H

#include <gmp.h>
#include <gmpxx.h>
#include "logger.hpp"

enum RSAOPERATION {
    GENERATE,
    ENCRYPT,
    DECRYPT,
    CRACK
};

class RSA {
    public:
        RSA();
        int parse(int argc, char **argv);
        int run();
    protected:
        int rsa();
        mpz_class generatePrime();
    private:
        Logger logger;
        RSAOPERATION op;
        int B;
        mpz_class P;
        mpz_class Q;
        mpz_class N;
        mpz_class E;
        mpz_class D;
};

#endif