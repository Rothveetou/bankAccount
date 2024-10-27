# bankAccount
**Bank Account Management System**
This project implements a basic bank account management system using C++. The bankAccount class encapsulates the properties and functionalities of a bank account, including account holder's name, account number, account type (checking/saving), balance, and interest rate.

**Features**
Dynamic Account Number Assignment: Utilizes a static member variable to automatically assign unique account numbers to each new bank account.
Data Storage: Supports an array of up to 10 bank account objects, allowing management of multiple customer accounts.
Member Functions: Provides functionalities to manipulate account data, such as depositing, withdrawing, and viewing account details.
Class Definition
The bankAccount class includes:

**Member Variables:**

std::string name: Name of the account holder.
int accountNumber: Unique account number.
std::string accountType: Type of account (checking or savings).
double balance: Current balance of the account.
double interestRate: Interest rate as a decimal.
Member Functions:

Constructor to initialize the account.
Functions for depositing and withdrawing funds.
Function to display account information.
**Usage**
The program demonstrates how to create and manage bank account objects, including user interactions to perform operations on their accounts.


**How to run the code**
- In terminal run g++ file_Name.cpp
- Run ./a.out
