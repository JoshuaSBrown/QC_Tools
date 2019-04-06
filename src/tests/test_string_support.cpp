
#include "../libcatnip/string_support.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace catnip;
using namespace std;

int main(void) {

  cerr << "Testing: splitSt" << endl;
  {
    string str = "Hello World    Lot of space";
    vector<string> v_str = splitSt(str);
    assert(v_str.at(0).compare("Hello") == 0);
    assert(v_str.at(1).compare("World") == 0);
    assert(v_str.at(2).compare("Lot") == 0);
    assert(v_str.at(3).compare("of") == 0);
    assert(v_str.at(4).compare("space") == 0);
  }

  cerr << "Testing: lastN" << endl;
  {
    string str = "Hi there!";
    string var = lastN(str, 3);
    assert(var.compare("re!") == 0);
  }

  cerr << "Testing: lastStringInPath" << endl;
  {
    string str = "Path/To/Important/File.cc";
    string fileName = lastStringInPath(str);
    assert(fileName.compare("File.cc") == 0);
  }

  cerr << "Testing: getPath" << endl;
  {
    string str = "Path/To/Important/File.cc";
    string filePath = getPath(str);
    cerr << filePath << endl;
    assert(filePath.compare("Path/To/Important/") == 0);
    string str2 = "File.cc";
    string str3 = getPath(str2);
    assert(str3.compare("") == 0);
  }

  cerr << "Testing: cut_end" << endl;
  {
    string str = "Important words";
    string var = cut_end(str, 3);
    assert(var.compare("Important wo") == 0);
  }

  cerr << "Testing: foundStrInSubStr" << endl;
  {
    string str = "This is my world.";
    assert(foundSubStrInStr(str, "my"));
    assert(!foundSubStrInStr(str, "nothing"));

    string str2 =
        " Alpha  occ. eigenvalues --  -10.20023 -10.19942 -10.19578 -10.19497  "
        "-0.76368";
    assert(foundSubStrInStr(str2, " Alpha  occ. eigenvalues --"));
  }

  cerr << "Testing: isAlphabetical" << endl;
  {
    string str = "This has spaces";
    assert(isAlphabetical(str) == false);
    string str2 = "JustLetters";
    assert(isAlphabetical(str2) == true);
    string str3 = "";
    assert(isAlphabetical(str3) == false);
  }

  cerr << "Testing: grabStrAfterFisrtOccurance" << endl;
  {
    string str = "Words and Numbers 023D-23 blah";
    string str2 = grabStrAfterFirstOccurance(str, "D");
    assert(str2.compare("-23 blah") == 0);
    string str3 = grabStrAfterFirstOccurance(str, "MOM");
    assert(str3.compare("") == 0);
  }

  cerr << "Testing: grabStrAfterLastOccurance" << endl;
  {
    string str = "This.is.a.file.name";
    string str2 = grabStrAfterLastOccurance(str, ".");
    assert(str2.compare("name") == 0);
    string str3 = "NoExtfilename";
    string str4 = grabStrAfterLastOccurance(str3, ".");
    assert(str4.compare("") == 0);
  }

  cerr << "Testing: grabStrBeforeFisrtOccurance" << endl;
  {
    string str = "Words and Numbers 023D-23 blah";
    string str2 = grabStrBeforeFirstOccurance(str, "D");
    assert(str2.compare("Words and Numbers 023") == 0);
    string str3 = grabStrBeforeFirstOccurance(str, "DADA");
    assert(str3.compare(str) == 0);
  }

  return 0;
}
