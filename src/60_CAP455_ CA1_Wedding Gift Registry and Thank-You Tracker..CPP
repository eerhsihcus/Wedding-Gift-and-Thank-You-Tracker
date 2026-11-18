#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_GIFTS = 50;

class Gift {
private:
    int registryID;
    string guestName;
    string giftDescription;
    string category;
    double estimatedValue;
    bool thankYouSent;

    static int nextID; // Static member for auto unique IDs

public:
    // Inline Getter Function
    inline int getID() const {
        return registryID;
    }

    // Set details using 'this' pointer
    void setDetails(string name, string desc, string cat, double val, bool status) {
        this->registryID = nextID++;
        this->guestName = name;
        this->giftDescription = desc;
        this->category = cat;
        this->estimatedValue = val;
        this->thankYouSent = status;
    }

    // Display individual record
    void display() const {
        cout << "\n----------------------------------------";
        cout << "\nRegistry ID      : " << registryID;
        cout << "\nGuest Name       : " << guestName;
        cout << "\nGift Description : " << giftDescription;
        cout << "\nCategory         : " << category;
        cout << "\nEstimated Value  : $" << fixed << setprecision(2) << estimatedValue;
        cout << "\nThank You Sent   : " << (thankYouSent ? "YES" : "NO");
        cout << "\n----------------------------------------" << endl;
    }

    // Friend function declaration for summary report
    friend void summaryReport(const Gift registry[], int count);
};

// Initialize static member outside class
int Gift::nextID = 2001;

// Friend Function Definition
void summaryReport(const Gift registry[], int count) {
    if (count == 0) {
        cout << "\nNo gift records found.\n";
        return;
    }

    double totalVal = 0.0;
    int sentCount = 0;

    for (int i = 0; i < count; i++) {
        totalVal += registry[i].estimatedValue; // Direct access to private member
        if (registry[i].thankYouSent) sentCount++;
    }

    cout << "\n===== SUMMARY REPORT =====";
    cout << "\nTotal Gifts Registered : " << count;
    cout << "\nTotal Estimated Value  : $" << fixed << setprecision(2) << totalVal;
    cout << "\nThank You Notes Sent   : " << sentCount;
    cout << "\nThank You Notes Pending: " << (count - sentCount);
    cout << "\n==========================\n";
}

int main() {
    Gift registry[MAX_GIFTS]; // Array of objects
    int count = 0;
    int choice;

    do {
        cout << "\n--- WEDDING GIFT REGISTRY MENU ---\n";
        cout << "1. Add New Gift\n";
        cout << "2. View All Gifts\n";
        cout << "3. Search Gift by ID\n";
        cout << "4. Summary Report\n";
        cout << "5. Exit\n";
        cout << "Enter choice (1-5): ";
        cin >> choice;
        cin.ignore(); // Clear newline buffer

        if (choice == 1) {
            if (count >= MAX_GIFTS) {
                cout << "Registry is full!\n";
                continue;
            }

            string name, desc, cat;
            double val;
            int status;

            cout << "Enter Guest Name: ";
            getline(cin, name);
            cout << "Enter Gift Description: ";
            getline(cin, desc);
            cout << "Enter Category: ";
            getline(cin, cat);
            cout << "Enter Estimated Value ($): ";
            cin >> val;
            cout << "Thank You Sent? (1 for Yes, 0 for No): ";
            cin >> status;

            registry[count].setDetails(name, desc, cat, val, status);
            cout << "Gift added successfully! Assigned ID: " << registry[count].getID() << endl;
            count++;

        } else if (choice == 2) {
            if (count == 0) {
                cout << "No gifts recorded yet.\n";
            } else {
                Gift *ptr = registry; // Pointer to array
                for (int i = 0; i < count; i++) {
                    (ptr + i)->display(); // Pointer arithmetic traversal
                }
            }

        } else if (choice == 3) {
            int searchID;
            cout << "Enter Registry ID to search: ";
            cin >> searchID;

            bool found = false;
            Gift *ptr = registry;
            for (int i = 0; i < count; i++) {
                if ((ptr + i)->getID() == searchID) {
                    cout << "\n[Gift Found]";
                    (ptr + i)->display();
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Gift ID not found.\n";

        } else if (choice == 4) {
            summaryReport(registry, count); // Call friend function

        } else if (choice == 5) {
            cout << "Exiting program. Goodbye!\n";
        } else {
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}