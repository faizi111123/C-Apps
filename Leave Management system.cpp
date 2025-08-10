    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <map>
    #include <vector>
    #include <iomanip> // for formatting output
    #include <ctime>
    #include <chrono>
    #include <unordered_map>
    #include <unordered_set>
    using namespace std;
    using namespace chrono;

   //Helper function to get current date
   string getCurrentDate() {
    // Get the current time
    std::time_t now = std::time(nullptr);

    // Convert to local time structure
    std::tm* localTime = std::localtime(&now);

    // Format the date as YYYY-MM-DD
    std::ostringstream dateStream;
    dateStream << std::put_time(localTime, "%Y-%m-%d");
    return dateStream.str();
}
    // Function to calculate hours worked between two times
int calculateHoursWorked(const string& timeIn, const string& timeOut) {
    // Assuming times are in HH:MM format
    int inHour = stoi(timeIn.substr(0, 2));
    int inMin = stoi(timeIn.substr(3, 2));
    int outHour = stoi(timeOut.substr(0, 2));
    int outMin = stoi(timeOut.substr(3, 2));

    int totalMinutesIn = inHour * 60 + inMin;
    int totalMinutesOut = outHour * 60 + outMin;

    return (totalMinutesOut - totalMinutesIn) / 60; // Return hours worked
}
// Function to get the week number from a date (YYYY-MM-DD)
int getWeekNumber(const string& date) {
    // Parse the date string
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    // Create a tm struct
    struct tm tmDate = {0};
    tmDate.tm_year = year - 1900; // Year since 1900
    tmDate.tm_mon = month - 1;   // Month (0-11)
    tmDate.tm_mday = day;

    // Convert to time_t and back to tm to populate other fields
    time_t timeSinceEpoch = mktime(&tmDate);
    struct tm* weekTime = localtime(&timeSinceEpoch);

    return weekTime->tm_yday / 7 + 1; // Divide day of the year by 7 to get the week number
}
bool doesEmployeeExist(int id, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int fileID;
        string temp;

        // Assuming the first field in the file is the employee ID
        ss >> fileID;
        if (fileID == id) {
            return true;
        }
    }

    file.close();
    return false;
}
    // IPerson Interface
    class IPerson {
    public:
        virtual void displayPersonDetails() = 0;
    };

    // ILeave Interface
    class ILeave {
    public:
        virtual void applyForLeave() = 0;
        virtual void getApproval() = 0;
    };

    // IAttendance Interface
    class IAttendance {
    public:
        virtual void recordAttendance(string empName,int empID, string date, string timeIn, string timeOut) = 0;
    };

    // IApproval Interface
    class IApproval {
    public:
        virtual void approveLeave() = 0;
    };

    // IReport Interface
    class IReport {
    public:
        virtual void generateReport() = 0;
    };

    // Base Person Class
    class Person : public IPerson {
    protected:
        string name;
        int age;
        string address;
        string contact;

    public:
        Person(string n, int a, string addr, string cont)
            : name(n), age(a), address(addr), contact(cont) {}

        void displayPersonDetails() override {
            cout << "Name: " << name << ", Age: " << age << ", Address: " << address
                << ", Contact: " << contact << endl;
        }
    };

    // Employee Class
    class Employee : public Person, public ILeave, public IAttendance {
    protected:
        int empID;
        string designation;
        double salary;
        string status; // Active/Inactive

    public:
        Employee(string n, int a, string addr, string cont, int id, string des, double sal)
            : Person(n, a, addr, cont), empID(id), designation(des), salary(sal), status("Active") {}

        void applyForLeave() override {
            cout << name << " has applied for leave." << endl;
        }

        void getApproval() override {
            cout << name << "'s leave request is awaiting approval." << endl;
        }
        int getID() const {
            return empID;
        }

        void recordAttendance(string empName,int empid, string date, string timeIn, string timeOut) override {
            cout << "Recording attendance for " << empName << endl;

            // Save attendance data to file
            ofstream inFile("incomingtime.txt", ios::app);
            inFile << empName << "," << date << "," << timeIn << endl;
            inFile.close();

            ofstream outFile("outgoingtime.txt", ios::app);
            outFile << empName << "," << date << "," << timeOut << endl;
            outFile.close();
        }

        void displayEmployeeDetails() {
            displayPersonDetails();
            cout << "Employee ID: " << empID << ", Designation: " << designation
                << ", Salary: " << salary << ", Status: " << status << endl;
        }

        void saveToFile() {
            ofstream file("employees.txt", ios::app);
            file << empID << "," << name << "," << designation << "," << salary << endl;
            file.close();
        }
    };

    // Supervisor Class
    class Supervisor : public Employee, public IApproval {
    public:
            Supervisor(string n, int a, string addr, string cont, int id, string des, double sal)
                : Employee(n, a, addr, cont, id, des, sal) {}

            void approveLeave() override {
        cout << "Supervisor " << name << " is reviewing pending leave requests.\n";

    // Open the file to read the leave requests
    ifstream leaveFile("leaves.txt");
    if (!leaveFile.is_open()) {
        cout << "Failed to open the leave requests file.\n";
        return;
    }

    vector<string> leaveRequests;
    vector<string> pendingLeaves;
    vector<int> pendingIndices;

    string line;
    int index = 0;

    // Read each line from the file and check for pending leave requests
    while (getline(leaveFile, line)) {
        leaveRequests.push_back(line); // Save all lines for later writing back

        if (line.find("Pending") != string::npos) { // Check if the line contains "Pending"
            pendingLeaves.push_back(line); // Save the pending leave request
            pendingIndices.push_back(index); // Save the index in the file
        }

        index++;
    }
    leaveFile.close();

    if (pendingLeaves.empty()) {
        cout << "No pending leave requests found.\n";
        return;
    }

    // Display the pending leave requests to the supervisor
    cout << "\nPending Leave Requests:\n";
    for (size_t i = 0; i < pendingLeaves.size(); i++) {
        cout << i + 1 << ". " << pendingLeaves[i] << "\n";
    }

    // Ask the supervisor which leave to approve
    int choice;
    cout << "Enter the index of the leave to approve (1 to " << pendingLeaves.size() << "): ";
    cin >> choice;

    // Validate the supervisor's choice
    if (choice < 1 || choice > (int)pendingLeaves.size()) {
        cout << "Invalid choice. Operation aborted.\n";
        return;
    }

    int selectedIndex = pendingIndices[choice - 1];

    // Update the selected leave request to Approved
    stringstream ss(pendingLeaves[choice - 1]);
    string leaveData[8];
    for (int i = 0; i < 8; i++) {
        getline(ss, leaveData[i], ',');
    }
    leaveData[4] = "Approved"; // Update status to Approved
    leaveData[5] = name;       // Add supervisor's name as approver

    // Create the updated leave record
    string updatedLeave = leaveData[0] + "," + leaveData[1] + " ," + leaveData[2] + " ," + leaveData[3] + ", Status: " + leaveData[4] + " , Approve by: " + leaveData[5];

    // Replace the original line in leaveRequests with the updated leave
    leaveRequests[selectedIndex] = updatedLeave;

    // Write all leave requests back to leaves.txt
    ofstream outFile("leaves.txt", ios::trunc);
    if (!outFile.is_open()) {
        cout << "Failed to open the leave requests file for updating.\n";
        return;
    }
    for (const auto& leave : leaveRequests) {
        outFile << leave << "\n";
    }
    outFile.close();

    // Write the approved leave request to approvedbysupervisor.txt
    ofstream approvedFile("approvedbysupervisor.txt", ios::app); // Append mode
    if (!approvedFile.is_open()) {
        cout << "Failed to open approvedbysupervisor.txt for writing.\n";
        return;
    }
    approvedFile << updatedLeave << "\n";
    approvedFile.close();

    cout << "Selected leave request has been approved and recorded in approvedbysupervisor.txt.\n";
    }
    };

    // CompanyDirector Class
    class CompanyDirector : public Employee, public IApproval {
    public:
        CompanyDirector(string n, int a, string addr, string cont, int id, string des, double sal)
            : Employee(n, a, addr, cont, id, des, sal) {}

        void approveLeave() override {
              cout << "Director " << name << " is reviewing approved earned leave requests from leaves.txt.\n";

    // Open the file to read leave requests
    ifstream leavesFile("leaves.txt");
    if (!leavesFile.is_open()) {
        cout << "Failed to open leaves.txt.\n";
        return;
    }

    vector<string> allLeaves;
    vector<string> eligibleLeaves;
    vector<int> eligibleIndices;

    string line;
    int index = 0;

    // Read each line and filter for "Earned" leaves with "Approved" status
    while (getline(leavesFile, line)) {
        allLeaves.push_back(line); // Save all lines for later use

        // Check if the line contains "Earned" and "Approved"
        if (line.find("Earned") != string::npos && line.find("Approved") != string::npos && line.find("Alice") != string::npos) {
            eligibleLeaves.push_back(line);
            eligibleIndices.push_back(index); // Save the index for later reference
        }

        index++;
    }
    leavesFile.close();

    if (eligibleLeaves.empty()) {
        cout << "No approved earned leave requests found in leaves.txt.\n";
        return;
    }

    // Display eligible leave requests to the director
    cout << "\nApproved Earned Leave Requests:\n";
    for (size_t i = 0; i < eligibleLeaves.size(); i++) {
        cout << i + 1 << ". " << eligibleLeaves[i] << "\n";
    }

    // Ask the director to choose a leave to approve
    int choice;
    cout << "Enter the index of the leave to approve (1 to " << eligibleLeaves.size() << "): ";
    cin >> choice;

    // Validate director's choice
    if (choice < 1 || choice > (int)eligibleLeaves.size()) {
        cout << "Invalid choice. Operation aborted.\n";
        return;
    }

    int selectedIndex = eligibleIndices[choice - 1];

    // Update the selected leave request for final approval
    stringstream ss(eligibleLeaves[choice - 1]);
    string leaveData[8];
    for (int i = 0; i < 8; i++) {
        getline(ss, leaveData[i], ',');
    }
    leaveData[4] = "Approved"; // Update status to "Final Approved"
    leaveData[5] = name;            // Director's name as the approver

    // Create the updated leave record
    string updatedLeave = leaveData[0] + "," + leaveData[1] + " ," + leaveData[2] + "," + leaveData[3] + ", Status: " + leaveData[4] + " , Approve by: " + leaveData[5];

    // Write the final approved leave to Approvedbydirector.txt
    ofstream directorFile("Approvedbydirector.txt", ios::app); // Append mode
    if (!directorFile.is_open()) {
        cout << "Failed to open Approvedbydirector.txt for writing.\n";
        return;
    }
    directorFile << updatedLeave << "\n";
    directorFile.close();

    // Update the status of the leave in leaves.txt
    ofstream outLeavesFile("leaves.txt", ios::trunc); // Truncate and rewrite leaves.txt
    if (!outLeavesFile.is_open()) {
        cout << "Failed to open leaves.txt for updating.\n";
        return;
    }

    // Rewrite all leaves to the file with updated status for the selected leave
    for (size_t i = 0; i < allLeaves.size(); i++) {
        if (i == (size_t)selectedIndex) {
            outLeavesFile << updatedLeave << "\n";
        } else {
            outLeavesFile << allLeaves[i] << "\n";
        }
    }
    outLeavesFile.close();

    cout << "Earned leave request has been finalized and recorded in Approvedbydirector.txt.\n";
        }
    };
  
   class Leave {
protected:
    int empID;
    string fromDate;
    string toDate;
    string address;
    string reason;
    string dateOfApplication;

public:
    Leave(string empID, string from, string to, string addr, string reas, string doa)
        : empID(stoi(empID)), fromDate(from), toDate(to), address(addr), reason(reas), dateOfApplication(doa) {}

    virtual void applyForLeave() = 0;  // Pure virtual function
    virtual void getApproval() = 0;  // Pure virtual function
};
// CasualLeave Class
class CasualLeave : public Leave {
private:
    static const int maxCasualLeavesPerYear = 15;
    static const int maxCasualLeaveAtATime = 4;
    int daysRequested;
    int totalLeavesAvailable =15;
public:
    CasualLeave(string empID, string from, string to, string addr, string reas, string doa)
        : Leave(empID, from, to, addr, reas, doa) {
        // Initially, load the remaining leaves from the file
        totalLeavesAvailable = loadLeavesFromFile(empID);

        // Calculate the number of days requested
        daysRequested = calculateDays(from, to);
    }

