
#include <print>

#include <boost/algorithm/is_palindrome.hpp>

int main(int argc, char** argv)
{
    for (auto&& str : { "asdf", "racecar" })
    {
        bool is_pal{ boost::algorithm::is_palindrome(str) };
        std::print("{} is palindrome? - {}\n", str, is_pal);
    }
    return 0;
}
