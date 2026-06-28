#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <limits>

using namespace std;

const int MAX_FLIGHTS = 100;
const int MAX_PASSENGERS = 100;

struct Passenger {
    string name;
    string passport;
};

struct Flight {
    int flightNumber;
    string destination;
    string departureTime;
    Passenger passengers[MAX_PASSENGERS];
    int passengerCount;
};

Flight flights[MAX_FLIGHTS];
int flightCount = 0;

bool isValidPassport(const string &passport) {
    return !passport.empty(); // Simple validation: non-empty passport
}

bool isValidFlightNumber(int flightNumber) {
    return flightNumber > 0; // Valid flight number must be a positive integer
}

bool isValidName(const string &name) {
    return !name.empty(); // Valid name must be non-empty
}

void loadFlights() {
    ifstream file("flights.txt");
    if (!file) {
        cout << "\n\t\t\tNo existing flight data found. Starting fresh." << endl;
        return;
    }

    while (file >> flights[flightCount].flightNumber) {
        file.ignore(); // ignore newline character
        getline(file, flights[flightCount].destination);
        getline(file, flights[flightCount].departureTime);
        file >> flights[flightCount].passengerCount;
        file.ignore(); // ignore newline character

        for (int i = 0; i < flights[flightCount].passengerCount; ++i) {
            getline(file, flights[flightCount].passengers[i].name);
            getline(file, flights[flightCount].passengers[i].passport);
        }
        flightCount++;
    }

    file.close();
}

void saveFlights() {
    ofstream file("flights.txt");
    if (!file) {
        cerr << "\n\t\t\tError opening file for writing." << endl;
        return;
    }

    for (int i = 0; i < flightCount; ++i) {
        file << flights[i].flightNumber << endl;
        file << flights[i].destination << endl;
        file << flights[i].departureTime << endl;
        file << flights[i].passengerCount << endl;
        for (int j = 0; j < flights[i].passengerCount; ++j) {
            file << flights[i].passengers[j].name << endl;
            file << flights[i].passengers[j].passport << endl;
        }
    }

    file.close();
}

void createFlight(int flightNumber, const string &destination, const string &departureTime) {
    if (flightCount >= MAX_FLIGHTS) {
        cout << "\n\t\t\tCannot add more flights. Maximum limit reached." << endl;
        return;
    }

    if (!isValidFlightNumber(flightNumber)) {
        cout << "\n\t\t\tInvalid flight number." << endl;
        return;
    }

    if (!isValidName(destination) || !isValidName(departureTime)) {
        cout << "\n\t\t\tInvalid destination or departure time." << endl;
        return;
    }

    flights[flightCount].flightNumber = flightNumber;
    flights[flightCount].destination = destination;
    flights[flightCount].departureTime = departureTime;
    flights[flightCount].passengerCount = 0;
    flightCount++;
    cout << "\n\t\t\tFlight created successfully!" << endl;
    saveFlights();
}

void addPassenger(int flightNumber, const string &name, const string &passport) {
    if (!isValidName(name) || !isValidPassport(passport)) {
        cout << "\n\t\t\tInvalid name or passport." << endl;
        return;
    }

    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            if (flights[i].passengerCount >= MAX_PASSENGERS) {
                cout << "\n\t\t\tCannot add more passengers. Maximum limit reached." << endl;
                return;
            }
            flights[i].passengers[flights[i].passengerCount].name = name;
            flights[i].passengers[flights[i].passengerCount].passport = passport;
            flights[i].passengerCount++;
            cout << "\n\t\t\tPassenger added successfully!" << endl;
            saveFlights();
            return;
        }
    }
    cout << "\n\t\t\tFlight not found." << endl;
}

void viewFlightDetails(int flightNumber) {
    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            cout << "\n\t\t\tFlight Number: " << flights[i].flightNumber << endl;
            cout << "\n\t\t\tDestination: " << flights[i].destination << endl;
            cout << "\n\t\t\tDeparture Time: " << flights[i].departureTime << endl;
            cout << "\n\t\t\tNumber of Passengers: " << flights[i].passengerCount << endl;
            return;
        }
    }
    cout << "\n\t\t\tFlight not found." << endl;
}

void viewPassengerList(int flightNumber) {
    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            cout << "\n\t\t\tPassenger List for Flight " << flights[i].flightNumber << ":" << endl;
            for (int j = 0; j < flights[i].passengerCount; ++j) {
                cout << "\n\t\t\tName: " << flights[i].passengers[j].name
                     << ",\n\t\t\tPassport: " << flights[i].passengers[j].passport << endl;
            }
            return;
        }
    }
    cout << "\n\t\t\tFlight not found." << endl;
}