   void applyForLeave() override {
        if (daysRequested > maxCasualLeaveAtATime) {
            cout << "You cannot apply for more than " << maxCasualLeaveAtATime << " casual leaves at a time." << endl;
        } else if (daysRequested <= maxCasualLeavesPerYear) {
            if (daysRequested <= totalLeavesAvailable) {
                // Store leave request in leaves.txt
                saveLeaveToFile();

                // Subtract leave days from total available leaves
                totalLeavesAvailable -= daysRequested;
                cout << "Casual leave applied for " << daysRequested << " days." << endl;
                cout << "Remaining leaves: " << totalLeavesAvailable << " days." << endl;

                // Update the remaining leaves in the file
                updateLeavesInFile();
            } else {
                cout << "You do not have enough leave balance. Total available leave: " << totalLeavesAvailable << " days." << endl;
            }
        } else {
            cout << "You have exceeded your total casual leave limit of " << maxCasualLeavesPerYear << " days for the year." << endl;
        }
    }

    void getApproval() override {
        cout << "Casual leave does not require approval." << endl;
    }

    // Helper function to save leave request to the file
    void saveLeaveToFile() {
        ofstream leavesFile("leaves.txt", ios::app); // Append mode
        if (leavesFile.is_open()) {
            leavesFile << "Employee ID: " << empID
                       << ", Leave Start Date: " << fromDate
                       << ", Leave End Date: " << toDate
                       <<", Leave Type : Casual"
                       << ", Days Requested: " << daysRequested
                       << ", Reason: " << reason
                       << ", Date of Application: " << dateOfApplication
                       <<", Status : Filed"<<endl;
            leavesFile.close();
            cout << "Leave details have been saved to leaves.txt." << endl;
        } else {
            cout << "Error: Could not open leaves.txt file for writing." << endl;
        }
    }

