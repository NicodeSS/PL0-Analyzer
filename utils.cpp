#include "utils.hpp"

Info::Info(int argc, char *argv[])
{
    bool out_to_file = false, outfile_specified = false;
    outfile = "";
    for (int opt; (opt = getopt(argc, argv, "aho:")) != -1;)
    {
        switch (opt)
        {
        case 'a':
            out_to_file = true;
            outfile_specified = false;
            break;
        case 'h':
            print_help(argv[0]);
            exit(1);
        case 'o':
            out_to_file = true;
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
    fin.open(infile);
    if (!fin.good())
        throw "Could not open input file\n";

    if (out_to_file)
    {
        outfile = outfile_specified ? outfile : generate_outfilename(infile);
        fout.open(outfile);
        if (!fout.good())
            throw "Could not open output file\n";
    }
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
              << "\t-h        \tPrint this help\n"
              << "\t-o outfile\tSpecify output file\n";
}

Info *info;