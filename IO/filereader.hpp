
#ifndef _FILEREADER_HPP_
#define _FILEREADER_HPP_

#include <string>
#include <fstream>
#include <map>

struct FilePackage{
  void * ptr;
};

class FileReader{
  public:
    FileReader(std::string fileName);    
    std::string getExt();
    void read();
  protected:

    typedef void (*FRfunc)(void *);

    // Private members
    void open();
    void close();
    virtual void registerSections_();
    virtual void validFileName_() {};
    void checkSections_();
    void readSection_(std::string tag);
    std::string startSection_(std::string tag);
    std::string fileExt_();
    bool fileExist_(); 
    // Private attributes
    // key - stores tag of the section
    // value - stores the pattern used to identify the section
    std::map<std::string,std::string> sectionHeaders_;
    // key - stores tag of the section
    // value - stores the section reader function
    std::map<std::string,FRfunc > sectionReaders_;

    std::string fileName_;
    std::ifstream fid_;
    bool fileOpen_;
};
#endif