    // Helper function to load available leaves for the employee from the file
    int loadLeavesFromFile(const string& empID) {
        ifstream leavesFile("casualremaining.txt");
        string line;
        int remainingLeaves = 0;
        bool employeeFound = false;

        // Read the file line by line to find the employee's leave balance
        while (getline(leavesFile, line)) {
            stringstream ss(line);
            string id, remainingLeavesStr;
            int leaves;

            ss >> id >> remainingLeavesStr >> leaves;  // assuming format: EMP123 RemainingLeaves 10
            if (id == empID) {
                remainingLeaves = leaves;  // found the employee, update remaining leaves
                employeeFound = true;
                break;
            }
        }
        leavesFile.close();

        if (!employeeFound) {
            // If the employee is not found, return the default leave balance (maxCasualLeavesPerYear)
            remainingLeaves = maxCasualLeavesPerYear;
            addEmployeeToFile(empID, remainingLeaves);
        }

        return remainingLeaves;
    }

    // Helper function to add a new employee entry to casualremaining.txt
    void addEmployeeToFile(const string& empID, int initialLeaves) {
        ofstream leavesFile("casualremaining.txt", ios::app);  // Append mode
        if (leavesFile.is_open()) {
            leavesFile << empID << " RemainingLeaves " << initialLeaves << endl;
            leavesFile.close();
            cout << "New employee added with " << initialLeaves << " leaves." << endl;
        } else {
            cout << "Error: Could not open casualremaining.txt for writing." << endl;
        }
    }

    // Helper function to update the remaining leaves in the file
    void updateLeavesInFile() {
        ifstream leavesFile("casualremaining.txt");
        ofstream tempFile("temp_casualremaining.txt");

        string line;
        bool employeeFound = false;

        // Read the original file and write to a temporary file
        while (getline(leavesFile, line)) {
            stringstream ss(line);
            int id, remainingLeavesStr;
            int leaves;

            ss >> id >> remainingLeavesStr >> leaves;

            if (id == empID) {
                // If we find the employee, update their remaining leaves
                tempFile << id << " RemainingLeaves " << totalLeavesAvailable << endl;
                employeeFound = true;
            } else {
                // Copy other lines as they are
                tempFile << line << endl;
            }
        }

        // If the employee was not found, add them as a new entry
        if (!employeeFound) {
            tempFile << empID << " RemainingLeaves " << totalLeavesAvailable << endl;
        }

        // Close both files
        leavesFile.close();
        tempFile.close();

        // Replace the original file with the temp file
        remove("casualremaining.txt");
        rename("temp_casualremaining.txt", "casualremaining.txt");

        cout << "Remaining leaves updated in casualremaining.txt." << endl;
    }

