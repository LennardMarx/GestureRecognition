#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

double fingerTipDistance(std::vector<std::vector<double>> coords) {
    return sqrt(pow(coords[8][0] - coords[4][0], 2) + pow(coords[8][1] - coords[4][1], 2));
}

int main()
{
    std::vector<std::vector<double>> coordinates;

    std::ifstream file("my_file.csv");

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }


    while (1) {

        std::string line;
        while (std::getline(file, line)) {
            std::vector<double> row;
            std::stringstream ss(line);
            std::string value;

            while (std::getline(ss, value, ',')) {
                row.push_back(std::stod(value));
            }

            coordinates.push_back(row);
        }

        // Print the coordinates
        // for (int i = 0; i < coordinates.size(); i++) {
        //     for (int j = 0; j < coordinates[i].size(); j++) {
        //         std::cout << coordinates[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

        std::cout << fingerTipDistance(coordinates) << std::endl;

    }
    return 0;
}