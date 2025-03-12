#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class engg151Signal
{
  private:
    double* signalArray;
    int index = 0;
    int length;  
  
  public:
    engg151Signal()
    {
      signalArray = new double[0];
      index = 0;
      length = 1;
    }
    
    engg151Signal (double arr[], int val1, int val2)
    {
      signalArray = new double[val2];
      for (int i = 0 ; i < val2; i++)
      {
        signalArray[i] = arr[i];
      }
      index = val1;
      length = val2;
    };
 
    bool importSignalFromFile (string filename)
    {
      // Needed to verify that file exists.
      // Also tries the .txt addition.
      ifstream file(filename.c_str());
      if (!file.good())
      {
        string changedFilepath = filename + ".txt";
        ifstream file(changedFilepath.c_str());
        if (file.good())
        {
          cout << "Something seems to be wrong." << endl;
          cout << "Appended .txt file and continuing..." << endl;
        }
        else
        {
          cout << "Something seems to be wrong with: " << filename
          << endl;
          cout << "Maybe try again?" << endl;
          return false;
        }
      }

      vector<double> varVector; // For storing signal
      vector<string> v; // For line 1 processing
      double currentlyTested, tempStore;
      double signal1, signal2; 

      // Storing error status is useful to make sure error message
      // only displays once per line
      bool errorPresent = false;

      string s;

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
              // earlier, the only way this can trigger
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
                }
                else
                {
                  tempStore = currentlyTested;
                }
              }
              else
              {        
                // The outer if statement checks if
                // the string can be parsed as a double.
                
                // Getting to this point would mean that
                // the second string is valid, but the index
                // should be an integer, not a double.

                // So we need to re-validate the earlier input
                // to see if it changes if it is parsed as an int.

                // Validating it first as a double is necessary, since
                // it is unknown at that point if there is a
                // second number.
                istringstream iss_word(v.at(0));
                if ((iss_word >> index)) 
                {
                  if (!iss_word.eof())
                  {
                    errorPresent = true;
                  }

                  // With index value validated, second string is
                  // now stored as the first signal.
                  varVector.push_back(currentlyTested);
                }
                else
                {
                  // Just for redundancy, although there
                  // shouldn't be any chance of this tripping
                  cout << "Major problem present at line 1." << endl;
                  return false;
                }
              }
            }
            else if (i == 1)
            {
              // If code only fails to parse the second string,
              // we treat the first as a signal and use the fallback
              // index value of 0.
              index = 0;
              
              // Since currentlyTested got wiped when trying
              // to validate we use the stored value.
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
            cout << "Error detected on the first line. "
              << "Approximate input used." << endl;
          }
        }
        else // Everything after first line
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
          }
          else if (!iss_word.eof()) 
          {
            cout << "Minor error detected at line " << n+1
              << ". Using approximate input." << endl;
          }
          
          varVector.push_back(currentlyTested);
        }
        n++;
      }
      
      signalArray = new double[varVector.size()];
      for (int i = 0; i < varVector.size(); i++)
      {
        signalArray[i] = varVector.at(i);
      }
      

      length = varVector.size();
      return true;
    }

    bool exportSignalToFile (string filename)
    {
      ofstream file(filename.c_str());
      
      if (!file.is_open())
      {
        cout << "Designated output file could not be opened." <<
          " Maybe check your inputs?" << endl;
        return false;
      }
      else
      {
        for (int i = 0; i < length; i++)
        {
          /*
          if (i == 0)
          {
            file << index << "\t" << signalArray[i] << endl;
          }
          else
          {
            file << "\t" << signalArray[i] << endl;;
          }
          */
          file << fixed << setprecision(6) << round(signalArray[i] / 0.000001) * 0.000001 << endl;
        }
        file.close();
      }
      return true;
    };
    
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

    double autoCorrelation()
    {
      double result = 0;

      for(int i = 0; i < length; i++)
      {
        result += signalArray[i] * signalArray[i];
      }

      return result;
    }

    void processSignal()
    {
      double average = 0;
      
      for(int i = 0; i < length; i++)
      {
        average += signalArray[i];
      }
      
      average = average / length;

      for(int i = 0; i < length; i++)
      {
        signalArray[i] -= average;
      }
    }

    double* data()
    {
      return signalArray;
    }
};