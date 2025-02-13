// Mayabason, Brix
// Tusi, Gena
// Sonon, Hugh
// ENGG 151.01 - Project 1

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>


using namespace std;

// 70 Character Long Line
//12345678901234567890123456789012345678901234567890123456789012345678

class engg151Signal
{
  public:
    engg151Signal ();
    // default constructor
    // suggested default: one-element signal with value 0.0, start index 0
    engg151Signal ( double * x, int start, int duration );
    bool importSignalFromFile ( string filename);
    // returns true if a valid signal was actuallly obtained from filename
    // returns false otherwise
    bool exportSignalToFile ( string filename );
    // returns true if the signal was successfully exported to a file
    // returns false otherwise
    int start(); 
    int end();
    int duration();
    // as implied by the names,
    // return the start index, end index, and duration of the signal
    // respectively
    double * data();
    // returns a pointer to an array of double containing the signal values
};

bool parseFileInput(string filepath, engg151Signal signalEx)
{
  while (true)
  {
    bool fileInputSuccessful = false;
    bool filepathChanged = false;

    while (!fileInputSuccessful)
    {
      ifstream file(filepath.c_str());
      if (!file.good())
      {
        string changedFilepath = filepath += ".txt";
        ifstream file(changedFilepath.c_str());
        if (!file.good())
        {
          cout << "Something seems to be wrong." << endl;
          cout << "Appended .txt file and continuing..." << endl;
          filepathChanged = true;
        } else
        {
          cout << "Something seems to be wrong with: " << filepath << endl;
          cout << "Maybe try again?" << endl;
          return false;
        }
      }

      vector<double> varVector;

      double a, b;
      string c, d;

      int index;
      double signal1, signal2;

      // cout << "Test" << endl;
      int firstLineErrors = 0;
      bool firstLine = false;
      bool indexPresent = false;
      string s;
      vector<string> v;

      int i = 0;
      int n = 0;
      for (string line; getline(file, line); )
      {
        istringstream iss(line);
        if (n == 0) // Needed to check if first line has two or one numbers.
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
            cout << "Serious error detected. Aborting program." << endl;
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
          if (!(iss >> b))
          {
            cout << "Error detected at line " << n+1 << endl;
            continue;
          }
          varVector.push_back(b);
        }
        
        if (n != 0)
        {
          varVector.push_back(a);
        } else
        {
          varVector.push_back(b);
        }
      }
      
    }
  }
}

int main(int argc, char * argv[])
{
  int i;
  for (i = 0; i < argc; i++)
  {
    cout << "Argument " << i << " = " << argv[i] << endl;
  }
  string filepath;
  // For debugging
  // filepath = "C:\\Users\\user\\Downloads\\basic-test-p3-engg21-2023-0.csv";
  // cout << "Hello World!" << endl;

  while (true)
  {
    
    // cout << "Hello World!" << endl;
    // Hardcoding for testing purposes
    /*
    cout << "Enter the filepath for the first file" << endl;
    cout << "Example: C:\\Users\\user 1\\Desktop\\first.text" << endl;
    cout << "Input: ";
    getline(cin, filepath);
    */

    bool fileInputSuccessful = false;

    while (!fileInputSuccessful)
    {
      filepath = "C:\\Users\\user\\engg-151\\x_rawn.text";
      ifstream file(filepath.c_str());
      
      if (!file.good())
      {
        cout << "That doesn't seem to be a file..." << endl;
        cout << "Take care not to add quotation marks"
          << " and try again" << endl;
        continue;
      }
      // cout << "Test2" << endl;
      vector<double> signalVector;

      int index;
      double a, b;
      double signal1, signal2;

      // cout << "Test" << endl;
      int firstLineErrors = 0;
      bool firstLine = false;
      bool indexPresent = false;
      string s;
      vector<string> v;

      int i = 0;
      int n = 0;
      for (string line; getline(file,line); )
      {
        istringstream iss(line);
        if (n == 0)
        {
        }
        else
        {
          if (!(iss >> b))
          {
            cout << "Invalid data detected at line " << n + 1 << endl;
            break;
          }
        }
        cout << a << b << endl;
        n++;
      }
    }
  }

  //return 0;
}
