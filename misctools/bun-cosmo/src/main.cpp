#include <iostream>
#include <filesystem>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cosmo.h>
#include "battery/embed.hpp"
#include "zip/zip.h"

std::string vector_join(const std::vector<std::string>& vec, const std::string& delimiter) {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0)
            oss << delimiter;
        oss << vec[i];
    }
    return oss.str();
}

int main(int argc, char *argv[]) {
    auto args = std::vector<std::string>(argv + 1, argv + argc);

    auto exe_path = std::filesystem::path(GetProgramExecutableName());
    auto internal_path = std::filesystem::path(exe_path.string() + "_internal");

    std::error_code errcode;
    auto exists = std::filesystem::exists(internal_path, errcode);
    if (errcode) {
      std::cerr << "std::filesystem::exists() " << internal_path << ": " << errcode << "\n";
      return 1;
    }
    if (exists) {
      std::filesystem::remove_all(internal_path, errcode);
      if (errcode) {
        std::cerr << "std::filesystem::remove_all() " << internal_path << ": " << errcode << "\n";
        return 1;
      }
    }

    // TODO: Declare as plain type
    auto platform_zip_embed = b::embed<BUN_RELEASES "/bun-linux-x64-baseline.zip">();
    std::string bun_relative_path;
    #if defined(__x86_64__) || defined(_M_X64)
        if (IsLinux()) {
            platform_zip_embed = b::embed<BUN_RELEASES "/bun-linux-x64-baseline.zip">();
            bun_relative_path = "bun-linux-x64-baseline/bun";
        } else if (IsWindows()) {
            platform_zip_embed = b::embed<BUN_RELEASES "/bun-windows-x64-baseline.zip">();
            bun_relative_path = "bun-windows-x64-baseline/bun.exe";
        } else if (IsXnu()) {
            platform_zip_embed = b::embed<BUN_RELEASES "/bun-darwin-x64-baseline.zip">();
            bun_relative_path = "bun-darwin-x64-baseline/bun";
        } else {
            std::cerr << "no prebuilt bun for current platform" << "\n";
            return 1;
        }
    #elif defined(__aarch64__) || defined(_M_ARM64)
        if (IsLinux()) {
            platform_zip_embed = b::embed<BUN_RELEASES "/bun-linux-aarch64.zip">();
            bun_relative_path = "bun-linux-aarch64/bun";
        } else if (IsXnu()) {
            platform_zip_embed = b::embed<BUN_RELEASES "/bun-darwin-aarch64.zip">();
            bun_relative_path = "bun-darwin-aarch64/bun";
        } else {
            std::cerr << "no prebuilt bun for current platform" << "\n";
            return 1;
        }
    #else
        #pragma message("not x86_64 or aarch64")
        std::cerr << "no prebuilt bun for current platform" << "\n";
        return 1;
    #endif
    auto err = zip_stream_extract(platform_zip_embed.data(), platform_zip_embed.size(), internal_path.c_str(), NULL, NULL);
    if (err) {
        std::cerr << "zip_stream_extract() " << platform_zip_embed.size() << " bytes => " << internal_path.c_str() << ": " << err << "\n";
        return 1;
    }

    auto exe_deleteme_path = std::filesystem::path(exe_path.string() + ".DELETEME");
    std::filesystem::rename(exe_path, exe_deleteme_path);

    auto bun_path = internal_path / bun_relative_path;
    std::filesystem::create_symlink(bun_path, exe_path);

    std::filesystem::remove(exe_deleteme_path, errcode);
    if (errcode) {
        // Ignore it.
    }

    err = execv(bun_path.c_str(), argv);
    if (err) {
        std::cerr << "execv() " << bun_path << " " << vector_join(args, " ") << ": " << err << "\n";
        return 1;
    }
    std::unreachable();
}