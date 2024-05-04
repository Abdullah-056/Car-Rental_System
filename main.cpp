#include <iostream>		//necessary for standard input and output operations.
#include <mysql.h>		//program to interact with a MySQL database
#include <mysqld_error.h>		//includes the MySQL specific error codes header file
#include <windows.h>		//Windows API header file.
#include <sstream>		//header file for string streams, which is used for string manipulation.
#include <algorithm>  // for std::all_of
#include <cctype>     // for ::isalpha
using namespace std;

const char* HOST = "localhost";		//This is the hostname or IP address of the MySQL server.
const char* USER = "root";		//This is the username for the MySQL connection.
const char* PW = "abdullah123";		//This is the password for the MySQL connection.
const char* DB = "mydb";		//This is the name of the MySQL database to be used.

bool isAvail(MYSQL* conn, string toS) {
    // Constructing a SQL query to check the availability of a car
    string comp = "SELECT Avail FROM cars WHERE Serial = '" + toS + "'";
    // Execute the SQL query
    if (mysql_query(conn, comp.c_str())) {
        // If there's an error in executing the query, print the error and return false
        cout << "Error: " << mysql_error(conn) << endl;
        return false;
    }
    // Store the result of the query
    MYSQL_RES* result = mysql_store_result(conn);
    if (result) {
        // If there is a result set
        MYSQL_ROW row = mysql_fetch_row(result); 
        if (row) {
            // If there is a row in the result set, extract the availability value
            int availability = atoi(row[0]);
            // Free the memory used by the result set
            mysql_free_result(result);
            // Return true if availability is 1, indicating the car is available
            return (availability == 1);
        }
        // Free the memory used by the result set
        mysql_free_result(result);
    }
    // Return false if there is no result set or row, indicating an error or non-availability
    return false;
}
string intToString(int value) {
    stringstream ss;
    ss << value;
    return ss.str();
}
bool isAlpha(const string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isalpha(str[i])) {
            return false;
        }
    }
    return true;
}

