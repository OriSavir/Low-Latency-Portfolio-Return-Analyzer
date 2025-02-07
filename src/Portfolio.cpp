#include "../include/Portfolio.hpp"
#include <math.h>
#include <iostream>
#include <xlnt/xlnt.hpp>



Portfolio::Portfolio(const std::string &filename) {
    records = read_records(filename);

    twr = computeTWR();
    ctwr = computeContinuousTWR();
    cwr = computeCWR();
    irr = computeIRR();
}

void Portfolio::computeReturnRates() {
    for (int i = 1; i < int(records.size()); i++) {
        records[i].return_rate = (records[i].value - (records[i-1].value + records[i-1].capital_flow)) / (records[i-1].value + records[i-1].capital_flow);
    }
}

double Portfolio::computeTWR() {
    if (records.size() <= 1) {
        return 0;
    }
    Record initial_value = records[0];
    double total_time = records[records.size()-1].time - initial_value.time;
    double product_of_returns = 1.0;
    for (int i = 1; i < int(records.size()); i++) {
        product_of_returns *= 1 + records[i].return_rate;
    }
    twr = pow((product_of_returns),((1/total_time))) - 1;
    return twr;
}

double Portfolio::computeContinuousTWR() {
    if (records.size() <= 1) {
        return 0;
    }
    if (twr == 0) {
        computeTWR();
    }
    ctwr = log(1 + twr);
    return ctwr;
}

double Portfolio::computeCWR() {
    if (records.size() <= 1) {
        return 0;
    }
    Record initial_value = records[0];
    double total_time = records[records.size()-1].time - initial_value.time;
    double sum_cap_flows = 0;
    for (int i = 1; i < int(records.size()); i++) {
        sum_cap_flows += records[i].capital_flow;
    }

}

double Portfolio::computeIRR(double guess) {
    if (records.size() <= 1) {
        return 0;
    }
    double epsilon = 0.0001;
    double difference = 1;
    while (difference > epsilon) {
        double sum = 0;
        for (int i = 1; i < int(records.size()); i++) {
            sum += records[i].capital_flow / pow(1 + guess, records[i].time - records[0].time);
        }
        difference = sum;
        guess += epsilon;
    }
    irr = guess;
    return irr;
}

void Portfolio::print_results() {
    std::cout << "Time-Weighted Rate of Return: " << twr << std::endl;
    std::cout << "Continuous Time-Weighted Rate of Return: " << ctwr << std::endl;
    std::cout << "Capital-Weighted Rate of Return: " << cwr << std::endl;
    std::cout << "Internal Rate of Return: " << irr << std::endl;
}

void Portfolio::write_results(const std::string &filename) {
    xlnt::workbook wb;
    xlnt::worksheet ws = wb.active_sheet();
    ws.cell("A1").value("Time-Weighted Rate of Return");
    ws.cell("B1").value(twr);
    ws.cell("A2").value("Continuous Time-Weighted Rate of Return");
    ws.cell("B2").value(ctwr);
    ws.cell("A3").value("Capital-Weighted Rate of Return");
    ws.cell("B3").value(cwr);
    ws.cell("A4").value("Internal Rate of Return");
    ws.cell("B4").value(irr);
    wb.save(filename);
}

std::vector<Record> Portfolio::read_records(const std::string &filename) {
    xlnt::workbook wb;
    
    try {
        wb.load(filename);
        xlnt::worksheet ws = wb.active_sheet();
        bool firstRow = true;
        int id = 0;
        for (auto row : ws.rows()) {
                if (firstRow) {
                    firstRow = false;
                    continue;
                }
                Record record;
                try {
                    record.id = id;
                    record.time = row[0].value<double>();
                    record.value = row[1].value<double>();
                    record.capital_flow = row[2].value<double>();
                    records.push_back(record);
                    id++;
                } catch (const xlnt::exception &e) {
                    std::cerr << "Error reading the file: " << e.what() << std::endl;
                }
            }
        }
    catch (const xlnt::exception &e) {
        std::cerr << "Error loading the file: " << e.what() << std::endl;
    }
    return records;
}






