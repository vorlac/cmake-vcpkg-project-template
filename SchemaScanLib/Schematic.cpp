//
// Created by Andrew on 2/12/2024.
//

#include "Schematic.h"

/**
 * Default constructor for the Schematic class
 * @param fpath The absolute file path to a schematic
 * @throws out_of_range if the file path is incorrect, or if the file does not end in ".pdf"
 */
Schematic::Schematic(const std::string& fpath)
{
    if (std::filesystem::path(fpath).extension() == ".pdf")
    {
        this->setup(fpath);
    }
    else
    {
        std::cerr << "Incorrect file path or type, cannot create Schematic object from: " << fpath
                  << "\n";
        throw std::out_of_range("Incorrect file path or type, skipping");
    }
}

// TODO: Might be unused, need to look into whether anything needs to be done here with smart
// pointers
Schematic::~Schematic()
{
}

/**
 * Setup function for a Schematic object. Sets private class members like the file path, the hash,
 * file name, etc. Additionally calls parsePages() to parse all of the pages in the .pdf
 * @param fpath The absolute file path to a schematic
 */
void Schematic::setup(const std::string& fpath)
{
    this->path_ = fpath;
    this->setHash();
    this->setInfo();
    this->setFileName();
    this->parsePages();
}

/**
 * Parses the .pdf file contained at the path that the Schematic object was created with (path_).
 * Reads the text one page at a time, and adds the string representation to the Schematic object's
 * parsed_pages_ vector. Utilizes the PoDoFo library
 */
void Schematic::parsePages()
{
    PoDoFo::PdfMemDocument document;
    document.Load(this->path_);
    auto& pages = document.GetPages();
    for (unsigned int i = 0; i < pages.GetCount(); ++i)
    {
        PoDoFo::PdfPage& page = pages.GetPageAt(i);
        std::vector<PoDoFo::PdfTextEntry> entries;
        page.ExtractTextTo(entries);
        std::string pageAsString;
        for (auto& entry : entries)
            pageAsString += entry.Text;
        this->parsed_pages_.push_back(pageAsString);
    }
}

/**
 * Sets the md5_hash_ value of a Schematic object to the md5 representation of the file contained at
 * the path_ value of the Schematic object Utilizes the hashlib2plus library
 * @return
 */
void Schematic::setHash()
{
    hashwrapper* wrapper = new md5wrapper();
    std::string hash;
    try
    {
        hash = wrapper->getHashFromFile(this->path_);
    }
    catch (hlException& e)
    {
        std::cerr << "Error computing file hash | " << e.error_number() << " | "
                  << e.error_message() << "\n";
    }
    delete wrapper;
    this->md5_hash_ = hash;
}

/**
 * Sets extra information for the Schematic object. Currently only does the page count, but will be
 * expanded to more in the future
 * TODO: ADD MORE FUNCTIONALITY OR CHANGE TO setPageCount()
 */
void Schematic::setInfo()
{
    PoDoFo::PdfMemDocument document;
    document.Load(this->path_);
    std::vector<PoDoFo::PdfTextEntry> entries;
    PoDoFo::PdfPageCollection& pages = document.GetPages();
    this->page_count_ = pages.GetCount();
}

/**
 * Gets the file name from the absolute path of the Schematic object
 * @return A string containing the file name
 */
void Schematic::setFileName()
{
    std::size_t found = this->path_.find_last_of('/');
    this->file_name_ = this->path_.substr(found + 1);
}

/**
 * Getter for the file name (extension included) contained in a Schematic object
 * @return A string, the file name
 */
std::string Schematic::getFileName() const
{
    return this->file_name_;
}

/**
 * Getter for the absolute file path (file name & extension included) contained in a Schematic
 * object
 * @return A string, the absolute path
 */
std::string Schematic::getFilePath() const
{
    return this->path_;
}

/**
 * Getter for the page count (number of pages parsed) of a Schematic object
 * @return An unsigned integer, the page count
 */
unsigned int Schematic::getPagecount() const
{
    return this->page_count_;
}

/**
 * Getter for the md5 hash of the file contained at the absolute path that the Schematic object was
 * created with
 * @return A string representation of the md5 hash of the file
 */
std::string Schematic::getMD5() const
{
    return this->md5_hash_;
}

/**
 * Getter for the parsed pages of a Schematic object
 * @return A vector of strings, containing the text contents of a parsed .pdf file
 */
std::vector<std::string> Schematic::getParsedPages() const
{
    return this->parsed_pages_;
}

/**
 * Getter for a single parsed page of a Schematic object
 * @param page The number of the page you wish to retrieve
 * @return A string, the parsed text contents of the specific page of the .pdf file
 */
std::string Schematic::getParsedPage(unsigned int page) const
{
    return this->parsed_pages_.at(page + 1);
}
