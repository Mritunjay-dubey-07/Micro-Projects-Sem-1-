
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <regex>
#include <ctime>
#include <iomanip>
#include <sstream>

class User {
public:
    std::string accountNumber;
    std::string ifscCode;
    std::string fullName;
    std::string email;
    std::string username;
    std::string password;
    std::string dateCreated;
    bool isActive;
    double balance;

    User() : isActive(true), balance(0.0) {}
    
    User(const std::string& accNum, const std::string& ifsc, const std::string& name,
         const std::string& mail, const std::string& user, const std::string& pass)
        : accountNumber(accNum), ifscCode(ifsc), fullName(name), email(mail),
          username(user), password(pass), isActive(true), balance(0.0) {
        
        // Set creation date
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        dateCreated = oss.str();
    }

    std::string toString() const {
        return accountNumber + "|" + ifscCode + "|" + fullName + "|" + 
               email + "|" + username + "|" + password + "|" + 
               dateCreated + "|" + (isActive ? "1" : "0") + "|" + 
               std::to_string(balance);
    }

    static User fromString(const std::string& data) {
        User user;
        std::vector<std::string> tokens;
        std::stringstream ss(data);
        std::string token;

        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 9) {
            user.accountNumber = tokens[0];
            user.ifscCode = tokens[1];
            user.fullName = tokens[2];
            user.email = tokens[3];
            user.username = tokens[4];
            user.password = tokens[5];
            user.dateCreated = tokens[6];
            user.isActive = (tokens[7] == "1");
            user.balance = std::stod(tokens[8]);
        }

        return user;
    }
};

class BankDatabase {
private:
    std::vector<User> users;
    std::map<std::string, std::string> ifscBanks; // IFSC -> Bank Name mapping
    const std::string DATABASE_FILE = "bank_users.db";
    const std::string IFSC_FILE = "ifsc_codes.db";

public:
    BankDatabase() {
        initializeIFSCCodes();
        loadDatabase();
    }

    ~BankDatabase() {
        saveDatabase();
    }

    void initializeIFSCCodes() {
        // Initialize with some common IFSC codes
        ifscBanks["BODD0000001"] = "Bank of Diddy - Main Branch";
        ifscBanks["BODD0000002"] = "Bank of Diddy - North Branch";
        ifscBanks["BODD0000003"] = "Bank of Diddy - South Branch";
        ifscBanks["BODD0000004"] = "Bank of Diddy - East Branch";
        ifscBanks["BODD0000005"] = "Bank of Diddy - West Branch";
        ifscBanks["HDFC0000001"] = "HDFC Bank - Sample Branch";
        ifscBanks["ICIC0000001"] = "ICICI Bank - Sample Branch";
        ifscBanks["SBIN0000001"] = "State Bank of India - Sample Branch";
        
        saveIFSCCodes();
    }

    void saveIFSCCodes() {
        std::ofstream file(IFSC_FILE);
        if (file.is_open()) {
            for (const auto& pair : ifscBanks) {
                file << pair.first << "|" << pair.second << std::endl;
            }
            file.close();
        }
    }

