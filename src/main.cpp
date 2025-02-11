#include "../include/Portfolio.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return -1;
    }
    std::string filename(argv[1]);
    Portfolio portfolio(filename);
    portfolio.print_results();
    portfolio.write_results("portfolio_results.xlsx");
    return 0;
}