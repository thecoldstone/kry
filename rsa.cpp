/**
 * @file rsa.cpp
 * @brief RSA Implementation
 * @author Nikita Zhukov
 * @date 29.04.2022
 */

#include <iostream>
#include <cstddef>
#include <stddef.h>
#include <stdio.h>
#include "rsa.hpp"

using namespace std;

unsigned long seed = time(NULL);
gmp_randclass randomizer = gmp_randclass(gmp_randinit_mt);

/**
 * @brief Construct a new RSA::RSA object
 *
 */
RSA::RSA()
{
    LOGGING = false;
}

/**
 * @brief Computes (base^exponent) % mod
 *
 * @param base
 * @param exp
 * @param mod
 * @return mpz_class
 */
mpz_class RSA::modPow(mpz_class base, mpz_class exp, mpz_class mod)
{
    mpz_class x = 1;
    mpz_class y = base;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            x = (x * y) % mod;

        y = (y * y) % mod;
        exp = exp >> 1;
    }

    return x % mod;
}

/**
 * @brief Computes modular multiplicative inverse
 *
 * @param a computation item
 * @param m modulo
 * @return mpz_class - multiplicative inverse
 */
mpz_class RSA::modInv(mpz_class a, mpz_class m)
{
    mpz_class m0 = m;
    mpz_class y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1)
    {
        mpz_class q = a / m;
        mpz_class t = m;

        m = a % m, a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0)
        x += m0;

    return x;
}

/**
 * @brief Computes a Jacobi symbol (a/n)
 *
 * @param a jacobi argument presented as the "numerator"
 * @param b jacobi argument presented as the "denominator"
 * @return int - jacobi symbol
 */