    int calculateDays(const string& from, const string& to) {
        // Helper function to convert a date string "YYYY-MM-DD" to the number of days since the year 0
        auto convertToDays = [](const string& date) {
            int year, month, day;
            char delimiter;
            stringstream dateStream(date);
            dateStream >> year >> delimiter >> month >> delimiter >> day;

            // Simple formula to calculate the number of days since 01-01-0000
            int daysInYear = (year - 1) * 365; // Approximate, ignoring leap years
            int daysInMonth = 0;

            // Days in months (non-leap year), assuming date format is "YYYY-MM-DD"
            int daysInMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            for (int i = 0; i < month - 1; i++) {
                daysInMonth += daysInMonths[i];
            }

            // Add days for the current month
            daysInMonth += day;

            return daysInYear + daysInMonth;
        };

        int fromDays = convertToDays(from);
        int toDays = convertToDays(to);

        return toDays - fromDays;
    }
};

class EarnedLeave : public Leave {
private:
    static const int maxEarnedLeavesPerYear = 21;
    int daysRequested;
 int totalLeavesAvailable;
public:
    EarnedLeave(string empID, string from, string to, string addr, string reas, string doa)
        : Leave(empID, from, to, addr, reas, doa) {
             totalLeavesAvailable = loadLeavesFromFile(empID);
        daysRequested = calculateDays(from, to);
    }

   void applyForLeave() override {
        if (daysRequested > maxEarnedLeavesPerYear) {
            cout << "You cannot apply for more than " << maxEarnedLeavesPerYear << " earned leaves in a year." << endl;
        } else {
            if (daysRequested <= totalLeavesAvailable) {
                // Store leave request in leaves.txt
                saveLeaveToFile();

                // Subtract leave days from total available leaves
                totalLeavesAvailable -= daysRequested;
                cout << "Earned leave applied for " << daysRequested << " days." << endl;
                cout << "Remaining leaves: " << totalLeavesAvailable << " days." << endl;

                // Update the remaining leaves in the file
                updateLeavesInFile();
            } else {
                cout << "You do not have enough leave balance. Total available leave: " << totalLeavesAvailable << " days." << endl;
            }
        }
    }

    void getApproval() override {
        cout << "Earned leave requires approval from your Supervisor or Director." << endl;
    }

    // Helper function to save leave request to the file
    void saveLeaveToFile() {
        ofstream leavesFile("leaves.txt", ios::app); // Append mode
        if (leavesFile.is_open()) {
            leavesFile << "Employee ID: " << empID
                       << ", Leave Start Date: " << fromDate
                       << ", Leave End Date: " << toDate
                         <<", Leave Type : Earned"
                       << ", Days Requested: " << daysRequested
                       << ", Reason: " << reason
                       << ", Date of Application: " << dateOfApplication
                       <<", Status : Pending"<<endl;
            leavesFile.close();
            cout << "Leave details have been saved to leaves.txt." << endl;
        } else {
            cout << "Error: Could not open leaves.txt file for writing." << endl;
        }
    }

    // Helper function to load available leaves for the employee from the file
    int loadLeavesFromFile(const string& empID) {
        ifstream leavesFile("Earnedremainig.txt");
        string line;
        int remainingLeaves = 0;
        bool employeeFound = false;

        // Read the file line by line to find the employee's leave balance
        while (getline(leavesFile, line)) {
            stringstream ss(line);
            string id, remainingLeavesStr;
            int leaves;

            ss >> id >> remainingLeavesStr >> leaves;  // assuming format: EMP123 RemainingLeaves 10
            if (id == empID) {
                remainingLeaves = leaves;  // found the employee, update remaining leaves
                employeeFound = true;
                break;
            }
        }
        leavesFile.close();

        if (!employeeFound) {
            // If the employee is not found, return the default leave balance (maxEarnedLeavesPerYear)
            remainingLeaves = maxEarnedLeavesPerYear;
            addEmployeeToFile(empID, remainingLeaves);
        }

        return remainingLeaves;
    }

    // Helper function to add a new employee entry to casualremaining.txt
    void addEmployeeToFile(const string& empID, int initialLeaves) {
        ofstream leavesFile("Earnedremainig.txt", ios::app);  // Append mode
        if (leavesFile.is_open()) {
            leavesFile << empID << " RemainingLeaves " << initialLeaves << endl;
            leavesFile.close();
            cout << "New employee added with " << initialLeaves << " leaves." << endl;
        } else {
            cout << "Error: Could not open casualremaining.txt for writing." << endl;
        }
    }

    // Helper function to update the remaining leaves in the file
    void updateLeavesInFile() {
        ifstream leavesFile("Earnedremainig.txt");
        ofstream tempFile("temp_Earnedremainig.txt");

        string line;
        bool employeeFound = false;

        // Read the original file and write to a temporary file
        while (getline(leavesFile, line)) {
            stringstream ss(line);
            int id, remainingLeavesStr;
            int leaves;

            ss >> id >> remainingLeavesStr >> leaves;

            if (id == empID) {
                // If we find the employee, update their remaining leaves
                tempFile << id << " RemainingLeaves " << totalLeavesAvailable << endl;
                employeeFound = true;
            } else {
                // Copy other lines as they are
                tempFile << line << endl;
            }
        }

        // If the employee was not found, add them as a new entry
        if (!employeeFound) {
            tempFile << empID << " RemainingLeaves " << totalLeavesAvailable << endl;
        }

        // Close both files
        leavesFile.close();
        tempFile.close();

        // Replace the original file with the temp file
        remove("Earnedremainig.txt");
        rename("temp_Earnedremainig.txt", "Earnedremainig.txt");

        cout << "Remaining leaves updated in Earnedremainig.txt." << endl;
    }
int calculateDays(const string& from, const string& to) {
        // Helper function to convert a date string "YYYY-MM-DD" to the number of days since the year 0
        auto convertToDays = [](const string& date) {
            int year, month, day;
            char delimiter;
            stringstream dateStream(date);
            dateStream >> year >> delimiter >> month >> delimiter >> day;

            // Simple formula to calculate the number of days since 01-01-0000
            int daysInYear = (year - 1) * 365; // Approximate, ignoring leap years
            int daysInMonth = 0;

            // Days in months (non-leap year), assuming date format is "YYYY-MM-DD"
            int daysInMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            for (int i = 0; i < month - 1; i++) {
                daysInMonth += daysInMonths[i];
            }

            // Add days for the current month
            daysInMonth += day;

            return daysInYear + daysInMonth;
        };

        int fromDays = convertToDays(from);
        int toDays = convertToDays(to);

        return toDays - fromDays;
    }
};

