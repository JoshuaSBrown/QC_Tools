#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <locale>
#include <functional>
#include <stdexcept>
#include <exception>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <sys/stat.h>
#include <iterator>
#include <math.h>
#include <list>

#include "io.hpp"
#include "../MATRIX/matrix.hpp"
#include "../STRING_SUPPORT/string_support.hpp"
#include "../PARAMETERS/parameters.hpp"

using namespace std;

void show_usage(string name) {
	
	cerr << "Usage: " << name << " <options(s)> SOURCES"
						<< '\n'
						<< "Options:\n"
						<< "\t-h,      --help             \tShow this help message\n"
						<< "\t-l_1,    --log1  PATH/LOGFILE\tSpecify the Gaussian .log file\n"
						<< "\t                            \tfor monomer 1 this is important\n"
						<< "\t                            \tfor determining the HOMO and LUMO\n"
						<< "\t-l_2,    --log2  PATH/LOGFILE\tSpecify the Gaussian .log file\n"
						<< "\t                            \tfor monomer 2 this is important\n"
						<< "\t                            \tfor determining the HOMO and LUMO\n"
						<< "\t-l_P,    --logP  PATH/LOGFILE\tSpecify the Gaussian .log file\n"
						<< "\t                            \tfor the dimer pair, must contain\n"
						<< "\t                            \tthe overlap matrix for the dimer\n"
						<< "\t-p_1,    --pun1  PATH/PUNFILE\tSpecify the Guassian .pun file\n"
						<< "\t                            \tfor the first monomer\n"
						<< "\t-p_2,    --pun2  PATH/PUNFILE\tSpecify the Guassian .pun file\n"
						<< "\t                            \tfor the second monomer\n"
						<< "\t-p_P,    --punP  PATH/PUNFILE\tSpecify the Gaussian .pun file\n"
						<< "\t                            \tshould be an interger value\n"
						<< endl;
}

int file_exist(char * name){
  struct stat buffer;
  return (stat (name, &buffer) == 0);
}

vector<double> readGausCoefLine(string line){
  vector<double> values;

  int indent=0;
  while((line.length()-indent)>=15){
    string str = line.substr(0+indent,11);
    str = trimmed(str);
    double temp_d = atof(str.c_str());
    str = line.substr(12+indent,3);
    str = trimmed(str);
    double temp_d2 = atof(str.c_str());
    values.push_back(temp_d*pow(10,temp_d2));
    indent += 15;
  }
  return values;
}

vector<double> readGausLogOrbLine(string line){
  
  vector<double> values;
  size_t found1 = 28;
  size_t index  = 28;
  string str = line.substr(found1,10);
  str = trimmed(str);
  while(!str.empty()){
    index = index+10;
    if(index<=line.length()){
      values.push_back(stod(str));
      found1 = found1+10;
      str = line.substr(found1,10);
      str = trimmed(str);
    }
  }
  return values;
}

