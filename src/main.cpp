#include <cstdint>
#include <iostream>
#include <string_view>

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <xieite/io/dump.hpp>

int main(int argc, char** argv)
{
    xieite::dump("Hello, world!");

    return 0;
}
