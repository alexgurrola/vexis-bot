// builds initial file for format testing
#include <iostream>
#include <fstream>

using namespace std;

int main () {
	
	ofstream fout("test");
	
	fout << "[links]\r\n";
	fout << "www.versidyne.com:\r\n";
	fout << "www.google.com:\r\n";
	fout << "\r\n";
	fout << "[data]\r\n";
	fout << "www.versidyne.com:\r\n";
	fout << "www.google.com:\r\n";
	fout << "\r\n";
	
	fout.close();
	
	return 0;
	
}

