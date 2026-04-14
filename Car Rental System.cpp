#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int MAX_CARS = 10;

string cars[MAX_CARS][4];
int rentalDays[MAX_CARS];
int totalCars = 0;
int totalIncome = 0;

void loadCars() {
    ifstream f("cars.txt");
    if (!f) return;
    totalCars = 0;
    while (f >> cars[totalCars][0]) {
        f.ignore();
        getline(f, cars[totalCars][1]);
        f >> cars[totalCars][2] >> cars[totalCars][3];
        rentalDays[totalCars] = 0;
        totalCars++;
    }
    f.close();	
}

void saveCars() {
    ofstream f("cars.txt");
    for (int i = 0; i < totalCars; i++) {
        f << cars[i][0] << "\n"
          << cars[i][1] << "\n"
          << cars[i][2] << "\n"
          << cars[i][3] << "\n";
    }
    f.close();
}

void showAllCars() {
    cout << "\n--- All Cars ---\n";
    for (int i = 0; i < totalCars; i++) {
        cout << "ID: " << cars[i][0]
             << ", Model: " << cars[i][1]
             << ", Rate: $" << cars[i][2]
             << ", Status: " << (cars[i][3] == "no" ? "Available" : "Rented")
             << endl;
    }
}

void showAvailableCars() {
    cout << "\n--- Available Cars ---\n";
    bool found = false;
    for (int i = 0; i < totalCars; i++) {
        if (cars[i][3] == "no") {
            cout << "ID: " << cars[i][0]
                 << ", Model: " << cars[i][1]
                 << ", Rate: $" << cars[i][2] << endl;
            found = true;
        }
    }
    if (!found) cout << "No cars are currently available.\n";
}

void rentCar(int car_id, int days) {
    for (int i = 0; i < totalCars; i++) {
        if (stoi(cars[i][0]) == car_id) {
            if (cars[i][3] == "no") {
                cars[i][3] = "yes";
                rentalDays[i] = days;
                cout << "Car " << cars[i][1]
                     << " rented for " << days << " days.\n";
                saveCars();
                return;
            } else {
                cout << "Car is already rented.\n";
                return;
            }
        }
    }
    cout << "Car not found.\n";
}

void returnCar(int car_id) {
    for (int i = 0; i < totalCars; i++) {
        if (stoi(cars[i][0]) == car_id) {
            if (cars[i][3] == "yes") {
                cars[i][3] = "no";
                int cost = stoi(cars[i][2]) * rentalDays[i];
                cout << "Car returned: " << cars[i][1] << endl;
                cout << "Total cost: $" << cost << endl;
                totalIncome += cost;
                rentalDays[i] = 0;
                saveCars();
                return;
            } else {
                cout << "Car was not rented.\n";
                return;
            }
        }
    }
    cout << "Car not found.\n";
}

void addCar() {
    if (totalCars >= MAX_CARS) {
        cout << "Car storage full.\n";
        return;
    }

    string id, model, rate;
    bool idExists;

    do {
        idExists = false;
        cout << "Enter Car ID: ";
        cin >> id;

        for (int i = 0; i < totalCars; i++) {
            if (cars[i][0] == id) {
                cout << "Error: This ID is already assigned.\n";
                idExists = true;
                break;
            }
        }
    } while (idExists);

    cin.ignore();
    cout << "Enter Model Name: ";
    getline(cin, model);
    cout << "Enter Daily Rate: ";
    cin >> rate;

    cars[totalCars][0] = id;
    cars[totalCars][1] = model;
    cars[totalCars][2] = rate;
    cars[totalCars][3] = "no";

    totalCars++;
    saveCars();

    cout << "Car added successfully.\n";
}

void removeCar(int car_id) {
    for (int i = 0; i < totalCars; i++) {
        if (stoi(cars[i][0]) == car_id) {

            if (cars[i][3] == "yes") {
                cout << "Cannot remove a rented car.\n";
                return;
            }

            for (int j = i; j < totalCars - 1; j++) {
                for (int k = 0; k < 4; k++)
                    cars[j][k] = cars[j + 1][k];
                rentalDays[j] = rentalDays[j + 1];
            }

            totalCars--;
            saveCars();
            cout << "Car removed successfully.\n";
            return;
        }
    }
    cout << "Car not found.\n";
}

void searchCar(string keyword) {
    cout << "\n--- Search Results ---\n";
    bool found = false;

    for (int i = 0; i < totalCars; i++) {
        if (cars[i][1].find(keyword) != string::npos) {
            cout << "ID: " << cars[i][0]
                 << ", Model: " << cars[i][1]
                 << ", Rate: $" << cars[i][2]
                 << ", Status: " << (cars[i][3] == "no" ? "Available" : "Rented")
                 << endl;
            found = true;
        }
    }

    if (!found) cout << "No matching car found.\n";
}

void showIncome() {
    cout << "\nTotal Rental Income: $" << totalIncome << endl;
}

int main() {
    loadCars();

    int choice, car_id, days;
    string keyword;

    if (totalCars == 0) {
        cars[0][0] = "1"; cars[0][1] = "Toyota Corolla"; cars[0][2] = "50"; cars[0][3] = "no";
        cars[1][0] = "2"; cars[1][1] = "Honda Civic"; cars[1][2] = "60"; cars[1][3] = "no";
        cars[2][0] = "3"; cars[2][1] = "Ford Mustang"; cars[2][2] = "100"; cars[2][3] = "no";
        totalCars = 3;
        saveCars();
    }

    do {
        cout << "\n===== Car Rental System =====\n";
        cout << "1. Show All Cars\n";
        cout << "2. Show Available Cars\n";
        cout << "3. Rent a Car\n";
        cout << "4. Return a Car\n";
        cout << "5. Add a New Car\n";
        cout << "6. Remove a Car\n";
        cout << "7. Search Car by Model\n";
        cout << "8. Show Total Income\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: showAllCars(); break;
            case 2: showAvailableCars(); break;
            case 3:
                cout << "Enter Car ID: ";
                cin >> car_id;
                cout << "Enter Days: ";
                cin >> days;
                rentCar(car_id, days);
                break;

            case 4:
                cout << "Enter Car ID: ";
                cin >> car_id;
                returnCar(car_id);
                break;

            case 5: addCar(); break;

            case 6:
                cout << "Enter Car ID to remove: ";
                cin >> car_id;
                removeCar(car_id);
                break;

            case 7:
                cout << "Enter model keyword: ";
                cin.ignore();
                getline(cin, keyword);
                searchCar(keyword);
                break;

            case 8: showIncome(); break;

            case 9:
                cout << "Exiting system...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 9);

    return 0;
}