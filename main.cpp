#include "FileManager.h"
#include "UIManager.h"
#include <ncurses.h>

int main()
{
    initscr();
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    

    // Define colors
    init_pair(1, COLOR_BLACK, COLOR_CYAN); // Header/Footer
    init_pair(2, COLOR_YELLOW, -1);        // Highlight
    init_pair(3, COLOR_WHITE, -1);         // Normal
    init_pair(4, COLOR_GREEN, -1);         // Success
    init_pair(5, COLOR_RED, -1);           // Error
    init_pair(6, COLOR_CYAN, -1);          // Help text
    init_pair(7, COLOR_BLUE, -1);          // Folder name

    FileManager fm;
    UIManager ui(fm);
    ui.start();

    endwin();
    return 0;
}
