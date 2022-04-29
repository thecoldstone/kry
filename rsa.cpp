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
 * @brief Function to calculate (base^exponent) % mod
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
 * @brief Counts Greate Common Divisor (GCD)
 *
 * @param a
 * @param b
 * @return mpz_class
 */
mpz_class RSA::gcd(mpz_class a, mpz_class b)
{
    return b == 0 ? a : gcd(b, a % b);
}

/**
 * @brief
 *
 * @param a
 * @param k
 * @return mpz_class
 */
mpz_class RSA::getPow(mpz_class a, mpz_class number)
{
    mpz_class result;
    mpz_class exp = (number - 1) / 2;

    mpz_powm(result.get_mpz_t(), a.get_mpz_t(), exp.get_mpz_t(), number.get_mpz_t());

    return result;
}

/**
 * @brief
 *
 * @param n
 * @param k
 * @return bool
 */
bool RSA::solovoyStrassen(const mpz_class n, int k)
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
 * @brief
 *
 * @param number
 */
void RSA::checkPrime(mpz_class &number)
{
    while (!solovoyStrassen(number, SSACCURACY))
    {
        number++ ++;
    }
};

/**
 * @brief
 *
 * @return mpz_class
 */
mpz_class RSA::generatePrime()
{
    return randomizer.get_z_bits(((B / 2)));
}

/**
 * @brief
 *
 * @return int
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
 *
 */
void RSA::encrypt()
{
    C = modPow(M, E, N);
}

/**
 * @brief Decrypts message
 *
 */
void RSA::decipher()
{
    M = modPow(C, D, N);
}

mpz_class RSA::bruteForce()
{
    int isBruteForced = false;
    mpz_class p;

    // mpz_t result;
    // mpz_init(result);
    // mpz_t divisor;
    // mpz_set_ui(divisor, 2);

    // for (int i = 0; !isBruteForced && i < ITERATIONS; i++)
    // {
    //     mpz_mod(result, n, divisor);

    //     if (mpz_cmp_ui(result, 0) == 0)
    //     {
    //         isBruteForced = true;
    //     }

    //     mpz_add_ui(divisor, divisor, 2);
    // }

    // return isBruteForced;
    return p;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool RSA::isPollardRho()
{
    if (N == 1)
    {
        P = N;
    }

    if (N % 2 == 0)
    {
        P = 2;
    }

    mpz_class x;
    mpz_class y;
    mpz_class c;
    mpz_class d;

    while (true)
    {
        randomizer.seed(seed);
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
        }
    }
}

/**
 * @brief
 *
 */
void RSA::crack()
{
    if (LOGGING)
    {
        log("Bruteforce(ing)...", true);
    }

    // P = bruteForce();
    // P =

    if (LOGGING)
    {
        log("Bruteforce(ing) failed...", true);
        log("Pollard's Rho(ing)...", true);
    }
    if (isPollardRho())
    {
    }
}

/**
 * @brief
 *
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