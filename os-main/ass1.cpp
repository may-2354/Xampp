#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string>

using namespace std;

// Function to display the menu
void displayMenu() {
    cout << "\n==== LINUX COMMAND SIMULATOR =====" << endl;
    cout << "1. Search for a word in a file (grep)" << endl;
    cout << "2. Copy a file (cp)" << endl;
    cout << "3. Copy a file and search for a word" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice (1-4): ";
}

// Function to search for a word in a file using grep
void searchWordInFile(const string& fileName, const string& wordToSearch) {
    string fileNameWithExt = fileName + ".txt";
    
    pid_t grep_pid = fork();
    
    if (grep_pid < 0) {
        cerr << "Failed to create child process for grep." << endl;
    } else if (grep_pid == 0) {
        // Child process for grep
        cout << "Searching for pattern: " << wordToSearch << " in file: " << fileNameWithExt << endl;
        cout << "Child process PID: " << getpid() << endl;
        
        // Execute grep command
        execlp("grep", "grep", "-n", wordToSearch.c_str(), fileNameWithExt.c_str(), NULL);
        
        // If execlp fails
        cerr << "Failed to execute grep program." << endl;
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        cout << "Parent process PID: " << getpid() << " waiting for grep to complete..." << endl;
        int status;
        wait(&status);
        
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                cout << "grep found matches with exit status: " << WEXITSTATUS(status) << endl;
            } else if (WEXITSTATUS(status) == 1) {
                cout << "The word '" << wordToSearch << "' was not found in the file '" << fileNameWithExt << "'." << endl;
            } else {
                cout << "grep encountered an error with exit status: " << WEXITSTATUS(status) << endl;
            }
        } else {
            cout << "grep process terminated abnormally." << endl;
        }
    }
}

// Function to copy a file using cp
void copyFile(const string& sourceFile, const string& destFile) {
    string sourceWithExt = sourceFile + ".txt";
    string destWithExt = destFile + ".txt";
    
    pid_t cp_pid = fork();
    
    if (cp_pid < 0) {
        cerr << "Failed to create child process for copy." << endl;
    } else if (cp_pid == 0) {
        // Child process for copy
        cout << "Copying file: " << sourceWithExt << " to " << destWithExt << endl;
        cout << "Child process PID: " << getpid() << endl;
        
        // Execute cp command
        execlp("cp", "cp", sourceWithExt.c_str(), destWithExt.c_str(), NULL);
        
        // If execlp fails
        cerr << "Failed to execute copy program." << endl;
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        cout << "Parent process PID: " << getpid() << " waiting for copy to complete..." << endl;
        int status;
        wait(&status);
        
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                cout << "File has been copied from first to second file successfully!" << endl;
            } else {
                cout << "Copy operation failed with exit status: " << WEXITSTATUS(status) << endl;
            }
        } else {
            cout << "Copy process terminated abnormally." << endl;
        }
    }
}

// Function to copy a file and search for a word in the copied file
void copyAndSearchFile(const string& sourceFile, const string& destFile, const string& wordToSearch) {
    string sourceWithExt = sourceFile + ".txt";
    string destWithExt = destFile + ".txt";
    
    pid_t main_pid = getpid();
    cout << "Main process PID: " << main_pid << endl;
    
    cout << "Source: " << sourceFile << ".txt, Destination: " << destFile << ".txt, Pattern: " << wordToSearch << endl;
    
    // Copy file using child process
    pid_t copy_pid = fork();
    if (copy_pid < 0) {
        cerr << "Failed to create child process for copy." << endl;
        exit(EXIT_FAILURE);
    } else if (copy_pid == 0) {
        // Child process for copy
        cout << "Copying file: " << sourceWithExt << " to " << destWithExt << endl;
        cout << "Child process PID for copy: " << getpid() << endl;
        
        // Execute cp command
        execlp("cp", "cp", sourceWithExt.c_str(), destWithExt.c_str(), NULL);
        
        // If execlp fails
        cerr << "Failed to execute copy program." << endl;
        exit(EXIT_FAILURE);
    } else {
        // Parent process waits for copy to complete
        int copy_status;
        wait(&copy_status);
        
        if (WIFEXITED(copy_status) && WEXITSTATUS(copy_status) == 0) {
            cout << "File has been copied from first to second file successfully!" << endl;
            cout << "------------------------------------------------------------" << endl;
            
            // Search in file using child process
            pid_t grep_pid = fork();
            if (grep_pid < 0) {
                cerr << "Failed to create child process for grep." << endl;
                exit(EXIT_FAILURE);
            } else if (grep_pid == 0) {
                // Child process for grep
                cout << "Searching for pattern: " << wordToSearch << " in file: " << destWithExt << endl;
                cout << "Child process PID for grep: " << getpid() << endl;
                
                // Execute grep command
                execlp("grep", "grep", "-n", wordToSearch.c_str(), destWithExt.c_str(), NULL);
                
                // If execlp fails
                cerr << "Failed to execute grep program." << endl;
                exit(EXIT_FAILURE);
            } else {
                // Parent process waits for grep to complete
                int grep_status;
                wait(&grep_status);
                
                if (WIFEXITED(grep_status)) {
                    if (WEXITSTATUS(grep_status) == 0) {
                        // grep found matches
                        cout << "grep found matches in " << destWithExt << endl;
                    } else if (WEXITSTATUS(grep_status) == 1) {
                        cout << "The word '" << wordToSearch << "' was not found in the file '" << destWithExt << "'." << endl;
                    } else {
                        cout << "grep encountered an error with exit status: " << WEXITSTATUS(grep_status) << endl;
                    }
                } else {
                    cout << "grep process terminated abnormally." << endl;
                }
            }
        } else {
            cout << "Copy operation failed with status: " << WEXITSTATUS(copy_status) << endl;
        }
    }
}

int main() {
    int choice;
    string fileName, sourceFile, destFile, wordToSearch;
    
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                // Search word in file (grep)
                cout << "Enter file name (without extension): ";
                cin >> fileName;
                cout << "Enter word to search: ";
                cin >> wordToSearch;
                
                searchWordInFile(fileName, wordToSearch);
                break;
            }
                
            case 2: {
                // Copy file (cp)
                cout << "Enter source file name (without extension): ";
                cin >> sourceFile;
                cout << "Enter destination file name (without extension): ";
                cin >> destFile;
                
                copyFile(sourceFile, destFile);
                break;
            }
                
            case 3: {
                // Copy and search operation
                cout << "Enter source file name (without extension): ";
                cin >> sourceFile;
                cout << "Enter destination file name (without extension): ";
                cin >> destFile;
                cout << "Enter word to search: ";
                cin >> wordToSearch;
                
                copyAndSearchFile(sourceFile, destFile, wordToSearch);
                break;
            }
                
            case 4:
                cout << "Exiting......" << endl;
                break;
                
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
    } while (choice != 4);
    
    return 0;
}