int RSA::jacobi(mpz_class a, mpz_class b)
{
    if (a == 0)
        return 0;
    a = a % b;

    int t = 1;
    mpz_class r;

    while (a != 0)
    {
        while (a % 2 == 0)
        {
            a = a / 2;
            r = b % 8;
            if (r == 3 || r == 5)
            {
                t = -t;
            }
        }

        swap(a, b);

        if (a % 4 == 3 && b % 4 == 3)
        {
            t = -t;
        }
        a = a % b;
    }

    if (b == 1)
    {
        return t;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Computes Greate Common Divisor (GCD) in the recursive manner
 *
 * @param a argument
 * @param b argument
 * @return mpz_class - greate common divisor
 */
mpz_class RSA::gcd(mpz_class a, mpz_class b)
{
    return b == 0 ? a : gcd(b, a % b);
}

/**
 * @brief Implementation of Solovay-Strassen Primarly test algorithm
 *
 * @param n primary number to check
 * @param k number of repetitions of the test
 * @return bool - is it a primary number
 */
bool RSA::solovayStrassen(const mpz_class n, int k)
{
    if (n == 2 || n == 3)
        return true;

    if (n == 1 || n % 2 == 0)
        return false;

    for (int i = 0; i < k; i++)
    {
        mpz_class a = randomizer.get_z_range(n);
        if (gcd(a, n) > 1)
        {
            return false;
        }

        mpz_class jacobian = (n + jacobi(a, n)) % n;

        if (jacobian == 0 || jacobian != modPow(a, (n - 1) / 2, n))
        {
            return false;
        }
    }

    return true;
};

/**
 * @brief Does the Solovay-Strassen test
 *
 * @param number number to check
 */
void RSA::checkPrime(mpz_class &number)
{
    while (!solovayStrassen(number, SSACCURACY))
    {
        number++ ++;
    }
};

/**
 * @brief Generates prime number
 *
 * @return mpz_class - prime number within specific range
 * based on passed B value
 */
mpz_class RSA::generatePrime()
{
    return randomizer.get_z_bits(((B / 2)));
}

/**
 * @brief Runs RSA algorithm
 */
void RSA::rsa()
{
    mpz_class phi;

    randomizer.seed(seed);

    P = generatePrime();
    Q = generatePrime();

    checkPrime(P);
    checkPrime(Q);

    N = P * Q;
    Phi = (P - 1) * (Q - 1);

    E = 3;
    while (gcd(E, Phi) != 1)
    {
        E++;
    }

    D = modInv(E, Phi);
}

/**
 * @brief Encryptes message
 */
void RSA::encrypt()
{
    C = modPow(M, E, N);
}

/**
 * @brief Decrypts message
 */
void RSA::decipher()
{
    M = modPow(C, D, N);
}

/**
 * @brief Brute force alogirthm to factorize
 */
void RSA::bruteForce()
{
    unsigned int divisor = 2;

    for (int i = 0; i < ITERATIONS; i++)
    {
        if (N % divisor == 0)
        {
            P = divisor;
            return;
        }

        divisor += 2;
    }
}

/**
 * @brief Implementation of Pollard Rho Algorithm to factorize big number
 */
void RSA::pollardRho()
{
    if (N == 1)
    {
        P = N;
        return;
    }

    if (N % 2 == 0)
    {
        P = 2;
        return;
    }

    mpz_class x;
    mpz_class y;
    mpz_class c;
    mpz_class d;

    while (true)
    {
        x = 2 + randomizer.get_z_range(N - 1);
        y = x;
        c = 1 + randomizer.get_z_range(N);
        d = 1;

        while (d == 1)
        {
            // Tortoise move: x(i + 1) = f(x(i))
            x = (modPow(x, 2, N) + c + N) % N;

            // Hare move: y(i + 1) = f(f(y(i)))
            y = (modPow(y, 2, N) + c + N) % N;
            y = (modPow(y, 2, N) + c + N) % N;

            // gcd |x-y| <> n
            d = gcd(abs(x - y), N);
            if (d == N)
            {
                break;
            }
        }

        // Check if d is our desired divisor
        if (d != N)
        {
            P = d;
            return;
        }
    }
}

/**
 * @brief Runs breaking methods to crack public modulus.
 * Bruteforce method and Pollards Rho.
 */
void RSA::crack()
{
    mpz_class _p = P;
    if (LOGGING)
    {
        log("Bruteforce(ing)...", true);
    }
    bruteForce();

    if (P != _p)
    {
        if (LOGGING)
        {
            log("Bruteforce cracked prime number!", true);
            logGMPVariable(P, true);
        }
        return;
    }

    if (LOGGING)
    {
        log("Pollard's Rho(ing)...", true);
    }
    pollardRho();

    if (P != _p)
    {
        if (LOGGING)
        {
            log("Pollard's Rho algorithm cracked prime number!", true);
            logGMPVariable(P, true);
        }
    }
}

/**
 * @brief Runs the RSA programme based on passed arguments from stdin.
 */
void RSA::run()
{
    switch (op)
    {
    case GENERATE:
        if (LOGGING)
            log("Generating keys", true);
        rsa();
        break;
    case ENCRYPT:
        if (LOGGING)
        {
            log("Encrypting", true);
            log("With variables:", true);
            logGMPVariable(E, false);
            logGMPVariable(N, false);
            logGMPVariable(M, true);
        }
        encrypt();
        break;
    case DECRYPT:
        if (LOGGING)
        {
            log("Decrypting", true);
            log("With variables:", true);
            logGMPVariable(D, false);
            logGMPVariable(N, false);
            logGMPVariable(C, true);
        }
        decipher();
        break;
    case CRACK:
        randomizer.seed(seed);
        if (LOGGING)
        {
            log("Cracking", true);
            log("With variables:", true);
            logGMPVariable(N, true);
        }
        crack();
        break;
    default:
        break;
    }

    printOutput();
}

/**
 * @brief Prints out results of RSA.
 */
void RSA::printOutput()
{
    switch (op)
    {
    case GENERATE:
        logGMPVariable(P, false);
        logGMPVariable(Q, false);
        logGMPVariable(N, false);
        logGMPVariable(E, false);
        logGMPVariable(D, false);
        break;
    case ENCRYPT:
        logGMPVariable(C, false);
        break;
    case DECRYPT:
        logGMPVariable(M, false);
        break;
    case CRACK:
        logGMPVariable(P, false);
        break;
    default:
        break;
    }
}