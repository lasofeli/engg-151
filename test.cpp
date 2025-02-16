#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

using namespace std;
int main()
{
    string s = "123abc";
    int ld;
    istringstream iss(s);
    iss >> ld;
    cout << iss.eof();
    /*
    if(!(iss >> ld >> std::ws))
    {
        cout << "didn't work" << endl;
    } else
    {
        if (!iss.eof())
        {
            cout << "test" << endl;
        }
    }
        */
    

}