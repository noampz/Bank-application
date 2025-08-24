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
    // savings accounts ID will be %2 == 0
    // while checking accounts ID will be %2 == 1
    // IMPORTANT: Main account will also be here (currently it will be the first checking account)
    std::vector<Account*> accounts;

    //explicitly a checking account
    CheckingAccount* main_account;

    std::vector<Loan*> loans;
    uint32_t id;
    Bank &bank;
    std::string full_name;
    size_t hashed_password;

    public:

    Customer(uint32_t id, std::string full_name, std::string password, Bank &bank);
    ~Customer();
    //gets
    uint32_t getID() const {return id;}
    size_t getHashedPassword() const {return hashed_password;}
    std::string getFullName() const {return full_name;}
    CheckingAccount* getMainAccount() const {return main_account;}

    // get by id
    Account* getAccount(uint32_t id) const;
    Loan* getLoan(uint32_t id) const;

    bool deleteAccount(uint32_t account_id);
    bool deleteLoan(uint32_t loan_id);

    uint32_t createCheckingAccount();
    uint32_t createSavingsAccount();
    uint32_t createLoan(double amount_of_money, Account* account);
    bool transferBetweenAccounts(uint32_t from_account_id, uint32_t to_account_id, double amount);

    // -2 -> if could not find loan or account
    // -1 -> if there is an error with the withdraw
    // 0 -> if now you paid all the loan (and it deletes the loan)
    // any other number is the amount of money that is needed to pay off all the loan
    double payLoan(uint32_t loan_id, uint32_t account_id, double amount);

    void updateLoans(int days);
    void updateSavingsAccounts(int days);
    
    double totalBalance();

    void printAllSavingsAccounts();
    void printAllCheckingAccounts();
    void printAllLoans();

    void printAllAccountsInCustomer();
};
