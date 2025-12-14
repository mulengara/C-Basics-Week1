#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ACCOUNTS 100
#define FILENAME "bank_data.dat"

// Structure to hold account information
typedef struct {
    char fullName[100];
    int accountNumber;
    char password[50];
    float balance;
    int isActive;
} BankAccount;

// Global variables
BankAccount accounts[MAX_ACCOUNTS];
int accountCount = 0;
int currentUserIndex = -1;

// Function prototypes
void loadAccountsFromFile();
void saveAccountsToFile();
void displayMainMenu();
void registerAccount();
int login();
void depositMoney();
void withdrawMoney();
void transferMoney();
void changePassword();
void displayAccountDetails();
int findAccountByNumber(int accountNumber);
int authenticateUser(int accountNumber, char *password);
void clearInputBuffer();
void pressEnterToContinue();

/**
 * Main function - Program entry point
 */
int main() {
    loadAccountsFromFile();
    
    printf("=============================================\n");
    printf("      WELCOME TO ONLINE BANKING SYSTEM\n");
    printf("=============================================\n");
    
    int choice;
    int loggedIn = 0;
    
    do {
        if (!loggedIn) {
            printf("\n--- Main Menu ---\n");
            printf("1. Register New Account\n");
            printf("2. Login\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            
            if (scanf("%d", &choice) != 1) {
                printf("❌ Invalid input! Please enter a number.\n");
                clearInputBuffer();
                continue;
            }
            
            switch (choice) {
                case 1:
                    registerAccount();
                    break;
                case 2:
                    loggedIn = login();
                    break;
                case 3:
                    printf("Thank you for using our banking system! 👋\n");
                    break;
                default:
                    printf("❌ Invalid choice! Please try again.\n");
            }
        } else {
            // User is logged in - show banking menu
            printf("\n--- Banking Menu ---\n");
            printf("1. Deposit Money\n");
            printf("2. Withdraw Money\n");
            printf("3. Transfer Money\n");
            printf("4. Change Password\n");
            printf("5. View Account Details\n");
            printf("6. Logout\n");
            printf("Enter your choice: ");
            
            if (scanf("%d", &choice) != 1) {
                printf("❌ Invalid input! Please enter a number.\n");
                clearInputBuffer();
                continue;
            }
            
            switch (choice) {
                case 1:
                    depositMoney();
                    break;
                case 2:
                    withdrawMoney();
                    break;
                case 3:
                    transferMoney();
                    break;
                case 4:
                    changePassword();
                    break;
                case 5:
                    displayAccountDetails();
                    break;
                case 6:
                    loggedIn = 0;
                    currentUserIndex = -1;
                    printf("✅ Successfully logged out!\n");
                    break;
                default:
                    printf("❌ Invalid choice! Please try again.\n");
            }
        }
    } while (choice != 3);
    
    saveAccountsToFile();
    return 0;
}

/**
 * Load accounts from file
 */
void loadAccountsFromFile() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("⚠️  No existing data found. Starting fresh.\n");
        return;
    }
    
    accountCount = fread(accounts, sizeof(BankAccount), MAX_ACCOUNTS, file);
    fclose(file);
    printf("✅ Loaded %d accounts from file.\n", accountCount);
}

/**
 * Save accounts to file
 */
void saveAccountsToFile() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("❌ Error saving data to file!\n");
        return;
    }
    
    fwrite(accounts, sizeof(BankAccount), accountCount, file);
    fclose(file);
    printf("✅ Data saved successfully!\n");
}

/**
 * Register a new bank account
 */
void registerAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("❌ Maximum account limit reached! Cannot create new account.\n");
        return;
    }
    
    BankAccount newAccount;
    
    printf("\n--- Account Registration ---\n");
    
    clearInputBuffer();
    printf("Enter Full Name: ");
    fgets(newAccount.fullName, sizeof(newAccount.fullName), stdin);
    newAccount.fullName[strcspn(newAccount.fullName, "\n")] = 0;
    
    // Generate unique account number
    newAccount.accountNumber = 100000 + accountCount + 1;
    
    printf("Set Password: ");
    fgets(newAccount.password, sizeof(newAccount.password), stdin);
    newAccount.password[strcspn(newAccount.password, "\n")] = 0;
    
    printf("Enter Initial Deposit: $");
    scanf("%f", &newAccount.balance);
    
    if (newAccount.balance < 0) {
        printf("❌ Initial deposit cannot be negative!\n");
        return;
    }
    
    newAccount.isActive = 1;
    
    accounts[accountCount] = newAccount;
    accountCount++;
    
    saveAccountsToFile();
    
    printf("\n✅ Account created successfully!\n");
    printf("📋 Your Account Details:\n");
    printf("   Account Holder: %s\n", newAccount.fullName);
    printf("   Account Number: %d\n", newAccount.accountNumber);
    printf("   Initial Balance: $%.2f\n", newAccount.balance);
    printf("🔒 Keep your account number and password safe!\n");
    
    pressEnterToContinue();
}

/**
 * User login function
 * @return 1 if login successful, 0 otherwise
 */