    void loadIFSCCodes() {
        std::ifstream file(IFSC_FILE);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                size_t pos = line.find('|');
                if (pos != std::string::npos) {
                    std::string ifsc = line.substr(0, pos);
                    std::string bank = line.substr(pos + 1);
                    ifscBanks[ifsc] = bank;
                }
            }
            file.close();
        }
    }

    bool validateAccountNumber(const std::string& accountNumber) {
        // Account number should be 10-12 digits
        std::regex pattern("^[0-9]{10,12}$");
        return std::regex_match(accountNumber, pattern);
    }

    bool validateIFSCCode(const std::string& ifscCode) {
        // IFSC format: 4 letters + 7 digits
        std::regex pattern("^[A-Z]{4}[0-9]{7}$");
        return std::regex_match(ifscCode, pattern);
    }

    bool validateEmail(const std::string& email) {
        std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        return std::regex_match(email, pattern);
    }

    bool isAccountNumberExists(const std::string& accountNumber) {
        return std::any_of(users.begin(), users.end(),
            [&accountNumber](const User& user) {
                return user.accountNumber == accountNumber;
            });
    }

    bool isUsernameExists(const std::string& username) {
        return std::any_of(users.begin(), users.end(),
            [&username](const User& user) {
                return user.username == username;
            });
    }

    bool isEmailExists(const std::string& email) {
        return std::any_of(users.begin(), users.end(),
            [&email](const User& user) {
                return user.email == email;
            });
    }

    bool isIFSCValid(const std::string& ifscCode) {
        return ifscBanks.find(ifscCode) != ifscBanks.end();
    }

    std::string getBankName(const std::string& ifscCode) {
        auto it = ifscBanks.find(ifscCode);
        return (it != ifscBanks.end()) ? it->second : "Unknown Bank";
    }

    enum class RegistrationResult {
        SUCCESS,
        INVALID_ACCOUNT_NUMBER,
        INVALID_IFSC_CODE,
        INVALID_EMAIL,
        ACCOUNT_NUMBER_EXISTS,
        USERNAME_EXISTS,
        EMAIL_EXISTS,
        IFSC_NOT_FOUND,
        EMPTY_FIELDS
    };

    RegistrationResult registerUser(const std::string& accountNumber,
                                  const std::string& ifscCode,
                                  const std::string& fullName,
                                  const std::string& email,
                                  const std::string& username,
                                  const std::string& password) {
        
        // Check for empty fields
        if (accountNumber.empty() || ifscCode.empty() || fullName.empty() ||
            email.empty() || username.empty() || password.empty()) {
            return RegistrationResult::EMPTY_FIELDS;
        }

        // Validate account number format
        if (!validateAccountNumber(accountNumber)) {
            return RegistrationResult::INVALID_ACCOUNT_NUMBER;
        }

        // Validate IFSC code format
        std::string upperIFSC = ifscCode;
        std::transform(upperIFSC.begin(), upperIFSC.end(), upperIFSC.begin(), ::toupper);
        if (!validateIFSCCode(upperIFSC)) {
            return RegistrationResult::INVALID_IFSC_CODE;
        }

        // Validate email format
        if (!validateEmail(email)) {
            return RegistrationResult::INVALID_EMAIL;
        }

        // Check if account number already exists
        if (isAccountNumberExists(accountNumber)) {
            return RegistrationResult::ACCOUNT_NUMBER_EXISTS;
        }

        // Check if username already exists
        if (isUsernameExists(username)) {
            return RegistrationResult::USERNAME_EXISTS;
        }

        // Check if email already exists
        if (isEmailExists(email)) {
            return RegistrationResult::EMAIL_EXISTS;
        }

        // Check if IFSC code is valid (exists in our bank)
        if (!isIFSCValid(upperIFSC)) {
            return RegistrationResult::IFSC_NOT_FOUND;
        }

        // Create new user
        User newUser(accountNumber, upperIFSC, fullName, email, username, password);
        users.push_back(newUser);
        
        return RegistrationResult::SUCCESS;
    }

    enum class LoginResult {
        SUCCESS,
        INVALID_CREDENTIALS,
        ACCOUNT_INACTIVE,
        USER_NOT_FOUND
    };

    LoginResult authenticateUser(const std::string& username, const std::string& password) {
        auto it = std::find_if(users.begin(), users.end(),
            [&username](const User& user) {
                return user.username == username;
            });

        if (it == users.end()) {
            return LoginResult::USER_NOT_FOUND;
        }

        if (!it->isActive) {
            return LoginResult::ACCOUNT_INACTIVE;
        }

        if (it->password != password) {
            return LoginResult::INVALID_CREDENTIALS;
        }

        return LoginResult::SUCCESS;
    }

    User* getUser(const std::string& username) {
        auto it = std::find_if(users.begin(), users.end(),
            [&username](const User& user) {
                return user.username == username;
            });
        
        return (it != users.end()) ? &(*it) : nullptr;
    }

    void updateUserBalance(const std::string& username, double newBalance) {
        User* user = getUser(username);
        if (user) {
            user->balance = newBalance;
        }
    }

    void saveDatabase() {
        std::ofstream file(DATABASE_FILE);
        if (file.is_open()) {
            for (const User& user : users) {
                file << user.toString() << std::endl;
            }
            file.close();
        }
    }

    void loadDatabase() {
        std::ifstream file(DATABASE_FILE);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    users.push_back(User::fromString(line));
                }
            }
            file.close();
        }
        loadIFSCCodes();
    }

    void displayAllUsers() {
        std::cout << "\n=== Bank of Diddy - User Database ===" << std::endl;
        std::cout << std::setw(15) << "Account No" << std::setw(12) << "IFSC" 
                  << std::setw(20) << "Full Name" << std::setw(25) << "Email"
                  << std::setw(15) << "Username" << std::setw(10) << "Balance" 
                  << std::setw(8) << "Active" << std::endl;
        std::cout << std::string(110, '-') << std::endl;

        for (const User& user : users) {
            std::cout << std::setw(15) << user.accountNumber 
                      << std::setw(12) << user.ifscCode
                      << std::setw(20) << user.fullName.substr(0, 19)
                      << std::setw(25) << user.email.substr(0, 24)
                      << std::setw(15) << user.username
                      << std::setw(10) << std::fixed << std::setprecision(2) << user.balance
                      << std::setw(8) << (user.isActive ? "Yes" : "No")
                      << std::endl;
        }
        std::cout << "\nTotal Users: " << users.size() << std::endl;
    }

    std::string getRegistrationResultMessage(RegistrationResult result) {
        switch (result) {
            case RegistrationResult::SUCCESS:
                return "Account created successfully!";
            case RegistrationResult::INVALID_ACCOUNT_NUMBER:
                return "Invalid account number format. Must be 10-12 digits.";
            case RegistrationResult::INVALID_IFSC_CODE:
                return "Invalid IFSC code format. Must be 4 letters + 7 digits.";
            case RegistrationResult::INVALID_EMAIL:
                return "Invalid email format.";
            case RegistrationResult::ACCOUNT_NUMBER_EXISTS:
                return "Account number already exists.";
            case RegistrationResult::USERNAME_EXISTS:
                return "Username already exists.";
            case RegistrationResult::EMAIL_EXISTS:
                return "Email already registered.";
            case RegistrationResult::IFSC_NOT_FOUND:
                return "IFSC code not found in our bank network.";
            case RegistrationResult::EMPTY_FIELDS:
                return "All fields are required.";
            default:
                return "Unknown error occurred.";
        }
    }

    std::string getLoginResultMessage(LoginResult result) {
        switch (result) {
            case LoginResult::SUCCESS:
                return "Login successful!";
            case LoginResult::INVALID_CREDENTIALS:
                return "Invalid username or password.";
            case LoginResult::ACCOUNT_INACTIVE:
                return "Account is inactive. Please contact support.";
            case LoginResult::USER_NOT_FOUND:
                return "User not found.";
            default:
                return "Unknown error occurred.";
        }
    }
};

