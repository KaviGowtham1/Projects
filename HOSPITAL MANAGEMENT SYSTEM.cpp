#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Patient Class
class Patient {
public:
    string name, diagnosis, address;
    int age, id;

    Patient() = default;

    void addPatient() {
        cout << "\nEnter Patient ID: ";
        cin >> id;
        cout << "Enter Patient Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Address: ";
        cin.ignore();
        getline(cin, address);
        cout << "Enter Diagnosis: ";
        getline(cin, diagnosis);
    }

    void displayPatient() const {
        cout << "\nPatient ID: " << id << "\nName: " << name << "\nAge: " << age
             << "\nAddress: " << address << "\nDiagnosis: " << diagnosis << endl;
    }

    void saveToFile() {
        ofstream outFile("patients.txt", ios::app);
        outFile << id << "," << name << "," << age << "," << address << "," << diagnosis << "\n";
        outFile.close();
    }

    static void viewPatients() {
        ifstream inFile("patients.txt");
        string line;
        cout << "\n--- List of Patients ---\n";
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }
};

// Doctor Class
class Doctor {
public:
    string name, specialization;
    int id;

    void addDoctor() {
        cout << "\nEnter Doctor ID: ";
        cin >> id;
        cout << "Enter Doctor Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Specialization: ";
        getline(cin, specialization);
    }

    void displayDoctor() const {
        cout << "\nDoctor ID: " << id << "\nName: " << name << "\nSpecialization: " << specialization << endl;
    }

    void saveToFile() {
        ofstream outFile("doctors.txt", ios::app);
        outFile << id << "," << name << "," << specialization << "\n";
        outFile.close();
    }

    static void viewDoctors() {
        ifstream inFile("doctors.txt");
        string line;
        cout << "\n--- List of Doctors ---\n";
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }
};

// Appointment Class
class Appointment {
public:
    string patientName, doctorName, date;

    void scheduleAppointment() {
        cout << "\nEnter Patient Name: ";
        cin.ignore();
        getline(cin, patientName);
        cout << "Enter Doctor Name: ";
        getline(cin, doctorName);
        cout << "Enter Appointment Date (DD/MM/YYYY): ";
        getline(cin, date);
    }

    void saveToFile() {
        ofstream outFile("appointments.txt", ios::app);
        outFile << patientName << "," << doctorName << "," << date << "\n";
        outFile.close();
    }

    static void viewAppointments() {
        ifstream inFile("appointments.txt");
        string line;
        cout << "\n--- List of Appointments ---\n";
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }
};

// Billing Class
class Billing {
public:
    string patientName;
    double treatmentCost, medicineCost;

    void generateBill() {
        cout << "\nEnter Patient Name: ";
        cin.ignore();
        getline(cin, patientName);
        cout << "Enter Treatment Cost: ";
        cin >> treatmentCost;
        cout << "Enter Medicine Cost: ";
        cin >> medicineCost;

        double total = treatmentCost + medicineCost;
        cout << "\n--- Bill Summary ---\n";
        cout << "Patient: " << patientName << "\nTotal Bill: $" << total << endl;
    }

    void saveToFile() {
        ofstream outFile("bills.txt", ios::app);
        outFile << patientName << "," << treatmentCost << "," << medicineCost << "\n";
        outFile.close();
    }

    static void viewBills() {
        ifstream inFile("bills.txt");
        string line;
        cout << "\n--- List of Bills ---\n";
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }
};

// Main Menu
void showMenu() {
    cout << "\n--- Hospital Management System ---\n";
    cout << "1. Add Patient\n";
    cout << "2. View Patients\n";
    cout << "3. Add Doctor\n";
    cout << "4. View Doctors\n";
    cout << "5. Schedule Appointment\n";
    cout << "6. View Appointments\n";
    cout << "7. Generate Bill\n";
    cout << "8. View Bills\n";
    cout << "9. Exit\n";
}

int main() {
    int choice;
    Patient patient;
    Doctor doctor;
    Appointment appointment;
    Billing billing;

    while (true) {
        showMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            patient.addPatient();
            patient.saveToFile();
            break;
        case 2:
            Patient::viewPatients();
            break;
        case 3:
            doctor.addDoctor();
            doctor.saveToFile();
            break;
        case 4:
            Doctor::viewDoctors();
            break;
        case 5:
            appointment.scheduleAppointment();
            appointment.saveToFile();
            break;
        case 6:
            Appointment::viewAppointments();
            break;
        case 7:
            billing.generateBill();
            billing.saveToFile();
            break;
        case 8:
            Billing::viewBills();
            break;
        case 9:
            cout << "Exiting... Thank you for using the Hospital Management System.\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

