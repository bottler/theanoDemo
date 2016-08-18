#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include<chrono>
#include<thread>
using std::system;
using std::string;
int main(int argc, char** argv){
  std::string s = "/csc/tinis/software/Core/CUDA/7.5.18/bin/nvlink ";
  for(int i=1; i<argc; ++i)
  {
	string arg = argv[i];
	for(int j=0; j<arg.size(); ++j)
	{
	  char c = arg[j];
	  if(c=='\t') s += "\\\t";
	  else if(c=='\\') s += "\\\\";
	  else if(c=='\n') s += "\\\n";
	  else if(c=='%') s += "\\%";
	  else if(c==' ') s += "\\ ";
	  else s += c;
	}
	s += ' ';
  }
  //std::cout<<s<<'\n';	
  std::system (s.c_str());
  std::chrono::seconds duration(5);
  std::this_thread::sleep_for(duration);
}