// Main function for testing the database
int main() {
    BankDatabase db;
    int choice;

    while (true) {
        std::cout << "\n=== Bank of Diddy - Database Management ===" << std::endl;
        std::cout << "1. Register New User" << std::endl;
        std::cout << "2. User Login" << std::endl;
        std::cout << "3. Display All Users" << std::endl;
        std::cout << "4. Update User Balance" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: {
                std::string accountNumber, ifscCode, fullName, email, username, password;
                
                std::cout << "\n--- User Registration ---" << std::endl;
                std::cout << "Account Number (10-12 digits): ";
                std::getline(std::cin, accountNumber);
                
                std::cout << "IFSC Code (e.g., BODD0000001): ";
                std::getline(std::cin, ifscCode);
                
                std::cout << "Full Name: ";
                std::getline(std::cin, fullName);
                
                std::cout << "Email: ";
                std::getline(std::cin, email);
                
                std::cout << "Username: ";
                std::getline(std::cin, username);
                
                std::cout << "Password: ";
                std::getline(std::cin, password);

                auto result = db.registerUser(accountNumber, ifscCode, fullName, email, username, password);
                std::cout << "\nResult: " << db.getRegistrationResultMessage(result) << std::endl;
                
                if (result == BankDatabase::RegistrationResult::SUCCESS) {
                    std::cout << "Bank: " << db.getBankName(ifscCode) << std::endl;
                }
                break;
            }
            
            case 2: {
                std::string username, password;
                
                std::cout << "\n--- User Login ---" << std::endl;
                std::cout << "Username: ";
                std::getline(std::cin, username);
                
                std::cout << "Password: ";
                std::getline(std::cin, password);

                auto result = db.authenticateUser(username, password);
                std::cout << "\nResult: " << db.getLoginResultMessage(result) << std::endl;
                
                if (result == BankDatabase::LoginResult::SUCCESS) {
                    User* user = db.getUser(username);
                    if (user) {
                        std::cout << "Welcome back, " << user->fullName << "!" << std::endl;
                        std::cout << "Account: " << user->accountNumber << std::endl;
                        std::cout << "Balance: $" << std::fixed << std::setprecision(2) << user->balance << std::endl;
                    }
                }
                break;
            }
            
            case 3:
                db.displayAllUsers();
                break;
            
            case 4: {
                std::string username;
                double balance;
                
                std::cout << "\n--- Update Balance ---" << std::endl;
                std::cout << "Username: ";
                std::getline(std::cin, username);
                
                std::cout << "New Balance: $";
                std::cin >> balance;

                User* user = db.getUser(username);
                if (user) {
                    db.updateUserBalance(username, balance);
                    std::cout << "Balance updated successfully!" << std::endl;
                } else {
                    std::cout << "User not found!" << std::endl;
                }
                break;
            }
            
            case 5:
                std::cout << "Saving database and exiting..." << std::endl;
                return 0;
            
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }

    return 0;
}