void removePassenger(int flightNumber, const string &name) {
    if (!isValidName(name)) {
        cout << "\n\t\t\tInvalid name." << endl;
        return;
    }

    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            for (int j = 0; j < flights[i].passengerCount; ++j) {
                if (flights[i].passengers[j].name == name) {
                    for (int k = j; k < flights[i].passengerCount - 1; ++k) {
                        flights[i].passengers[k] = flights[i].passengers[k + 1];
                    }
                    flights[i].passengerCount--;
                    cout << "\n\t\t\tPassenger removed successfully!" << endl;
                    saveFlights();
                    return;
                }
            }
            cout << "\n\t\t\tPassenger not found." << endl;
            return;
        }
    }
    cout << "\n\t\t\tFlight not found." << endl;
}

void searchFlight(int flightNumber) {
    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            cout << "\n\t\t\tFlight found:" << endl;
            cout << "\n\t\t\tFlight Number: " << flights[i].flightNumber << endl;
            cout << "\n\t\t\tDestination: " << flights[i].destination << endl;
            cout << "\n\t\t\tDeparture Time: " << flights[i].departureTime << endl;
            cout << "\n\t\t\tNumber of Passengers: " << flights[i].passengerCount << endl;
            return;
        }
    }
    cout << "\n\t\t\tFlight not found." << endl;
}

void searchPassenger(const string &name) {
    if (!isValidName(name)) {
        cout << "\n\t\t\tInvalid name." << endl;
        return;
    }

    for (int i = 0; i < flightCount; ++i) {
        for (int j = 0; j < flights[i].passengerCount; ++j) {
            if (flights[i].passengers[j].name == name) {
                cout << "\n\t\t\tPassenger found in Flight " << flights[i].flightNumber << ":" << endl;
                cout << "\n\t\t\tName: " << flights[i].passengers[j].name
                     << ",\n\t\t\tPassport: " << flights[i].passengers[j].passport << endl;
                return;
            }
        }
    }
    cout << "\n\t\t\tPassenger not found." << endl;
}

void updateFlightDetails(int flightNumber, const string &newDestination, const string &newDepartureTime) {
    if (!isValidName(newDestination) || !isValidName(newDepartureTime)) {
        cout << "\n\t\t\tInvalid destination or departure time." << endl;
        return;
    }

    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            flights[i].destination = newDestination;
            flights[i].departureTime = newDepartureTime;
            cout << "\n\t\t\tFlight details updated successfully!" << endl;
            saveFlights();
            return;
        }
    }
    cout << "\n\t\t\tFlight not found." << endl;
}

void updatePassengerDetails(int flightNumber, const string &name, const string &newName, const string &newPassport) {
    if (!isValidName(newName) || !isValidPassport(newPassport)) {
        cout << "\n\t\t\tInvalid new name or new passport." << endl;
        return;
    }

    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            for (int j = 0; j < flights[i].passengerCount; ++j) {
                if (flights[i].passengers[j].name == name) {
                    flights[i].passengers[j].name = newName;
                    flights[i].passengers[j].passport = newPassport;
                    cout << "\n\t\t\tPassenger details updated successfully!" << endl;
                    saveFlights();
                    return;
                }
            }
            cout << "\n\t\t\tPassenger not found." << endl;
            return;
        }
    }
    cout << "\n\t\t\tFlight not found." << endl;
}

void listAllFlights() {
    if (flightCount == 0) {
        cout << "\n\t\t\tNo flights available." << endl;
        return;
    }
    for (int i = 0; i < flightCount; ++i) {
        cout << "\n\t\t\tFlight Number: " << flights[i].flightNumber
             << ",\n\t\t\tDestination: " << flights[i].destination
             << ",\n\t\t\tDeparture Time: " << flights[i].departureTime << endl;
    }
}

void listAllPassengers() {
    if (flightCount == 0) {
        cout << "\n\t\t\tNo flights available." << endl;
        return;
    }
    for (int i = 0; i < flightCount; ++i) {
        cout << "\n\t\t\tPassengers in Flight " << flights[i].flightNumber << ":" << endl;
        for (int j = 0; j < flights[i].passengerCount; ++j) {
            cout << "\n\t\t\tName: " << flights[i].passengers[j].name
                 << ",\n\t\t\tPassport: " << flights[i].passengers[j].passport << endl;
        }
    }
}

void countFlights() {
    cout << "\n\t\t\tTotal number of flights: " << flightCount << endl;
}

