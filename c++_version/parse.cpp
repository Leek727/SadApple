#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string compress(const string & str);

int main()
{
    int max = 20;
    string filename;
    string data; 

    cin >> data; // Get user input from the keyboard

    string file_num = data.substr(0,max); // get first 6 characters as the header - for filename

    //remove placeholders and create filename
    size_t found = file_num.find("-"); 
    if (found != string::npos){
        file_num = file_num.substr(0,found);
    }
    filename = file_num + ".txt";

    ofstream output;
    output.open(filename);
    output << compress(data.substr(max, data.length())); // Display the input value
    output.close();

    return 0;
}

string compress(const string & str){
    int i = str.size();
    string letters;

    for (int j = 0; j < i; ++j){
        int count = 1;
        while (str[j] == str[j+1]){
            count++;
            j++;
        }
        letters += std::to_string(count);
        letters.push_back(str[j]);
    }
    return letters;
}