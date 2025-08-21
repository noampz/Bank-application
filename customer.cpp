#include "customer.hpp"
#include "bank.hpp"

Customer::Customer(uint32_t id, Bank &bank) : id(id), bank(bank){}

Customer::~Customer()
{
    int length = accounts.size();
    for (Account* account : accounts)
    {
        delete account;
    }
    accounts.clear();

    for (Loan* loan : loans)
    {
        delete loan;
    }
    loans.clear();
}

//returns null if there is not an account with this ID is not found
Account* Customer::getAccount(uint32_t id) const
{
    for (Account* account : accounts)
        if (account->getID() == id)
            return account;
    std::cout << "account with the ID: " << id << " was not found\n";
    return NULL;
}

//returns null if there is not a loan with this ID is not found
Loan* Customer::getLoan(uint32_t id) const
{
    for (Loan* loan : loans)
        if (loan->getID() == id)
            return loan;
    std::cout << "account with the ID: " << id << " was not found\n";
    return NULL;
}

uint32_t Customer::createCheckingAccount()
{
    uint32_t account_id = (id + (accounts.size() + 5 * 1000)) * 2;
    if (account_id % 2 == 0)
        account_id++;
    accounts.push_back(new CheckingAccount(account_id));
    return account_id;
}

uint32_t Customer::createSavingsAccount()
{
    uint32_t account_id = (id + (accounts.size() + 5 * 1000)) * 2 + 1;
    if (account_id % 2 == 1)
        account_id++;
    accounts.push_back(new SavingsAccount(bank.getBasicInterest(), account_id));
    return account_id;
}

//recives amount_of_money to put it in a loan and an account to put the money in it
//if returns 0 that means that there want an error
uint32_t Customer::createLoan(double amount_of_money, Account* account)
{
    if (amount_of_money < 0)
        return 0;

    int interest = bank.approveLoan(this, amount_of_money);
    if (interest == -1)
    {
        std::cout << "the Bank did not approve the loan.\n";
        return 0;
    }

    account->deposit(amount_of_money);
    uint32_t loan_id = id + loans.size();
    loans.push_back(new Loan(interest, amount_of_money, loan_id));
    
    return loan_id;
}

bool Customer::transferBetweenAccounts(uint32_t from_account_id, uint32_t to_account_id, double amount)
{
    Account *from_account = getAccount(from_account_id);
    Account *to_account = getAccount(to_account_id);
    if (from_account == NULL || to_account == NULL)
        return false;
        
    //if failed with withdraw it will not even do the deposit thingy
    //the deposit does not suppose to fail if the withdraw succeded so this line is correct
    //and the check for the deposit is not requierd
    return from_account->withdraw(amount) != -1 && to_account->deposit(amount) != -1;
}

void Customer::updateLoans(int days)
{
    for (Loan* loan : loans)
    {
        loan->addInterest(days);
    }
}

void Customer::updateSavingsAccounts(int days)
{
    for (Account* account : accounts)
    {
        if (account->getID() % 2 == 0)
            account->updateBalance(days);
    }
}

double Customer::totalBalance()
{
    double total_balance = 0;
    for (Account* account : accounts)
        total_balance += account->getBalance();
    for (Loan* loan : loans)
        total_balance -= loan->GetDebt();
    return total_balance;
}

double Customer::payLoan(uint32_t loan_id, uint32_t account_id, double amount)
{
    Account *account = getAccount(account_id);
    Loan *loan = getLoan(loan_id);

    if (account == NULL || loan == NULL)
    {
        std::cout << "could not find the account or the loan (or both)\n";
        return -2;
    }

    if (account->withdraw(amount) == -1)
    {
        std::cout << "There is an error with the withdraw from the account.\n";
        return -1;
    }

    double result = loan->payLoan(amount);

    switch ((int)result)
    {
    case -2:
        std::cout << "giving back to the account " << -loan->GetDebt() << "$";
        account->deposit(-loan->GetDebt());

    case 0:
        //if the result is 0.4 for example it will round to 0
        //so we have to check if this isnt the case
        if (result != 0 && result != 2)
        {
            std::cout << "bruh";
            return result;
        }

        deleteLoan(loan_id);
        return 0;

    default:
        return result;
    }
}

bool Customer::deleteLoan(uint32_t loan_id)
{
    bool found = false;
    int length = loans.size();
    std::vector<Loan*> loans_copy;
    loans_copy.reserve(length - 1);
    for (int i = length - 1; i >= 0; i--)
    {
        if (loans.back()->getID() == loan_id)
        {
            found = true;
            delete loans.back();
        }
        else
        {
            loans_copy.push_back(loans.back());
        }
        loans.pop_back();
    }
    loans = loans_copy;
    return found;
}

bool Customer::deleteAccount(uint32_t account_id)
{
    bool found = false;
    int length = accounts.size();
    std::vector<Account*> accounts_copy;
    accounts_copy.reserve(length - 1);
    for (int i = length - 1; i >= 0; i--)
    {
        if (accounts.back()->getID() == account_id)
        {
            found = true;
            delete accounts.back();
        }
        else
        {
            accounts_copy.push_back(accounts.back());
        }
        accounts.pop_back();
    }
    accounts = accounts_copy;
    return found;
}



void Customer::printAllSavingsAccounts()
{
    for (Account* account : accounts)
    {
        if (account->getID() % 2 == 0)
            std::cout << "\tID: " << account->getID() << " | balance: " << account->getBalance() << "$ " << "interest rate: " << account->getInterestRate() << "\n";
    }
}

void Customer::printAllCheckingAccounts()
{
    for (Account* account : accounts)
    {
        if (account->getID() % 2 == 1)
            std::cout << "\tID: " << account->getID() << " | balance: " << account->getBalance() << "$\n";
    }
}


void Customer::printAllLoans()
{
    for (Loan* loan : loans)
    {
        std::cout << "\tID: " << loan->getID() << " | debt: " << -loan->GetDebt() << "$\n";
    }
}

void Customer::printAllAccountsInCustomer()
{
    std::cout << "Your Checking Accounts: \n";
    printAllCheckingAccounts();
    std::cout << "Your Savings Accounts: \n";
    printAllSavingsAccounts();
    std::cout << "Your Loans: \n";
    printAllLoans();
}