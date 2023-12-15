#pragma once

#include <filesystem>

class Variables {
public:
    std::filesystem::path currentPath = "C:\\";
};

extern Variables globalVariables;