class OfficialLeave : public Leave {
public:
    OfficialLeave(string empID, string from, string addr, string reas, string doa)
        : Leave(empID, from, from, addr, reas, doa) {}

    void applyForLeave() override {
        // Save leave details to leaves.txt
        saveLeaveToFile();
        cout << "Official leave granted for official visits." << endl;
    }

    void getApproval() override {
        cout << "Official leave requires approval from your supervisor." << endl;
    }

private:
    // Helper function to save leave details to leaves.txt
    void saveLeaveToFile() {
        ofstream leavesFile("leaves.txt", ios::app); // Open file in append mode
        if (leavesFile.is_open()) {
            leavesFile << "Employee ID: " << empID
                       << ", Leave Date: " << fromDate
                        <<", Leave Type : Official"
                       << ", Address: " << address
                       << ", Reason: " << reason
                       << ", Date of Application: " << dateOfApplication   
                       <<", Status : Pending"<<endl;
            leavesFile.close();
            cout << "Leave details have been saved to leaves.txt." << endl;
        } else {
            cout << "Error: Could not open leaves.txt file for writing." << endl;
        }
    }
};

class UnpaidLeave : public Leave {
public:
    UnpaidLeave(string empID, string from, string to, string addr, string reas, string doa)
        : Leave(empID, from, to, addr, reas, doa) {}

    void applyForLeave() override {
         saveLeaveToFile();
        cout << "Unpaid leave requested. No salary will be given during this period." << endl;
    }

    void getApproval() override {
        cout << "Unpaid leave requires approval from your supervisor or director." << endl;
    }
    private:
     void saveLeaveToFile() {
        ofstream leavesFile("leaves.txt", ios::app); // Open file in append mode
        if (leavesFile.is_open()) {
            leavesFile << "Employee ID: " << empID
                       << ", Leave Date: " << fromDate
                        <<", Leave Type : Unpaid"
                       << ", Address: " << address
                       << ", Reason: " << reason
                       << ", Date of Application: " << dateOfApplication   
                       <<", Status : Pending"<<endl;
            leavesFile.close();
            cout << "Leave details have been saved to leaves.txt." << endl;
        } else {
            cout << "Error: Could not open leaves.txt file for writing." << endl;
        }
    }
};
    // Report Class
    class Report : public IReport {
    public:
        void generateReport() override {
            int choice;
            cout << "\nGenerate Reports:\n";
            cout << "1. Employees with low attendance\n";
            cout << "2. Employees with outstanding leaves\n";
            cout << "3. Attendance details for an employee\n";
            cout << "4. Leave details for an employee\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                showLowAttendance();
                break;
            case 2:
                showOutstandingLeaves();
                break;
            case 3:
                showAttendanceDetails();
                break;
            case 4:
                showLeaveDetails();
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }

    private:
        void showLowAttendance() {
        ifstream inFile("employee_yearly_attendance.txt");
    if (!inFile.is_open()) {
        cout << "Error: Could not open yearly attendance records file.\n";
        return;
    }

    map<int, int> employeeHours; // Map to store employee ID and their total hours worked
    string line;

    // Reading yearly attendance data
    while (getline(inFile, line)) {
        stringstream ss(line);
        int empID, weekNumber, hours;
        char delimiter;

        // Parse the line in the format: empID,weekNumber,hoursWorked
        ss >> empID >> delimiter >> weekNumber >> delimiter >> hours;
        employeeHours[empID] += hours; // Sum hours for each employee
    }
    inFile.close();

    cout << "Enter the attendance threshold percentage (e.g., 80): ";
    int threshold;
    cin >> threshold;

    cout << "\nEmployees with attendance below " << threshold << "%:\n";
    cout << setw(15) << "Employee ID" << setw(25) << "Total Hours Worked" << setw(20) << "Attendance (%)\n";

    for (const auto& pair : employeeHours) {
        int totalHours = pair.second;
        double percentage = (totalHours / 2080.0) * 100; // Assuming 2080 hours as full-time work for a year
        if (percentage < threshold) {
            cout << setw(15) << pair.first
                 << setw(25) << totalHours
                 << setw(20) << fixed << setprecision(2) << percentage << "%\n";
        }
    }
        }

       void showOutstandingLeaves() {
    ifstream leaveFile("leaves.txt");
    if (!leaveFile.is_open()) {
        cout << "Error: Could not open leave records file.\n";
        return;
    }

    cout << "\nEmployees with outstanding leaves (Pending or Filed):\n";
    cout << setw(10) << "Employee ID" << setw(20) << "Leave Type" << setw(20) << "Status" << "\n";

    string line;
    bool recordFound = false;

    while (getline(leaveFile, line)) {
        stringstream ss(line);
        int empID, daysRequested;
        string leaveType, fromDate, toDate, reason, dateOfApplication, status, approveBy;

        // Parsing the line based on the new format:
        // Employee ID: empID, Leave Start Date: fromDate, Leave End Date: toDate,
        // Leave Type: leaveType, Days Requested: daysRequested, Reason: reason, 
        // Date of Application: dateOfApplication, Status: status, Approve by: approveBy
        ss >> empID >> leaveType >> fromDate >> toDate >> daysRequested >> reason >> dateOfApplication >> status;
cout<<status;
        // Check if the leave status is "Pending" or "Filed"
        if (status == " Pending" || status == " Filed") {
            recordFound = true;
            // Output the employee leave details with leave status
            cout << setw(10) << empID << setw(20) << leaveType << setw(20) << status << "\n";
        }
    }

    leaveFile.close();

    if (!recordFound) {
        cout << "No employees with outstanding leaves found.\n";
    }
}

void showAttendanceDetails() {
    cout << "Enter Employee ID to view attendance details: ";
    int empID;
    cin >> empID;

    ifstream inFile("attendance.txt");
    if (!inFile.is_open()) {
        cout << "Error: Could not open attendance records file.\n";
        return;
    }

    cout << "\nAttendance Details for Employee ID: " << empID << "\n";
    cout << setw(15) << "Employee Name" 
         << setw(15) << "Employee ID" 
         << setw(15) << "Date" 
         << setw(15) << "Hours Worked\n";

    string line;
    bool recordFound = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string empName, date;
        int id, hours;
        char delimiter;

        // Parse the line in the format: empName,empID,date,hoursWorked
        getline(ss, empName, ',');
        ss >> id >> delimiter;
        getline(ss, date, ',');
        ss >> hours;

        if (id == empID) {
            recordFound = true;
            cout << setw(15) << empName 
                 << setw(15) << id 
                 << setw(15) << date 
                 << setw(15) << hours << "\n";
        }
    }
    inFile.close();

    if (!recordFound) {
        cout << "No attendance records found for Employee ID: " << empID << ".\n";
    }
}

