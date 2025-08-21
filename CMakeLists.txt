cmake_minimum_required(VERSION 3.20)
project(BankApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# List all source files
set(SOURCES
    app.cpp
    bank.cpp
    customer.cpp
    account.cpp
    checkingAccount.cpp
    savingsAccount.cpp
    loan.cpp
    time.cpp
)

add_executable(bank_app ${SOURCES})