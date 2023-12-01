#include <cstdint>
#include <iostream>
#include <string_view>

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <openssl/crypto.h>

int main(int argc, char** argv)
{
    constexpr std::string_view hi{ "hello world" };
    fmt::print(fmt::emphasis::blink | fmt::fg(fmt::color::coral), "{}\n", hi);

    const uint32_t ssl_major{ OPENSSL_version_major() };
    const uint32_t ssl_minor{ OPENSSL_version_minor() };
    const uint32_t ssl_patch{ OPENSSL_version_patch() };

    fmt::print(fmt::emphasis::underline | fmt::fg(fmt::color::beige),
               "Using OpenSSL Version: {}.{}.{}\n", ssl_major, ssl_minor, ssl_patch);

    std::cin.get();

    return 0;
}
