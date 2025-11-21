#include <filesystem>
#include "../../include/FileSystem/FileSystem.hpp"

bool FileSystem::CheckFileExists(const std::filesystem::path& filePath)
{
    return std::filesystem::exists(filePath);
}

void FileSystem::CheckFileExtension(const std::filesystem::path& filePath, const std::string& extension)
{
    if (filePath.extension() != extension)
        throw std::runtime_error("Invalid file extension");
}

std::string FileSystem::ToUpper(const std::string& s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string FileSystem::ToLower(const std::string& s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::vector<std::string> FileSystem::Split(std::string line, std::string delim)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = line.find(delim)) != std::string::npos)
    {
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + delim.size());
    }
    tokens.push_back(line);

    return tokens;
}

namespace fs = std::filesystem;
std::vector<std::filesystem::path> FileSystem::GetFilesInDirectory(const std::string& directoryPath)
{
    std::vector<std::filesystem::path> filePaths;

    try
    {
        if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
        {
            std::cerr << "Directory does not exist or invalid\n";
            return filePaths;
        }

        for (const auto &entry : fs::directory_iterator(directoryPath))
        {
            if (fs::is_regular_file(entry.status()))
                filePaths.push_back(entry.path().string());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error iterating on directory: " << e.what() << std::endl;
    }

    return filePaths;
}