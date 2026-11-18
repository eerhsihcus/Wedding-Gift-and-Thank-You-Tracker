#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

using namespace std;

const char* FILE_NAME = "wedding_registry.dat";

class Gift {
private:
    int registryID;
    char guestName[50];
    char giftDescription[50];
    char category[30];
    double estimatedValue;
    bool thankYouSent;

    static int nextID;

public:
    // 1. Default Constructor
    Gift() {
        registryID = 0;
        strcpy(guestName, "");
        strcpy(giftDescription, "");
        strcpy(category, "");
        estimatedValue = 0.0;
        thankYouSent = false;
    }

    // 2. Parameterized Constructor
    Gift(const char* name, const char* desc, const char* cat, double val, bool status) {
        registryID = nextID++;
        strcpy(guestName, name);
        strcpy(giftDescription, desc);
        strcpy(category, cat);
        estimatedValue = val;
        thankYouSent = status;
    }

    // 3. Copy Constructor
    Gift(const Gift &g) {
        registryID = g.registryID;
        strcpy(guestName, g.guestName);
        strcpy(giftDescription, g.giftDescription);
        strcpy(category, g.category);
        estimatedValue = g.estimatedValue;
        thankYouSent = g.thankYouSent;
    }

    // 4. Destructor
    ~Gift() {
        // Destructor called automatically when object goes out of scope
    }

    // Inline Getters & Setters
    inline int getID() const { return registryID; }
    inline double getValue() const { return estimatedValue; }
    inline bool isSent() const { return thankYouSent; }
    inline void setStatus(bool status) { thankYouSent = status; }

    // Sync static nextID with existing file records on restart
    static void syncID() {
        ifstream fin(FILE_NAME, ios::binary);
        if (!fin) return;
        Gift temp;
        int maxID = 2000;
        while (fin.read((char*)&temp, sizeof(Gift))) {
            if (temp.registryID > maxID) {
                maxID = temp.registryID;
            }
        }
        nextID = maxID + 1;
        fin.close();
    }

    // Display record
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

    friend void summaryReport();
};

int Gift::nextID = 2001;

// 1. Add Gift & Append to Binary File
void addGift() {
    char name[50], desc[50], cat[30];
    double val;
    int status;

    cin.ignore();
    cout << "Enter Guest Name: ";
    cin.getline(name, 50);
    if (strlen(name) == 0) {
        cout << "Guest name cannot be empty.\n";
        return;
    }

    cout << "Enter Gift Description: ";
    cin.getline(desc, 50);
    cout << "Enter Category: ";
    cin.getline(cat, 30);
    cout << "Enter Estimated Value ($): ";
    cin >> val;
    if (val <= 0) {
        cout << "Value must be positive.\n";
        return;
    }
    cout << "Thank You Sent? (1 for Yes, 0 for No): ";
    cin >> status;

    // Parameterized constructor creates the object
    Gift g(name, desc, cat, val, (status == 1));

    // Write fixed-size binary record to file
    ofstream fout(FILE_NAME, ios::binary | ios::app);
    fout.write((char*)&g, sizeof(Gift));
    fout.close();

    cout << "Gift saved permanently! Assigned ID: " << g.getID() << endl;
}

// 2. View All Gifts from Binary File
void viewAllGifts() {
    ifstream fin(FILE_NAME, ios::binary);
    if (!fin) {
        cout << "\nNo records found. File does not exist yet.\n";
        return;
    }

    Gift g;
    bool hasRecords = false;
    while (fin.read((char*)&g, sizeof(Gift))) {
        g.display();
        hasRecords = true;
    }
    fin.close();

    if (!hasRecords) cout << "\nNo gifts recorded yet.\n";
}

// 3. Random Access Search using seekg
void searchGiftByID() {
    int searchID;
    cout << "Enter Registry ID to search: ";
    cin >> searchID;

    ifstream fin(FILE_NAME, ios::binary);
    if (!fin) {
        cout << "No records found.\n";
        return;
    }

    int recordIndex = searchID - 2001;
    if (recordIndex >= 0) {
        fin.seekg(recordIndex * sizeof(Gift), ios::beg);
        Gift g;
        if (fin.read((char*)&g, sizeof(Gift)) && g.getID() == searchID) {
            cout << "\n[Gift Found via seekg]";
            g.display();
            fin.close();
            return;
        }
    }

    fin.close();
    cout << "Gift ID " << searchID << " not found.\n";
}

// 4. Random Access Update using seekp
void updateThankYouStatus() {
    int searchID;
    cout << "Enter Registry ID to update Thank You status: ";
    cin >> searchID;

    fstream file(FILE_NAME, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "No records found.\n";
        return;
    }

    Gift g;
    bool found = false;
    while (file) {
        long pos = file.tellg();
        if (!file.read((char*)&g, sizeof(Gift))) break;

        if (g.getID() == searchID) {
            int newStatus;
            cout << "Current Status: " << (g.isSent() ? "YES" : "NO") << endl;
            cout << "Enter New Status (1 for Yes, 0 for No): ";
            cin >> newStatus;

            g.setStatus(newStatus == 1);

            file.seekp(pos, ios::beg);
            file.write((char*)&g, sizeof(Gift));
            cout << "Record updated in file successfully!\n";
            found = true;
            break;
        }
    }
    file.close();

    if (!found) cout << "Gift ID not found.\n";
}

// 5. Friend Function Summary Report from File
void summaryReport() {
    ifstream fin(FILE_NAME, ios::binary);
    if (!fin) {
        cout << "\nNo records available for summary.\n";
        return;
    }

    Gift g;
    int count = 0, sentCount = 0;
    double totalVal = 0.0;

    while (fin.read((char*)&g, sizeof(Gift))) {
        count++;
        totalVal += g.estimatedValue;
        if (g.thankYouSent) sentCount++;
    }
    fin.close();

    cout << "\n===== PERSISTENT SUMMARY REPORT =====";
    cout << "\nTotal Gifts Registered : " << count;
    cout << "\nTotal Estimated Value  : $" << fixed << setprecision(2) << totalVal;
    cout << "\nThank You Notes Sent   : " << sentCount;
    cout << "\nThank You Notes Pending: " << (count - sentCount);
    cout << "\n=====================================\n";
}

int main() {
    Gift::syncID();
    int choice;

    do {
        cout << "\n--- WEDDING GIFT REGISTRY (CA2 - PERSISTENT) ---\n";
        cout << "1. Add New Gift\n";
        cout << "2. View All Gifts\n";
        cout << "3. Search Gift by ID (seekg)\n";
        cout << "4. Update Thank You Status (seekp)\n";
        cout << "5. Summary Report\n";
        cout << "6. Exit\n";
        cout << "Enter choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1: addGift(); break;
            case 2: viewAllGifts(); break;
            case 3: searchGiftByID(); break;
            case 4: updateThankYouStatus(); break;
            case 5: summaryReport(); break;
            case 6: cout << "Exiting. All data saved in " << FILE_NAME << ".\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}