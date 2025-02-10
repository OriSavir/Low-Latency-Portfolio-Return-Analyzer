#include "../include/Portfolio.hpp"
#include <math.h>
#include <iostream>
#include <xlnt/xlnt.hpp>



Portfolio::Portfolio(const std::string &filename) {
    records = read_records(filename);
    if (record_count == 0) {
        std::cerr << "Error reading the file (the records vector was read empty)" << std::endl;
    }
    record_count = records.size();
    twr = computeTWR();
    ctwr = computeContinuousTWR();
    cwr = computeCWR();
    irr = computeIRR(1e-5, 100, 0.1);
}

void Portfolio::computeReturnRates() {
    for (int i = 1; i < int(record_count); i++) {
        records[i].return_rate = (records[i].value - (records[i-1].value + records[i-1].capital_flow)) / (records[i-1].value + records[i-1].capital_flow);
    }
}

double Portfolio::computeTWR() {
    if (record_count <= 1) {
        return 0;
    }
    Record initial_value = records[0];
    double total_time = records[record_count-1].time - initial_value.time;
    double product_of_returns = 1.0;
    for (int i = 1; i < int(record_count); i++) {
        product_of_returns *= 1 + records[i].return_rate;
    }
    twr = pow((product_of_returns),((1/total_time))) - 1;
    return twr;
}

double Portfolio::computeContinuousTWR() {
    if (record_count <= 1) {
        return 0;
    }
    if (twr == 0) {
        computeTWR();
    }
    ctwr = log(1 + twr);
    return ctwr;
}

double Portfolio::computeCWR() {
    if (record_count <= 1) {
        return 0;
    }
    Record initial_value = records[0];
    Record final_value = records[record_count-1];
    double total_time = final_value.time - initial_value.time;
    double sum_cap_flows = 0;
    double sum_time_scaled_cap_flows = 0;
    for (int i = 1; i < int(record_count); i++) {
        sum_cap_flows += records[i].capital_flow;
        sum_time_scaled_cap_flows += records[i].capital_flow * ((final_value.time - records[i].time) / final_value.time);
    }
    double r_cwr_t = (final_value.value - initial_value.value - sum_cap_flows) / (initial_value.value + sum_time_scaled_cap_flows);
    cwr = pow(1 + r_cwr_t, 1 / total_time) - 1;
    return cwr;
}


//Implementation of the Newton-Raphson method to compute the IRR (similar to gradient descent techniques)
double Portfolio::computeIRR(double tolerance, int max_iterations, double guess) {
    double R_I = guess;

    for (int iter = 0; iter < max_iterations; iter++) {
        double F_RI = records[0].value;
        double F_prime_RI = 0;

        for (size_t i = 1; i < record_count - 1; i++) {
            Record rec = records[i];
            if(rec.capital_flow != 0) {
                double time_diff = rec.time - records[0].time;
                double discount_factor = pow(1 + R_I, time_diff);
                F_RI += rec.capital_flow / discount_factor;
                F_prime_RI -= time_diff * rec.capital_flow / (discount_factor * (1 + R_I));
            }
        }
        
        double time_diff = records[record_count - 1].time - records[0].time;
        double last_discount_factor = pow(1 + R_I, time_diff);
        F_RI -= records[record_count - 1].value / last_discount_factor;
        F_prime_RI += (time_diff) * records[record_count - 1].value / (last_discount_factor * (1 + R_I));

        double new_RI = R_I - F_RI / F_prime_RI;

        if (fabs(new_RI - R_I) < tolerance) {
            return new_RI;
        }

        R_I = new_RI;
    }
    std::cerr << "The IRR could not be computed within max_iterations of " << max_iterations << std::endl;
    return -1;
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






