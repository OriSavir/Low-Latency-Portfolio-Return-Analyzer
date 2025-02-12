# 📈 Portfolio Performance Calculator

A high-performance **portfolio performance calculator** that computes key financial metrics, including **Time Weighted Returns**, **Capital Weighted Return**, and the **Internal Rate of Return (IRR)** using the **Newton-Raphson method**. The project is implemented in **C++** and leverages **CMake** for build configuration.

---

## 🚀 Features
- Optimized calculation of portfolio TWR, CWR, IRR 
- Efficient **Internal Rate of Return (IRR) computation** based on the **Newton-Raphson method** for root-finding
- Supports **low-latency performance calculations**
- Uses **C++17** and **CMake**
- Utilizes **[xlnt](https://github.com/tfussell/xlnt)** for Excel file handling

---

## 🛠️ Setup Instructions

### **1. Clone the Repository**
```sh
git clone --recursive https://github.com/YOUR_GITHUB_USERNAME/portfolio_analysis.git
cd portfolio_analysis
```

### **2. Build the tool**
```sh
make
cd portfolio_analysis_build
```
### **3. Set up the Excel file in the following format in the first three columns**

| time  | value  | capital_flow |
|-------|--------|-------------|
| (float) | (currency) | (currency, optional) |

- **time**: A floating-point number representing the time at which the value is recorded.
- **value**: The portfolio value at the given time.
- **capital_flow**: The capital inflow (+) or outflow (-), which may be empty for some rows.

### **4. Call the tool with the excel filename**
```sh
portfolio <filename.xlsx>
```

### **5. Examine the output!**

