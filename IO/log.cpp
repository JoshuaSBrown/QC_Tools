
#include <string>
#include <map>
#include <exception>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost;

map<string,map<string,vector<double>>> logReader::getOrb(void){
  
  path p{this->fileName_};
  if(!exists(p)) throw invalid_argument("The file "+fileName_+" does not exist");
  if(!is_regular_file(p)) throw invalid_argument("The file "+fileName_+" is not a regular file");
  if(is_directory) throw invalid_argument(""+fileName_+" is a directory");

  if(!iequals(filesystem::extension(fileName_),".log")) throw invalid_argument("Wrong file type, you need a .log file");

  regex orbTitle("      Gross orbital populations:");
  regex nexTitle("         Condensed to atoms");
  smatch matchStr;
  string line;
  ifstream myfile(fileName_);
  if(myfile.is_open()){
    while(getline(myfile,line))
    {
      if(regex_search(line,matchStr,orbTitle))
      {
        getline(myfile,line);
        while(getline(myfile,line))
        {
          vector<string> columnsStr;
          algorithm::split(columnsStr,line,is_any_of("\t "),token_compress_on); 
          
          size_t atm_line_size = columnsStr.size();

          string elemType = columnsStr.at(2);
          string orbType  = columnsStr.at(3);
        
          vector<double> orbVals;

          // The number of columns will depend on whether an unrestricted
          // restricted calculation was used
          if(columnStr==8){
            orbVals.push_back(lexical_cast<double>(columnsStr.at(4)));
            orbVals.push_back(lexical_cast<double>(columnsStr.at(5)));
            orbVals.push_back(lexical_cast<double>(columnsStr.at(6)));
            orbVals.push_back(lexical_cast<double>(columnsStr.at(7)));
          }else{
            orbVals.push_back(lexical_cast<double>(columnsStr.at(4)));
          }

          map<std::string,std::vector<double>>> orbMap;
          orbMap[orbType] = orbVals;
          orb_[elemType]   = orbMap;
        
          getline(myfile,line);
          algorithm::split(columnsStr,line,is_any_of("\t "),token_compress_on); 
          while(columnsStr.size()<atm_line_size)
          {
            orbType = columnsStr.at(1);
            if(columnStr==8){
              orbVals.push_back(lexical_cast<double>(columnsStr.at(2)));
              orbVals.push_back(lexical_cast<double>(columnsStr.at(3)));
              orbVals.push_back(lexical_cast<double>(columnsStr.at(4)));
              orbVals.push_back(lexical_cast<double>(columnsStr.at(5)));
            }else{
              orbVals.push_back(lexical_cast<double>(columnsStr.at(2)));
            }

            map<std::string,std::vector<double>>> orbMap;
            orbMap[orbType] = orbVals;
            orb_[elemType]   = orbMap;

            getline(myfile,line);
            algorithm::split(columnsStr,line,is_any_of("\t "),token_compress_on); 
            if(regex_search(line,matchStr,nexTitle)) break;

          }
            
          if(regex_search(line,matchStr,nexTitle)) break;
        }
      }
    }
  }

  return orb_;
}