// Function to get a valid input for a string that should contain only alphabetic characters
string getValidAlphaInput(const string& prompt) {
    string input;
    do {
        cout << prompt;
        cin >> input;
        if (!isAlpha(input)) {
            cout << "Invalid input! Should contain only alphabetic characters." << endl;
        }
    } while (!isAlpha(input));
    return input;
}
int main() {
    MYSQL* conn;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
        cout << "Error: " << mysql_error(conn) << endl;
    } else {
        cout << "You are Logged into the database" << endl;
    }
    bool exitProgram = false;
    
    int serial[] = {1, 2, 3};
	string brand[] = {"Honda", "Toyota", "Suzuki"};
	string model[] = {"Civic", "Yaris", "Alto"};
	int rent[] = {80, 50, 35};
	
	// Add sample cars to the database
	for (int i = 0; i < 3; ++i) {
    // Check if the Serial No. already exists in the database
    string checkQuery = "SELECT 1, Avail FROM cars WHERE Serial = " + intToString(serial[i]) + " LIMIT 1";
    if (mysql_query(conn, checkQuery.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
    } else {
        MYSQL_RES* result = mysql_store_result(conn);
        if (result) {
            MYSQL_ROW row = mysql_fetch_row(result);
            int numRows = mysql_num_rows(result);
            int availability = (numRows > 0) ? atoi(row[1]) : -1; // -1 if the car doesn't exist

            if (availability == 0) {
                // Car exists but is not available, update availability to 1
                string updateAvailabilityQuery = "UPDATE cars SET Avail = '1' WHERE Serial = " + intToString(serial[i]);
                if (mysql_query(conn, updateAvailabilityQuery.c_str())) {
                    cout << "Error: " << mysql_error(conn) << endl;
                } else {
                    cout << "Availability of Car with Serial No. " << serial[i] << " updated to 1." << endl;
                }
            } else if (availability == -1) {
                // Car doesn't exist, allow adding
                string insertQuery = "INSERT INTO cars (Serial, Brand, Model, Rent, Avail) VALUES (" + intToString(serial[i]) + ", '" + brand[i] + "', '" + model[i] + "', " + intToString(rent[i]) + ", 1)";

                if (mysql_query(conn, insertQuery.c_str())) {
                    cout << "Error: " << mysql_error(conn) << endl;
                } else {
                    cout << "Car with Serial No. " << serial[i] << " added for rent successfully." << endl;
                }
            } else {
                // Car already exists and is available, notify the user
                cout << "Car with Serial No. " << serial[i] << " already exists and is available." << endl;
            }

            mysql_free_result(result);
        }
    }
}
Sleep(2000);
system("cls");

    while (!exitProgram) {
        int userType;
        cout << "Are you a Buyer or a Seller. Enter your choice : " << endl;
        cout << "1. Buyer" << endl;
        cout << "2. Seller" << endl;
        cout << "0. Exit Program" << endl;
        cout << "Enter your choice: ";
        cin >> userType;

        if (userType == 2) {
            // Seller section
            int continueSelling = 1;

            while (continueSelling != 0) {
                int serial;
                cout << "Enter Serial No. of Car for Rent: ";
                cin >> serial;

                // Check if the Serial No. already exists in the database
                string checkQuery = "SELECT 1 FROM cars WHERE Serial = " + intToString(serial) + " LIMIT 1";
                // Execute the MySQL query to check if the serial number exists
                if (mysql_query(conn, checkQuery.c_str())) {
                    cout << "Error: " << mysql_error(conn) << endl;
                } else {
                    // Retrieve the result of the query
		            MYSQL_RES* result = mysql_store_result(conn);
		            // Get the number of rows in the result
		            int numRows = mysql_num_rows(result);
		            // Free the memory used by the result set
            		mysql_free_result(result);
					// Check if the serial number already exists in the database
                    if (numRows > 0) {
                        cout << "Car with Serial No. " << serial << " already exists in the database." << endl;
                    } else {
                        // Car doesn't exist, allow the user to add it
                        string brand, model;
                        int rent;
                        bool avail;

                        // Car doesn't exist, allow the user to add it
						
						// Function to check if a string contains only alphabetic characters
						
						do {
						    cout << "Enter Car Brand: ";
						    cin >> brand;
						
						    // Check if the brand contains only alphabetic characters
						    if (!isAlpha(brand)) {
						        cout << "Invalid input! Car brand should contain only alphabetic characters." << endl;
						    }
						
						} while (!isAlpha(brand));
						
						do {
						    cout << "Enter Car Model: ";
						    cin >> model;
						
						    // Check if the model contains only alphabetic characters
						    if (!isAlpha(model)) {
						        cout << "Invalid input! Car model should contain only alphabetic characters." << endl;
						    }
						
						} while (!isAlpha(model));
						
						// Enter Car Rent
						cout << "Enter Car Rent: ";
						cin >> rent;
						
						// Assuming the car is available when added by the seller
						avail = true;


                        // Insert new car into the database
                        string insertQuery = "INSERT INTO cars (Serial, Brand, Model, Rent, Avail) VALUES (" + intToString(serial) + ", '" + brand + "', '" + model + "', " + intToString(rent) + ", " + intToString(avail) + ")";
                        if (mysql_query(conn, insertQuery.c_str())) {
                            cout << "Error: " << mysql_error(conn) << endl;
                        } else {
                            cout << "Car with Serial No. " << serial<< " "<<brand<<" and "<<model<< " added for rent successfully." << endl;
                        }
                    }

                    // Prompt the user to exit or continue selling cars
                    cout << "If you have changed your mind, click 0 to exit, otherwise, click 1 to continue selling: ";
                    cin >> continueSelling;
                }
            }
        } else if (userType == 1) {
            // Buyer section
            bool exit = false;
            while (!exit) {
                system("cls");
                cout << "Welcome to Car Rental Service" << endl;
                cout << "******************************" << endl;
                cout << "List of Cars:" << endl;
                string listQuery = "SELECT Serial, Brand, Model, Rent FROM cars WHERE Avail = '1'";
                if (mysql_query(conn, listQuery.c_str())) {
                    cout << "Error: " << mysql_error(conn) << endl;
                } else {
                    MYSQL_RES* result = mysql_store_result(conn);
                    if (result) {
                        int num = mysql_num_fields(result);
                        MYSQL_ROW row;
                        while ((row = mysql_fetch_row(result))) {
                            for (int i = 0; i < num; i++) {
                                cout << " " << row[i];
                            }
                            cout << endl;
                        }
                        mysql_free_result(result);
                    }
                }

                int val;
                cout << "\n1. Select a Car " << endl;
                cout << "0. Exit " << endl;
                cout << "Enter your choice = ";
                cin >> val;

                if (val == 1) {
                    // Code for listing available cars

                    int selectedSerial;
                    cout << "Enter Serial No. of Car to Rent: ";
                    cin >> selectedSerial;

                    // Check if the selected Serial No. exists and the car is available
                    string checkAvailabilityQuery = "SELECT 1 FROM cars WHERE Serial = " + intToString(selectedSerial) + " AND Avail = '1' LIMIT 1";
                    if (mysql_query(conn, checkAvailabilityQuery.c_str())) {
                        cout << "Error: " << mysql_error(conn) << endl;
                    } else {
                        MYSQL_RES* availabilityResult = mysql_store_result(conn);
                        int numRows = mysql_num_rows(availabilityResult);
                        mysql_free_result(availabilityResult);

                        if (numRows > 0) {
                            // Update the database to mark the selected car as booked
                            string updateAvailabilityQuery = "UPDATE cars SET Avail = '0' WHERE Serial = " + intToString(selectedSerial);
                            if (mysql_query(conn, updateAvailabilityQuery.c_str())) {
                                cout << "Error: " << mysql_error(conn) << endl;
                            } else {
                                cout << "You have selected the car with Serial No. " << selectedSerial << " for rent." << endl;
                                
                            }
                        } else {
                            cout << "Sorry, the selected car is not available." << endl;
                        }
                        Sleep(3000);
                    }
                } else if (val == 0) {
                    exit = true;
                    cout << "You are redirected to Main Page " << endl;
                    Sleep(2000);
                }
            }
        } else if (userType == 0) {
            exitProgram = true;
        } else {
            cout << "Invalid Choice. Enter 0 if you want to exit" << endl;
        }
    }
	string deleteQuery = "DELETE FROM cars";
    if (mysql_query(conn, deleteQuery.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
    } else {
        cout << "All records deleted before program termination." << endl;
    }
    mysql_close(conn);
    return 0;
}

