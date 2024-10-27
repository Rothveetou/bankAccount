

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

//Account class
class bankAccount
{
    private:
        string accountHolderName; // Richard
        int accoutNumber; // 00203056
        string accountType; // checking/saving
        double balance;  // $1,000,000
        double interestRate;  // 0.5 per year
        static int accountNumberCounter; 

    public:
        //constructor
        bankAccount(string accountName, string type, double initialBalance, double interestRate): accountHolderName(accountName),accountType(type),balance(initialBalance),interestRate(interestRate) {
            accoutNumber =  ++accountNumberCounter;
        }

    void accountDetail() const {
        cout << "Account Holder Name: " << accountHolderName << endl;
        cout << "Account Number: " << accoutNumber << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: $" << balance << fixed << setprecision(2) << endl;
        cout << "Interest Rate: " << interestRate * 100 << fixed << setprecision(2) << "%" << endl;
        cout << "Current value in the " <<(char) tolower(accountType[0]) << accountType.substr(1) << " account: "<< balance << endl;
        cout << setfill('-') << setw(20) << '-' << endl;
    }
    void deposit(double amount) {
        balance += amount;
        // split the accoutType char to lowercase then enter all lowerCase characters
        cout << accountHolderName << " has been successfully deposited $" << amount << fixed << setprecision(2)  << " into your " <<(char) tolower(accountType[0]) << accountType.substr(1) << endl;
    }
    void withdraw(double amount) {
        // validate
        if(amount <= balance) {
            balance -= amount;
            cout << accountHolderName << " has been successfully withdraw $" << amount << fixed << setprecision(2) << " from your " << (char) tolower(accountType[0]) << accountType.substr(1) << endl;
        }
        else {
            cerr << "Insufficient funds to complete the transaction. " << endl;
        }
    }
    void calculateInterest() {
        balance += (balance * interestRate);
        cout << "Interest applied successfully: " << interestRate << endl;
    }
};


int bankAccount::accountNumberCounter = 1000000 + (rand() % 1001);  // generate a random number of account numbers 0-9

int main () {
    // Clear the screen on every operating sytems
   
   #ifdef _WIN32
        system("cls");  // window
    #else
        system("clear"); // linuxx and mac
    #endif

    // Create an array of 10 customers
    bankAccount customers[10] = {
        bankAccount("Richhard", "Checking", 2000, 0.1),
        bankAccount("Alice", "Savings", 1500, 0.05),
        bankAccount("Bob", "Checking", 3200, 0.02),
        bankAccount("Charlie", "Savings", 4000, 0.03),
        bankAccount("David", "Checking", 2500, 0.04),
        bankAccount("Eva", "Savings", 1800, 0.06),
        bankAccount("Frank", "Checking", 2700, 0.07),
        bankAccount("Grace", "Savings", 2200, 0.08),
        bankAccount("Hannah", "Checking", 3100, 0.09),
        bankAccount("Ian", "Savings", 3500, 0.1),

    };
    // Display details of each customer
    for (int i = 0; i < 10; i++) {
        customers[i].accountDetail();

    }

    customers[0].deposit(400);
    customers[0].withdraw(150);
    customers[0].calculateInterest();
    customers[0].accountDetail();

    customers[1].deposit(2);
    customers[1].accountDetail();

    return 0;
}

