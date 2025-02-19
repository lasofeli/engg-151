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
  /*
  private:
    
  */
  public:
    double* signalArray;
    int index = 0; 
    engg151Signal (){};
    // default constructor
    // suggested default: one-element signal with value 0.0, start index 0
    engg151Signal ( double * x, int start, int duration );
    bool importSignalFromFile (string filepath)
    {
      // Needed to verify that file exists. Also tries the .txt addition.
      ifstream file(filepath.c_str());
      if (!file.good())
      {
        string changedFilepath = filepath + ".txt";
        ifstream file(changedFilepath.c_str());
        if (file.good())
        {
          cout << "Something seems to be wrong." << endl;
          cout << "Appended .txt file and continuing..." << endl;
        } else
        {
          cout << "Something s  eems to be wrong with: " << filepath << endl;
          cout << "Maybe try again?" << endl;
          return false;
        }
      }

      vector<double> varVector; // For storing signal

      double a, b;
      string c, d;

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
          // two elements or if it reaches the end.
          for (int i = 0; i < 2 || i > v.size(); i++)
          {
            // Following two lines check if the string
            // currently being tested
            // can be processed into a double.
            istringstream iss_word(v.at(i));
            if (iss_word >> currentlyTested) // Code for if it can
            {
              // Since the fail-bit for istringstream does
              // not trip if it stops because of trailing letters,
              // this has to be checked manually.

              // Following checks if iss_word still has remaining
              // characters in the stream. Since we split it up
              // via spaces earlier, the only way this can trigger
              // is if there were trailing characters.
              if (!iss_word.eof())
              {
                errorPresent = true;
              }

              // At this point, it is still unknown if
              // the second string is parsable.
              // So the first value obtained is set aside.
              if (i == 0)
              {
                tempStore = currentlyTested;
              } else
              {        
                // The outer if statement checks if
                // the string can be parsed as a double.
                
                // Getting to this point would mean that
                // the second string is valid, but the index
                // should be an integer, not a double.

                // So we need to re-validate the earlier input
                // to see if it changes if it is parsed as an int.

                // Validating it first as a double is necessary, since
                // it is unknown at that point if there is a second number.
                istringstream iss_word(v.at(0));
                if ((iss_word >> index)) 
                {
                  indexPresent = true;

                  if (!iss_word.eof())
                  {
                    errorPresent = true;
                  }

                  // With index value validated, second string is
                  // now stored as the first signal.
                  varVector.push_back(currentlyTested);
                } else // Just for redundancy, although there shouldn't be any chance of this tripping
                {
                  cout << "Major problem present at line 1." << endl;
                  return false;
                }
              }
            } else if (i == 1)
            {
              // If code only fails to parse the second string,
              // we treat the first as a signal and use the fallback
              // index value of 0.
              index = 0;
              
              // Since currentlyTested got wiped when trying to validate
              // we use the stored value.
              varVector.push_back(tempStore);
            } 
            else // If the code fails to parse at the first string
            {
                cout << "Major problem detected at line 1." << endl;
                return false;
            }
          }

          if (errorPresent) // Error reporting
          {
            cout << "Error detected on the first line. Approximate input used." << endl;
          }
        } else // Everything after first line
        {
          v.clear();
          while (getline(iss_line, s, ' '))
          {
            v.push_back(s);
          }

          if (v.size() == 0 || v.at(0) == "\t")
          {
            // If the line was empty.
            cout << "Empty line at line " << n+1;
            cout << ". Skipping..." << endl;
            n++;
            continue;
          }
          
          istringstream iss_word(v.at(0));
          if (!(iss_word >> currentlyTested))
          {
            cout << "Major error detected at line " << n+1;
            cout << ". Skipping line." << endl;
            n++;
            continue;
          } else if (!iss_word.eof()) 
          {
            cout << "Minor error detected at line " << n+1
              << ". Using approximate input." << endl;
          }
          
          varVector.push_back(currentlyTested);
        }
        n++;
      }


      cout << "I read a signal with an index of " << index <<
        " and duration of " << varVector.size() << " :D" << endl;
      
      signalArray = new double[varVector.size()];
      for (int i = 0; i < varVector.size(); i++)
      {
        signalArray[i] = varVector.at(i);
        cout << signalArray[i] << endl;
      }
    }
    // returns true if a valid signal was actuallly obtained from filename
    // returns false otherwise
    bool exportSignalToFile ( string filename );
    // returns true if the signal was successfully exported to a file
    // returns false otherwise
    int start()
    {
      return index;
    } 
    int end()
    {
      return index + sizeof(signalArray) - 1;
    }
    int duration()
    {
      return sizeof(signalArray);
    }
    // as implied by the names,
    // return the start index, end index, and duration of the signal
    // respectively
    double* data()
    {
      return signalArray;
    }
    // returns a pointer to an array of double containing the signal values
};

/*
engg151Signal normalizedXCorr ( engg151Signal x, engg151Signal y)
{
  engg151Signal normalizedSignal;
  
}
  */

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
  filepath = "C:\\Users\\user\\Downloads\\engg-151-main\\x_rawn_with_comment.text";
  engg151Signal test;

  test.importSignalFromFile(filepath);
  //return 0;
}
