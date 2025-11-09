#include "FileManager.h"
#include <filesystem>
#include <fstream>
#include <sys/stat.h>

namespace fs = std::filesystem;


// Constructor for FileManager
FileManager::FileManager() {
    currentPath = fs::current_path().string();
}


// List files in the current directory
std::vector<std::string> FileManager::listFiles() const {
    std::vector<std::string> files;
    try {
        for (const auto &entry : fs::directory_iterator(currentPath))
            files.push_back(entry.path().filename().string());
    } catch (...) {}
    return files;
}


// Change the current directory
bool FileManager::changeDirectory(const std::string &dir) {
    fs::path newPath = (dir == "..") ? fs::path(currentPath).parent_path()
                                     : fs::path(currentPath) / dir;
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        currentPath = fs::canonical(newPath).string();
        return true;
    }
    return false;
}


// Get the current directory path
std::string FileManager::getCurrentPath() const {
    return currentPath;
}

bool FileManager::createFile(const std::string &filename) {
    try {
        std::ofstream file((fs::path(currentPath) / filename).string());
        return file.good();
    } catch (...) { return false; }
}

// Create a new directory
bool FileManager::createDirectory(const std::string &dirname) {
    try {
        return fs::create_directory(fs::path(currentPath) / dirname);
    } catch (...) { return false; }
}


// Delete a file
bool FileManager::deleteFile(const std::string &filename) {
    try { return fs::remove(fs::path(currentPath) / filename); }
    catch (...) { return false; }
}

// Copy a file
bool FileManager::copyFile(const std::string &src, const std::string &dest) {
    try {
        fs::copy(fs::path(currentPath) / src, fs::path(currentPath) / dest,
                 fs::copy_options::overwrite_existing);
        return true;
    } catch (...) { return false; }
}

// Move a file
bool FileManager::moveFile(const std::string &src, const std::string &dest) {
    try {
        fs::rename(fs::path(currentPath) / src, fs::path(currentPath) / dest);
        return true;
    } catch (...) { return false; }
}


// Search files by keyword
std::vector<std::string> FileManager::searchFiles(const std::string &keyword) const {
    std::vector<std::string> results;
    try {
        for (auto &entry : fs::recursive_directory_iterator(currentPath)) {
            if (entry.path().filename().string().find(keyword) != std::string::npos)
                results.push_back(entry.path().string());
        }
    } catch (...) {}
    return results;
}

// Get file permissions
std::string FileManager::getPermissions(const std::string &filename) const {
    struct stat info;
    std::string perms = "---------";
    if (stat((fs::path(currentPath) / filename).c_str(), &info) == 0) {
        perms[0] = (info.st_mode & S_IRUSR) ? 'r' : '-';
        perms[1] = (info.st_mode & S_IWUSR) ? 'w' : '-';
        perms[2] = (info.st_mode & S_IXUSR) ? 'x' : '-';
        perms[3] = (info.st_mode & S_IRGRP) ? 'r' : '-';
        perms[4] = (info.st_mode & S_IWGRP) ? 'w' : '-';
        perms[5] = (info.st_mode & S_IXGRP) ? 'x' : '-';
        perms[6] = (info.st_mode & S_IROTH) ? 'r' : '-';
        perms[7] = (info.st_mode & S_IWOTH) ? 'w' : '-';
        perms[8] = (info.st_mode & S_IXOTH) ? 'x' : '-';
    }
    return perms;
}

// Set file permissions
bool FileManager::setPermissions(const std::string &filename, const std::string &mode) {
    fs::path filePath = fs::path(currentPath) / filename;
    try {
        fs::perms newPerms = fs::perms::none;
        if (mode.size() == 9) {
            if (mode[0] == 'r') newPerms |= fs::perms::owner_read;
            if (mode[1] == 'w') newPerms |= fs::perms::owner_write;
            if (mode[2] == 'x') newPerms |= fs::perms::owner_exec;
            if (mode[3] == 'r') newPerms |= fs::perms::group_read;
            if (mode[4] == 'w') newPerms |= fs::perms::group_write;
            if (mode[5] == 'x') newPerms |= fs::perms::group_exec;
            if (mode[6] == 'r') newPerms |= fs::perms::others_read;
            if (mode[7] == 'w') newPerms |= fs::perms::others_write;
            if (mode[8] == 'x') newPerms |= fs::perms::others_exec;
        }
        fs::permissions(filePath, newPerms, fs::perm_options::replace);
        return true;
    } catch (...) {
        return false;
    }
}

// Check if a path is a directory
bool FileManager::isDirectory(const std::string &name) const {
    return fs::is_directory(fs::path(currentPath) / name);
}


// Get free space in the current directory (in GB)
unsigned long long FileManager::getFreeSpace() const {
    try {
        auto space = fs::space(currentPath);
        return space.available / (1024 * 1024 * 1024);
    } catch (...) { return 0; }
}


// Get file size
unsigned long long FileManager::getFileSize(const std::string &name)
{
    std::string fullPath = currentPath + "/" + name;
    struct stat st;
    if (stat(fullPath.c_str(), &st) == 0)
        return st.st_size;
    return 0;
}