void countPassengers(int flightNumber) {
    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            cout << "\n\t\t\tTotal number of passengers in Flight " << flightNumber
                 << ": " << flights[i].passengerCount << endl;
            return;
        }
    }
    cout << "\n\t\t\tFlight not found." << endl;
}

void displayAirplaneArt() {
    cout << R"(  
  				_|_
			--o--o--(_)--o--o--
    )" << endl;
}

int main() {
    loadFlights(); // Load existing flights from file

    int choice;
    do {
        cout <<	"\n\t\t=====================================" << endl;
        cout << "        \n\t\t\tFlight Management System      " << endl;
        cout << "\t\t=====================================" << endl;
        cout << "\t\t\t1.  Create a new flight" << endl;
        cout << "\t\t\t2.  Add passengers to a flight" << endl;
        cout << "\t\t\t3.  View flight details" << endl;
        cout << "\t\t\t4.  View passenger list" << endl;
        cout << "\t\t\t5.  Remove a passenger from a flight" << endl;
        cout << "\t\t\t6.  Search for a flight by flight number" << endl;
        cout << "\t\t\t7.  Search for a passenger by name" << endl;
        cout << "\t\t\t8.  Update flight details" << endl;
        cout << "\t\t\t9.  Update passenger details" << endl;
        cout << "\t\t\t10. List all flights" << endl;
        cout << "\t\t\t11. List all passengers across all flights" << endl;
        cout << "\t\t\t12. Count the number of flights" << endl;
        cout << "\t\t\t13. Count the number of passengers in a specific flight" << endl;
        cout << "\t\t\t14. Exit" << endl;
        cout << "\t\t=====================================" << endl;
        cout << "\n\t		Enter your choice: ";

        // Get user choice with input validation
        if (!(cin >> choice)) {
            cout << "\n\t\t\tInvalid input. Please enter a valid integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                int flightNumber;
                string destination, departureTime;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                cin.ignore();
                cout << "\n\t\t\tEnter destination: ";
                getline(cin, destination);
                cout << "\n\t\t\tEnter departure time: ";
                getline(cin, departureTime);
                createFlight(flightNumber, destination, departureTime);
                break;
            }
            case 2: {
                int flightNumber;
                string name, passport;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                cin.ignore();
                cout << "\n\t\t\tEnter passenger name: ";
                getline(cin, name);
                cout << "\n\t\t\tEnter passport number: ";
                getline(cin, passport);
                addPassenger(flightNumber, name, passport);
                break;
            }
            case 3: {
                int flightNumber;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                viewFlightDetails(flightNumber);
                break;
            }
            case 4: {
                int flightNumber;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                viewPassengerList(flightNumber);
                break;
            }
            case 5: {
                int flightNumber;
                string name;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                cin.ignore();
                cout << "\n\t\t\tEnter passenger name: ";
                getline(cin, name);
                removePassenger(flightNumber, name);
                break;
            }
            case 6: {
                int flightNumber;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                searchFlight(flightNumber);
                break;
            }
            case 7: {
                string name;
                cin.ignore();
                cout << "\n\t\t\tEnter passenger name: ";
                getline(cin, name);
                searchPassenger(name);
                break;
            }
            case 8: {
                int flightNumber;
                string destination, departureTime;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                cin.ignore();
                cout << "\n\t\t\tEnter new destination: ";
                getline(cin, destination);
                cout << "\n\t\t\tEnter new departure time: ";
                getline(cin, departureTime);
                updateFlightDetails(flightNumber, destination, departureTime);
                break;
            }
            case 9: {
                int flightNumber;
                string name, newName, newPassport;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                cin.ignore();
                cout << "\n\t\t\tEnter passenger name: ";
                getline(cin, name);
                cout << "\n\t\t\tEnter new name: ";
                getline(cin, newName);
                cout << "\n\t\t\tEnter new passport number: ";
                getline(cin, newPassport);
                updatePassengerDetails(flightNumber, name, newName, newPassport);
                break;
            }
            case 10:
                listAllFlights();
                break;
            case 11:
                listAllPassengers();
                break;
            case 12:
                countFlights();
                break;
            case 13: {
                int flightNumber;
                cout << "\n\t\t\tEnter flight number: ";
                cin >> flightNumber;
                countPassengers(flightNumber);
                break;
            }
            case 14:
                cout << "\n\t\t\tExiting the system." << endl;
                break;
            default:
                cout << "\n\t\t\tInvalid choice. Please try again." << endl;
                break;
        }
        displayAirplaneArt(); // Display airplane art after each choice
    } while (choice != 14);

    return 0;
}