Parameters check_arguments(char * argv[], int argc  ){
	
  Parameters par;
  string log;
	
	if(argc <= 1) {
		show_usage(argv[0]);
		throw runtime_error("Must provide arguments");
	}

	int logP_flag;
	int log1_flag;
	int log2_flag;
	int err_exit_flag;

	logP_flag = 0;
	log1_flag = 0;
	log2_flag = 0;
	err_exit_flag = 0;

	string temp;
	string HOMO1log;
	string HOMO2log;
	string P_log;
	string arg;
	string ext;

	for( int i=1;i<argc;i++){
		arg=argv[i];
		if((arg=="-h")||(arg=="--help")){
			show_usage(argv[0]);
			throw invalid_argument("No parameters were passed");
		}else if ((arg=="-l_2")||(arg=="--log2")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					temp = argv[i+1];
					ext = lastN(temp,4);
					if(ext==".log"){
						if(file_exist(const_cast<char*>((temp).c_str()))==0){
							cerr << "ERROR The file "+(temp)+" does not exist!" << endl;
							err_exit_flag = -1;
						}else{
							log1_flag = 1;
							HOMO1log = temp;
              par.setLog1(temp);
						}
						i++;
					}else{
						cerr << "WARNING --log2 option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --log2 option requires *.log file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --log2 option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else if ((arg=="-l_1")||(arg=="--log1")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					temp = argv[i+1];
					ext = lastN(temp,4);
					if(ext==".log"){
						if(file_exist(const_cast<char*>((temp).c_str()))==0){
							cerr << "ERROR The file "+(temp)+" does not existi!" << endl;
							err_exit_flag = -1;
						}else{
							log2_flag = 1;
							HOMO2log = temp;
              par.setLog2(temp);
						}
						i++;
					}else{
						cerr << "WARNING --log1 option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --log1 option requires *.log file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --log1 option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else if ((arg=="-l_P")||(arg=="--logP")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					log = argv[i+1];
					ext = lastN(log,4);
					if(ext==".log"){
						if(file_exist(const_cast<char*>((log).c_str()))==0){
							cerr << "ERROR The file "+(log)+" does not exist!" << endl;
							err_exit_flag = -1;
						}else{
							logP_flag = 1;
              par.setLogP(argv[i+1]);
						}
						i++;
					}else{
						cerr << "WARNING --logP option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --logP option requires *.log file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --logP option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else if ((arg=="-p_2")||(arg=="--pun2")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					string pun2 = argv[i+1];
					ext = lastN(pun2,4);
					if(ext==".pun"){
						if(file_exist(const_cast<char*>((pun2).c_str()))==0){
							cerr << "ERROR The file "+(pun2)+" does not exist!" << endl;
							err_exit_flag = -1;
						}
            par.setPun2(pun2);
						//Check if log file exist with same name as pun2 file
						temp = cut_end(pun2,4);
						temp = temp + ".log";
						if(file_exist(const_cast<char*>((temp).c_str()))!=0){
							if(log1_flag==0){
								log1_flag = 1;
								HOMO1log = temp;
                par.setLog1(temp);
							}
						}
						i++;
					}else{
						cerr << "WARNING --pun2 option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --pun2 option requires *.pun file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --pun2 option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else if ((arg=="-p_1")||(arg=="--pun1")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					string pun1 = argv[i+1];
					ext = lastN(pun1,4);
					if(ext==".pun"){
						if(file_exist(const_cast<char*>((pun1).c_str()))==0){
							cerr << "ERROR The file "+(pun1)+" does not exist!" << endl;
							err_exit_flag = -1;
						}
						//Check if log file exist with same name as pun1 file
            par.setPun1(pun1);
						temp = cut_end(pun1,4);
						temp = temp + ".log";
						if(file_exist(const_cast<char*>((temp).c_str()))!=0){
							if(log2_flag==0){
								log2_flag = 1;
								HOMO2log = temp;
                par.setLog2(temp);
							}
						}
						i++;
					}else{
						cerr << "WARNING --pun1 option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --pun1 option requires *.pun file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --pun1 option requires one argument!" << endl;
				err_exit_flag = -1;
			}
	
		}else if ((arg=="-p_P")||(arg=="--punP")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					string punP = argv[i+1];
					ext = lastN(punP,4);
					if(ext==".pun"){
						if(file_exist(const_cast<char*>((punP).c_str()))==0){
							cerr << "ERROR: The file "+(punP)+" does not exist!" << endl;
							err_exit_flag = -1;
						}
						//Check if log file exist with same name as pun1 file
						temp = cut_end(punP,4);
						temp = trimmed(temp + ".log");
            par.setPunP(punP);
						if(file_exist(const_cast<char*>(temp.c_str()))!=0){
							logP_flag = 2;
							P_log = temp;
						}
						i++;
					}else{
						cerr << "WARNING --punP option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --punP option requires *.pun file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --punP option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else {

			cerr << "WARNING the following is not an option " << arg << "\n";
		}
	}

  par.setHOMO_MO1Alpha(log_getHOMO(HOMO1log,"Alpha"));
  cout << "getting beta" << endl;
  par.setHOMO_MO1Beta(log_getHOMO(HOMO1log,"Beta"));
  par.setHOMO_MO2Alpha(log_getHOMO(HOMO2log,"Alpha"));
  par.setHOMO_MO2Beta(log_getHOMO(HOMO2log,"Beta"));

  
	if(logP_flag ==2 && err_exit_flag!=-1){
    par.setLogP(P_log);
	}

	if(argc<7){
		cerr << "ERROR Must have a *.pun file for each monomer and the dimer,\n";
		cerr << "and a *.log file for the dimer containing the overlap matrix,\n";
		cerr << "to execute. By default the program will check to see if the\n";
		cerr << "log file has the same name as the pun files" << endl;
		show_usage(argv[0]);
		err_exit_flag = -1;
	}
	
	if(err_exit_flag==-1){
		throw invalid_argument("problem with executable arguments");
	}
  cerr << "Return"<< endl;
	return par;
}

int check_string_input(string str){

	if(str=="--log1" || str=="-l_1"){
		return 0;
	}else if(str=="--log2" || str=="-l_2"){
		return 0;
	}else if(str=="--logP" || str=="-l_P"){
		return 0;
	}else if(str=="--help" || str=="-h"){
		return 0;
	}else if(str=="--pun1" || str=="-p_1"){
		return 0;
	}else if(str=="--pun2" || str=="-p_2"){
		return 0;
	}else if(str=="--punP" || str=="-p_P"){
		return 0;
	}else{
		return -1;
	}
}

string fileExt(string path){
  path = lastStringInPath(path);
  path = trimmed(path);
  return path.substr(path.find("."));
}

bool fileValid(string fileName, string ext){

  string extension = fileExt(fileName);
	if(extension.compare(ext)==0){
		if(file_exist(const_cast<char*>((fileName).c_str()))==0){
			cerr << "The file "+(fileName)+" does not exist." << endl;
      return false;
		}
	}else{
		cerr << "The file "+(fileName)+" does not have the correct extension." << endl;
		cerr << "The file must be a "+ext+" file." << endl;
		return false;
	}

  return true;
}

int pun_countMO(string pun,string orb_type){
  
  if(!fileValid(pun,".pun")) return -1;
  if(!orb_type.compare("Alpha")==0 && 
     !orb_type.compare("Beta")==0){
     cerr << "Unrecognized orbital type" << endl;
     return -1;
  }

	size_t found;
  int MO;
	ifstream PunFile;
	PunFile.open(const_cast<char*>((pun).c_str()),ifstream::in);
	if(PunFile.is_open()){
		MO = 0;
    string line;
		while(getline(PunFile,line)){
			if(((int)(found=line.find(orb_type)))!=-1){
        MO++;
			}
		}
	}
	PunFile.close();
	return MO;
}

int pun_getMO(string pun, Matrix *mat_Coef, string orb_type){

  if(!fileValid(pun,".pun")) return -1;
  if(!orb_type.compare("Alpha")==0 && 
     !orb_type.compare("Beta")==0){
     cerr << "Unrecognized orbital type" << endl;
     return -1;
  }

	int flag;

	ifstream PunFile;

	int MO = pun_countMO(pun,orb_type);
		
	(*mat_Coef).resize(MO,MO);

  bool start_recording;
	
	PunFile.open(const_cast<char*>((pun).c_str()),ifstream::in);
	if(PunFile.is_open()){
		int i = 0;
		int j = 0;
		//Skip the first line
    string line;
		getline(PunFile,line);

		while(getline(PunFile,line) && i<=MO ){
      size_t found;
      string orb_energy = orb_type+" MO OE";
			flag = (int)(found=line.find(orb_energy));

			if(flag!=-1){
        ++i;
        j=0;
        start_recording = true;
			}else if(start_recording && i<=MO){
        auto coef = readGausCoefLine(line);
        for( auto itr : coef ){
          ++j;
          (*mat_Coef).set_elem(itr,i,j);
        }
			}
		}
	}

	return 0;
}

int log_countMO(string log, string orb_type){

  if(!fileValid(log,".log")) return -1;
  if(!orb_type.compare("Alpha")==0 && 
     !orb_type.compare("Beta")==0){
     cerr << "Unrecognized orbital type" << endl;
     return -1;
  }

	
	/* If there is more than one set of coefficients will read
   * only from the first one */

	//string str;
	ifstream LogFile;

  int MO = 0;
	LogFile.open(const_cast<char*>((log).c_str()),ifstream::in);
	if(LogFile.is_open()){

		int hint = 0;
	  string line;
    int flag1 = 0;
    int flag2 = 0;
		while(getline(LogFile,line) && ( hint==0 || (flag1!=-1 || flag2!=-1))){


      size_t found1, found2;
			flag1 = (int)(found1=line.find(orb_type+"  occ. eigenvalues -- "));
			flag2 = (int)(found2=line.find(orb_type+" virt. eigenvalues -- "));
			
			if(flag1!=-1 || flag2!=-1){
        hint = 1;
        auto values = readGausLogOrbLine(line);
        MO+=static_cast<int>(values.size());
			}
		}
	}
	return MO;
}

int log_getS(string log, Matrix *mat_S, int MO){

	string ext;
	ext = lastN(log,4);
	if(ext==".log"){
		if(file_exist(const_cast<char*>((log).c_str()))==0){
			cerr << "The file "+(log)+" does not exist." << endl;
			cerr << "Cannot execute log_getS function." << endl;
			return -1;
		}
	}else{
		cerr << "The file "+(log)+" does not have the correct extension." << endl;
		cerr << "The file must be a .log file, it should have the same" << endl;
		cerr << "format as the Gaussian .log file, cannot execute " << endl;
		cerr << "log_getS function." << endl;
		return -1;
	}
	
	if(MO<=0){
		cerr << "The total number of Molecular orbitals" << endl;
		cerr << "submitted to the log_getS function must" << endl;
		cerr << "be greater than 0." << endl;
		return -1;
	}

	int flag1;
	int flag2;
	int count1;
	int count2;
	int count2Correction;
	int count3;
	int Total;
	int Column;
	int MatrixCol;
	int MatrixColInit;
	int index;
	double val;

	/* Only need to read the first overlap matrix
     * I am not sure what the others are for */
	int hint;

	size_t found1;
	
	string str1;
	string str2;
	string line;
	ifstream LogFile;

	(*mat_S).resize(MO,MO);

	LogFile.open(const_cast<char*>((log).c_str()),ifstream::in);
	if(LogFile.is_open()){

		flag2 = 0;
		hint = 0;

		while(getline(LogFile,line)){
			flag1 = (int)(found1=line.find(" *** Overlap ***"));

      if( LogFile.peek()==EOF) break;

			if(flag1!=-1){
				
				if(hint==1){
					break;
				}
				MatrixColInit = 1;
				count2Correction = 0;
				hint = 1;			
				flag2 = 1;
				//Skip first line
				getline(LogFile,line);
				str1 = line.substr(8,10);
				str1 = trimmed(str1);
				count1 = 1;
				Total = MO;
				while (count1<=MO){
					count2 = 1;
					Column = 1;
					while (count2<=Total){
						getline(LogFile,line);
						count3 = 1;
						index = 8;
						MatrixCol = MatrixColInit;

						while (count3<=Column){
							str1 = line.substr(index,9);
							str1 = trimmed(str1);
							str2 = line.substr(index+10,3);
							str2 = trimmed(str2);
							val = stod(str1)*pow(10,stod(str2));
							index = index+14;
							(*mat_S).set_elem(val,count2+count2Correction,MatrixCol);
							if((count2+count2Correction)!=MatrixCol){
								(*mat_S).set_elem(val,MatrixCol,count2+count2Correction);
							}
							count3++;
							MatrixCol++;
						}
						if (Column<5){
							Column++;
						}
						count2++;
					}	
					count2Correction = count2Correction+5;
					getline(LogFile,line);
					MatrixColInit = MatrixColInit+5;
					Total = Total-5;
					count1++;
				}
			}
		}
	}

	if(flag2==0){
		cerr << "The file "+(log)+" does not appear to contain the" << endl;
		cerr << "overlap matrix, cannot execute log_getS function, be" << endl;
		cerr << "sure to include the following keywords in your gaussian" << endl;
		cerr << "run file." << endl;
		cerr << "iop(3/33=4) pop=full" << endl;
		return -1;
	}

	return 0;
}

int log_getHOMO(string log, string orb_type){

  if(!fileValid(log,".log")) return -1;
  if(!orb_type.compare("Alpha")==0 && 
     !orb_type.compare("Beta")==0){
     cerr << "Unrecognized orbital type" << endl;
     return -1;
  }

	int HOMO;

	ifstream LogFile;

	size_t found;

	int MO = log_countMO(log,orb_type);

	LogFile.open(const_cast<char*>((log).c_str()),ifstream::in);
	if(LogFile.is_open()){

		int flag = 0;
		HOMO = 0;
		int j = 0;
    string line;
		while(std::getline(LogFile,line) && j<MO && flag < 3){

			if(((int)(found=line.find("*** Overlap ***")))!=-1){
				flag = 1;
				getline(LogFile,line);
			}
			if(((int)(found=line.find(orb_type+"  occ")))!=-1){
				flag = 2;
			}
			if(((int)(found=line.find(orb_type+" virt")))!=-1){
				flag = 3;
			}
			if(flag==2){
				int indent = 0;
				int k = 1;
				while(k<=5 && ((int)line.length())>=(29+indent+9) ){
					indent += 10;
					k++;
					j++;
					HOMO++;
				}
			}
		}
	}
  
  cout << "HOMO found " << HOMO << endl;
	return HOMO;
}

vector<double> log_getMOEnergies(string log,string orb_type){

  vector<double> Energies;

  if(!fileValid(log,".log")) return Energies;
  if(!orb_type.compare("Alpha")==0 && 
     !orb_type.compare("Beta")==0){
     cerr << "Unrecognized orbital type" << endl;
     return Energies;
  }

	string str;
	string line;
	ifstream LogFile;
  cout << "Reading from " << log << endl;
	LogFile.open(const_cast<char*>((log).c_str()),ifstream::in);
	if(LogFile.is_open()){

    // If there is more than one set of coefficients will read
    // only from the first one 

		int hint = 0;
		while(getline(LogFile,line)){

	    size_t found1;
	    size_t found2;
			int flag1 = (int)(found1=line.find(" "+orb_type+"  occ. eigenvalues -- "));
			int flag2 = (int)(found2=line.find(" "+orb_type+" virt. eigenvalues -- "));
			

			if(flag1!=-1 || flag2!=-1){

				// This means we have found a 
        // second set of coefficents the second set
        // is likely to be the more up to date one 
        // so we will use it instead
				if(hint==1){
          Energies.clear(); 
				}

        auto vec_str = splitSt(line);
        for( size_t inc=4 ;inc<vec_str.size();inc++ ){
          Energies.push_back((double)atof(vec_str.at(inc).c_str()));
        }
			}

      if( LogFile.peek()==EOF) break;
		}
	}
	return Energies;
}

vector<vector<double>> log_getCoord(string log){

  vector<vector<double>> Coord;
  vector<double> X;
  vector<double> Y;
  vector<double> Z;
  cerr << "in getCoord" << endl;

  if(!fileValid(log,".log")) return Coord;

	string str;
	string line;
	ifstream LogFile;
  cerr << "File is valid" << endl;

	LogFile.open(const_cast<char*>((log).c_str()),ifstream::in);
	if(LogFile.is_open()){

    // If there is more than one set of coefficients will read
    // only from the first one 

		int hint = 0;
		while(getline(LogFile,line)){


	    size_t found1;
      size_t found2;

			int flag1 = (int)(found1=line.find("Center     Atomic      Atomic             Coordinates (Angstroms)"));
			int flag2 = (int)(found2=line.find("Center     Atomic     Atomic              Coordinates (Angstroms)"));

      if(flag2!=-1) {
        flag1 = flag2;
        found1 = found2;
      }
  
			if(flag1!=-1){

        cerr << "Hint value " << hint << endl;
				// This means we have found a 
        // second set of coefficents the second set
        // is likely to be the more up to date one 
        // so we will use it instead
				if(hint==1){
          cerr << "Clearing values" << endl;
          Coord.clear(); 
          X.clear();
          Y.clear();
          Z.clear();
				}

        // Skip the 2nd and third line 
        getline(LogFile,line);
        getline(LogFile,line);
        getline(LogFile,line);

        // While the line does not match the end of the table read in the coordinates
        while((((int)(found1=line.find("---------------------------------------------------------------------")))==-1)){
          auto vec_str = splitSt(line);
          X.push_back((double)atof(vec_str.at(3).c_str()));
          Y.push_back((double)atof(vec_str.at(4).c_str()));
          Z.push_back((double)atof(vec_str.at(5).c_str()));
          getline(LogFile,line);
        }
        hint=1;
			}

      if( LogFile.peek()==EOF) break;
		}
	}else{
    cerr << "ERROR unable to open log file" << endl;
    exit(1);
  }
  Coord.push_back(X);
  Coord.push_back(Y);
  Coord.push_back(Z);
	return Coord;
}

vector<int> log_countAtomBasisFunc(string log){
  vector<int> OrbBasisCount;

  if(!fileValid(log,".log")) return OrbBasisCount;

	string str;
	string line;
	ifstream LogFile;

	LogFile.open(const_cast<char*>((log).c_str()),ifstream::in);
	if(LogFile.is_open()){

    // If there is more than one set of coefficients will read
    // only from the first one 

		int hint = 0;
		while(getline(LogFile,line)){

	    size_t found1;
			int flag1 = (int)(found1=line.find("     Gross orbital populations:"));
			

			if(flag1!=-1){

				// This means we have found a 
        // second set of coefficents the second set
        // is likely to be the more up to date one 
        // so we will use it instead
				if(hint==1){
          OrbBasisCount.clear(); 
				}

        // Skip the 2nd line 
        getline(LogFile,line);
        getline(LogFile,line);
  
        // Count the number of rows that are in the line when a new atom is 
        // found
        auto vec_str = splitSt(line);
        int rowCountAtmLine = static_cast<int>(vec_str.size());
        // While the line does not match the end of this orbital populations part of the graph
        int BasisCount = 1;
        getline(LogFile,line);
        while((((int)(found1=line.find("        Condensed to atoms (all electrons):")))==-1)){
          vec_str = splitSt(line);
          if(static_cast<int>(vec_str.size())==rowCountAtmLine){
            OrbBasisCount.push_back(BasisCount);
            BasisCount = 1;
          }else{
            ++BasisCount;
          }
          getline(LogFile,line);
        }
        OrbBasisCount.push_back(BasisCount);
			}

      if( LogFile.peek()==EOF) break;
		}
  }
  return OrbBasisCount;
}

bool restricted(int MOPAlpha, int MOPBeta){
  if(MOPBeta==0){
    cout <<"Assuming Restricted HF" << endl;
    cout <<"Number of Alpha MO " << MOPAlpha << endl;
    return true;
  }
    cout <<"Assuming Unrestricted HF" << endl;
    cout <<"Number of Alpha MO " << MOPAlpha << endl;
    cout <<"Number of Beta  MO " << MOPBeta  << endl;
  return false;
}

vector<Matrix *> readFilesCoef(Parameters par, string orb_type ){

  vector<Matrix *> MatCo;

  Matrix * mat_P_Coef = new Matrix;
  Matrix * mat_1_Coef = new Matrix;
  Matrix * mat_2_Coef = new Matrix;
  pun_getMO(par.getPunP(),mat_P_Coef,orb_type);
  pun_getMO(par.getPun1(),mat_1_Coef,orb_type);
  pun_getMO(par.getPun2(),mat_2_Coef,orb_type);

  MatCo.push_back(mat_P_Coef);
  MatCo.push_back(mat_1_Coef);
  MatCo.push_back(mat_2_Coef);
  
  return MatCo;
}

vector<Matrix *> readFilesEnergies(Parameters par, string orb_type ){

  vector<Matrix *> MatCo;

  vector<double> vecP_MO_Energy; 
  vector<double> vec2_MO_Energy; 
  vector<double> vec1_MO_Energy; 
  vecP_MO_Energy = log_getMOEnergies(par.getLogP(),orb_type);
  vec2_MO_Energy = log_getMOEnergies(par.getLog2(),orb_type);
  vec1_MO_Energy = log_getMOEnergies(par.getLog1(),orb_type);
  // Convert vectors to matrices
  Matrix * mat_P_OE = new Matrix(vecP_MO_Energy.size());
  Matrix * mat_1_OE = new Matrix(vec1_MO_Energy.size());
  Matrix * mat_2_OE = new Matrix(vec2_MO_Energy.size());

  int index = 1;
  for( auto it : vecP_MO_Energy){
    mat_P_OE->set_elem(it,index);
    index++;
  }
  index = 1;
  for( auto it : vec1_MO_Energy){
    mat_1_OE->set_elem(it,index);
  }
  index = 1;
  for( auto it : vec2_MO_Energy){
    mat_2_OE->set_elem(it,index);
  }
  
  MatCo.push_back(mat_P_OE);
  MatCo.push_back(mat_1_OE);
  MatCo.push_back(mat_2_OE);

  return MatCo;
}
