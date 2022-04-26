#include <iostream>
#include <string>
#include <getopt.h>

#include "rsa.hpp"
#include "parser.hpp"

using namespace std;

/**
 * @brief Help function
 * 
 * @todo Fix printed help text
 */
void help()
{
    cout << "Example: ./kry [-g B, -e E N M, -d D N C, -b N]" << endl;
    cout << "\tB - request key length (public modulus) in bites" << endl;
    cout << "\t\t -b N (int)" << endl;
    cout << "\tE - encrypts message" << endl;
    cout << "\t\t -e E (hex) N (hex) M (hex)" << endl;
}

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
 * @return rc - int 
 */
int RSA::parse(int argc, char **argv)
{
    int rc = EXIT_SUCCESS;
    string str;
    int opt, index;
    static const char *sOptions = "g:e:d:b:l";

    if (argc <= 1)
    {
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
                if((rc = !isNumber(optarg))) {
                    break;
                };
                B = stoi(optarg);
                op = GENERATE;
                break;
            case 'e': // Encrypts
            case 'd': // Decrypts
                if (argc < 5) {
                    rc = EXIT_FAILURE;
                    break;
                }
                rc = !isHex(argv[2]);
                rc = !isHex(argv[3]);
                rc = !isHex(argv[4]);

                if(rc) {
                    break;
                }

                switch (opt)
                {
                case 'e':
                    E = mpz_class(argv[2]);
                    N = mpz_class(argv[3]);
                    M = mpz_class(argv[4]);
                    op=ENCRYPT;
                    break;
                case 'd':
                    D = mpz_class(argv[2]);
                    N = mpz_class(argv[3]);
                    C = mpz_class(argv[4]);
                    op=DECRYPT;
                    break;
                default:
                    break;
                }

                if (argc <= 5) break;

                if(strcmp(argv[5], "-l") == 0) {
                    LOGGING = true;
                }

                break;
            case 'b': // Cracks
                rc = !isHex(optarg);
                op=CRACK;
                break;
            case 'l': // Makes logging active
                LOGGING = true;
                break;
            default:
                help();
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