#include "bank.hpp"
#include <Windows.h>

void clearScreen() 
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

//returns true if wants to quit false if not
bool starterMenu(Bank *bank, Customer **customer)
{
    clearScreen();
    std::cout << "-----MENU-----\n\n";
    std::cout << "Sign up -> 1\n";
    std::cout << "Sign in -> 2\n"; 
    std::cout << "Quit -> 3\n";

    int answer;
    std::cin >> answer;

    switch (answer)
    {   
    case 1:
    {
        uint32_t id = bank->createCustomer();
        std::cout << "you are now a customer\n and your ID is: ";
        std::cout << id << "\n";
        *customer = bank->getCustomer(id);
        std::cout << "You are now logged into your new account.\n";
        Sleep(2000);
        clearScreen();
        return false;
    }
    case 2:
    {
        int try_num = 0;
        do 
        {
            uint32_t id;

            if (try_num > 0)
            {
                std::cout << "this is try num " << try_num << " out of 3 tries\n\n";
            }

            std::cout << "what is your customer ID? ";
            std::cin >> id;
            *customer = bank->getCustomer(id);
            try_num++;
        } while (try_num < 3 && *customer == NULL);
        
        if (*customer == NULL)
        {
            Sleep(2000);
            return true;
        }
        
        std::cout << "\nYou are now logged into your account.\n";
        Sleep(2000);
        clearScreen();
        return false;
    }
    case 3:
    {
        std::cout << "Quitting...\n";
        delete bank;
        return true;
    }

    default:
    {
        std::cout << "your input needs to be between 1 to 3.\n";
        Sleep(2000);
        return starterMenu(bank, customer);
    }
    }
}

void printSecondMenu()
{
    std::cout << "-----MENU-----\n";

    std::cout << "\n------ACCOUNT OPERATION------\n";
    std::cout << "Make a new checking account -> 1\n";
    std::cout << "Make a new savings account -> 2\n";
    std::cout << "Get a Loan -> 3\n";
    std::cout << "Pay Loan -> 4\n";
    std::cout << "Transfer money from account to a diffrent account -> 5\n";
    std::cout << "Transfer money to someone else -> 6\n";

    std::cout << "\nSign out -> 7\n";

    std::cout << "\n-----DELETE-----\n";
    std::cout << "Delete customer -> 8\n";
    std::cout << "Delete account -> 9\n";

    std::cout << "\n-----TIME SETTINGS-----\n";
    std::cout << "Fast forward (change days) -> 10\n";

    std::cout << "\nQuit -> 11\n";
}



