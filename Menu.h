#include <iostream>
#include <ctime> // Include <ctime> for seeding the random number generator

using namespace std;
// Prototypes for Functions.
int menu();                          // Function to ask for Data set to use out of the three given.
int choose_from_SNAP();             // Function to ask for Data set out of the SNAP by Stanford
int choose_from_Doctor_Who();       // Function to ask for Data set out of Doctor-Who.

int choose_from_SNAP()
{
    int choice;
    cout << "Select the Dataset from SNAP by Stanford" << endl;
    cout << "1. email-Eron.txt" << endl;
    cout << "2. email-EuAll.txt" << endl;
    cin >> choice;
    while (choice != 1 && choice != 2)
    {
        cout << "Please Enter a valid option" << endl;
        choice = menu();
    }
    return choice;
}

int choose_from_Doctor_Who()
{
     int choice;
    cout << "Select the Dataset from Doctor-Who by Manuel Dileo" << endl;
    cout << "1. classic-who.csv" << endl;
    cout << "2. doctorwho.csv" << endl;
    cout << "3. new-who.csv" << endl;
    cin >> choice;
    while (choice != 1 && choice != 2  && choice != 3)
    {
        cout << "Please Enter a valid option" << endl;
        choice = menu();
    }
    return choice;
    
}

int menu()
{
    int choice;
    cout << "Select the Dataset to Use" << endl;
    cout << "1. SNAP by Stanford" << endl;
    cout << "2. Doctor-Who by Manuel Dileo" << endl;
    cin >> choice;
    while (choice != 1 && choice != 2)
    {
        cout << "Please Enter a valid option" << endl;
        choice = menu();
    }
    return choice;
}


