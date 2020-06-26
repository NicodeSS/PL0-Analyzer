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
        while (!info->fin.eof())
            parse();
        info->fin.close();
        result_print();
        end = clock();
        std::cout << std::setprecision(3) << std::endl
                  << "Analysis Completed." << std::endl
                  << "Time Costed: " << (double)(end - start) / CLOCKS_PER_SEC << "s\n";
    }
    catch (const char *msg)
    {
        std::cerr << msg << std::endl;
        print_help(argv[0]);
    }
    return 0;
}