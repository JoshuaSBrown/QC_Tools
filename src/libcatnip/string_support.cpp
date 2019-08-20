
#include "string_support.hpp"
#include <algorithm>
#include <iostream>
#include <istream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
/////////////////////////////////////
// Internal functions
/////////////////////////////////////

namespace catnip {

const string PATH_SEPERATOR_ =
#ifdef _WIN32
    "\\/";
#else
    "/";
#endif
// const std::string PATH_SEPERATOR_;
// FROM
// https://stackoverflow.com/questions/12971499/how-to-get-the-file-separator-symbol-in-standard-c-c-or
// A compatible edit for running on windows Command line

// trim from start (in place)
void ltrim_(string &s) {
  s.erase(s.begin(),
          find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
}

// trim from end (in place)
void rtrim_(string &s) {
  s.erase(
      find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
      s.end());
}

// Split a string up by spaces
vector<string> splitSt(const string & input) {
  istringstream iss(input);
  list<string> tokens;
  copy(istream_iterator<string>(iss), istream_iterator<string>(),
       back_inserter(tokens));

  vector<string> values;
  for (std::string item : tokens) {
    if (!item.empty()) values.push_back(item);
  }
  return values;
}

string lastStringInPath(const string& input) {
  size_t found;
  string line;
  line = input;
  while (
      (int)(found = line.find_first_of(PATH_SEPERATOR_)) !=
      -1) {  // http://www.cplusplus.com/reference/string/string/find_first_of/
    line = line.substr(found + 1, line.size());
  }
  return line;
}

string getPath(string input) {
  size_t found = 0;
  size_t found2 = found;
  string val = "";
  string line = input;
  while ((int)found != -1) {
    found = line.find_first_of(
        PATH_SEPERATOR_);  // http://www.cplusplus.com/reference/string/string/find_first_of/
    line = line.substr(found + 1, line.size());
    found2 += found + 1;
    val = input.substr(0, found2);
  }
  return val;
}

string lastN(string input, int n) { return input.substr(input.size() - n); }

string cut_end(string input, int n) {
  return input.substr(0, input.size() - n);
}

string firstN(string input, int n) { return input.substr(0, n); }

string cut_beg(string input, int n) { return input.substr(n, input.size()); }

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

void removeSpace(char *s) {
  char *s2 = s;
  do {
    if (*s2 != ' ') *s++ = *s2;
  } while (*s2++);
}

string grabStrAfterFirstOccurance(string s, string occ) {
  size_t str_len = occ.length();
  size_t pos = s.find(occ);
  if (pos == string::npos) {
    return "";
  }
  return s.substr(pos + str_len);
}

string grabStrAfterLastOccurance(string s, string occ) {
  auto pos = s.find_last_of(occ);
  if (pos == string::npos) return "";
  return s.substr(pos + 1);
}

string grabStrAfterLastOccuranceInclusive(string s, string occ) {
  auto pos = s.find_last_of(occ);
  if (pos == string::npos) return "";
  return s.substr(pos);
}

string grabStrBeforeFirstOccurance(string s, string occ) {
  size_t pos = s.find(occ);
  if (pos == string::npos) return s;
  return s.substr(0, pos);
}

string grabStrBeforeLastOccurance(string s, string occ) {
  auto pos = s.find_last_of(occ);
  if (pos == string::npos) return s;
  return s.substr(0, pos);
}

string grabStrBeforeLastOccuranceInclusive(string s, string occ) {
  auto pos = s.find_last_of(occ);
  if (pos == string::npos) return s;
  return s.substr(0, pos + 1);
}

bool foundSubStrInStr(string s, string sub_s) {
  return s.find(sub_s) != string::npos;
}

bool isAlphabetical(std::string s) {
  return find_if(s.begin(), s.end(), not1(ptr_fun((int (*)(int))isalpha))) ==
             s.end() &&
         !s.empty();
}

}  // namespace catnip
