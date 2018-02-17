
#include "string_support.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

int main(void){
  
  cerr << "Testing: splitSt" << endl;
  {
    string str = "Hello World    Lot of space";
    vector<string> v_str = splitSt(str);
    assert(v_str.at(0).compare("Hello")==0);
    assert(v_str.at(1).compare("World")==0);
    assert(v_str.at(2).compare("Lot")==0);
    assert(v_str.at(3).compare("of")==0);
    assert(v_str.at(4).compare("space")==0);
    
  }

  cerr << "Testing: lastN" << endl;
  {
    string str = "Hi there!";
    string var = lastN(str,3);
    assert(var.compare("re!")==0);
  }

  cerr << "Testing: lastStringInPath" << endl;
  {
    string str = "Path/To/Important/File.cc";
    string fileName = lastStringInPath(str);
    assert(fileName.compare("File.cc")==0);
  }  

  cerr << "Testing: cut_end" << endl;
  {
    string str = "Important words";
    string var = cut_end(str,3);
    assert(var.compare("Important wo")==0);
  }

  return 0;
}
