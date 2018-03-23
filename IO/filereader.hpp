
#ifndef _FILEREADER_HPP_
#define _FILEREADER_HPP_

#include <string>
#include <fstream>
#include <maps>

class FileReader{
  private:

    typedef void (*SectionReader)(void);

    // Private members
    void open();
    void close();
    virtual void registerSections();
    virtual void validFileName() {};
    void checkSections();
    std::string fileExt_();
 
    // Private attributes
    // key - stores tag of the section
    // value - stores the pattern used to identify the section
    std::map<std::string,std::string> sectionsHeaders_;
    // key - stores tag of the section
    // value - stores the section reader function
    std::map<std::string,SectionReader> sectionReaders_;

    std::string fileName_;
    std::ifstream fid_;
    bool fileOpen_;
  public:
    FileReader(string fileName);    
    std::string getExt();
    void read();
    
};
#endif
