#include "../include/Portfolio.hpp"

int main(int argc, char *argv[]) {
    Portfolio portfolio("data.csv");
    portfolio.print_results();
    portfolio.write_results("portfolio_results.csv");
    return 0;
}