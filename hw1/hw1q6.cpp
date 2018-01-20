#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void reverse(ifstream &file) {

	string word;

	if (file >> word) {
		reverse(file);
		cout << word << " ";
	}
}

int main(int argc, char *argv[]) {

	// receive filename as command line parameter 
	string filename = argv[1];

	// create instance of ifstream class and open file
	// must convert variable into c string 
	ifstream inFile(filename.c_str());

	// read in number of words
	int n;
	inFile >> n;

	// call the recursive function
	reverse(inFile);

	inFile.close();

	return 0;
}





