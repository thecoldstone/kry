#include <iostream>
#include <cstddef>
#include <stddef.h>
#include "rsa.hpp"

using namespace std;

unsigned long int seed = time(NULL);
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
 * @brief Modulo operation
 *
 * @param base
 * @param exp
 * @param mod
 * @return mpz_class
 */
mpz_class RSA::mod(mpz_class base, mpz_class exp, mpz_class mod)
{
    mpz_class x = 1;
    mpz_class y = base;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            x = (x * y) % mod;

        y = (y * y) % mod;
        exp = exp / 2;
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

        if (jacobian == 0 || jacobian != mod(a, (n - 1) / 2, n))
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
 * @brief
 *
 */
void RSA::encrypt()
{
    C = mod(M, E, N);
}

/**
 * @brief
 *
 */
void RSA::decipher()
{
    M = mod(C, D, N);
}

/**
 * @brief
 *
 */
void RSA::crack()
{
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
            logVariable("E", E.get_str());
            logVariable("N", N.get_str());
            logVariable("M", M.get_str());
        }
        encrypt();
        break;
    case DECRYPT:
        if (LOGGING)
        {
            log("Decrypting", true);
            log("With variables:", true);
            logVariable("D", D.get_str());
            logVariable("N", N.get_str());
            logVariable("C", C.get_str());
        }
        decipher();
        break;
    case CRACK:
        if (LOGGING)
            log("Cracking", true);
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
        logVariable("P", P.get_str());
        logVariable("Q", Q.get_str());
        logVariable("N", N.get_str());
        logVariable("E", E.get_str());
        logVariable("D", D.get_str());
        break;
    case ENCRYPT:
        logVariable("C", C.get_str());
        break;
    case DECRYPT:
        logVariable("D", D.get_str());
        break;
    default:
        break;
    }
}