#ifndef RP_H
#define RP_H

#include <map>

enum instructions {
    Notdefined,
    instructionClear,
    instructionExit,
    instructionEmbeddedArgs,
    instuctionCreateDir,
    instructionCreateFile,
    instructionHelp
};

extern std::map<std::string, instructions> s_mapIns;

void MainConstructor();
void CommandsHelp();
void ErrorMessage(const std::string& errorDescription, int type);

#endif