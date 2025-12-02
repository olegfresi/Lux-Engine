#include "../../include/FileSystem/FileIO.hpp"

namespace lux::filesys
{
    FileReader::FileReader(const std::string& filePath) : m_filePath(filePath)
    {
        OpenFile();
    }


    FileReader::~FileReader()
    {
        Close();
    }


    bool FileReader::IsOpen() const
    {
        return m_file.is_open();
    }

    std::string FileReader::FindLineContaining(const std::string& keyword)
    {
        if (!m_file.is_open())
            return "";

        std::string line;
        m_file.seekg(0);

        while (std::getline(m_file, line))
        {
            if (line.find(keyword) != std::string::npos)
                return line;
        }

        return "";
    }

    std::string FileReader::GetValueForKey(const std::string& key, char separator)
    {
        if (!m_file.is_open())
            return "";

        std::string line;
        m_file.seekg(0);

        while (std::getline(m_file, line))
        {
            size_t pos = line.find(separator);
            if (pos != std::string::npos)
            {
                std::string foundKey = Trim(line.substr(0, pos));
                if (foundKey == key)
                    return Trim(line.substr(pos + 1));
            }
        }

        return "";
    }

    std::unordered_map<std::string, std::string> FileReader::ReadAsKeyValue(char separator)
    {
        std::unordered_map<std::string, std::string> data;
        if (!m_file.is_open())
            return data;

        std::string line;
        m_file.seekg(0);

        while (std::getline(m_file, line))
        {
            size_t pos = line.find(separator);
            if (pos != std::string::npos)
            {
                std::string key = Trim(line.substr(0, pos));
                std::string value = Trim(line.substr(pos + 1));

                if (!key.empty() && !value.empty())
                    data[key] = value;
            }
        }

        return data;
    }

    void FileReader::Close()
    {
        if (m_file.is_open())
            m_file.close();
    }

    bool FileReader::GetLine(std::string &line)
    {
        if (std::getline(m_file, line))
            return true;

        return false;
    }

    bool FileReader::HasNextLine() const
    {
        return !m_file.eof();
    }

    std::string FileReader::GetNextLine()
    {
        std::string line;
        if (std::getline(m_file, line))
        {
            ++m_currentLine;
            return line;
        }

        return "";
    }

    void FileReader::Reset()
    {
        m_file.clear();
        m_file.seekg(0, std::ios::beg);
        m_currentLine = 0;
    }

    size_t FileReader::GetCurrentLine() const { return m_currentLine; }

    std::vector<std::string> FileReader::SplitLine(const std::string& line, const std::string& delimiter) const
    {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = 0;

        while ((end = line.find(delimiter, start)) != std::string::npos)
        {
            tokens.push_back(line.substr(start, end - start));
            start = end + delimiter.length();
        }

        tokens.push_back(line.substr(start));

        return tokens;
    }

    void FileReader::OpenFile()
    {
        m_file.open(m_filePath);
        if (!m_file)
        {
            CORE_CRITICAL( "Failed opening file '{}'", m_filePath );
            throw std::runtime_error("Failed opening file");
        }
    }

    std::string FileReader::Trim(const std::string& str)
    {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            return "";

        size_t last = str.find_last_not_of(" \t\r\n");

        return str.substr(first, (last - first + 1));
    }
}