void showLeaveDetails() {
    cout << "Enter Employee ID to view leave details: ";
    int empID;
    cin >> empID;

    // Show all leave records from leaves.txt
    ifstream leaveFile("leaves.txt");
    if (!leaveFile.is_open()) {
        cout << "Error: Could not open leave records file.\n";
        return;
    }

    cout << "\nLeave Details for Employee ID: " << empID << "\n";
    cout << setw(15) << "Leave Start Date" << setw(20) << "Leave End Date" 
         << setw(15) << "Leave Type" << setw(15) << "Status" 
         << setw(15) << "Approved By\n";

    string line;
    bool recordFound = false;

    while (getline(leaveFile, line)) {
        stringstream ss(line);
        string temp, leaveStartDate, leaveEndDate, leaveType, status, approvedBy;
        int id;

        // Parse fields assuming updated format
        getline(ss, temp, ','); // Employee ID
        id = stoi(temp.substr(temp.find(":") + 1));
        getline(ss, temp, ','); // Leave Start Date
        leaveStartDate = temp.substr(temp.find(":") + 1);
        getline(ss, temp, ','); // Leave End Date
        leaveEndDate = temp.substr(temp.find(":") + 1);
        getline(ss, temp, ','); // Leave Type
        leaveType = temp.substr(temp.find(":") + 1);
        getline(ss, temp, ','); // Status
        status = temp.substr(temp.find(":") + 1);
        getline(ss, temp); // Approved By
        approvedBy = temp.substr(temp.find(":") + 1);

        if (id == empID) {
            recordFound = true;
            cout << setw(15) << leaveStartDate << setw(20) << leaveEndDate 
                 << setw(15) << leaveType << setw(15) << status 
                 << setw(15) << approvedBy << "\n";
        }
    }
    leaveFile.close();

    if (!recordFound) {
        cout << "No leave records found for Employee ID: " << empID << ".\n";
    }

    // Show remaining casual leaves from casualremaining.txt
    ifstream casualFile("casualremaining.txt");
    if (!casualFile.is_open()) {
        cout << "Error: Could not open casual leave records file.\n";
        return;
    }

    int casualRemaining = -1;
    while (getline(casualFile, line)) {
        stringstream ss(line);
        int id, remaining;
        string temp;
        ss >> id >> temp >> remaining;  // Parse "ID RemainingLeaves Remaining"

        if (id == empID) {
            casualRemaining = remaining;
            break;
        }
    }
    casualFile.close();

    // Show remaining earned leaves from earnedremaining.txt
    ifstream earnedFile("Earnedremainig.txt");
    if (!earnedFile.is_open()) {
        cout << "Error: Could not open earned leave records file.\n";
        return;
    }

    int earnedRemaining = -1;
    while (getline(earnedFile, line)) {
        stringstream ss(line);
        int id, remaining;
        string temp;
        ss >> id >> temp >> remaining;  // Parse "ID RemainingLeaves Remaining"

        if (id == empID) {
            earnedRemaining = remaining;
            break;
        }
    }
    earnedFile.close();

    // Display remaining leaves
    cout << "\nRemaining Leaves for Employee ID: " << empID << "\n";
    cout << setw(20) << "Casual Leaves" << setw(20) << "Earned Leaves\n";
    cout << setw(20) << (casualRemaining == -1 ? "Not Found" : to_string(casualRemaining)) 
         << setw(20) << (earnedRemaining == -1 ? "Not Found" : to_string(earnedRemaining)) << "\n";
}

        
    };

   // Guard Class
class Guard : public Employee, public IAttendance {
public:
    Guard(string n, int a, string addr, string cont, int id = 0, string des = "Guard", double sal = 25000)
        : Employee(n, a, addr, cont, id, des, sal) {}

    string name; // Guard's name
    unordered_map<int, unordered_map<int, int>> empWeeklyHours; // Map to store weekly hours for employees
    unordered_map<int, unordered_set<string>> empAttendanceDates; // Map to track recorded dates per employee

