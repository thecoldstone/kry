/**
 * @file parser.cpp
 * @brief Parser implementation
 * @author Nikita Zhukov
 * @date 29.04.2022
 */

#include <iostream>
#include <string>
#include <getopt.h>

#include "rsa.hpp"
#include "parser.hpp"

using namespace std;

/**
 * @brief Help function
 */
void help()
{
    cout << "Example: ./kry [-g B, -e E N M, -d D N C, -b N]" << endl;

    cout << "\t-g - Generates the keys" << endl;
    cout << "\t\tB (int) - request key length of the public modulus in bites" << endl;

    cout << "\t-e - Encryptes the message" << endl;
    cout << "\t\tE (hex) - public exponent (mostly equals 3)" << endl;
    cout << "\t\tN (hex) - public modulus" << endl;
    cout << "\t\tM (hex) - message to encrypt (ACCEPTS ONLY INTEGER VALUES)" << endl;

    cout << "\t-d - Decrypts the message" << endl;
    cout << "\t\tD (hex) - private exponent" << endl;
    cout << "\t\tN (hex) - public modulus" << endl;
    cout << "\t\tC (hex) - message to decrypt (ACCEPTS ONLY INTEGER VALUES)" << endl;

    cout << "\t-b - Generates the keys" << endl;
    cout << "\t\tN (hex) - public modulus" << endl;

    cout << "\t-l - Activates logging" << endl;
    cout << "\t\t " << endl;
}

/**
 * @brief Checks if string represents a hex value
 *
 * @param str
 * @return true - it's a hex
 * @return false - it's not a hex
 */
bool isHex(string str)
{

    if (str.rfind("0x", 0) != 0)
    {
        return false;
    }

    for (char &c : str.erase(0, 2))
    {
        if (!isdigit(c) && (toupper(c) < 'A' || toupper(c) > 'F'))
            return false;
    }

    return true;
}

/**
 * @brief Checks if string represents a number
 *
 * @param str
 * @return true - it's a number
 * @return false - it's not a number
 */
bool isNumber(string str)
{
    for (char &c : str)
    {
        if (!isdigit(c))
            return false;
    }

    return true;
}

/**
 * @brief Parses passed arguments
 *
 * @param argc
 * @param argv
 * @return rc - int success factor 
 */
int RSA::parse(int argc, char **argv)
{
    int rc = EXIT_SUCCESS;
    string str;
    int opt;
    static const char *sOptions = "g:e:d:b:l";

    if (argc <= 1)
    {
        logError("Arguments are missing.");
        rc = EXIT_FAILURE;
    }
    else
    {
        while ((opt = getopt(argc, argv, sOptions)) != EOF)
        {
            switch (opt)
            {
            case 0:
                break;
            case 'g': // Generates RSA public and private keys
                if ((rc = !isNumber(optarg)))
                {
                    logError("Integer value was expected.");
                    break;
                };
                B = stoi(optarg);
                op = GENERATE;
                break;
            case 'e': // Encrypts
            case 'd': // Decrypts
                if (argc < 5)
                {
                    rc = EXIT_FAILURE;
                    break;
                }
                rc = !isHex(argv[2]);
                rc = !isHex(argv[3]);
                rc = !isHex(argv[4]);

                if (rc)
                {
                    logError("Values are not hex format.");
                    break;
                }

                switch (opt)
                {
                case 'e':
                    E = mpz_class(argv[2]);
                    N = mpz_class(argv[3]);
                    M = mpz_class(argv[4]);
                    op = ENCRYPT;
                    break;
                case 'd':
                    D = mpz_class(argv[2]);
                    N = mpz_class(argv[3]);
                    C = mpz_class(argv[4]);
                    op = DECRYPT;
                    break;
                default:
                    break;
                }

                if (argc <= 5)
                    break;

                if (strcmp(argv[5], "-l") == 0)
                {
                    LOGGING = true;
                }

                break;
            case 'b': // Cracks
                rc = !isHex(optarg);

                if (rc)
                {
                    logError("Public modulus is not hex format.");
                    break;
                }

                N = mpz_class(optarg);

                op = CRACK;
                break;
            case 'l': // Makes logging active
                LOGGING = true;
                break;
            default:
                rc = EXIT_FAILURE;
            }
        }
    }

    if (rc == EXIT_FAILURE)
    {
        help();
    }

    return rc;
}