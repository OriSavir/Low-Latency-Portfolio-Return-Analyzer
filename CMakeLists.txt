cmake_minimum_required(VERSION 3.20)
project(PortfolioCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(dependencies/xlnt)

add_executable(portfolio src/main.cpp src/Portfolio.cpp)

target_link_libraries(portfolio PRIVATE xlnt)