// Thank you, ChatGPT

#include "file.h"

void replace_char(std::string& str, char replace_this, char replace_with) {
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (str[i] == replace_this) {
            str[i] = replace_with;
        }
    }
}
std::string get_dll_path() {
    char DllPath[MAX_PATH] = {0};
    GetModuleFileNameA((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
    return DllPath;
}
std::string get_exploit_path() {
    std::string dll_path = get_dll_path();
    std::string bin_path = dll_path.substr(0, dll_path.rfind('\\'));
    return bin_path.substr(0, bin_path.rfind('\\'));
}
bool is_within_workspace(std::string& file_path) {
    if (file_path.find("..") != std::string::npos) {
        return false;
    }
    std::string workspace_path = get_exploit_path() + XorString("\\workspace\\");
    std::transform(file_path.begin(), file_path.end(), file_path.begin(), ::tolower);
    std::transform(workspace_path.begin(), workspace_path.end(), workspace_path.begin(), ::tolower);
    return (file_path.find(workspace_path) == 0);
}
std::string fix_path(std::string& path) {
    replace_char(path, '/', '\\');
    return get_exploit_path() + XorString("\\workspace\\") + path;
}
const char* readfile_internal (const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return nullptr;
    }

    // Determine the size of the file
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate a buffer to hold the file contents
    char* buffer = new char[size + 1];

    // Read the file into the buffer
    file.read(buffer, size);
    buffer[size] = '\0';

    // Close the file and return the buffer
    file.close();
    return buffer;
}
bool writefile_internal(const std::string& path, const char* contents, size_t size) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(contents, size);
    file.close();
    return true;
}

bool isfile(std::string path) {
    path = fix_path(path);
    return std::filesystem::is_regular_file(path);
}
bool isfolder(std::string path) {
    path = fix_path(path);
    if (is_within_workspace(path)) {
        return std::filesystem::is_directory(path);
    } else {
        return false;
    }
}
const char* readfile(std::string path) {
    path = fix_path(path);
    if (is_within_workspace(path)) {
        return readfile_internal(path);
    } else {
        return nullptr;
    }
}
bool delfile(std::string path) {
    path = fix_path(path);
    if (is_within_workspace(path) && std::filesystem::is_regular_file(path)) {
        std::filesystem::remove(path);
        return true;
    } else {
        return false;
    }
}
bool delfolder(std::string path) {
    path = fix_path(path);
    if (is_within_workspace(path) && std::filesystem::is_directory(path)) {
        std::filesystem::remove(path);
        return true;
    } else {
        return false;
    }
}
bool writefile(std::string path, const char* data) {
    path = fix_path(path);
    if (is_within_workspace(path)) {
        writefile_internal(path, data, strlen(data));
        return true;
    } else {
        return false;
    }
}
std::vector<std::string>* listfiles(std::string original_path) {
    replace_char(original_path, '/', '\\');
    // Create a vector to store the file paths
    std::vector<std::string>* file_paths = new std::vector<std::string>;

    // Get the full path to the workspace folder
    std::string workspace_path = get_exploit_path() + XorString("\\workspace\\");

    // Concatenate the original path with the workspace path to get the full path
    std::string full_path = workspace_path + original_path;

    // Append the wildcard character to the path to search for all files
    full_path += "\\*";

    // Initialize the WIN32_FIND_DATA structure
    WIN32_FIND_DATAA ffd;

    // Find the first file in the directory
    HANDLE hFind = FindFirstFileA(full_path.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        return nullptr;
    }

    // Iterate over the files in the directory
    do {
        // Skip the "." and ".." entries
        if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0 || (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            continue;
        }
        // Get the full path of the file
        std::string file_path = original_path + "\\" + std::string(ffd.cFileName);
        //file_path.erase(0, workspace_path.length());
        // Add the file path to the vector
        file_paths->push_back(file_path);
    } while (FindNextFileA(hFind, &ffd) != 0);

    // Close the directory
    FindClose(hFind);

    // Return the vector of file paths
    return file_paths;
}






std::vector<std::string>* listfolders(std::string original_path) {
    replace_char(original_path, '/', '\\');
    // Create a vector to store the file paths
    std::vector<std::string>* file_paths = new std::vector<std::string>;

    // Get the full path to the workspace folder
    std::string workspace_path = get_exploit_path() + XorString("\\workspace\\");

    // Concatenate the original path with the workspace path to get the full path
    std::string full_path = workspace_path + original_path;

    // Append the wildcard character to the path to search for all files
    full_path += "\\*";

    // Initialize the WIN32_FIND_DATA structure
    WIN32_FIND_DATAA ffd;

    // Find the first file in the directory
    HANDLE hFind = FindFirstFileA(full_path.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        return nullptr;
    }

    // Iterate over the files in the directory
    do {
        // Skip the "." and ".." entries
        if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0 || (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            continue;
        }
        // Get the full path of the file
        std::string file_path = original_path + "\\" + std::string(ffd.cFileName) + "\\";
        //file_path.erase(0, workspace_path.length());
        // Add the file path to the vector
        file_paths->push_back(file_path);
    } while (FindNextFileA(hFind, &ffd) != 0);

    // Close the directory
    FindClose(hFind);

    // Return the vector of file paths
    return file_paths;
}