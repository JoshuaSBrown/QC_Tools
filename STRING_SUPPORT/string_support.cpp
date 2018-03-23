
#include "string_support.hpp"
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <istream>
#include <iterator>

using namespace std;
/////////////////////////////////////
// Internal functions 
/////////////////////////////////////

// trim from start (in place)
void ltrim_(string &s) {
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
}

// trim from end (in place)
void rtrim_(string &s) {
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
}

// Split a string up by spaces
vector<string> splitSt(string input){
  istringstream iss(input);
  list<string> tokens;
  copy(istream_iterator<string>(iss),
       istream_iterator<string>(),
       back_inserter(tokens));

  vector<string> values;
  for( std::string item : tokens) {
    if(!item.empty()) values.push_back(item);
  }
  return values;
}

string lastStringInPath(string input){
  size_t found;
  string line;
  line = input;
  while ((int)(found=line.find("/"))!=-1){
    line = line.substr(found+1,line.size());
  }
  return line;
}

string lastN(string input, int n) {
  return input.substr(input.size()-n);
}

string cut_end(string input, int n) {
  return input.substr(0,input.size()-n);
}

string firstN(string input, int n) {
  return input.substr(0,n);
}

string cut_beg(string input, int n) {
  return input.substr(n,input.size());
}

// trim from both ends (in place)
void trim(string &s) {
  ltrim_(s);
  rtrim_(s);
}

// trim from start (copying)
string ltrimmed(string s) {
  ltrim_(s);
  return s;
}

// trim from end (copying)
string rtrimmed(string s) {
  rtrim_(s);
  return s;
}


// trim from both ends (copying)
string trimmed(string s) {
  trim(s);
  return s;
}

void removeSpace(char* s) {
  char* s2 = s;
  do {
    if (*s2 != ' ')
      *s++ = *s2;
  } while (*s2++);
}

bool foundSubStrInStr(string s, string sub_s){
  return s.find(sub_s)!=-1;
}
