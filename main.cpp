#include <iostream>

#include "parser.hpp"

int main(int argc, char *argv[])
{
    clock_t start, end;
    try
    {
        info = new Info(argc, argv);
        start = clock();
        init();
        while (info->fin)
            parse();
        info->fin.close();
        end = clock();
        std::cout << "Analysis Completed." << std::endl;
        std::cout << "./" << info->infile << " is a valid PL/0 program." << std::endl;
        result_print();
        std::cout << std::setprecision(3)
                  << "Time Costed: " << (double)(end - start) / CLOCKS_PER_SEC << "s\n";
    }
    catch (const char *msg)
    {
        std::cerr << msg << std::endl;
        print_help(argv[0]);
    }
    return 0;
}