#pragma once

#include <fstream>

namespace pepp::io
{
    enum FileFlags {
        kFileInput = 1,
        kFileOutput = 2,
        kFileApp = 8,
        kFileTrunc = 16,
        kFileBinary = 32
    };

    class File {
    public:
        File() = default;
        File(const File& other) = default;

        File(std::string_view filename, int flags);
        File(File&& other);

        void Write(std::string_view text);
        void Write(const std::vector<std::uint8_t>& data);
        void Write(void* data, size_t size);
        bool Exists();
        std::vector<std::uint8_t> Read();
        std::uintmax_t GetSize();

        File& operator=(File&& rhs);

    private:
        std::string   m_filename;
        int           m_flags;
        std::ofstream m_out_file;
        std::ifstream m_in_file;
    };
}