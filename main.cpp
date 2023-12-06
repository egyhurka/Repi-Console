/*
    Build program: c++ main.cpp -o rpc.exe

*/

#include <Windows.h>
#include <conio.h>

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

using std::fstream, std::string;

enum instructions {
    NotDefined,
    instructionClear,
    instructionC,
    instructionDs,
    instructionFs,
    instructionSs
};

std::filesystem::path currentPath = std::filesystem::current_path();
static std::map<std::string, instructions> s_mapIns;
static char uInput[_MAX_PATH];

static void Initialize();
void fileHandler(std::filesystem::path path, bool method, bool recursive);

int main() {
    Initialize();
    const char *title = "Repi console | v: 0.01";
    SetConsoleTitleA(title);
    std::wcout.imbue(std::locale::classic());
    while (1) {
        std::wcout << currentPath.wstring() << "-$: ";
        std::cin.getline(uInput, _MAX_PATH);
        switch (s_mapIns[uInput]) {
            case instructionClear:
                system("cls");
                break;
            case instructionC:
                system("cls");
                break;
            case instructionDs:
                fileHandler(currentPath.string(), true, false);
                break;
            case instructionFs:
                fileHandler(currentPath.string(), false, false);
                break;
            case instructionSs:
                fileHandler(currentPath, false, true);
                break;
            default:
                break;
        }
    }
}

void Initialize() {
    s_mapIns["clear"] = instructionClear;
    s_mapIns["c"] = instructionC;
    s_mapIns["ds"] = instructionDs;
    s_mapIns["fs"] = instructionFs;
    s_mapIns["ss"] = instructionSs;
}

void handlerPrinter(std::wstring fileName, bool method, long long int size) {
    if (method) {
        std::wcout << L"DIR \t??? byte\t" << fileName << L"\n";
    } else {
        std::wcout << L"FILE\t" << std::to_wstring(size) << L" byte \t" << fileName << L"\n";
    }

    return;
}

void fileHandler(std::filesystem::path path, bool method, bool recursive) {  // method: { true: dir; false: file }, recursive: { true: on; false: off }

    std::filesystem::path validpath;
    long long int bytes = 0;
    int fileNum = 0;
    int dirNum = 0;

    if (!path.empty()) {
        validpath = path;
    } else {
        validpath = currentPath;
    }

    if (recursive) {
        for (const auto &entry : std::filesystem::recursive_directory_iterator(validpath)) {
            std::wstring sFilename = entry.path().filename();
            if (entry.is_directory()) {
                dirNum++;
                handlerPrinter(sFilename, true, 0);
            } else if (entry.is_regular_file()) {
                fileNum++;
                int size = entry.file_size();
                bytes += size;
                handlerPrinter(sFilename, false, size);
            }
        }
    } else {
        for (const auto &entry : std::filesystem::directory_iterator(validpath)) {
            std::wstring sFilename = entry.path().filename();
            if (method && entry.is_directory()) {
                dirNum++;
                handlerPrinter(sFilename, true, 0);
            } else if (!method && entry.is_regular_file()) {
                fileNum++;
                int size = entry.file_size();
                bytes += size;
                handlerPrinter(sFilename, false, size);
            }
        }
    }

    if (!method && !recursive) {
        std::cout << "\t" << fileNum << " FILE(S) " << std::to_string(bytes) << " Byte\n";
    } else if (method && !recursive) {
        std::cout << "\t\t" << dirNum << " DIR(S)\n";
    }

    if (recursive) {
        std::cout << "\t" << fileNum << " FILE(S) " << std::to_string(bytes) << " Byte\n";
        std::cout << "\t\t" << dirNum << " DIR(S)\n";
    }

    return;
}