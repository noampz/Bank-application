#pragma once
#include "vector"
#include "loan.hpp"
#include "account.hpp"
#include "checkingAccount.hpp"
#include "savingsAccount.hpp"
#include <string.h>
#include <functional>

class Bank;

class Customer
{
    private:

    //explicitly a savings account
    std::vector<SavingsAccount*> savings_accounts;

    //explicitly a checking account
    CheckingAccount* main_account;

    std::vector<Loan*> loans;
    uint32_t id;
    Bank &bank;
    std::string full_name;
    size_t hashed_password;

    //it will only be called by the constructor
    uint32_t createMainAccount();

    public:

    Customer(uint32_t id, std::string full_name, std::string password, Bank &bank);
    ~Customer();
    //gets
    uint32_t getID() const {return id;}
    size_t getHashedPassword() const {return hashed_password;}
    std::string getFullName() const {return full_name;}
    CheckingAccount* getMainAccount() const {return main_account;}

    //returns nullptr if there is not a savings account with this ID
    SavingsAccount* getSavingsAccount(uint32_t id) const;
    //returns nullptr if there is not a loan with this ID
    Loan* getLoan(uint32_t id) const;

    bool deleteAccount(uint32_t account_id);
    bool deleteLoan(uint32_t loan_id);

    
    uint32_t createSavingsAccount();
    //recives amount_of_money to put it in a loan and an account to put the money in it
    //if returns 0 that means that there want an error
    uint32_t createLoan(double amount_of_money, Account* account);

    // insert 0 in ID if wants to use main account
    bool transferBetweenAccounts(uint32_t from_account_id, uint32_t to_account_id, double amount);

    // -2 -> if could not find loan or account
    // -1 -> if there is an error with the withdraw
    // 0 -> if now you paid all the loan (and it deletes the loan)
    // any other number is the amount of money that is needed to pay off all the loan
    //if ID = 0 then it will use the main account
    double payLoan(uint32_t loan_id, uint32_t account_id, double amount);

    void updateLoans(int days);
    void updateSavingsAccounts(int days);
    
    double totalBalance();

    void printAllSavingsAccounts();
    void printMainAccount();
    void printAllLoans();

    void printAllAccountsInCustomer();
};
