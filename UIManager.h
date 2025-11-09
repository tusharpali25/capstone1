#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "FileManager.h"
#include <string>
#include <vector>

// A simple UI manager for console-based file explorer
class UIManager
{
private:
    FileManager &fm;
    int highlight;
    int offset;

    void drawHeader();
    void drawFooter(const std::vector<std::string> &files);
    void drawInstructions();
    void displayFiles(const std::vector<std::string> &files);
    void showHelp();
    void showShellPrompt();
    std::string prompt(const std::string &label);

public:
    UIManager(FileManager &fileManager);
    void start();
};

#endif