//this is the menu the user will get after he is already signed in
//returns -1 if wants to show this menu again
// 1 if wants to sign out
// 0 if wants to quit
int secondMenu(Bank *bank, Customer **customer)
{
    clearScreen();
    if (*customer == NULL)
    {
        std::cout << "the program tried to run the second menu without any customer connected.\n";
        std::cout << "Quiting...\n";
        Sleep(3000);
        return 0;
    }

    std::cout << "+----your Customer ID: " << (*customer)->getID() << " ----+\n";
    std::cout << "+----Your Accounts/Loans----+\n";
    (*customer)->printAllAccountsInCustomer();
    printSecondMenu();


    int answer;

    std::cin >> answer;

    // if not returning somthing in the switch it will return later 0
    switch (answer)
    {
    case 1:
    {
        int id = (*customer)->createCheckingAccount();
        if (id == -1)
        {
            std::cout << "there was an error with creating a Checking account.";
            Sleep(3000);
            break;
        }
        std::cout << "created Checking Account with the ID: "<< id << "\n";
        break;
    }
    case 2:
    {
        int id = (*customer)->createSavingsAccount();
    
        if (id == -1)
        {
            std::cout << "there was an error with creating a Savings account.";
            Sleep(3000);
            break;
        }
        std::cout << "created a Savings Account with the ID: "<< id << "\n";
        break;
    }
    case 3:
    {
        double amount;
        std::cout << "what is the amount of money for the loan? ";
        std::cin >> amount;

        uint32_t account_id;
        std::cout << "\nwhat is the ID of the account from which you want to recive the money? ";
        std::cin >> account_id;
        Account * account = (*customer)->getAccount(account_id);
        if (account == NULL)
            break;

        uint32_t loan_answer = (*customer)->createLoan(amount, account);
        if (loan_answer == 0)
        {
            std::cout << "Error occured\n";
            Sleep(3000);
        }
        else
            std::cout << "\ncreated loan with the ID: " << loan_answer << "\n";

        break;
    }
    case 4:
    {
        std::cout << "what is the ID of the loan? ";

        uint32_t loan_id;
        std::cin >> loan_id;

        uint32_t account_id;
        std::cout << "\nwhat is the ID of the account from which you want to pay the loan? ";
        std::cin >> account_id;

        double amount;
        std::cout << "\nhow much do you want to pay for loan (-1 if you want to pay all the loan)? ";
        std::cin >> amount;
        if (amount == -1)
        {
            std::cout << "ok";
            double temp = (*customer)->payLoan(loan_id, account_id, (*customer)->getLoan(loan_id)->GetDebt());
            std::cout << temp;
            if (temp == 0)
                std::cout << "\nyou finished paying the loan! congrats!!!";
        }
        else
        {
            if ((*customer)->payLoan(loan_id, account_id, amount) > 0)
                std::cout << "\nyou still need to pay: " << (*customer)->getLoan(loan_id)->GetDebt() << "$ to finish paying the loan.\n";
        }
        Sleep(3000);
        break;
    }
    case 5:
    {
        uint32_t withdraw_account_id;
        std::cout << "\nwhat is the ID of the account from which you want to send the money? ";
        std::cin >> withdraw_account_id;

        uint32_t deposit_account_id;
        std::cout << "\nwhat is the ID of the account from which you want to send the money? ";
        std::cin >> deposit_account_id;

        double amount;
        std::cout << "\nhow much do you want to transfer? ";
        std::cin >> amount;
        if((*customer)->transferBetweenAccounts(withdraw_account_id, deposit_account_id, amount))
            std::cout << "\ntransfer was succceful.\n";
        else
            std::cout << "\ntransfer failed.\n";
        break;
    }
    case 6:
    {
        uint32_t withdraw_account_id;
        std::cout << "\nwhat is the ID of the account from which you want to send the money? ";
        std::cin >> withdraw_account_id;

        uint32_t deposit_customer_id;
        std::cout << "\nwhat is the ID of the customer that you want to send him money? ";
        std::cin >> deposit_customer_id;

        Customer *deposit_customer = bank->getCustomer(deposit_customer_id);

        if (deposit_customer == NULL)
            break;

        uint32_t deposit_account_id;
        std::cout << "\nwhat is the ID of the account inside the customer account that you want to send money to? ";
        std::cin >> deposit_account_id;

        double amount;
        std::cout << "\nhow much do you want to transfer? ";
        std::cin >> amount;

        if(bank->transfer(**customer, withdraw_account_id, *deposit_customer, deposit_account_id, amount))
            std::cout << "\ntransfer was succceful.\n";
        else
            std::cout << "\ntransfer failed.\n";

        break;
    }
    case 8:
        //deleting customer and signing out
        std::cout << "deleting customer...\n";
        bank->deleteCustomer((*customer)->getID());
        std::cout << "signing out...";
        Sleep(3000);
        *customer = NULL;
        return 1;

    case 7:

        std::cout << "signing out...";
        *customer = nullptr;
        return 1;

    case 9:
    {
        uint32_t account_id;
        std::cout << "\nwhat is the ID of the account that you want to delete? ";
        std::cin >> account_id;
        Account *account = (*customer)->getAccount(account_id);
        if (account == NULL)
            break;

        if (account->getBalance() > 0)
        {
            std::cout << "\nWarning! the account still has " << account->getBalance() << "$ in it.\n";
            // maybe help him put the money in another account?
        }
        
        std::cout << "\ndeleting account...\n";
        (*customer)->deleteAccount(account_id);
        break;
    }
    case 10:
    {
        int amount;
        std::cout << "how many days do you want to fast forward? ";
        std::cin >> amount;
        bank->changeDay(amount);
        Time time;
        std::cout << "\nwe are now at day " << time.day << "\n";
        break;
    }
    case 11:
        std::cout << "Quitting...\n";
        delete bank;
        return 0;

    default:
        //TODO: somthing else
        std::cout << "Quitting...\n";
        return 0;
    }
    return -1; // maybe change this
}

//I dont want to tell the user about customer so Im telling him "account"
// insted of customer
// and Checking account and Savings account insted of account
int main()
{

    Bank bank = Bank();
    Customer *customer = NULL;
    while (true)
    {
        
        if (starterMenu(&bank, &customer))
            return 1;
        
        int output;
        do
            output = secondMenu(&bank, &customer);
        while(output == -1);

        if (output == 1)
            continue;
        
        if (output == 0)
        {
            return 0;
        }

    }
}