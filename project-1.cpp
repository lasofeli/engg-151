// Mayabason, Brix
// Tusi, Gena
// Sonon, Hugh
// ENGG 151.01 - Project 1

/*
IMPORTANT:
Code is not yet formatted properly
- the 70 char limit (idk if he still does this)
- the use 'new' rule
- still need to format xraw and yraw into ave data
- whole bunch of logging/reporting of what's happening
  - esp. for file output processor
- error handling for file output processor
- code formatting
- create automated testing script
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
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
    int length;
    engg151Signal() {
      signalArray = new double[0];
      index = 0;
      length = 1;
    }
    engg151Signal (double arr[], int val1, int val2){
      signalArray = new double[val2];
      for (int i = 0 ; i < val2; i++)
      {
        signalArray[i] = arr[i];
      }
      index = val1;
      length = val2;
    };
    // default constructor
    // suggested default: one-element signal with value 0.0, start index 0
    
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
          while (iss_line >> s)
          {
              v.push_back(s);
          }
          
          // Signal file format is that only the first two
          // might matter, so rest is discarded.
          // Thus, loop should terminate either when it evaluates
          // two elements or if it reaches the end.
          for (int i = 0; i < 2 && i < v.size(); i++)
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
              // Unless there's only one value. Then, it's
              // already stored as our first signal.
              if (i == 0)
              {
                if (v.size() == 1)
                {
                  varVector.push_back(currentlyTested);
                } else
                {
                  tempStore = currentlyTested;
                }
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
          while (iss_line >> s)
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
      

      length = varVector.size();
      return true;
    }
    // returns true if a valid signal was actuallly obtained from filename
    // returns false otherwise
    bool exportSignalToFile ( string filename )
    {
      ofstream file(filename.c_str());
      
      if (!file.is_open())
      {
        cout << "Output could not be written. Maybe check your inputs?" << endl;
        return false;
      } else
      {
        for (int i = 0; i < length; i++)
        {
          // For testing;
          /*
          if (i == 0)
          {
            file << index << "\t" << signalArray[i] << endl;
          } else
          */
          {
            file << fixed << setprecision(6) << round(signalArray[i] / 0.000001) * 0.000001 << endl;;
          }
        }
        file.close();
      }
      return true;
    };
    // returns true if the signal was successfully exported to a file
    // returns false otherwise
    int start()
    {
      return index;
    } 
    int end()
    {
      return index + length - 1;
    }
    int duration()
    {
      return length;
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

engg151Signal normalizedXCorr (engg151Signal signalX, engg151Signal signalY)
{
  int lowerBound = signalX.start() - signalY.end();
  int upperBound = signalX.end() - signalY.start();
  cout << upperBound - lowerBound + 1 << endl;
  double* correlatedSignal = new double[upperBound - lowerBound + 1];

  int index;
  int diff = signalX.start() - signalY.start();
  

  
  double xx = 0;
  double yy = 0;
  for (int i = 0; i < signalX.duration(); i++)
  {
    xx += (signalX.data())[i] * (signalX.data())[i];
  }
  for (int i = 0; i < signalY.duration(); i++)
  {
    yy += (signalY.data())[i] * (signalY.data())[i];
  }
  // Loops through values for l

  int i = 0;
  double xSignalValue;
  double ySignalValue;
  for (int l = lowerBound; l <= upperBound; l++)
  {
    // Conducts the summing of products
    double sumproduct = 0;
    
    int lowerProductBound = min(signalX.start(), signalY.start() + l);
    int upperProductBound = max(signalX.end(), signalY.end() + l);

    for (int j = lowerProductBound;
      j <= upperProductBound;
      j++)
    { 
      xSignalValue = 0;
      ySignalValue = 0;
      if (j < signalX.start() || j > signalX.end())
      {
        xSignalValue = 0;
      } else
      {
        xSignalValue = (signalX.data())[j - signalX.start()];
      }
      if (j < signalY.start() + l || j > signalY.end() + l)
      {
        ySignalValue = 0;
      } else
      {
        ySignalValue = (signalY.data())[j - l - signalY.start()];
      }
      
      sumproduct += xSignalValue * ySignalValue;
    }
    correlatedSignal[i] = sumproduct / (sqrt(xx * yy));
    i++;
  }
  engg151Signal normalizedCorrsignal = engg151Signal(correlatedSignal, lowerBound, upperBound - lowerBound + 1);
  return normalizedCorrsignal;
}

int main(int argc, char * argv[])
{
  int i;
  for (i = 0; i < argc; i++)
  {
    cout << "Argument " << i << " = " << argv[i] << endl;
  }
  string inputFile1, inputFile2, outputFile;

  if (i > 1)
  {
    inputFile1 = argv[1];
    inputFile2 = argv[2];
    outputFile = argv[3];
  }
  
  //inputFile1 = "test_set_56_1.txt";
  //inputFile2 = "test_set_56_2.txt";
  //outputFile = "namaste.txt";

  // For debugging
  // filepath = "C:\\Users\\user\\Downloads\\basic-test-p3-engg21-2023-0.csv";
  // cout << "Hello World!" << endl;
  //inputFile1 = "x_rawn.text";
  engg151Signal signal1, signal2;

  signal1.importSignalFromFile(inputFile1);
  //inputFile2 = "y_rawn.text";
  signal2.importSignalFromFile(inputFile2);
  engg151Signal result = normalizedXCorr(signal1, signal2);

  cout << "Your results have an index of " << result.start() <<
    " and duration of " << result.duration() << " :D" << endl;
  cout << "First number is " << (result.data())[0];
  
  result.exportSignalToFile(outputFile);
  //return 0;
}
