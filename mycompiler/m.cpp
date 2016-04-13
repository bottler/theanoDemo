#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include<chrono>
#include<thread>
using std::system;
using std::string;
int main(int argc, char** argv){
  std::string s = "g++ ";
  for(int i=1; i<argc; ++i)
  {
	string arg = argv[i];
	for(int j=0; j<arg.size(); ++j)
	{
	  char c = arg[j];
	  if(c==' ' || c=='\\' || c=='\t' || c=='\n' || c=='%')
		s += '\\';
	  s += c;
	}
	s += ' ';
  }
  //std::cout<<s<<'\n';	
  std::system (s.c_str());
  std::chrono::seconds duration(5);
  std::this_thread::sleep_for(duration);
}

