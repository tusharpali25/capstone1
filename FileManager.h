#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>


class FileManager {
private:
    std::string currentPath;

public:
    FileManager();

    std::vector<std::string> listFiles() const;
    bool changeDirectory(const std::string &dir);
    std::string getCurrentPath() const;
    bool createFile(const std::string &filename);
    bool createDirectory(const std::string &dirname);
    bool deleteFile(const std::string &filename);
    bool copyFile(const std::string &src, const std::string &dest);
    bool moveFile(const std::string &src, const std::string &dest);
    unsigned long long getFileSize(const std::string &name);
    std::vector<std::string> searchFiles(const std::string &keyword) const;
    std::string getPermissions(const std::string &filename) const;
    bool setPermissions(const std::string &filename, const std::string &mode);   
    bool isDirectory(const std::string &name) const;
    unsigned long long getFreeSpace() const;
};

#endif
