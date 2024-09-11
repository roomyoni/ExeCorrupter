#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem> 
#include <string>

namespace fs = std::filesystem;

const unsigned char rawData[16] = {
    0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0x00, 0x00
};

void breakExe(const std::string& filePath) {
    std::fstream exeFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!exeFile.is_open()) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return;
    }

    // Remove the first 16 bytes of the PE header
    exeFile.seekp(0, std::ios::beg);
    unsigned char zeroData[16] = { 0 };
    exeFile.write(reinterpret_cast<char*>(zeroData), sizeof(zeroData));

    exeFile.close();
    std::cout << "PE header broken: " << filePath << std::endl;
}

void repairExe(const std::string& filePath) {
    std::fstream exeFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!exeFile.is_open()) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return;
    }

    // Write back the original PE header
    exeFile.seekp(0, std::ios::beg);
    exeFile.write(reinterpret_cast<const char*>(rawData), sizeof(rawData));

    exeFile.close();
    std::cout << "PE header repaired: " << filePath << std::endl;
}

void breakAllExesInFolder(const std::string& folderPath) {
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() == ".exe") {
            breakExe(entry.path().string());
        }
    }
}

void repairAllExesInFolder(const std::string& folderPath) {
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() == ".exe") {
            repairExe(entry.path().string());
        }
    }
}

void showMenu() {
    std::cout << "\nOptions:\n";
    std::cout << "1. Break EXE\n";
    std::cout << "2. Repair EXE\n";
    std::cout << "3. Break all EXEs in folder\n";
    std::cout << "4. Repair all EXEs in folder\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    std::string filePath;
    std::string folderPath;

    while (true) {
        showMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore();  // to avoid issues with getline after cin

        switch (choice) {
        case 1:
            std::cout << "Enter the path of the EXE file: ";
            std::getline(std::cin, filePath);
            breakExe(filePath);
            break;
        case 2:
            std::cout << "Enter the path of the EXE file: ";
            std::getline(std::cin, filePath);
            repairExe(filePath);
            break;
        case 3:
            std::cout << "Enter the folder path: ";
            std::getline(std::cin, folderPath);
            breakAllExesInFolder(folderPath);
            break;
        case 4:
            std::cout << "Enter the folder path: ";
            std::getline(std::cin, folderPath);
            repairAllExesInFolder(folderPath);
            break;
        case 5:
            std::cout << "Exiting...\n";
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
