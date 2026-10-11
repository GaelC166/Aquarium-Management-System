#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <cctype>
using namespace std;

struct Food {
    string type;
    double costPerPound;
};

int sizeRank(const string& size) {
    if (size == "S") return 1;
    if (size == "M") return 2;
    if (size == "L") return 3;
    if (size == "XL") return 4;
    return 0;
}

class Fish {
private:
    string name;
    string size;
    string diet;
    string description;
    int population;
public:
    Fish(string n, string s, string d, string desc, int p)
        : name(n), size(s), diet(d), description(desc), population(p < 0 ? 0 : p) {}
    string getName() const { return name; }
    string getSize() const { return size; }
    string getDiet() const { return diet; }
    string getDescription() const { return description; }
    int getPopulation() const { return population; }
    void setPopulation(int p) { population = p < 0 ? 0 : p; }
    void changePopulation(int amount) { setPopulation(population + amount); }
    void printSummary() const {
        string label = size == "S" ? "Small" : size == "M" ? "Medium" : size == "L" ? "Large" : "Extra Large";
        cout << name << " | " << label << " | " << diet << " | Population: " << population
             << " | " << description << '\n';
    }
};

class Tank {
protected:
    string name;
    int maxOccupancy;
    Food meat;
    Food plants;
    vector<Fish> fish;
public:
    Tank(string n, int capacity, double meatPrice = 5.0, double plantPrice = 2.5)
        : name(n), maxOccupancy(capacity), meat{"Meat", meatPrice}, plants{"Plants", plantPrice} {}
    virtual ~Tank() = default;
    string getName() const { return name; }
    int totalPopulation() const {
        int total = 0;
        for (const Fish& f : fish) total += f.getPopulation();
        return total;
    }
    virtual bool populateTank(const Fish& incoming) {
        if (incoming.getPopulation() <= 0 || totalPopulation() + incoming.getPopulation() > maxOccupancy) return false;
        for (Fish& f : fish) {
            if (f.getName() == incoming.getName() && f.getSize() == incoming.getSize() &&
                f.getDiet() == incoming.getDiet() && f.getDescription() == incoming.getDescription()) {
                f.changePopulation(incoming.getPopulation());
                return true;
            }
        }
        fish.push_back(incoming);
        return true;
    }
    void printTankPop() const {
        cout << "\n" << name << " (" << totalPopulation() << "/" << maxOccupancy << ")\n";
        if (fish.empty()) cout << "No fish in this tank.\n";
        for (size_t i = 0; i < fish.size(); ++i) {
            cout << i + 1 << ". ";
            fish[i].printSummary();
        }
    }
    void printFoodCost() const {
        double daily = 0;
        for (const Fish& f : fish) {
            double price = f.getDiet() == "Meat" ? meat.costPerPound : plants.costPerPound;
            daily += f.getPopulation() * 0.1 * price;
        }
        cout << name << " estimated daily feeding cost: $" << fixed << setprecision(2) << daily << '\n';
    }
    bool adjustFoodCost(double meatPrice, double plantPrice) {
        if (meatPrice < 0 || plantPrice < 0) return false;
        meat.costPerPound = meatPrice;
        plants.costPerPound = plantPrice;
        return true;
    }
    bool removeFish(int index, int count) {
        if (index < 0 || index >= static_cast<int>(fish.size()) || count <= 0 || count > fish[index].getPopulation()) return false;
        fish[index].changePopulation(-count);
        if (fish[index].getPopulation() == 0) fish.erase(fish.begin() + index);
        return true;
    }
    bool removeFish(int index) {
        if (index < 0 || index >= static_cast<int>(fish.size())) return false;
        fish.erase(fish.begin() + index);
        return true;
    }
    friend bool transferFish(Tank& source, Tank& destination, int index, int count);
};

class SpecialTank : public Tank {
private:
    string sizeLimit;
public:
    SpecialTank(string n, int capacity, string limit)
        : Tank(n, capacity, 5.5, 2.5), sizeLimit(limit) {}
    bool populateTank(const Fish& incoming) override {
        if (sizeRank(incoming.getSize()) > sizeRank(sizeLimit)) return false;
        return Tank::populateTank(incoming);
    }
};

