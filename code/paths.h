#pragma once
#include <cstdio>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
inline bool open_paths(int argc, char **argv, FILE *&input, std::string &output, std::string &answer)
{
    if (argc != 3)
    {
        std::fprintf(stderr, "Usage: %s <input.in> <output-prefix>\n", argv[0]);
        return false;
    }
    fs::path prefix = fs::u8path(argv[2]);
    output = prefix.string() + ".out";
    answer = prefix.string() + ".ans";
    if (fs::exists(output) || fs::exists(answer))
    {
        std::fprintf(stderr, "Output already exists; choose a new prefix.\n");
        return false;
    }
    input = std::fopen(argv[1], "r");
    if (!input)
    {
        std::perror("Input");
        return false;
    }
    std::error_code error;
    if (prefix.has_parent_path())
        fs::create_directories(prefix.parent_path(), error);
    if (error)
    {
        std::fprintf(stderr, "Cannot create output directory: %s\n", error.message().c_str());
        std::fclose(input);
        return false;
    }
    return true;
}
