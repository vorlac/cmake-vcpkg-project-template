#include <utility>

#include <fmt/format.h>
#include <mysqlx/xapi.h>

int main()
{
    auto test = mysqlx_opt_type_enum::MYSQLX_OPT_HOST;
    fmt::println("test = {}", std::to_underlying(test));
}
