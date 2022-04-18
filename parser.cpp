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
    char *next;
    static const char *sOptions = "g:e:d:b:";

    if (argc <= 1)
    {
        rc = EXIT_FAILURE;
    }
    else
    {
        logger.log("Parsing arguments");

        while ((opt = getopt(argc, argv, sOptions)) != EOF)
        {
            switch (opt)
            {
            case 0:
                break;
            // Generates RSA public and private keys
            case 'g': 
                rc = !isNumber(optarg);
                this->B = stoi(optarg);
                this->op = GENERATE;
                break;
            case 'e': // Encrypts
                index = optind - 1;
                while (index < argc)
                {
                    next = strdup(argv[index]);
                    index++;
                    if (next[0] != '-')
                    {
                        cout << next;
                    }
                    else
                        break;
                }
                this->op=ENCRYPT;
                break;
            case 'd':
                cout << optarg;
                this->op=DECRYPT;
                break;
            case 'b':
                rc = !isHex(optarg);
                this->op=CRACK;
                break;
            default:
                help();
                rc = EXIT_FAILURE;
            }
        }
    }

    if (rc == 1)
    {
        help();
    }

    return rc;
}