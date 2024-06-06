#include <array>
#include <iostream>
#include <span>
#include <string_view>

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <zmq.hpp>

int main(int argc, char** argv)
{
    constexpr std::string_view hi{ "hello world" };
    fmt::print(fmt::emphasis::blink | fmt::fg(fmt::color::coral), "{}\n", hi);

    auto buffer = std::array{ 1, 2, 3, 4 };
    auto test = zmq::const_buffer(buffer.data(), buffer.size());
    const int* data = reinterpret_cast<const int*>(test.data());
    fmt::print("{}", std::span{ data, data + 4 });

    std::cin.get();

    return 0;
}
