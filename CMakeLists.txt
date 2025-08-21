cmake_minimum_required(VERSION 3.20)
project(BankApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(HEADERS
    src/bank.hpp
    src/customer.hpp
    src/account.hpp
    src/checkingAccount.hpp
    src/savingsAccount.hpp
    src/loan.hpp
    src/time.hpp
)

set(SOURCES
    src/app.cpp
    src/bank.cpp
    src/customer.cpp
    src/account.cpp
    src/checkingAccount.cpp
    src/savingsAccount.cpp
    src/loan.cpp
    src/time.cpp
)

add_executable(bank_app ${SOURCES} ${HEADERS})