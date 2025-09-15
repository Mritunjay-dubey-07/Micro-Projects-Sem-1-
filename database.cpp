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
        // Database saves are handled explicitly where needed
    }

    void initializeIFSCCodes() {
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
        std::regex pattern("^[0-9]{10,12}$");
        return std::regex_match(accountNumber, pattern);
    }

    bool validateIFSCCode(const std::string& ifscCode) {
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

    enum class LoginResult {
        SUCCESS,
        INVALID_CREDENTIALS,
        EMPTY_FIELDS
    };

    RegistrationResult registerUser(const std::string& accountNumber,
                                  const std::string& ifscCode,
                                  const std::string& fullName,
                                  const std::string& email,
                                  const std::string& username,
                                  const std::string& password) {
        
        if (accountNumber.empty() || ifscCode.empty() || fullName.empty() ||
            email.empty() || username.empty() || password.empty()) {
            return RegistrationResult::EMPTY_FIELDS;
        }

        if (!validateAccountNumber(accountNumber)) {
            return RegistrationResult::INVALID_ACCOUNT_NUMBER;
        }

        std::string upperIFSC = ifscCode;
        std::transform(upperIFSC.begin(), upperIFSC.end(), upperIFSC.begin(), ::toupper);
        if (!validateIFSCCode(upperIFSC)) {
            return RegistrationResult::INVALID_IFSC_CODE;
        }

        if (!validateEmail(email)) {
            return RegistrationResult::INVALID_EMAIL;
        }

        if (isAccountNumberExists(accountNumber)) {
            return RegistrationResult::ACCOUNT_NUMBER_EXISTS;
        }

        if (isUsernameExists(username)) {
            return RegistrationResult::USERNAME_EXISTS;
        }

        if (isEmailExists(email)) {
            return RegistrationResult::EMAIL_EXISTS;
        }

        if (!isIFSCValid(upperIFSC)) {
            return RegistrationResult::IFSC_NOT_FOUND;
        }

        User newUser(accountNumber, upperIFSC, fullName, email, username, password);
        users.push_back(newUser);
        
        return RegistrationResult::SUCCESS;
    }

    LoginResult verifyLogin(const std::string& username, const std::string& password) {
        if (username.empty() || password.empty()) {
            return LoginResult::EMPTY_FIELDS;
        }

        for (const auto& user : users) {
            if (user.username == username && user.password == password && user.isActive) {
                return LoginResult::SUCCESS;
            }
        }
        
        return LoginResult::INVALID_CREDENTIALS;
    }

    void saveDatabase() {
        std::ofstream file(DATABASE_FILE, std::ios_base::app); // Append to the file
        if (file.is_open()) {
            file << users.back().toString() << std::endl;
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
    }

    std::string getRegistrationResultMessage(RegistrationResult result) {
        switch (result) {
            case RegistrationResult::SUCCESS:
                return "SUCCESS";
            case RegistrationResult::INVALID_ACCOUNT_NUMBER:
                return "ERROR: Invalid account number format. Must be 10-12 digits.";
            case RegistrationResult::INVALID_IFSC_CODE:
                return "ERROR: Invalid IFSC code format. Must be 4 letters + 7 digits.";
            case RegistrationResult::INVALID_EMAIL:
                return "ERROR: Invalid email format.";
            case RegistrationResult::ACCOUNT_NUMBER_EXISTS:
                return "ERROR: Account number already exists.";
            case RegistrationResult::USERNAME_EXISTS:
                return "ERROR: Username already exists.";
            case RegistrationResult::EMAIL_EXISTS:
                return "ERROR: Email already registered.";
            case RegistrationResult::IFSC_NOT_FOUND:
                return "ERROR: IFSC code not found in our bank network.";
            case RegistrationResult::EMPTY_FIELDS:
                return "ERROR: All fields are required.";
            default:
                return "ERROR: Unknown error occurred.";
        }
    }

    std::string getLoginResultMessage(LoginResult result) {
        switch (result) {
            case LoginResult::SUCCESS:
                return "SUCCESS";
            case LoginResult::INVALID_CREDENTIALS:
                return "ERROR: Credentials not correct";
            case LoginResult::EMPTY_FIELDS:
                return "ERROR: Username and password are required.";
            default:
                return "ERROR: Unknown error occurred.";
        }
    }
};

// Main function now accepts command line arguments
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "ERROR: Invalid number of arguments." << std::endl;
        return 1;
    }

    BankDatabase db;
    std::string operation = argv[1];
    
    if (operation == "register") {
        if (argc != 8) {
            std::cout << "ERROR: Registration requires 6 arguments." << std::endl;
            return 1;
        }
        
        std::string accountNumber = argv[2];
        std::string ifscCode = argv[3];
        std::string fullName = argv[4];
        std::string email = argv[5];
        std::string username = argv[6];
        std::string password = argv[7];

        auto result = db.registerUser(accountNumber, ifscCode, fullName, email, username, password);
        std::cout << db.getRegistrationResultMessage(result) << std::endl;

        if (result == BankDatabase::RegistrationResult::SUCCESS) {
            db.saveDatabase();
        }
    }
    else if (operation == "login") {
        if (argc != 4) {
            std::cout << "ERROR: Login requires 2 arguments." << std::endl;
            return 1;
        }
        
        std::string username = argv[2];
        std::string password = argv[3];

        auto result = db.verifyLogin(username, password);
        std::cout << db.getLoginResultMessage(result) << std::endl;
    }
    else {
        std::cout << "ERROR: Unknown operation. Use 'register' or 'login'." << std::endl;
        return 1;
    }

    return 0;
}
