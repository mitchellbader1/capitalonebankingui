#include <cpr/cpr.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

bool newUser = false;
std::string currLine, id, firstName, lastName, streetNumber, streetName, state, city, zip, type, nickname, rewards, balance, accountNumber, accid, medium, date, status, amount, desc, payeeid, credit, monthly;
std::string getAccountNum();

int main(int argc, char** argv) {
    while (1) // check is user needs to do Capital One new customer setup
    {
        std::cout << "Are you a new user? Y/N?\n";
        std::getline(std::cin, currLine);

        if (currLine == "Y")
        {
            newUser = true;
            break;
        }
        else if (currLine == "N")
        {
            newUser = false;
            break;
        }
        else
        {
            std::cout << "Please enter a valid response.\n";
        }
    }
    if (newUser == true) // new customer setup
    {
        std::cout << "Welcome to the Capital One Banking System! Please enter your first name:\n";
        std::getline(std::cin, firstName);
        std::cout << "\nPlease enter your last name:\n";
        std::getline(std::cin, lastName);
        std::cout << "\nWhat is the street number of your place of residence?:\n";
        std::getline(std::cin, streetNumber);
        std::cout << "\nWhat is the street you live on?:\n";
        std::getline(std::cin, streetName);
        std::cout << "\nWhat is the city you live in?:\n";
        std::getline(std::cin, city);
        std::cout << "\nWhat is the state you live in (Abbreviation)?:\n";
        std::getline(std::cin, state);
        std::cout << "\nLastly, what is your zip code?:\n";
        std::getline(std::cin, zip);
        // use stringstream to combine info into a singular string
        std::ostringstream string;
        string << "{ \"first_name\": \"" << firstName << "\", \"last_name\": \"" << lastName << "\", \"address\": { \"street_number\": \"" << streetNumber << "\", \"street_name\": \"" << streetName << "\", \"city\": \"" << city << "\", \"state\": \"" << state << "\", \"zip\": \"" << zip << "\"} }";
        std::string body = string.str();
        // create POST request with key to create customer with details specified in body
        cpr::Response r = cpr::Post(cpr::Url{ "api.nessieisreal.com/customers?key=57d190d65a504769663de6ef0c1f6a4a" },
            cpr::Header{ {"Content-Type", "application/json"} },
            cpr::Body{ body });
        //save id in text to variable
        id = r.text;
        int pos = id.find("_id");
        id.erase(0, pos + 6);
        id = id.substr(0, id.size() - 3);

        if (r.status_code == 201)
        {
            std::cout << "Customer creation successful!\n";
        }
        else
        {
            std::cout << "Customer creation unsuccessful.\n";
        }
    }
    if (newUser == false)
    {
        std::cout << "What is your ID number?\n";
        std:getline(std::cin, id);
    }
    while (1) // main interface used once customer signs in
    {
        std::cout << "\nOptions:\nA: Make an account\nB: Deposit to an account\nC: Withdraw from an account\nD: View all accounts\nE: Create a transfer\nF: Apply for a loan\nQ: Quit and sign off\n";
        std::getline(std::cin, currLine);
        if (currLine == "Q") // quit and sign off the Capital One banking interface
        {
            std::cout << "\n\nHave a nice day!\n\n";
            break;
        }
        if (currLine == "A") // 
        {
            //get the parameters of the new account
            std::cout << "Please enter your account type (Credit Card, Savings, Checking):\n";
            std::getline(std::cin, type);
            std::cout << "\nPlease enter a nickname for this account:\n";
            std::getline(std::cin, nickname);
            std::cout << "\nHow many reward points does this account have?:\n";
            std::getline(std::cin, rewards);
            std::cout << "\nWhat is the initial balance of this account?:\n";
            std::getline(std::cin, balance);
        
            //generate random 16 digit account number
            accountNumber = getAccountNum();

            std::ostringstream accountInfo;
            accountInfo << "{ \"type\": \"" << type << "\", \"nickname\": \"" << nickname << "\", \"rewards\": " << rewards << ", \"balance\": " << balance << ", \"account_number\": \"" << accountNumber << "\"}";
            std::string body = accountInfo.str();

            cpr::Response r = cpr::Post(cpr::Url{ "api.nessieisreal.com/customers/" + id + "/accounts?key=57d190d65a504769663de6ef0c1f6a4a" },
            cpr::Header{ {"Content-Type", "application/json"} },
            cpr::Body{body});

            if (r.status_code == 201)
            {
                std::cout << "Account creation successful!\n";
            }
            else
            {
                std::cout << "Account creation unsuccessful. Please try again.\n";
            }

            //std::cout << r.text << "\n\n" << body;
        }
        if (currLine == "B") // user wants to deposit to an account
        {
            // prompt user for necessary details to create deposit
            std::cout << "Please enter your account ID:\n";
            std::getline(std::cin, accid);
            std::cout << "\nPlease enter the medium of deposit (\"balance\" or \"rewards\"):\n";
            std::getline(std::cin, medium);
            std::cout << "\nWhat is the date of this deposit? (YYYY-MM-DD):\n";
            std::getline(std::cin, date);
            std::cout << "\nWhat is the status of this transaction?:\n";
            std::getline(std::cin, status);
            std::cout << "\nHow much is being deposited?:\n";
            std::getline(std::cin, amount);
            std::cout << "\nWhat is the description of this transaction?\n";
            std::getline(std::cin, desc);
            // create deposit thru api
            std::ostringstream transactionInfo;
            transactionInfo << "{\"medium\": \""+ medium + "\", \"transaction_date\": \"" + date + "\", \"status\": \"" + status + "\", \"amount\": " + amount + ", \"description\": \"" + desc + "\"}";
            std::string body = transactionInfo.str();

            cpr::Response r = cpr::Post(cpr::Url{ "api.nessieisreal.com/accounts/" + accid + "/deposits?key=57d190d65a504769663de6ef0c1f6a4a" },
                cpr::Header{ {"Content-Type", "application/json"} },
                cpr::Body{ body });
            std::cout << "\n\n" << r.text << "\n\n";
        }
        if (currLine == "C") // user wants to withdraw from an account
        {
            // prompt user for necessary details to create withdrawal
            std::cout << "Please enter your account ID:\n";
            std::getline(std::cin, accid);
            std::cout << "\nPlease enter the medium of withdrawal (\"balance\" or \"rewards\"):\n";
            std::getline(std::cin, medium);
            std::cout << "\nWhat is the date of this withdrawal? (YYYY-MM-DD):\n";
            std::getline(std::cin, date);
            std::cout << "\nWhat is the status of this transaction?:\n";
            std::getline(std::cin, status);
            std::cout << "\nHow much is being withdrawn?:\n";
            std::getline(std::cin, amount);
            std::cout << "\nWhat is the description of this withdrawal?\n";
            std::getline(std::cin, desc);
            // create withdrawal
            std::ostringstream transactionInfo;
            transactionInfo << "{\"medium\": \"" << medium << "\", \"transaction_date\": \"" << date << "\", \"status\": \"" << status << "\", \"amount\": " << amount << ", \"description\": \"" << desc << "\"}";
            std::string body = transactionInfo.str();

            cpr::Response r = cpr::Post(cpr::Url{ "api.nessieisreal.com/accounts/" + accid + "/withdrawals?key=57d190d65a504769663de6ef0c1f6a4a" },
                cpr::Header{ {"Content-Type", "application/json"} },
                cpr::Body{ body });

            std::cout << r.text << "\n\n\n";
        }
        if (currLine == "D") // user wants to view accounts
        {
            cpr::Response r = cpr::Get(cpr::Url{ "api.nessieisreal.com/customers/" + id + "/accounts?key=57d190d65a504769663de6ef0c1f6a4a" });
            std::cout << r.text;
        }
        if (currLine == "E") // user wants to create a transfer
        {
            // prompt user for necessary details to create transfer
            std::cout << "Please enter the paying account's ID:\n";
            std::getline(std::cin, accid);
            std::cout << "\nPlease enter the medium of transaction (\"balance\" or \"rewards\"):\n";
            std::getline(std::cin, medium);
            std::cout << "\nPlease enter the recipient account's ID:\n";
            std::getline(std::cin, payeeid);
            std::cout << "\nHow much is being transferred?:\n";
            std::getline(std::cin, amount);
            std::cout << "\nWhat is the date of this transfer? (YYYY-MM-DD):\n";
            std::getline(std::cin, date);
            std::cout << "\nWhat is the status of this transaction?:\n";
            std::getline(std::cin, status);
            std::cout << "\nWhat is the description of this transfer?\n";
            std::getline(std::cin, desc);
            // create transfer via nessie
            std::ostringstream transactionInfo;
            transactionInfo << "{\"medium\": \"" + medium + "\", \"payee_id\": \"" + payeeid + "\", \"transaction_date\": \"" + date + "\", \"amount\": " + amount + ", \"status\": \"" + status + "\", \"amount\": " + amount + ", \"description\": \"" + desc + "\"}";
            std::string body = transactionInfo.str();
            
            cpr::Response r = cpr::Post(cpr::Url{ "api.nessieisreal.com/accounts/" + accid + "/transfers?key=57d190d65a504769663de6ef0c1f6a4a" },
                cpr::Header{ {"Content-Type", "application/json"} },
                cpr::Body{ body });
            std::cout << "\n\n" << r.text << "\n\n";
        }
        if (currLine == "F") // user wants to apply for a loan
        {
            // prompt user for necessary details to create loan
            std::cout << "Please enter the receiving account's ID:\n";
            std::getline(std::cin, accid);
            std::cout << "\nPlease enter the type of loan (\"home\", \"auto\", or \"small business\"):\n";
            std::getline(std::cin, type);
            std::cout << "\nWhat is the status of this loan?:\n";
            std::getline(std::cin, status);
            std::cout << "\nWhat is your credit score?:\n";
            std::getline(std::cin, credit);
            std::cout << "\nWhat is the monthly payment on this loan?:\n";
            std::getline(std::cin, monthly);
            std::cout << "\nWhat is the amount of this loan?\n";
            std::getline(std::cin, amount);
            std::cout << "\nWhat is the description of this loan?\n";
            std::getline(std::cin, desc);
            // create loan
            std::ostringstream transactionInfo;
            transactionInfo << "{\"type\": \"" + type + "\", \"status\": \"" + status + "\", \"credit_score\": " + credit + ", \"monthly_payment\": " + monthly + ", \"amount\": " + amount + ", \"description\": \"" + desc + "\"}";
            std::string body = transactionInfo.str();

            cpr::Response r = cpr::Post(cpr::Url{ "api.nessieisreal.com/accounts/" + accid + "/loans?key=57d190d65a504769663de6ef0c1f6a4a" },
                cpr::Header{ {"Content-Type", "application/json"} },
                cpr::Body{ body });
            std::cout << "\n\n" << r.text << "\n\n";
        }
    }
}

std::string getAccountNum()
{
    std::string random;
    srand(time(NULL)); // set seed for randomization
    // add 16 random digits 0-9 to a string then return
    for (int i = 0; i < 16; i++)
    {
        std::string curr = std::to_string(rand() % 10);
        random.append(curr);
    }
    return random;
}
