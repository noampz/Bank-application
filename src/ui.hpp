#pragma once
#include "bank.hpp"
#include <iostream>
#include <string>
#include <Windows.h>

class UI
{
    private:

    Bank *bank;
    Customer **customer;

    public:

    UI(Bank *bank, Customer **customer);
    void clearScreen();
    inline void clearNewLineInBuffer(); 
    bool checkIfInputFailed();
    //this will be the menu with 3 options:
    // sign up, sign in, quit
    void printFirstMenu();
    bool firstMenu();
    
    bool signUp();
    bool signIn();

    void printMainMenu();
    int mainMenu();

    void createSavingsAccount();
    void createLoan();
    void payLoan();
    void transferBetweenAccounts();
    void transferBetweenCustomers();
    void deleteCustomer();
    void deleteSavingsAccount();
    void changeTime();
};

