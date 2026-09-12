#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>
using namespace std;


void initial_syntax(){
    cout << "===========================================\n";
    cout << "  Welcome to SCESNA Assembler\n";
    cout << "  SIDDHCESNA-III 16-bit Processor Toolchain\n";
    cout << "===========================================\n";
    return;
}

vector<string> fetch_file(string filename){

    vector<string> filecontent;
    ifstream inputfile(filename);
    string current_line;
    if(!inputfile.is_open()){
        cerr << "couldn't able to fetch the file" << endl;
    }
    
    while(getline(inputfile, current_line)){
        filecontent.push_back(current_line);
    }

    inputfile.close(); // free up memory;

    return filecontent;
}

int main(){
    
    string filename, input;
    vector<string> code;
    initial_syntax();
    while(true){

        getline(cin,input);

        if(input == "exit" || input == "quit"){
            break;
        }

        if(input == "assemble"){

            cout << "Enter the filename" << endl;
            getline(cin, input);
            code = fetch_file(input);
            
        }

        if(input == "help"){
            initial_syntax();
        }
    }
    return 0;
}

