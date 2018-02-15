
#include "string_support.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main(void){
  
  string str = "Hello World    Lot of space";
  vector<string> v_str = splitSt(str);

  for(string t_str : v_str ){
    cout << t_str << endl;
  }

  return 0;
}
