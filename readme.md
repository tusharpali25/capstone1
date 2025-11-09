# File Explorer (C++ + ncurses)

## Overview
This is a simple **File Explorer for Linux terminals**, built using **C++** and the **ncurses** library.
It provides a keyboard-driven interface to browse directories, manage files and folders, and perform common file operations directly in the terminal.
No mouse is required, making it ideal for command-line environments.

---

## Features
- **Navigation**: Browse folders using arrow keys and enter/backspace.
- **File Operations**: Create, delete, copy, and move files or folders.
- **Permissions Management**: View and edit Unix-style file permissions (e.g., `rwxr-xr--`).
- **Search Functionality**: Search for files by name within the current directory tree.
- **File Information**: Display file size, type (DIR or FIL), and permissions.
- **Colorful UI**: Responsive terminal interface with color-coded elements.
- **Built-in Help**: Comprehensive help menu accessible via 'h' key with all command instructions.

---

## Prerequisites
- **C++ Compiler**: g++ (GCC) version 7 or higher.
- **ncurses Library**: For terminal UI handling.
- **Operating System**: Linux (tested on Ubuntu/WSL; may work on other Unix-like systems).

---

## Installation
Install the required dependencies on your system.

### For Ubuntu or Debian-based systems (including WSL):
```bash
sudo apt update
sudo apt install g++ libncurses5-dev libncursesw5-dev -y
```

### For Fedora/CentOS/RHEL:
```bash
sudo dnf install gcc-c++ ncurses-devel  # or yum for older versions
```

### For Arch Linux:
```bash
sudo pacman -S gcc ncurses
```

### For macOS (using Homebrew):
```bash
brew install gcc ncurses
```
*Note: macOS support is experimental; some features like permissions may not work identically due to filesystem differences.*

---

## Building
1. Clone or download the repository to your local machine.
2. Navigate to the project directory:
   ```bash
   cd file_explorer
   ```
3. Build the executable using the provided Makefile:
   ```bash
   make
   ```
   This compiles `main.cpp`, `FileManager.cpp`, and `UIManager.cpp` into the `file_explorer` binary.

---

## Running
After building, run the application:
```bash
./file_explorer
```
Or use the Makefile shortcut:
```bash
make run
```

The application will start in the current working directory. Use keyboard controls to navigate and perform actions.

---

## Usage
The interface displays the current directory path, a list of files/folders with their types, sizes, and permissions, and a footer with status information.

### Key Bindings
- **Navigation**:
  - `↑/↓` (Arrow keys): Move selection up/down.
  - `Enter`: Open selected folder (files cannot be opened directly).
  - `Backspace` or `←`: Go up one directory level.

- **File Operations**:
  - `c`: Create a new file (prompts for name).
  - `d`: Delete selected file/folder (permanent, no undo).
  - `y`: Copy selected file (prompts for destination name/path).
  - `m`: Move/rename selected file/folder (prompts for new name/path).

- **Other Features**:
  - `s`: Search for files by keyword (searches recursively in current directory).
  - `p`: View/edit permissions of selected file (enter new permissions in `rwxrwxrwx` format).
  - `h`: Show help menu with detailed instructions.
  - `q`: Quit the application.

### File Permissions
Permissions are displayed in Unix style (e.g., `rwxr-xr--`):
- Positions: Owner (1-3), Group (4-6), Others (7-9).
- Letters: `r` (read), `w` (write), `x` (execute), `-` (no permission).

When editing, enter exactly 9 characters (e.g., `rwxr--r--`).

### Tips
- The application starts in the directory from which it was launched.
- Search is case-sensitive and matches filenames containing the keyword.
- For large directories, use search to quickly find files.
- Press any key to dismiss messages or return from help/search results.

---

## Project Structure
- `main.cpp`: Entry point, initializes ncurses and starts the UI.
- `FileManager.h/cpp`: Handles file system operations (list, create, delete, etc.).
- `UIManager.h/cpp`: Manages the ncurses-based user interface.
- `Makefile`: Build script for compiling the project.
- `readme.md`: This documentation.
- `.gitignore`: Excludes build artifacts and temporary files.

---


---
