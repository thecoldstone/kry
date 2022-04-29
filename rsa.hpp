/**
 * @file rsa.hpp
 * @brief RSA header
 * @author Nikita Zhukov
 * @date 29.04.2022
 */

#ifndef _RSA_H
#define _RSA_H

#include <gmp.h>
#include <gmpxx.h>
#include "logger.hpp"

#define SSACCURACY 100
#define ITERATIONS 1000000

enum RSAOPERATION
{
    GENERATE,
    ENCRYPT,
    DECRYPT,
    CRACK
};

class RSA : private Logger
{
public:
    RSA();
    int parse(int argc, char **argv);
    void run();

protected:
    void rsa();
    void encrypt();
    void decipher();
    void bruteForce();
    void pollardRho();
    void crack();

private:
    RSAOPERATION op;
    int B;
    mpz_class P;
    mpz_class Q;
    mpz_class N;
    mpz_class E;
    mpz_class D;
    mpz_class M;
    mpz_class C;

    mpz_class Phi;

    void printOutput();
    bool solovayStrassen(const mpz_class number, int k);
    void checkPrime(mpz_class &number);
    int jacobi(const mpz_class a, const mpz_class n);

    mpz_class modPow(mpz_class base, mpz_class exp, mpz_class mod);
    mpz_class modInv(mpz_class a, mpz_class b);
    mpz_class gcd(mpz_class a, mpz_class b);
    mpz_class generatePrime();
};

#endif