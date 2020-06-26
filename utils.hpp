#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <getopt.h>

/* store fstream info */
class Info
{
public:
    std::string infile, outfile;
    std::ifstream fin;
    std::ofstream fout;
    Info(int argc, char *argv[]);
    std::string generate_outfilename(std::string);
};

/* print usage */
void print_help(const char *);

#endif