#include <iostream>
#include <string>
#include <thread>
using namespace std;

void run(bool& stop){
	string cmd;
	while (!stop){
		cout<<">>";
		//cin>>cmd;
	}
}

void CommandStart(bool& stop){
	thread t(run,stop);
}