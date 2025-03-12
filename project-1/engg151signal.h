/* 
Mayabason, Brix
Tusi, Gena
Sonon, Hugh
ENGG 151.01 - A
Project 1: Normalized Cross-correlation
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

      vector<double> varVector;
      vector<string> v;
      double currentlyTested, tempStore;
      double signal1, signal2; 
      
      bool errorPresent = false;

      string s;

      int n = 0;

      for (string line; getline(file, line); )
      {
        istringstream iss_line(line);
        if (n == 0)
        {
          while (iss_line >> s)
          {
              v.push_back(s);
          }
          
          for (int i = 0; i < 2 && i < v.size(); i++)
          {
            istringstream iss_word(v.at(i));
            if (iss_word >> currentlyTested)
            {
              if (!iss_word.eof())
              {
                errorPresent = true;
              }

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
                istringstream iss_word(v.at(0));
                if ((iss_word >> index)) 
                {
                  if (!iss_word.eof())
                  {
                    errorPresent = true;
                  }

                  varVector.push_back(currentlyTested);
                }
                else
                {
                  cout << "Major problem present at line 1." << endl;
                  return false;
                }
              }
            }
            else if (i == 1)
            {
              index = 0;
              
              varVector.push_back(tempStore);
            } 
            else
            {
                cout << "Major problem detected at line 1." << endl;
                return false;
            }
          }

          if (errorPresent)
          {
            cout << "Error detected on the first line. "
              << "Approximate input used." << endl;
          }
        }
        else
        {
          v.clear();
          while (iss_line >> s)
          {
              v.push_back(s);
          }
          
          if (v.size() == 0 || v.at(0) == "\t")
          {
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
          if (i == 0)
          {
            file << index << "\t" << signalArray[i] << endl;
          }
          else
          {
            file << "\t" << signalArray[i] << endl;;
          }
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

    void printSignal()
    {
      for (int i = 0; i < length; i++)
      {
        if (i == 0)
        {
          cout << index << "\t" << signalArray[i] << endl;
        }
        else
        {
          cout << "\t" << signalArray[i] << endl;
        }
      }
    }
};
