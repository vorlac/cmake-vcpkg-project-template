//
// Created by Andrew on 2/12/2024.
//

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include <hashlibpp.h>
#include <podofo/podofo.h>


#ifndef SCHEMASEARCH_SCHEMATIC_H
  #define SCHEMASEARCH_SCHEMATIC_H

class Schematic
{
public:
    Schematic(const std::string& fpath);
    ~Schematic();
    // Functions
    std::string getFileName() const;
    std::string getFilePath() const;
    unsigned int getPagecount() const;
    std::string getMD5() const;
    std::vector<std::string> getParsedPages() const;
    std::string getParsedPage(unsigned int page) const;

private:
    // Class Members
    std::string file_name_;
    std::string path_;
    unsigned int page_count_ = 0;
    std::string md5_hash_;
    std::vector<std::string> parsed_pages_;
    // Functions
    void setup(const std::string& fpath);
    void setHash();
    void parsePages();
    void setInfo();
    void setFileName();
};

#endif  // SCHEMASEARCH_SCHEMATIC_H
