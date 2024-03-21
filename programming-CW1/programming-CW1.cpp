#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

// Function to decrypt the password using the Caesar cipher 
string decryptPassword(string password) {
    string decryptedPassword = "";
    for (int i = 0; i < password.length(); i++) {
        if (isalpha(password[i])) {
            char c = isupper(password[i]) ? 'A' : 'a';
            decryptedPassword += char(int(password[i] - c + 26 - 1) % 26 + c);
        }
        else {
            decryptedPassword += password[i];
        }
    }
    return decryptedPassword;
}

// Function to encrypt the password using the Caesar cipher 
string encryptPassword(string password) {
    string encryptedPassword = "";
    for (int i = 0; i < password.length(); i++) {
        if (isalpha(password[i])) {
            char c = isupper(password[i]) ? 'A' : 'a';
            encryptedPassword += char(int(password[i] - c + 1) % 26 + c);
        }
        else {
            encryptedPassword += password[i];
        }
    }
    return encryptedPassword;
}



// Function to delete a password 
void deletePassword(string username, string website) {
    ifstream userFile;
    ofstream tempFile;
    userFile.open("users.txt");
    tempFile.open("temp.txt");
    if (userFile.is_open() && tempFile.is_open()) {
        string line;
        bool userFound = false;
        while (getline(userFile, line)) {
            string decryptedUsername = decryptPassword(line.substr(0, line.find(":")));
            if (decryptedUsername == username) {
                userFound = true;
                tempFile << line << endl;
            }
            else {
                tempFile << line;
            }
        }
        userFile.close();
        tempFile.close();

        ifstream tempUserFile;
        tempUserFile.open("temp.txt");
        ofstream newPasswordFile;
        newPasswordFile.open("newPasswords.txt", ios_base::app);
        string encryptedUsername = "";
        string decryptedPassword = "";
        if (tempUserFile.is_open() && newPasswordFile.is_open()) {
            while (getline(tempUserFile, line)) {
                string decryptedUsername = decryptPassword(line.substr(0, line.find(":")));
                encryptedUsername = line.substr(0, line.find(":"));
                if (decryptedUsername == username) {
                    string websiteToDelete = "";
                    bool passwordFound = false;
                    while (getline(tempUserFile, line)) {
                        string decryptedWebsite = decryptPassword(line.substr(0, line.find(":")));
                        if (decryptedWebsite == website) {
                            passwordFound = true;
                        }
                        else {
                            if (passwordFound) {
                                newPasswordFile << line << endl;
                            }
                            else {
                                newPasswordFile << encryptedUsername << ":" << line << endl;
                            }
                        }
                    }
                    newPasswordFile.close();
                    remove("passwords.txt");
                    rename("newPasswords.txt", "passwords.txt");
                    cout << "Password deleted successfully" << endl;
                }
            }
        }
    }
}

// Function to authenticate a user 
bool authenticateUser(string username, string password) {
    ifstream userFile;
    userFile.open("users.txt");
    if (userFile.is_open()) {
        string line;
        bool userFound = false;
        while (getline(userFile, line)) {
            string decryptedUsername = decryptPassword(line.substr(0, line.find(":")));
            if (decryptedUsername == username) {
                userFound = true;
                string decryptedPasswordInFile = decryptPassword(line.substr(line.find(":") + 1));
                if (decryptedPasswordInFile == password) {
                    userFile.close();
                    return true;
                }
                else {
                    cout << "Error: Incorrect password" << endl;
                    break;
                }
            }
        }
        userFile.close();
    }
    else {
        cout << "Error: Unable to open the users file" << endl;
    }
    return false;
}

// Function to generate a random password string 
auto generatePassword(int length) {
    string password = "";
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+{}[]|;:,.<>?";
    for (int i = 0; i < length; i++) {
        password += characters[rand() % characters.length()];
    }
    return password;
}

// Function to add a new user 
void addUser(string username, string password) {
    ofstream userFile;
    userFile.open("users.txt", ios_base::app);
    if (userFile.is_open()) {
        userFile << encryptPassword(username) << ":" << encryptPassword(password) << endl;
        userFile.close();
        cout << "User added successfully" << endl;
    }
    else {
        cout << "Error: Unable to open the user file" << endl;
    }
}

