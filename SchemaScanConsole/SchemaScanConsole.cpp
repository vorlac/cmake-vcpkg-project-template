#include <iostream>

#include <SchemaScanLib.h>

int main(int argc, char* argv[])
{
    try
    {
        Schematic schematic(argv[0]);
        std::cout << "Parsing " << schematic.getFileName() << "\n";
        std::cout << "----------------------------------------\n";
        std::cout << "MD5 Hash: " << schematic.getMD5() << "\n";
        std::cout << "Pages found: " << schematic.getPagecount() << "\n";
        for (int i = 1; i <= schematic.getPagecount(); ++i)
        {
            std::cout << "Page " << i << ": \n";
            std::cout << schematic.getParsedPage(i) << "\n";
        }
        std::cout << "Parsing complete!";
    }
    catch (std::out_of_range& e)
    {
        std::cout << "Invalid, please try again"
                  << "\n";
    }
}
