// Mayabason, Brix
// Tusi, Gena
// Sonon, Hugh
// ENGG 151.01 - Project 1

/*
IMPORTANT:
Code is not yet formatted properly
- the 70 char limit (idk if he still does this)
- the use 'new' rule
*/

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

bool parseFileInput(string filepath)
{
  bool fileInputSuccessful = false;

  // Needed to verify that file exists. Also tries the .txt addition.
  ifstream file(filepath.c_str());
  if (!file.good())
  {
    string changedFilepath = filepath += ".txt";
    ifstream file(changedFilepath.c_str());
    if (!file.good())
    {
      cout << "Something seems to be wrong." << endl;
      cout << "Appended .txt file and continuing..." << endl;
    } else
    {
      cout << "Something seems to be wrong with: " << filepath << endl;
      cout << "Maybe try again?" << endl;
      return false;
    }
  }

  vector<double> varVector; // For storing signal

  double a, b;
  string c, d;

  int index;
  double currentlyTested, tempStore;
  string discardedString;
  double signal1, signal2; 

  // Storing error status is useful to make sure error message
  // only displays once per line
  bool errorPresent = false;
  bool firstLine = false;
  bool indexPresent = false;
  string s;
  vector<string> v;

  int n = 0;
  // Iterator for first line validator
  int i = 0;
  for (string line; getline(file, line); )
  {
    istringstream iss_line(line);
    if (n == 0)
    {
      // Split the string up so that we can extract
      // only the first two. Done to disregard
      // rest of the potential comments

      // Conversion to string is done to allow comparison
      // before and after iss.
      while (getline(iss_line, s, ' '))
      {
          v.push_back(s);
      }
      
      // Signal file format is that only the first two
      // might matter, so rest is discarded.
      // Thus, loop should terminate either when it evaluates
      // two elements or if it's evaluated the lone element in
      // a string that has only one element, whichever comes first.
      for (int i = 0; i < 2 || i > v.size(); i++)
      {
        // Following two lines check if the string
        // currently being tested
        // can be processed into a double.
        istringstream iss_word(v.at(i));
        if (iss_word >> currentlyTested) // Code for if it can
        {
          // Since the fail-bit for istringstream does
          // not trip if it discards trailing letters,
          // this has to be checked manually.

          if (!((iss_word >> currentlyTested >> std::ws).eof()))
          {
            errorPresent = true;
          }
          
          // Simple way of checking is to see if there are
          // bits that were not taken by the "currentlyTested"
          if (v.size() > 1 && (discardedString != v.at(i+1) || discardedString == ""))
          {
            errorPresent = true;
          }
          if (discardedString != v.at(i+1))
          {
            errorPresent = true;
          }

          /*
          // At this point, it is still unknown if
          // the second string could be an input.
          // So the first value obtained is set aside
          // The outer function loops to assess the
          // second string.
          if (i == 0)
          {
            tempStore = currentlyTested;
          } else
          {
          */     
            // The outer if statement checks if
            // the string can be parsed as a double.
            
            // Getting to this point would mean that
            // the second string is valid, but the index
            // should be an integer, not a double.

            // So we need to re-validate the earlier input
            // to see if it changes if it is parsed as an int.

            // Validating it first as a double is necessary, since
            // it is unknown at that point if there is a second number.
          if (i == 1)
          {
            istringstream iss(v.at(0));
            if (!(iss >> index)) 
            {
                cout << "Major problem detected at line." << endl;
                break;
            } else
            {
              // Same checking to see if the length changed.
              if (to_string(index).length() != v.at(i).length())
              {
                errorPresent = true;
              }

              indexPresent = true;
            }
          }
        } else if (i == 0) // If the code fails to parse at the first string
        {
            cout << "Major problem detected at line 1." << endl;
            break;
        } else // If the code only fails to parse at the second string.
        {
            indexPresent = false;
            break;
        }
      }
      varVector.push_back(currentlyTested);
      if (errorPresent)
      {
        cout << "Error detected on the first line. Approximate input used." << endl;
      }
    } else
    {
      if (!(iss >> b))
      {
        // If fatal parsing error happens, code skips
        // this line and continues.
        cout << "Error detected at line " << n+1;
        cout << ". Skipping line." << endl;
        continue;
      }
      v.clear();
      istringstream iss(line);
      while (getline(iss, s, ' '))
      {
        v.push_back(s);
      }
      if (to_string(b).length() != v.at(0).length())
      {
        // Compared to above, this is a softer error that
        // doesn't completely stop the code.
        cout << "Input is potentially malformed at line "
          << n + 1 << ". Approximation used." << endl;
      }

      varVector.push_back(b);
    }
    n++;
  }

  if (!indexPresent)
  {
    index = 0;
  }
  cout << "I read an index of " << index <<" :D" << endl;
  for (double i: varVector)
    cout << i << endl;
  return true;
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

    filepath = "C:\\Users\\user\\engg-151\\x_rawn_with_comment.text";
    parseFileInput(filepath);
    /*
    while (!fileInputSuccessful)
    {
      filepath = "C:\\Users\\user\\engg-151\\x_rawn.text";
    }
    */
    break;
  }

  //return 0;
}