class HoldingTank : public Tank {
public:
    HoldingTank(string n, int capacity) : Tank(n, capacity, 4.5, 2.0) {}
    bool receiveIncomingFish(const Fish& incoming) { return populateTank(incoming); }
    using Tank::removeFish;
};

bool transferFish(Tank& source, Tank& destination, int index, int count) {
    if (&source == &destination || index < 0 || index >= static_cast<int>(source.fish.size()) ||
        count <= 0 || count > source.fish[index].getPopulation()) return false;
    Fish selected = source.fish[index];
    selected.setPopulation(count);
    if (!destination.populateTank(selected)) return false;
    source.removeFish(index, count);
    return true;
}

int readInt(const string& prompt, int minimum, int maximum) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minimum && value <= maximum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid number. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readPrice(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0 && value <= 1000000) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Enter a valid nonnegative price.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(const string& prompt) {
    string result;
    do {
        cout << prompt;
        getline(cin, result);
        if (result.empty()) cout << "This field cannot be empty.\n";
    } while (result.empty());
    return result;
}

int main() {
    HoldingTank holding("Holding Tank", 100);
    Tank general("General Tank", 80);
    SpecialTank special("Special Tank", 40, "M");
    vector<Tank*> tanks = {&holding, &general, &special};
    while (true) {
        cout << "\nAQUARIUM MANAGEMENT SYSTEM\n"
             << "1. Receive fish in holding tank\n"
             << "2. View all tanks\n"
             << "3. Transfer fish\n"
             << "4. Remove fish from holding tank\n"
             << "5. Flush a species from holding tank\n"
             << "6. View daily feeding costs\n"
             << "7. Change tank food prices\n"
             << "0. Exit\n";
        int choice = readInt("Choose an option: ", 0, 7);
        if (choice == 0) break;
        if (choice == 1) {
            string name = readText("Species name: ");
            int sizeChoice = readInt("Size (1=S, 2=M, 3=L, 4=XL): ", 1, 4);
            string sizes[] = {"S", "M", "L", "XL"};
            int dietChoice = readInt("Diet (1=Meat, 2=Plants): ", 1, 2);
            string description = readText("Description: ");
            int count = readInt("Population: ", 1, 100000);
            Fish incoming(name, sizes[sizeChoice - 1], dietChoice == 1 ? "Meat" : "Plants", description, count);
            cout << (holding.receiveIncomingFish(incoming) ? "Fish received.\n" : "Not enough capacity.\n");
        } else if (choice == 2) {
            for (Tank* tank : tanks) tank->printTankPop();
        } else if (choice == 3) {
            for (size_t i = 0; i < tanks.size(); ++i) cout << i + 1 << ". " << tanks[i]->getName() << '\n';
            int from = readInt("Source tank: ", 1, 3) - 1;
            int to = readInt("Destination tank: ", 1, 3) - 1;
            tanks[from]->printTankPop();
            int index = readInt("Fish number: ", 1, 100000) - 1;
            int count = readInt("How many fish to transfer: ", 1, 100000);
            cout << (transferFish(*tanks[from], *tanks[to], index, count)
                ? "Transfer completed.\n" : "Transfer failed: check species, size limit, population, or capacity.\n");
        } else if (choice == 4 || choice == 5) {
            holding.printTankPop();
            int index = readInt("Fish number: ", 1, 100000) - 1;
            bool success;
            if (choice == 4) {
                int count = readInt("Number to remove: ", 1, 100000);
                success = holding.removeFish(index, count);
            } else {
                success = holding.removeFish(index);
            }
            cout << (success ? "Fish removed.\n" : "Removal failed.\n");
        } else if (choice == 6) {
            for (Tank* tank : tanks) tank->printFoodCost();
        } else if (choice == 7) {
            for (size_t i = 0; i < tanks.size(); ++i) cout << i + 1 << ". " << tanks[i]->getName() << '\n';
            int tankIndex = readInt("Tank number: ", 1, 3) - 1;
            double meatPrice = readPrice("Meat price per pound: $");
            double plantPrice = readPrice("Plant price per pound: $");
            tanks[tankIndex]->adjustFoodCost(meatPrice, plantPrice);
            cout << "Prices updated.\n";
        }
    }
    return 0;
}
