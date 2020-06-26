#include "utils.hpp"

Info::Info(int argc, char *argv[])
{
    bool outfile_specified = false;
    for (int opt; (opt = getopt(argc, argv, "o:")) != -1;)
    {
        switch (opt)
        {
        case 'o':
            outfile_specified = true;
            outfile = optarg;
            break;
        default:
            throw "Invalid options\n";
        }
    }

    if (argc < optind + 1)
        throw "No input file specified\n";

    infile = argv[optind];
    outfile = outfile_specified ? outfile : generate_outfilename(infile);

    fin.open(infile);
    fout.open(outfile);

    if (!fin.good())
        throw "Could not open input file\n";

    if (!fout.good())
        throw "Could not open output file\n";
}

std::string Info::generate_outfilename(std::string infile)
{
    int pos;
    if ((pos = infile.rfind('.')) != std::string::npos)
        infile = infile.substr(0, pos);
    infile.append(".out");
    return infile;
}

void print_help(const char *filename)
{
    std::cout << "Usage: " << filename << " [options] infile\n"
              << "Options:\n"
              << "\t-o outfile\tSpecify output file\n";
}

Info *info;