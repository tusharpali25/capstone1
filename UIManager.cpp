#include "UIManager.h"
#include <ncurses.h>
#include <iomanip>
#include <sstream>
#include <cstdlib>

UIManager::UIManager(FileManager &fileManager)
    : fm(fileManager), highlight(0), offset(0) {}

// Draw header
void UIManager::drawHeader()
{
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(0, 0, "  LINUX FILE EXPLORER Author: PRIYATOSH NAYAK  ");
    int cols = getmaxx(stdscr);
    for (int i = 22; i < cols; i++)
        printw(" ");
    attroff(COLOR_PAIR(1) | A_BOLD);
}

// Draw footer with current path and prompt
void UIManager::drawFooter(const std::vector<std::string> &files)
{
    int rows = getmaxy(stdscr);
    attron(COLOR_PAIR(1));
    unsigned long long freeGB = fm.getFreeSpace();
    std::ostringstream ss;
    ss << fm.getCurrentPath() << ":$ ";
    mvprintw(rows - 3, 0, " %-*s", getmaxx(stdscr) - 1, ss.str().c_str());
    attroff(COLOR_PAIR(1));
}

// Draw instructions at the bottom
void UIManager::drawInstructions()
{
    int rows = getmaxy(stdscr);
    attron(COLOR_PAIR(6));
    mvprintw(rows - 2, 0,
             "[Up|Down] Move  [Enter] Open  [Backspace] Up  "
             "[c] touch  [C] mkdir  [d] rm  [y] cp  [m] mv  "
             "[s] find  [p] chmod  [h] Help  [q] Quit");
    attroff(COLOR_PAIR(6));
}

// Prompt for user input
std::string UIManager::prompt(const std::string &label)
{
    echo();
    curs_set(1);
    char input[256];
    int rows = getmaxy(stdscr);
    mvprintw(rows - 1, 0, "%s: ", label.c_str());
    clrtoeol();
    getnstr(input, 255);
    noecho();
    curs_set(0);
    return std::string(input);
}

// Display files and directories
void UIManager::displayFiles(const std::vector<std::string> &files)
{
    clear();
    drawHeader();
    mvprintw(2, 0, "[DIR] Path: %s", fm.getCurrentPath().c_str());

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int visible = rows - 8;

    if (highlight < offset)
        offset = highlight;
    else if (highlight >= offset + visible)
        offset = highlight - visible + 1;

    attron(A_BOLD | COLOR_PAIR(6));
    mvprintw(3, 2, "%-6s %-40s %-10s %-12s", "TYPE", "NAME", "SIZE", "PERMS");
    attroff(A_BOLD | COLOR_PAIR(6));

    for (int i = 0; i < visible && (i + offset) < (int)files.size(); ++i)
    {
        int index = i + offset;
        bool isDir = fm.isDirectory(files[index]);

        std::string perms = fm.getPermissions(files[index]);
        unsigned long long size = 0;
        if (!isDir)
            size = fm.getFileSize(files[index]);

        std::ostringstream sizeStr;
        if (isDir)
            sizeStr << "--";
        else if (size < 1024)
            sizeStr << size << " B";
        else if (size < 1024 * 1024)
            sizeStr << std::fixed << std::setprecision(1) << (size / 1024.0) << " KB";
        else
            sizeStr << std::fixed << std::setprecision(1) << (size / (1024.0 * 1024.0)) << " MB";

        if (isDir)
            attron(COLOR_PAIR(7) | A_BOLD);
        if (index == highlight)
            attron(COLOR_PAIR(2) | A_BOLD);

        mvprintw(i + 4, 2, "%-6s %-40s %-10s %-12s",
                 isDir ? "[DIR]" : "[FIL]",
                 files[index].c_str(),
                 sizeStr.str().c_str(),
                 perms.c_str());

        if (index == highlight)
            attroff(COLOR_PAIR(2) | A_BOLD);
        if (isDir)
            attroff(COLOR_PAIR(7) | A_BOLD);
    }

    drawFooter(files);
    drawInstructions();
    refresh();
}

// Show help menu
void UIManager::showHelp()
{
    clear();
    attron(A_BOLD | COLOR_PAIR(6));
    mvprintw(1, 2, "HELP MENU - FILE EXPLORER COMMANDS");
    attroff(A_BOLD | COLOR_PAIR(6));

    const char *helpText[] = {
        "[Up / Down] - Move Selection",
        "    Use arrow keys to navigate through files and folders.",
        "",
        "[Enter] - Open Folder/File",
        "    Opens the selected folder and displays its contents.",
        "    Opens the selected file with the default editor.",
        "",
        "[Backspace] - Go Up",
        "    Move one directory level up (to the parent folder).",
        "",
        "[c] - Create File",
        "    Prompts you to enter a new file name in the current directory.",
        "    Example: entering 'notes.txt' will create that file here.",
        "",
        "[d] - Delete File / Folder",
        "    Deletes the selected item permanently.",
        "    Use with caution — there is no undo.",
        "",
        "[y] - Copy File",
        "    Copies the selected file to a new location or name.",
        "    Example: 'report.txt' → enter 'backup_report.txt' to copy in same folder.",
        "    You can also specify a full path, e.g. '../backup/report.txt'.",
        "",
        "[m] - Move / Rename File",
        "    Moves or renames the selected file/folder.",
        "    Example: rename 'old.txt' → 'new.txt', or move to '../docs/new.txt'.",
        "",
        "[s] - Search Files",
        "    Prompts for a keyword and lists all matching files/folders in current directory.",
        "    Press any key to return to the main view after seeing results.",
        "",
        "[p] - View / Edit Permissions",
        "    Displays current Unix-style permissions (e.g., rwxr-xr--).",
        "    Enter 'y' to modify them — you will then be prompted to input new ones.",
        "    Example: 'rwxr--r--' means owner can read/write/execute, group read-only, others read-only.",
        "",
        "[h] - Show Help",
        "    Displays this help menu.",
        "",
        "[q] - Quit",
        "    Exits the file explorer safely.",
    };

    int row = 3;
    for (int i = 0; i < (int)(sizeof(helpText) / sizeof(helpText[0])); ++i)
    {
        mvprintw(row++, 4, "%s", helpText[i]);
    }

    attron(COLOR_PAIR(6));
    mvprintw(row + 1, 2, "Press any key to return...");
    attroff(COLOR_PAIR(6));

    refresh();
    getch();
}

