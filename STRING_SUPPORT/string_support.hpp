
#ifndef _STRING_SUPPORT_HPP_
#define _STRING_SUPPORT_HPP_

#include <string>
#include <vector>

// Split a std::string up by spaces
std::vector<std::string> splitSt(std::string input);

std::string lastStringInPath(std::string input);

std::string lastN(std::string input, int n);

std::string cut_end(std::string input, int n);

std::string firstN(std::string input, int n);

std::string cut_beg(std::string input, int n);

// trim from both ends (in place)
void trim(std::string &s);

// trim from start (copying)
std::string ltrimmed(std::string s);

// trim from end (copying)
std::string rtrimmed(std::string s);

// trim from both ends (copying)
std::string trimmed(std::string s);

int file_exist(std::string name);

void removeSpace(std::string s);

#endif
