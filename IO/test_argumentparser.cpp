
#include "argumentparser.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include "../MATRIX/matrix.hpp"

using namespace std;

int main(void){

  cerr << "Testing: ArgumentParser" << endl;
  cerr << "Testing: constructor" << endl;
  {
    vector<string> flag = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    set<vector<string>> flags;
    flags.insert(flag);
    ArgumentParser ArgPars(flags);
  }

  cerr << "Testing: showUsage" << endl;
  {
    vector<string> flag1 = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    vector<string> flag2 = {"-p_1","--punfile-mon1","File containing monomer 1"};
    vector<string> flag3 = {"-homo","--homo-level","HOMO molecular orbital"};
    
    set<vector<string>> flags;
    flags.insert(flag1);
    flags.insert(flag2);
    flags.insert(flag3);

    ArgumentParser ArgPars(flags);
    ArgPars.showUsage();
  }  

  cerr << "Testing: setFlagRule" << endl;
  {
    vector<string> flag1 = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    vector<string> flag2 = {"-p_1","--punfile-mon1","File containing monomer 1"};
    vector<string> flag3 = {"-homo","--homo-level","HOMO molecular orbital"};
    
    set<vector<string>> flags;
    flags.insert(flag1);
    flags.insert(flag2);
    flags.insert(flag3);

    ArgumentParser ArgPars(flags);
   
    string val = ".pun";
    ArgPars.setFlagRule("--punfile-pair","FILE_EXT","ALLOWED_FILE_TYPE",val);
    string val2 = ".7";
    ArgPars.setFlagRule("--punfile-pair","FILE_EXT","ALLOWED_FILE_TYPE",val2);
    string val3 = ".orb";
    ArgPars.setFlagRule("--punfile-pair","FILE_EXT","ALLOWED_FILE_TYPE",val3);
  }

  cerr << "Testing: parse" << endl;
  {
    vector<string> flag1 = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    vector<string> flag2 = {"-p_1","--punfile-mon1","File containing monomer 1"};
    vector<string> flag3 = {"-homo","--homo-level","HOMO molecular orbital"};
    
    set<vector<string>> flags;
    flags.insert(flag1);
    flags.insert(flag2);
    flags.insert(flag3);

    ArgumentParser ArgPars(flags);
   
    string val = ".pun";
    string val2 = ".7";
    string val3 = ".orb";
    set<string> exts = {val,val2,val3};
    ArgPars.setFlagRule("--punfile-pair","FILE_EXT","ALLOWED_FILE_TYPE",exts);
    ArgPars.setFlagRule("--punfile-mon1","FILE_EXT","ALLOWED_FILE_TYPE",exts);

    char * argv[] = {"calc_J","--punfile-pair", "file.pun", "--punfile-mon1","file.orb"};
    int argc = 5;
    ArgPars.parse(argv,argc); 
  }

  cerr << "Testing: get" << endl;
  {
    vector<string> flag1 = {"-p_P","--punfile-pair","File containing dimer of two "
      "monomers. This file should have the .pun extension."};
    vector<string> flag2 = {"-p_1","--punfile-mon1","File containing monomer 1"};
    vector<string> flag3 = {"-homo","--homo-level","HOMO molecular orbital"};
    
    set<vector<string>> flags;
    flags.insert(flag1);
    flags.insert(flag2);
    flags.insert(flag3);

    ArgumentParser ArgPars(flags);
   
    string val = ".pun";
    string val2 = ".7";
    string val3 = ".orb";
    set<string> exts = {val,val2,val3};
    ArgPars.setFlagRule("--punfile-pair","FILE_EXT","ALLOWED_FILE_TYPE",exts);
    ArgPars.setFlagRule("--punfile-mon1","FILE_EXT","ALLOWED_FILE_TYPE",exts);

    char * argv[] = {"calc_J","--punfile-pair", "file.pun", "--punfile-mon1","file.orb"};
    int argc = 5;
    ArgPars.parse(argv,argc); 
  
    string fileName = ArgPars.getStr("--punfile-pair");
    assert(fileName.compare("file.pun")==0);
    string fileName2 = ArgPars.getStr("--punfile-mon1");
    assert(fileName2.compare("file.orb")==0);

  }


  return 0;
}
