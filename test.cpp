#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

int main ()
{
    string filepath = "C:\\Users\\user\\engg-151\\x_rawn_with_comment.text";
    ifstream file(filepath.c_str());
    int index;
    double signal1, signal2;
    // b = 4;
    int n = 0;
    
    bool firstLine = false;
    bool indexPresent = false;
    string s;
    vector<string> v;

    int firstLineErrors = 0;
    int i = 0;

    for (string line; getline(file, line); )
      {
        // cout << n;
        
        istringstream iss(line);
        if (n == 0)
        {
            while (getline(iss, s, ' '))
            {
                v.push_back(s);
            }
            
            while (i < 2)
            {
                istringstream iss(v.at(i));
                if (iss >> signal1)
                {
                    if (!isdigit(v.at(i).back()))
                    {
                        firstLineErrors = 1;
                    }
                    if (i == 0)
                    {
                        signal2 = signal1;
                    } else {
                        istringstream iss(v.at(0));
                        if (!(iss >> index))
                        {
                            firstLineErrors = 2;
                            break;
                        }
                        indexPresent = true;
                        char * p;
                        strtol(v.at(0).c_str(), &p, 10);

                        if (*p != 0)
                        {
                            firstLineErrors = 1;
                        }
                    }
                } else if (i == 0)
                {
                    firstLineErrors = 2;
                    break;
                } else
                {
                    index = 0;
                    break;
                }

                i++;
            }
            if (firstLineErrors == 2)
            {
                cout << "Serious errors" << endl;
                break;
            } else if (firstLineErrors == 1)
            {
                cout << "Error detected on the first line. Approximate input used." << endl;
            }

            if (indexPresent == true)
            {
                cout << "Index: " << index << ", signal: " << signal1 << endl;
            } else
            {
                cout << "Signal: " << signal2 << endl;
            }
        }
        else
        {
            // iss >> b >> c;
            // cout << "I got " << b << " and " << c << endl;
        }
//      cout << "A: " << a << ", B: " << b << endl;
      n++;
    }
}