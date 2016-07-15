/* This is the source code of Brain Programming Language.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Luiz Peres, 2016.
 */

#include <iostream>
#include <fstream>
#include <streambuf>

#include "ArgsHandler.h"

const std::string BRAIN_VERSION =  "0.8";
const std::string BRAIN_FORMAT = "Please execute Brain with the command: brain <options...> <filename>\n";
const std::string BRAIN_HELP = "Use the identifier '--help' to get information about the settings\n";
const std::string BRAIN_OPT_ERR = "You can not use more than one type of optimization at time.\n";

void ArgsHandler::handle(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "Brain version "
                  << BRAIN_VERSION << "." << std::endl
                  << BRAIN_FORMAT
                  << BRAIN_HELP;
        exit(-1);
    }

    for (int i = 1; i < argc; i++) {
        std::string str(argv[i]);

        if (str.compare("--help") == 0 || str.compare("-help") == 0) {
            std::cout << "\n"
                      << "-version\tShows the current version of Brain\n"
                      << "-emit-llvm\tEmits LLVM IR code for the given input\n"
                      << "-emit-ast\tEmits the AST for the given input\n"
                      << "-v\t\tUses verbose mode for the output\n"
                      << "-O0\t\tGenerates output code with no optmizations\n"
                      << "-O1\t\tOptimizes Brain generated output code (Default)\n"
                      << "\n";
            exit(0);
        }
        else if (str.compare("--version") == 0 ||
                 str.compare("-version") == 0) {
            std::cout << "Brain version " << BRAIN_VERSION << ".\n"
                      << BRAIN_HELP;
            exit(0);
        }
        else if (str.compare("-emit-llvm") == 0) {
            ArgsOptions::instance()->add_option(BO_IS_EMITTING_LLVM);
        }
        else if (str.compare("-emit-ast") == 0) {
            ArgsOptions::instance()->add_option(BO_IS_EMITTING_AST);
        }
        else if (str.compare("-v") == 0) {
            ArgsOptions::instance()->add_option(BO_IS_VERBOSE);
        }
        else if (str.compare("-O0") == 0) {
            if (ArgsOptions::instance()->has_option(BO_IS_OPTIMIZING_O1)) {
                std::cout << BRAIN_OPT_ERR;
                exit(-1);
            }

            ArgsOptions::instance()->add_option(BO_IS_OPTIMIZING_O0);
        }
        else if (str.compare("-O1") == 0)
        {
            if (ArgsOptions::instance()->has_option(BO_IS_OPTIMIZING_O0)) {
                std::cout << BRAIN_OPT_ERR;
                exit(-1);
            }

            ArgsOptions::instance()->add_option(BO_IS_OPTIMIZING_O1);
        }
        else if ((str.size() > 2 && str.substr(str.size()-2, 2) == ".b") ||
                 (str.size() > 3 && str.substr(str.size()-3, 3) == ".br") ||
                 (str.size() > 6 && str.substr(str.size()-6, 6) == ".brain")) {
            std::ifstream t(str);
            std::string str_file((std::istreambuf_iterator<char>(t)),
                                 std::istreambuf_iterator<char>());
            if (str_file.empty()) {
                std::cout << "No such file '" << str << "'\n"
                          << BRAIN_FORMAT;
                exit(-1);
            }
            _string_file = str_file;
        }
        else if (str.find("-") == 0) {
            std::cout << "Unsupported option \"" << str << "\"\n"
                      << BRAIN_HELP;
            exit(-1);
        }
        else {
            std::cout << "No such file '" << str << "'\n"
                      << BRAIN_FORMAT
                      << BRAIN_HELP;
            exit(-1);
        }
    }

    if (_string_file.empty()) {
        std::cout << "No input files\n"
                  << BRAIN_FORMAT;
        exit(-1);
    }
}

std::string ArgsHandler::get_string_file()
{
  return _string_file;
}
