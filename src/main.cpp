#include <cstdint>
#include <iostream>
#include <string_view>

#include <curl/curl.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/std.h>

static size_t callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
    CURL* curl{ nullptr };
    CURLcode res{};

    std::string buffer{};

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        std::cout << buffer << std::endl;
        std::cout << res << std::endl;
    }

    return 0;
}
