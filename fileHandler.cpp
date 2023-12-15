#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include <filesystem>
#include <fstream>

#include "includes/utilities.h"
#include "includes/variables.h"

/*
bool Permission(const std::filesystem::path& path) {
    std::filesystem::file_status status = std::filesystem::status(path);
    try {
        return (status.permissions() & std::filesystem::perms::owner_read) != std::filesystem::perms::none;
    } catch (const std::exception& e) {}
}
*/

void FileEndPrinter(std::wstring& fileName, bool method, std::uintmax_t size) {
    if (method) {
        std::wcout << L"DIR \t??? byte\t" << fileName << L"\n";
    } else {
        std::wcout << L"FILE\t" << std::to_wstring(size) << L" byte \t" << fileName << L"\n";
    }
}

void PrintDetails(int num, std::uintmax_t bytes, bool method) {
    if (method) {
        if (num != 0) {
            std::cout << "\t" << num << " FILE(S) " << std::endl;
        } else {
            std::cout << "\t" << "NO DIRICTORIES" << std::endl;
        }
    } else {
        if (bytes != 0 && num != 0) {
            double gigabytes = static_cast<double>(bytes) / std::pow(1024, 3);
            double megabytes = static_cast<double>(bytes) / (1024.0 * 1024.0);
            std::string _bytes;
            if (gigabytes < 1) {
                _bytes = std::to_string(megabytes) + " Megabytes\n";
            } else {
                _bytes = std::to_string(gigabytes) + " Gigabytes\n";
            }
            std::cout << "\t" << num << " FILE(S) " << std::to_string(bytes) << " Byte or " << _bytes;
        } else {
            std::cout << "\t" << "NO FILES" << std::endl;
        }
    }
}

void FilePrinter(const std::filesystem::path& path, bool method, bool recursive, bool printBothTypes) {  // method: { true: dir; false: file }, recursive: { true: on; false: off }
    std::filesystem::path validPath = path.empty() ? globalVariables.currentPath : path;
    std::uintmax_t bytes = 0;
    int fileNum = 0;
    int dirNum = 0;

    if (validPath == "C:\\" && recursive) {
        ErrorMessage("c error", 2);
        return;
    }

    if (recursive) {
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(validPath)) {
                std::wstring sFileName = entry.path().filename();
                if (entry.is_directory()) {
                    dirNum++;
                    FileEndPrinter(sFileName, true, 0);
                } else if (entry.is_regular_file()) {
                    fileNum++;
                    std::uintmax_t size = entry.file_size();
                    bytes += size;
                    FileEndPrinter(sFileName, false, size);
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        } catch (...) {
            std::cerr << std::endl;
        }
        PrintDetails(dirNum, 0, true);
        PrintDetails(fileNum, bytes, false);
    } else {
        for (const auto& entry : std::filesystem::directory_iterator(validPath)) {
            std::wstring sFileName = entry.path().filename();
            if (printBothTypes || (method && entry.is_directory()) || (!method && entry.is_regular_file())) {
                if (entry.is_directory()) {
                    dirNum++;
                    FileEndPrinter(sFileName, true, 0);
                } else if (entry.is_regular_file()) {
                    fileNum++;
                    std::uintmax_t size = entry.file_size();
                    bytes += size;
                    FileEndPrinter(sFileName, false, size);
                }
            }
        }
    }

    if (!printBothTypes) {
        if (!recursive) {
            if (method) PrintDetails(dirNum, 0, true);
            else PrintDetails(fileNum, bytes, false);
        }
    } else {
        PrintDetails(dirNum, 0, true);
        PrintDetails(fileNum, bytes, false);
    }
}

bool starts_with(const std::string& str, const char prefix) {
    return !str.empty() && str[0] == prefix;
}

bool ends_with(const std::string& str, const char suffix) {
    return !str.empty() && str[str.length() - 1] == suffix;
}