int login() {
    int accountNumber;
    char password[50];
    
    printf("\n--- Login ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    clearInputBuffer();
    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;
    
    int accountIndex = authenticateUser(accountNumber, password);
    
    if (accountIndex != -1) {
        currentUserIndex = accountIndex;
        printf("✅ Login successful! Welcome back, %s!\n", accounts[accountIndex].fullName);
        pressEnterToContinue();
        return 1;
    } else {
        printf("❌ Login failed! Invalid account number or password.\n");
        pressEnterToContinue();
        return 0;
    }
}

/**
 * Authenticate user credentials
 */
int authenticateUser(int accountNumber, char *password) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber && 
            accounts[i].isActive && 
            strcmp(accounts[i].password, password) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Deposit money into account
 */
void depositMoney() {
    float amount;
    
    printf("\n--- Deposit Money ---\n");
    printf("Current Balance: $%.2f\n", accounts[currentUserIndex].balance);
    printf("Enter amount to deposit: $");
    
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("❌ Invalid amount! Please enter a positive number.\n");
        clearInputBuffer();
        return;
    }
    
    accounts[currentUserIndex].balance += amount;
    saveAccountsToFile();
    
    printf("✅ Deposit successful!\n");
    printf("💰 New Balance: $%.2f\n", accounts[currentUserIndex].balance);
    
    pressEnterToContinue();
}

/**
 * Withdraw money from account
 */
void withdrawMoney() {
    float amount;
    
    printf("\n--- Withdraw Money ---\n");
    printf("Current Balance: $%.2f\n", accounts[currentUserIndex].balance);
    printf("Enter amount to withdraw: $");
    
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("❌ Invalid amount! Please enter a positive number.\n");
        clearInputBuffer();
        return;
    }
    
    if (amount > accounts[currentUserIndex].balance) {
        printf("❌ Insufficient funds! Available balance: $%.2f\n", accounts[currentUserIndex].balance);
        return;
    }
    
    accounts[currentUserIndex].balance -= amount;
    saveAccountsToFile();
    
    printf("✅ Withdrawal successful!\n");
    printf("💰 New Balance: $%.2f\n", accounts[currentUserIndex].balance);
    
    pressEnterToContinue();
}

/**
 * Transfer money between accounts
 */
void transferMoney() {
    int targetAccountNumber;
    float amount;
    
    printf("\n--- Transfer Money ---\n");
    printf("Your Current Balance: $%.2f\n", accounts[currentUserIndex].balance);
    printf("Enter recipient's account number: ");
    scanf("%d", &targetAccountNumber);
    
    if (targetAccountNumber == accounts[currentUserIndex].accountNumber) {
        printf("❌ Cannot transfer to your own account!\n");
        return;
    }
    
    int targetIndex = findAccountByNumber(targetAccountNumber);
    if (targetIndex == -1) {
        printf("❌ Recipient account not found!\n");
        return;
    }
    
    printf("Recipient: %s\n", accounts[targetIndex].fullName);
    printf("Enter amount to transfer: $");
    scanf("%f", &amount);
    
    if (amount <= 0) {
        printf("❌ Invalid amount! Please enter a positive number.\n");
        return;
    }
    
    if (amount > accounts[currentUserIndex].balance) {
        printf("❌ Insufficient funds! Available balance: $%.2f\n", accounts[currentUserIndex].balance);
        return;
    }
    
    // Perform transfer
    accounts[currentUserIndex].balance -= amount;
    accounts[targetIndex].balance += amount;
    saveAccountsToFile();
    
    printf("✅ Transfer successful!\n");
    printf("💰 Your New Balance: $%.2f\n", accounts[currentUserIndex].balance);
    
    pressEnterToContinue();
}

/**
 * Change user password
 */
void changePassword() {
    char currentPassword[50];
    char newPassword[50];
    char confirmPassword[50];
    
    printf("\n--- Change Password ---\n");
    
    clearInputBuffer();
    printf("Enter Current Password: ");
    fgets(currentPassword, sizeof(currentPassword), stdin);
    currentPassword[strcspn(currentPassword, "\n")] = 0;
    
    // Verify current password
    if (strcmp(accounts[currentUserIndex].password, currentPassword) != 0) {
        printf("❌ Current password is incorrect!\n");
        return;
    }
    
    printf("Enter New Password: ");
    fgets(newPassword, sizeof(newPassword), stdin);
    newPassword[strcspn(newPassword, "\n")] = 0;
    
    printf("Confirm New Password: ");
    fgets(confirmPassword, sizeof(confirmPassword), stdin);
    confirmPassword[strcspn(confirmPassword, "\n")] = 0;
    
    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("❌ Passwords do not match!\n");
        return;
    }
    
    if (strlen(newPassword) < 4) {
        printf("❌ Password must be at least 4 characters long!\n");
        return;
    }
    
    strcpy(accounts[currentUserIndex].password, newPassword);
    saveAccountsToFile();
    
    printf("✅ Password changed successfully!\n");
    
    pressEnterToContinue();
}

/**
 * Display account details
 */
void displayAccountDetails() {
    printf("\n--- Account Details ---\n");
    printf("Account Holder: %s\n", accounts[currentUserIndex].fullName);
    printf("Account Number: %d\n", accounts[currentUserIndex].accountNumber);
    printf("Current Balance: $%.2f\n", accounts[currentUserIndex].balance);
    printf("Account Status: %s\n", accounts[currentUserIndex].isActive ? "Active" : "Inactive");
    printf("🔒 Password: ********\n");
    
    pressEnterToContinue();
}

/**
 * Find account by account number
 */
int findAccountByNumber(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber && accounts[i].isActive) {
            return i;
        }
    }
    return -1;
}

/**
 * Clear input buffer
 */
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Wait for user to press Enter
 */
void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
    getchar();
}
