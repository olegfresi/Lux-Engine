/*
 * Project: TestProject
 * File: FileUtils.hpp
 * Author: olegfresi
 * Created: 17/02/25 17:10
 *
 * Copyright © 2025 olegfresi
 *
 * Licensed under the MIT License. You may obtain a copy of the License at:
 *
 *     https://opensource.org/licenses/MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "../Application/Assertion.hpp"

struct FileSystem
{
    static bool CheckFileExists(const std::filesystem::path& filePath);
    static void CheckFileExtension(const std::filesystem::path& filePath, const std::string& extension);

    static std::string ToUpper(const std::string& s);
    static std::string ToLower(const std::string& s);

    static std::vector<std::string> Split(std::string& line, const std::string& delim);
    static std::vector<std::filesystem::path> GetFilesInDirectory(const std::string& directoryPath);
};