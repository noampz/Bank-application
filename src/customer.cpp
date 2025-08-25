#include "customer.hpp"
#include "bank.hpp"

Customer::Customer(uint32_t id, std::string full_name, std::string password, Bank &bank) : id(id), bank(bank), full_name(full_name)
{
    std::hash<std::string> hasher;
    hashed_password = hasher(password);
    createMainAccount();
}

Customer::~Customer()
{
    int length = savings_accounts.size();
    for (SavingsAccount* savings_account : savings_accounts)
    {
        delete savings_account;
    }
    savings_accounts.clear();

    for (Loan* loan : loans)
    {
        delete loan;
    }
    loans.clear();

    delete main_account;
}


SavingsAccount* Customer::getSavingsAccount(uint32_t id) const
{
    for (SavingsAccount* savings_account : savings_accounts)
        if (savings_account->getID() == id)
            return savings_account;
    std::cout << "account with the ID: " << id << " was not found\n";
    return nullptr;
}


Loan* Customer::getLoan(uint32_t id) const
{
    for (Loan* loan : loans)
        if (loan->getID() == id)
            return loan;
    std::cout << "account with the ID: " << id << " was not found\n";
    return nullptr;
}

uint32_t Customer::createMainAccount()
{
    uint32_t account_id = 0;
    main_account = new CheckingAccount(account_id);
    return account_id;
}

uint32_t Customer::createSavingsAccount()
{
    uint32_t account_id = id / (1 + (rand() % (id / 1000)));
    savings_accounts.push_back(new SavingsAccount(bank.getBasicInterest(), account_id));
    return account_id;
}


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
    Account *from_account = nullptr;
    if (from_account_id == 0)
        from_account = main_account;
    else
        from_account = getSavingsAccount(from_account_id);

    Account *to_account = nullptr;
    if (to_account_id == 0)
        to_account = main_account;
    else
        to_account = getSavingsAccount(to_account_id);

    if (from_account == nullptr || to_account == nullptr)
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
    for (SavingsAccount* savings_account : savings_accounts)
    {
        savings_account->updateBalance(days);
    }
}

double Customer::totalBalance()
{
    double total_balance = 0;
    for (SavingsAccount* savings_account : savings_accounts)
        total_balance += savings_account->getBalance();
    for (Loan* loan : loans)
        total_balance -= loan->GetDebt();
    total_balance += main_account->getBalance();
    return total_balance;
}


double Customer::payLoan(uint32_t loan_id, uint32_t account_id, double amount)
{
    Account *account = nullptr;
    if (main_account->getID() == 0)
        account = main_account;
    else
        account = getSavingsAccount(account_id);
    
    Loan *loan = getLoan(loan_id);

    if (account == nullptr || loan == nullptr)
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
        std::cout << "giving back to the account " << -loan->GetDebt() << "$\n";
        account->deposit(-loan->GetDebt());

    case 0:
        //if the result is 0.4 for example it will round to 0
        //so we have to check if this isnt the case
        if (result != 0 && result != 2)
        {
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
    int length = savings_accounts.size();
    std::vector<SavingsAccount*> savings_accounts_copy;
    savings_accounts_copy.reserve(length - 1);
    for (int i = length - 1; i >= 0; i--)
    {
        if (savings_accounts.back()->getID() == account_id)
        {
            found = true;
            delete savings_accounts.back();
        }
        else
        {
            savings_accounts_copy.push_back(savings_accounts.back());
        }
        
        savings_accounts.pop_back();
    }
    savings_accounts = savings_accounts_copy;
    return found;
}



void Customer::printAllSavingsAccounts()
{
    for (SavingsAccount* saving_account : savings_accounts)
    {
        std::cout << "\tID: " << saving_account->getID() << " | balance: " << saving_account->getBalance() << "$ " << "interest rate: " << saving_account->getInterestRate() << "\n";
    }
}

void Customer::printMainAccount()
{
    std::cout << "\tID: " << main_account->getID() << " | balance: " << main_account->getBalance() << "$\n";
    
}


void Customer::printAllLoans()
{
    for (Loan* loan : loans)
    {
        std::cout << "\tID: " << loan->getID() << " | debt: " << -loan->GetDebt() << "$ | interest rate: " << loan->getInterestRate() << "\n";
    }
}

void Customer::printAllAccountsInCustomer()
{
    std::cout << "Your Main (checking) Account \n";
    printMainAccount();
    std::cout << "Your Savings Accounts: \n";
    printAllSavingsAccounts();
    std::cout << "Your Loans: \n";
    printAllLoans();
}