void ChangeDirectory(const std::string& input) {
    std::string formatted;
    if (input.length() > 2) formatted = input.substr(3);
    else return;

    if (starts_with(formatted, '"') && ends_with(formatted, '"')) {
        std::string path = formatted.substr(formatted.find_first_not_of('\"'), formatted.find_last_not_of('\"'));
        if (std::filesystem::exists(path)) {
            globalVariables.currentPath = path;
        } else {
            ErrorMessage(path, 1);
        }
    } else if (formatted == ".." || formatted == ".") {
        globalVariables.currentPath = globalVariables.currentPath.parent_path();
    } else if (formatted == "./") {
        globalVariables.currentPath = "C:\\";
    } else if (input.find("cd") != std::string::npos){
        std::filesystem::path path = globalVariables.currentPath / formatted;
        if (std::filesystem::exists(path)) {
            globalVariables.currentPath = path;
        } else {
            ErrorMessage(input, 1);
        }
    } else {
        if (input != "cd")
            ErrorMessage(input, 0);
    }
}

void Config(const std::string& fullString) {
    std::vector<std::pair<std::string, std::string>> pairedArgs;
    std::string rawArgs = fullString.substr(8, fullString.length());
    std::string argName;
    std::string argVal;
    bool argMode = false;
    for (int i = 0; i < rawArgs.length(); i++) {
        if (rawArgs[i] == '-') {
            if (!argName.empty() && !argVal.empty()) {
                pairedArgs.emplace_back(argName, argVal);
                argName = "";
                argVal = "";
                argMode = false;
            }
        } else if (rawArgs[i] == ' ') {
            argMode = true;
            continue;
        }

        if (!argMode && rawArgs[i] != '-') {
            argName += rawArgs[i];
        } else if (argMode && rawArgs[i] != '-') {
            argVal += rawArgs[i];
        }
    }
    if (!argName.empty() && !argVal.empty()) {
        pairedArgs.emplace_back(argName, argVal);
    }
}

void CreateDirectory(std::string& fullString) {
    std::string dirname = fullString.substr(4);
    std::filesystem::path dirPath = globalVariables.currentPath / dirname;
    if (!dirname.empty() && dirname.find_first_not_of(' ') != std::string::npos && !exists(dirPath))
        std::filesystem::create_directory(globalVariables.currentPath / dirname);
}

void CreateFile(std::string& fullString) {
    std::string fileName = fullString.substr(4);
    std::filesystem::path filePath = globalVariables.currentPath / fileName;
    if (!fileName.empty() && fileName.find_first_not_of(' ') != std::string::npos && !exists(filePath)) {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open())
            file.close();
    }
}

void EmbeddedCommands(std::string& fullCommand) {
    std::string foundedCommand;
    std::vector<std::string> possibleCommands = {"config", "cd", "mkd", "mkf", "ds", "fs", "ss", "ssc"};
    for (const std::string& e : possibleCommands) {
        if (fullCommand.find(e) != std::string::npos) {
            foundedCommand = e;
        }
    }
    if (foundedCommand == possibleCommands[0]) {
        Config(fullCommand);
    } else if (foundedCommand == possibleCommands[1]) {
        ChangeDirectory(fullCommand);
    } else if (foundedCommand == possibleCommands[2]) {
        CreateDirectory(fullCommand);
    } else if (foundedCommand == possibleCommands[3]) {
        CreateFile(fullCommand);
    } else if (foundedCommand == possibleCommands[4]) {
        FilePrinter(globalVariables.currentPath, true, false, false);
    } else if (foundedCommand == possibleCommands[5]) {
        FilePrinter(globalVariables.currentPath, false, false, false);
    } else if (foundedCommand == possibleCommands[6]) {
        FilePrinter(globalVariables.currentPath, false, true, false);
    } else if (foundedCommand == possibleCommands[7]) {
        FilePrinter(globalVariables.currentPath, false, false, true);
    }
}