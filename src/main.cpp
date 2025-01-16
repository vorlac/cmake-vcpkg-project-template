#include <cstdint>
#include <iostream>
#include <string_view>

#include <boost/filesystem.hpp>
#include <cryptopp/aes.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <json/json.h>
#include <libscrypt.h>
#include <snappy.h>

int main(int argc, char** argv)
{
    constexpr std::string_view hi{ "hello world" };
    fmt::print(fmt::emphasis::blink | fmt::fg(fmt::color::coral), "{}\n", hi);

    auto cwd = boost::filesystem::current_path();
    fmt::print(fmt::emphasis::underline | fmt::fg(fmt::color::beige),
               "boost::filesystem::current_path() = {}\n", cwd.string());

    std::cin.get();

    return 0;
}
