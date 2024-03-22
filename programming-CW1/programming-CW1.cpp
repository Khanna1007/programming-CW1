#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

// Global variable to track the current logged-in users
string loggedInUser;

// Encrypts the password using the Caesar cipher
string encryptPassword(string password) {
    string encryptedPassword = "";
    for (char i : password) {
        if (isalpha(i)) {
            char base = isupper(i) ? 'A' : 'a';
            encryptedPassword += char(int(i - base + 1) % 26 + base);
        }
        else {
            encryptedPassword += i;
        }
    }
    return encryptedPassword;
}

// Decrypts the password using the Caesar cipher
string decryptPassword(string password) {
    string decryptedPassword = "";
    for (char i : password) {
        if (isalpha(i)) {
            char base = isupper(i) ? 'A' : 'a';
            decryptedPassword += char((int(i - base - 1) + 26) % 26 + base);
        }
        else {
            decryptedPassword += i;
        }
    }
    return decryptedPassword;
}

// Generates a random password
string generatePassword(int length) {
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+{}[]|;:,.<>?";
    string password = "";
    srand(time(nullptr)); // Initialize random seed
    for (int i = 0; i < length; i++) {
        password += characters[rand() % characters.length()];
    }
    return password;
}

// Adds a new user
void addUser(const string& username, const string& password) {
    ofstream userFile(username + "_users.txt");
    if (userFile.is_open()) {
        userFile << encryptPassword(password) << endl;
        userFile.close();
        cout << "User added successfully." << endl;
    }
    else {
        cout << "Error: Unable to open the user file." << endl;
    }
}

// Authenticates a user
bool authenticateUser(const string& username, const string& password) {
    ifstream userFile(username + "_users.txt");
    if (userFile.is_open()) {
        string storedPassword;
        getline(userFile, storedPassword);
        userFile.close();
        if (decryptPassword(storedPassword) == password) {
            loggedInUser = username;
            return true;
        }
    }
    return false;
}

// Adds a new password for the currently logged-in user
void addPassword(const string& website, const string& password) {
    if (!loggedInUser.empty()) {
        ofstream passFile(loggedInUser + "_passwords.txt", ios::app);
        if (passFile.is_open()) {
            passFile << encryptPassword(website) << ":" << encryptPassword(password) << endl;
            passFile.close();
            cout << "Password added successfully." << endl;
        }
        else {
            cout << "Error: Unable to open the password file." << endl;
        }
    }
}

// Retrieves a password for the currently logged-in user
string retrievePassword(const string& website) {
    if (!loggedInUser.empty()) {
        ifstream passFile(loggedInUser + "_passwords.txt");
        if (passFile.is_open()) {
            string line;
            while (getline(passFile, line)) {
                size_t separator = line.find(':');
                string encryptedWebsite = line.substr(0, separator);
                string encryptedPassword = line.substr(separator + 1);
                if (decryptPassword(encryptedWebsite) == website) {
                    passFile.close();
                    return decryptPassword(encryptedPassword);
                }
            }
            passFile.close();
        }
        else {
            cout << "Error: Unable to open the password file." << endl;
        }
    }
    return "";
}

// Deletes a password for the currently logged-in user
void deletePassword(const string& website) {
    if (!loggedInUser.empty()) {
        ifstream passFile(loggedInUser + "_passwords.txt");
        ofstream tempFile("temp.txt");
        if (passFile.is_open() && tempFile.is_open()) {
            string line;
            bool found = false;
            while (getline(passFile, line)) {
                size_t separator = line.find(':');
                string encryptedWebsite = line.substr(0, separator);
                if (decryptPassword(encryptedWebsite) != website) {
                    tempFile << line << endl;
                }
                else {
                    found = true;
                }
            }
            passFile.close();
            tempFile.close();
            if (found) {
                remove((loggedInUser + "_passwords.txt").c_str());
                rename("temp.txt", (loggedInUser + "_passwords.txt").c_str());
                cout << "Password for " << website << " deleted successfully." << endl;
            }
            else {
                cout << "No password found for " << website << "." << endl;
                remove("temp.txt"); // Clean up the temporary file if no matching website was found
            }
        }
        else {
            cout << "Error: Unable to access the password file." << endl;
        }
    }
}

// Function to list all passwords for the currently logged-in user
void listPasswords() {
    if (!loggedInUser.empty()) {
        ifstream passFile(loggedInUser + "_passwords.txt");
        if (passFile.is_open()) {
            string line;
            bool hasPasswords = false;
            cout << "Stored passwords:" << endl;
            while (getline(passFile, line)) {
                size_t separator = line.find(':');
                string encryptedWebsite = line.substr(0, separator);
                string encryptedPassword = line.substr(separator + 1);
                cout << "Website: " << decryptPassword(encryptedWebsite)
                    << ", Password: " << decryptPassword(encryptedPassword) << endl;
                hasPasswords = true;
            }
            if (!hasPasswords) {
                cout << "No passwords stored." << endl;
            }
            passFile.close();
        }
        else {
            cout << "Error: Unable to open the password file." << endl;
        }
    }
}

int main() {
    while (true) {
        int choice;
        cout << "1. Sign up\n2. Login\n3. Exit\nEnter your choice: ";
        cin >> choice;

        string username, password, website;

        switch (choice) {
        case 1: // Sign up
            cout << "Enter a username: ";
            cin >> username;
            cout << "Enter a password: ";
            cin >> password;
            addUser(username, password);
            break;
        case 2: // Login
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;
            if (authenticateUser(username, password)) {
                cout << "Login successful." << endl;
                bool userSession = true;
                while (userSession) {
                    cout << "1. Add a new password\n2. Generate a password\n3. Retrieve a password\n4. Delete a password\n5. List all passwords\n6. Logout\nEnter your choice: ";
                    cin >> choice;
                    switch (choice) {
                    case 1: // Add a new password
                        cout << "Enter the website: ";
                        cin >> website;
                        cout << "Enter the password: ";
                        cin >> password;
                        addPassword(website, password);
                        break;
                    case 2: // Generate a password
                        cout << "Generated password: " << generatePassword(10) << endl;
                        break;
                    case 3: // Retrieve a password
                        cout << "Enter the website: ";
                        cin >> website;
                        password = retrievePassword(website);
                        if (!password.empty()) {
                            cout << "Password: " << password << endl;
                        }
                        else {
                            cout << "No password found for " << website << "." << endl;
                        }
                        break;
                    case 4: // Delete a password
                        cout << "Enter the website: ";
                        cin >> website;
                        deletePassword(website);
                        break;
                    case 5: // List all passwords
                        listPasswords();
                        break;
                    case 6: // Logout
                        loggedInUser.clear();
                        userSession = false;
                        cout << "Logged out successfully." << endl;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                    }
                }
            }
            else {
                cout << "Invalid username or password." << endl;
            }
            break;
        case 3: // Exit
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
