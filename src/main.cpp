#include <cstdint>
#include <iostream>
#include <string_view>

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <zydis/MetaInfo.h>

int main(int argc, char** argv)
{
    constexpr std::string_view hi{ "hello world" };
    fmt::print(fmt::emphasis::blink | fmt::fg(fmt::color::coral), "{}\n", hi);
    auto test = ZydisCategoryGetString(ZydisInstructionCategory::ZYDIS_CATEGORY_AES);
    fmt::print(fmt::emphasis::underline | fmt::fg(fmt::color::beige), "Test str: {}\n", test);
    return 0;
}