    // Helper function to load weekly hours from the file
    void loadWeeklyHoursFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) return;

        empWeeklyHours.clear();
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            int empID, weekNumber, hoursWorked;
            char delimiter;

            ss >> empID >> delimiter >> weekNumber >> delimiter >> hoursWorked;
            empWeeklyHours[empID][weekNumber] = hoursWorked;
        }

        file.close();
    }

    // Helper function to save weekly hours to the file
    void saveWeeklyHoursToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) return;

        for (const auto& emp : empWeeklyHours) {
            for (const auto& week : emp.second) {
                file << emp.first << "," << week.first << "," << week.second << endl;
            }
        }

        file.close();
    }

    // Helper function to load attendance dates from attendance.txt
    void loadAttendanceDatesFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) return;

        empAttendanceDates.clear();
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string empName, date;
            int empID;
            char delimiter;

            // Parse the line in the format: empName,empID,date,hoursWorked
            getline(ss, empName, ',');
            ss >> empID >> delimiter;
            getline(ss, date, ',');

            empAttendanceDates[empID].insert(date); // Track the date for the employee
        }

        file.close();
    }

    // Function to calculate hours worked between two times
    int calculateHoursWorked(const string& timeIn, const string& timeOut) {
        int inHour = stoi(timeIn.substr(0, 2));
        int inMin = stoi(timeIn.substr(3, 2));
        int outHour = stoi(timeOut.substr(0, 2));
        int outMin = stoi(timeOut.substr(3, 2));

        int totalMinutesIn = inHour * 60 + inMin;
        int totalMinutesOut = outHour * 60 + outMin;

        return (totalMinutesOut - totalMinutesIn) / 60; // Return hours worked
    }

    // Function to get the week number from a date (YYYY-MM-DD)
    int getWeekNumber(const string& date) {
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        struct tm tmDate = {0};
        tmDate.tm_year = year - 1900;
        tmDate.tm_mon = month - 1;
        tmDate.tm_mday = day;

        time_t timeSinceEpoch = mktime(&tmDate);
        struct tm* weekTime = localtime(&timeSinceEpoch);

        return weekTime->tm_yday / 7 + 1;
    }

    // Function to record attendance
    void recordAttendance(string empName, int empID, string date, string timeIn, string timeOut) override {
        cout << "Guard " << name << " is recording attendance for " << empName << " on " << date << "." << endl;

        // Load existing weekly hours from the file
        loadWeeklyHoursFromFile("employee_yearly_attendance.txt");

        // Load existing attendance dates from attendance.txt
        loadAttendanceDatesFromFile("attendance.txt");

        // Check if the employee has already marked attendance for the date
        if (empAttendanceDates[empID].count(date) > 0) {
            cout << "Error: Attendance for employee " << empID << " on " << date << " is already recorded." << endl;
            return;
        }

        // Save incoming time
        ofstream inFile("incomingtime.txt", ios::app);
        inFile << empID << "," << date << "," << timeIn << endl;
        inFile.close();

        // Save outgoing time
        ofstream outFile("outgoingtime.txt", ios::app);
        outFile << empID << "," << date << "," << timeOut << endl;
        outFile.close();

        // Calculate hours worked
        int hoursWorked = calculateHoursWorked(timeIn, timeOut);

        // Calculate the week number
        int weekNumber = getWeekNumber(date);

        // Update the map with the new hours worked
        empWeeklyHours[empID][weekNumber] += hoursWorked;

        // Save the updated weekly hours back to the file
        saveWeeklyHoursToFile("employee_yearly_attendance.txt");

        // Save attendance data to attendance.txt
        ofstream attendanceFile("attendance.txt", ios::app);
        attendanceFile << empName << "," << empID << "," << date << "," << hoursWorked << endl;
        attendanceFile.close();

        // Update empAttendanceDates with the new date
        empAttendanceDates[empID].insert(date);

        cout << "Attendance recorded: " << empName << " worked " << hoursWorked
             << " hours on week " << weekNumber << "." << endl;
    }

    // Function to print weekly hours for debugging
    void printWeeklyHours(int empID) {
        cout << "Weekly hours for employee " << empID << ":" << endl;
        for (const auto& week : empWeeklyHours[empID]) {
            cout << "Week " << week.first << ": " << week.second << " hours" << endl;
        }
    }
    };
    bool doesEmployeeExist(int empID, const vector<Employee>& employees) {
        // Iterate through the list of employees
        for (const auto& emp : employees) {
            // Compare the given empID with each employee's ID
            if (emp.getID() == empID) {
                // If a match is found, return true
                return true;
            }
        }
        // If no match is found, return false
        return false;
    }
    // Main Function
    int main() {
    // Create Guard, Supervisor, and Director objects
    Guard guard("Guard",35, "Security Room", "555-9090");
    Supervisor sup("Supervisor", 40, "456 Street", "555-4567", 2, "Team Lead", 60000);
    CompanyDirector dir("Director", 50, "789 Street", "555-7890", 3, "CEO", 100000);

    vector<Employee> employees = {sup, dir}; // Adding hardcoded Supervisor and Director to the employee list

    int choice;
    string role;

// Main loop to run the menu
while (true) {
    // Ask user for role (Admin, Employee, Supervisor, Director)
    cout << "\nEnter your role (Admin/Employee/Supervisor/Director) or 'Exit' to quit: ";
    cin >> role;

    if (role == "Exit") {
        cout << "Exiting the program...\n";
        break; // Exit the loop if the user chooses 'Exit'
    }

    if (role == "Admin") {
        // Admin Menu: Can add employees
        do {
            cout << "\nAdmin Menu:\n";
            cout << "1. Add Employee\n";
            cout << "2. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                string name, address, phone, position;
    int age, id;
    double salary;

    cout << "Enter employee ID: ";
    cin >> id;

    // Check if the employee already exists
    if (doesEmployeeExist(id, "employees.txt")) {
        cout << "Error: Employee with ID " << id << " already exists in the system." << endl;
        return 0;
    }

    cin.ignore(); // To ignore the newline character left in the input buffer
    cout << "Enter employee name: ";
    getline(cin, name);
    cout << "Enter age: ";
    cin >> age;
    cout << "Enter address: ";
    cin.ignore(); // To ignore the newline character left in the input buffer
    getline(cin, address);
    cout << "Enter phone: ";
    getline(cin, phone);
    cout << "Enter position: ";
    getline(cin, position);
    cout << "Enter salary: ";
    cin >> salary;

    // Create employee object and save it to the list
    Employee emp(name, age, address, phone, id, position, salary);
    employees.push_back(emp);
    emp.saveToFile(); // Save employee data to file
    cout << "Employee added successfully!" << endl;
            }
            case 2:
                cout << "Exiting Admin menu.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 2);

    } else if (role == "Employee") {
        // Employee Menu: Can record attendance, apply for leave, and generate reports
        do {
            cout << "\nEmployee Menu:\n";
            cout << "1. Record Attendance\n";
            cout << "2. Apply for Leave\n";
            cout << "3. Generate Reports\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                string empName, date, timeIn, timeOut;
                int empID;
                cout << "Enter your employee ID: ";
                cin >> empID;

                // Check if the employee exists
                if (!doesEmployeeExist(empID, employees)) {
                    cout << "Employee with ID " << empID << " does not exist.\n";
                    break;
                }
                cout << "Enter  Employee name : ";
                cin.ignore();
                getline(cin, empName);
                cout << "Enter date (YYYY-MM-DD): ";        
                cin >> date;
                cout << "Enter time in (HH:MM AM/PM): ";
                cin.ignore();
                getline(cin, timeIn);
                cout << "Enter time out (HH:MM AM/PM): ";
                getline(cin, timeOut);
                guard.recordAttendance(empName,empID, date, timeIn, timeOut);
                break;
            }
            case 2: {
                            do {
                        cout << "\nEmployee Leave Menu:\n";
                        cout << "1. Apply for Casual Leave\n";
                        cout << "2. Apply for Earned Leave\n";
                        cout << "3. Apply for Official Leave\n";
                        cout << "4. Apply for Unpaid Leave\n";
                        cout << "5. Exit\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        switch (choice) {
                        case 1: {
                            string empID, from, to, addr, reason, doa;
                            cout << "Enter your Employee ID: ";
                            cin >> empID;
                            cout << "Enter leave start date (YYYY-MM-DD): ";
                            cin >> from;
                            cout << "Enter leave end date (YYYY-MM-DD): ";
                            cin >> to;
                            cout << "Enter your address: ";
                            cin.ignore();  // Clear newline
                            getline(cin, addr);
                            cout << "Enter reason for leave: ";
                            getline(cin, reason);
                            doa=getCurrentDate();

                            CasualLeave casualLeave(empID, from, to, addr, reason, doa);
                            casualLeave.applyForLeave();
                            casualLeave.getApproval();
                            break;
                        }
                        case 2: {
                            string empID, from, to, addr, reason, doa;
                            cout << "Enter your Employee ID: ";
                            cin >> empID;
                            cout << "Enter leave start date (YYYY-MM-DD): ";
                            cin >> from;
                            cout << "Enter leave end date (YYYY-MM-DD): ";
                            cin >> to;
                            cout << "Enter your address: ";
                            cin.ignore();  // Clear newline
                            getline(cin, addr);
                            cout << "Enter reason for leave: ";
                            getline(cin, reason);
                             doa=getCurrentDate();
                            EarnedLeave earnedLeave(empID, from, to, addr, reason, doa);
                            earnedLeave.applyForLeave();
                            earnedLeave.getApproval();
                            break;
                        }
                        case 3: {
                            string empID, from, to, addr, reason, doa;
                            cout << "Enter your Employee ID: ";
                            cin >> empID;
                            cout << "Enter leave apply date (YYYY-MM-DD): ";
                            cin >> from;
                            cout << "Enter your address: ";
                            cin.ignore();  // Clear newline
                            getline(cin, addr);
                            cout << "Enter reason for leave: ";
                            getline(cin, reason);
                            doa=getCurrentDate();

                            OfficialLeave officialLeave(empID, from, addr, reason, doa);
                            officialLeave.applyForLeave();
                            officialLeave.getApproval();
                            break;
                        }
                        case 4: {
                            string empID, from, to, addr, reason, doa;
                            cout << "Enter your Employee ID: ";
                            cin >> empID;
                            cout << "Enter leave start date from year (YYYY-MM-DD): ";
                            cin >> from;
                            cout << "Enter leave end date to year (YYYY-MM-DD): ";
                            cin >> to;
                            cout << "Enter address of leave: ";
                            cin.ignore();  // Clear newline
                            getline(cin, addr);
                            cout << "Enter reason for leave: ";
                            getline(cin, reason);
                            doa=getCurrentDate();

                            UnpaidLeave unpaidLeave(empID, from, to, addr, reason, doa);
                            unpaidLeave.applyForLeave();
                            unpaidLeave.getApproval();
                            break;
                        }
                        case 5:
                            cout << "Exiting Employee Leave Menu.\n";
                            break;
                        default:
                            cout << "Invalid option. Please try again.\n";
                            break;
                        }
                    } while (choice != 5);
                    break;
            }
            case 3: {
                Report report;
                report.generateReport();
                break;
            }
            case 4:
                cout << "Exiting Employee Menu.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 4);

    } else if (role == "Supervisor") {
        // Supervisor Menu
        do {
            cout << "\nSupervisor Menu:\n";
            cout << "1. Approve Leave\n";
            cout << "2. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                sup.approveLeave();
                break;
            }
            case 2:
                cout << "Exiting Supervisor menu.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 2);

    } else if (role == "Director") {
        // Director Menu
        do {
            cout << "\nDirector Menu:\n";
            cout << "1. Approve Leave\n";
            cout << "2. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                dir.approveLeave();
                break;
            }
            case 2:
                cout << "Exiting Director menu.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 2);

    } else {
        cout << "Invalid role. Please enter a valid role or type 'Exit' to quit.\n";
    }
}

return 0;

}
