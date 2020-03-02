#ifndef COMMON_FS_FILEUTILS_H_
#define COMMON_FS_FILEUTILS_H_

#include "../base/Base.h"
#include <dirent.h>

namespace niok {
namespace fs {

enum class FileType {
    UNKNOWN = 0,
    NOTEXIST,
    REGULAR,
    DIRECTORY,
    SYM_LINK,
    CHAR_DEV,
    BLOCK_DEV,
    FIFO,
    SOCKET
};


class FileUtils final {
public:
    FileUtils() = delete;

    // Get the directory part of a path
    static std::string dirname(const char *path);
    // Get the base part of a path
    static std::string basename(const char *path);

    // return the size of the given file
    static size_t fileSize(const char* path);
    // return the type of the given file
    static FileType fileType(const char* path);
    // Return the file type name
    static const char* getFileTypeName(FileType type);
    // Return the last update time for the given file (UNIX Epoch time)
    static time_t fileLastUpdateTime(const char* path);

    // Tell if stdin attached to a TTY
    static bool isStdinTTY();
    // Tell if stdout atached to a TTY
    static bool isStdoutTTY();
    // Tell if stderr attached to a TTY
    static bool isStderrTTY();
    // Tell if the given fd attached to a TTY
    static bool isFdTTY(int fd);

    // Remove the given file or directory (with its content)
    //
    // When the given path is a directory and recursively == false,
    // removal will fail if the directory is not empty. If recursive == true,
    // all content will be removed, including the sub-directories and their
    // contents
    //
    // The method returns true if removal succeeded or the given file
    // or directory does not exist.
    static bool remove(const char* path, bool recursively = false);
    // Create the directory
    //
    // It will make the parent directories as needed
    // (much like commandline "mkdir -p")
    static bool makeDir(const std::string& dir, uint32_t mode = 0775);
    // Check the path is exist
    static bool exist(const std::string& path);

    /**
     * List all entities in the given directory, whose type matches
     * the given file type
     *
     * The function returns either fullpath or just the filename
     *
     * You can also pass in a wildcard pattern, which makes the function
     * to only return entities whose names match the pattern
     */
    static std::vector<std::string> listAllTypedEntitiesInDir(
        const char* dirpath,
        FileType type,
        bool returnFullPath,
        const char* namePattern);
    /**
     * List all files in the given directory
     *
     * Internally, it calls listAllTypedEntitiesInDir()
     */
    static std::vector<std::string> listAllFilesInDir(
        const char* dirpath,
        bool returnFullPath = false,
        const char* namePattern = nullptr);
    /**
     * List all sub-directories in the given directory
     *
     * Internally, it calls listAllTypedEntitiesInDir()
     */
    static std::vector<std::string> listAllDirsInDir(
        const char* dirpath,
        bool returnFullPath = false,
        const char* namePattern = nullptr);

};

}  // namespace fs
}  // namespace niok

#endif  // COMMON_FS_FILEUTILS_H_