// Main UI loop
void UIManager::start()
{
    std::vector<std::string> files = fm.listFiles();
    int ch;

    while (true)
    {
        displayFiles(files);
        ch = getch();

        switch (ch)
        {
        case KEY_UP:
            if (highlight > 0)
                highlight--;
            break;

        case KEY_DOWN:
            if (highlight < (int)files.size() - 1)
                highlight++;
            break;

        case 10:
        {
            if (files.empty())
                break;
            std::string target = files[highlight];
            if (fm.isDirectory(target))
            {
                fm.changeDirectory(target);
                files = fm.listFiles();
                highlight = 0;
                offset = 0;
            }
            else
            {

                std::string command = "start \"\" \"" + fm.getCurrentPath() + "/" + target + "\"";
                system(command.c_str());
                mvprintw(getmaxy(stdscr) - 1, 0, "[INFO] Opening '%s' with default editor.", target.c_str());
            }
            break;
        }

        case KEY_LEFT:
        case KEY_BACKSPACE:
        case 127:
            fm.changeDirectory("..");
            files = fm.listFiles();
            highlight = 0;
            offset = 0;
            break;

        case 'c':
        {
            std::string name = prompt("touch");
            if (name.empty())
                break;
            bool ok = fm.createFile(name);
            mvprintw(getmaxy(stdscr) - 1, 0, ok ? "[OK] File created." : "[ERR] Failed to create file.");
            files = fm.listFiles();
            break;
        }

        case 'C':
        {
            std::string name = prompt("mkdir");
            if (name.empty())
                break;
            bool ok = fm.createDirectory(name);
            mvprintw(getmaxy(stdscr) - 1, 0, ok ? "[OK] Directory created." : "[ERR] Failed to create directory.");
            files = fm.listFiles();
            break;
        }

        case 'd':
        {
            if (files.empty())
                break;
            std::string name = files[highlight];
            bool ok = fm.deleteFile(name);
            mvprintw(getmaxy(stdscr) - 1, 0, ok ? "[OK] File deleted." : "[ERR] Delete failed.");
            files = fm.listFiles();
            if (highlight > 0)
                highlight--;
            break;
        }

        case 'y':
        {
            if (files.empty())
                break;
            std::string src = files[highlight];
            std::string dest = prompt("cp");
            if (dest.empty())
                break;
            bool ok = fm.copyFile(src, dest);
            mvprintw(getmaxy(stdscr) - 1, 0, ok ? "[OK] File copied." : "[ERR] Copy failed.");
            files = fm.listFiles();
            break;
        }

        case 'm':
        {
            if (files.empty())
                break;
            std::string src = files[highlight];
            std::string dest = prompt("mv");
            if (dest.empty())
                break;
            bool ok = fm.moveFile(src, dest);
            mvprintw(getmaxy(stdscr) - 1, 0, ok ? "[OK] File moved." : "[ERR] Move failed.");
            files = fm.listFiles();
            break;
        }

        case 's':
        {
            std::string key = prompt("find");
            if (key.empty())
                break;
            auto results = fm.searchFiles(key);
            clear();
            mvprintw(1, 2, "Search results for '%s':", key.c_str());
            int i = 3;
            for (auto &r : results)
            {
                mvprintw(i++, 4, "%s", r.c_str());
                if (i >= getmaxy(stdscr) - 2)
                    break;
            }
            mvprintw(getmaxy(stdscr) - 1, 2, "Press any key to return...");
            refresh();
            getch();
            files = fm.listFiles();
            break;
        }

        case 'p':
        {
            if (files.empty())
                break;
            std::string name = files[highlight];
            std::string perms = fm.getPermissions(name);
            mvprintw(getmaxy(stdscr) - 3, 0, "%s : %s", name.c_str(), perms.c_str());
            refresh();

            std::string choice = prompt("chmod");
            if (choice.empty())
                break;
            if (choice.size() == 9)
            {
                bool ok = fm.setPermissions(name, choice);
                mvprintw(getmaxy(stdscr) - 1, 0,
                         ok ? "[OK] Permissions updated." : "[ERR] Failed to update perms.");
            }
            else
            {
                mvprintw(getmaxy(stdscr) - 1, 0, "[ERR] Invalid format (use rwxrwxrwx).");
            }
            break;
        }

        case 'h':
            showHelp();
            break;

        case 'q':
            return;
        }
    }
}