// Function to retrieve user information 
string getUserData(string username) {
    ifstream userFile;
    userFile.open("users.txt");
    if (userFile.is_open()) {
        string line;
        while (getline(userFile, line)) {
            string decryptedUsername = decryptPassword(line.substr(0, line.find(":")));
            if (decryptedUsername == username) {
                userFile.close();
                return decryptPassword(line.substr(line.find(":") + 1));
            }
        }
        userFile.close();
    }
    else {
        cout << "Error: Unable to open the user file" << endl;
    }
    return "";
}

// Function to add a new password 
void addPassword(const string& website, const string& username, const string& password) {
    ofstream passwordFile;
    passwordFile.open("passwords.txt", ios_base::app);
    if (passwordFile.is_open()) {
        passwordFile << encryptPassword(website) << ":" << encryptPassword(username) << ":" << encryptPassword(password) << endl;
        passwordFile.close();
        cout << "Password added successfully" << endl;
    }
    else {
        cout << "Error: Unable to open the password file" << endl;
    }
}


// Function to retrieve a password 
string retrievePassword(const string& username, const string& website) {
    ifstream passwordFile;
    passwordFile.open("passwords.txt");
    if (passwordFile.is_open()) {
        string line;
        while (getline(passwordFile, line)) {
            string decryptedWebsite = decryptPassword(line.substr(0, line.find(":")));
            string decryptedUsername = decryptPassword(line.substr(line.find(":") + 1, line.rfind(":") - line.find(":") - 1));
            if (decryptedWebsite == website && decryptedUsername == username) {
                string decryptedPassword = decryptPassword(line.substr(line.rfind(":") + 1));
                passwordFile.close();
                return decryptedPassword;
            }
        }
        passwordFile.close();
        cout << "Error: Username or website not found" << endl;
    }
    else {
        cout << "Error: Unable to open the password file" << endl;
    }
    return "";
}

int main() {
    int choice;
    string username, password, website, userInput, encryptedUsername, decryptedPassword;

signup_login:
    cout << "1. Sign up" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1: // Sign up
        cout << "Enter a username: ";
        cin >> username;
        cout << "Enter a password: ";
        cin >> password;

        addUser(username, password);
        cout << "User added successfully" << endl;
        break;
    case 2: // Login
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        if (authenticateUser(username, password)) {
            cout << "Login successful" << endl;

            while (true) {
                cout << "1. Add a new password" << endl;
                cout << "2. Generate a password" << endl;
                cout << "3. Retrieve a password" << endl;
                cout << "4. Delete a password" << endl;
                cout << "5. Logout" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    cout << "Enter the website: ";
                    cin >> website;
                    cout << "Enter Username: ";
                    cin >> username;
                    cout << "Enter the password: ";
                    cin >> password;
                    addPassword(website, username, password);
                    break;
                case 2:
                    cout << "Enter the website: ";
                    cin >> website;
                    cout << "Generated password: " << generatePassword(10) << endl;
                    break;
                case 3:
                    cout << "Enter Username: ";
                    cin >> username;
                    cout << "Enter the website: ";
                    cin >> website;
                    decryptedPassword = retrievePassword(username, website);
                    if (decryptedPassword != "") {
                        cout << "Retrieved password: " << decryptedPassword << endl;
                    }
                    else {
                        cout << "Password not found" << endl;
                    }
                    break;
                case 4:
                    cout << "Enter Username: ";
                    cin >> username;
                    cout << "Enter the website: ";
                    cin >> website;

                    deletePassword(username, website);
                    cout << "successfully deleted user and password" << endl;
                    break;
                    
                case 5:
                    cout << "Logout successful" << endl;
                    goto signup_login;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
        }
        else {
            cout << "Error: Invalid username or password" << endl;
            break;
        }
    case 3: // Exit
        return 0;
    default:
        cout << "Invalid choice. Please try again." << endl;
        break;
    }

    // If there was an error or invalid attempt, display options again
    goto signup_login;
}
