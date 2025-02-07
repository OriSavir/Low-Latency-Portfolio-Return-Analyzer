#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <vector>
#include <string>
#include "Record.hpp"

class Portfolio {
    private:
        std::vector<Record> records;
        double twr;
        double ctwr;
        double cwr;
        double irr;
    public:
        Portfolio(const std::string &filename);

        std::vector<Record> read_records(const std::string &filename);

        void computeReturnRates();

        double computeTWR();
        double computeContinuousTWR();
        double computeCWR();
        double computeIRR(double guess=0.1);
        
        void print_results();
        void write_results(const std::string &filename);
};

#endif // PORTFOLIO_HPP