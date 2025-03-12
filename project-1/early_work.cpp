/*
  Mayabason, Brix
  Sonon, Hugh
  Tusi, Gena
  
ENGG 151.01 -  A 
Project 1
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
  public:
    double* signalArray;
    int index = 0;
    int length;

    engg151Signal() {
      signalArray = new double[0];
      index = 0;
      length = 1;
    }

    engg151Signal(double arr[], int val1, int val2) {
      signalArray = new double[val2];
      for (int i = 0; i < val2; i++) {
        signalArray[i] = arr[i];
      }
      index = val1;
      length = val2;
    };

    bool importSignalFromFile(string filepath) {
      ifstream file(filepath.c_str());
      if (!file.good()) {
        string changedFilepath = filepath + ".txt";
        ifstream file(changedFilepath.c_str());
        if (file.good()) {
          cout << "Something seems to be wrong." << endl;
          cout << "Appended .txt file and continuing..." << endl;
        } else {
          cout << "Something seems to be wrong with: " << filepath << endl;
          cout << "Maybe try again?" << endl;
          return false;
        }
      }

      vector<double> varVector;
      double currentlyTested, tempStore;
      bool errorPresent = false;
      string s;
      vector<string> v;
      int n = 0;

      for (string line; getline(file, line); ) {
        istringstream iss_line(line);
        if (n == 0) {
          while (iss_line >> s) {
              v.push_back(s);
          }
          for (int i = 0; i < 2 && i < v.size(); i++) {
            istringstream iss_word(v.at(i));
            if (iss_word >> currentlyTested) {
              if (!iss_word.eof()) {
                errorPresent = true;
              }
              if (i == 0) {
                if (v.size() == 1) {
                  varVector.push_back(currentlyTested);
                } else {
                  tempStore = currentlyTested;
                }
              } else {
                istringstream iss_word(v.at(0));
                if ((iss_word >> index)) {
                  if (!iss_word.eof()) {
                    errorPresent = true;
                  }
                  varVector.push_back(currentlyTested);
                } else {
                  cout << "Major problem present at line 1." << endl;
                  return false;
                }
              }
            } else if (i == 1) {
              index = 0;
              varVector.push_back(tempStore);
            } else {
              cout << "Major problem detected at line 1." << endl;
              return false;
            }
          }
          if (errorPresent) {
            cout << "Error detected on the first line. Approximate input used." << endl;
          }
        } else {
          v.clear();
          while (iss_line >> s) {
              v.push_back(s);
          }
          if (v.size() == 0 || v.at(0) == "\t") {
            cout << "Empty line at line " << n+1;
            cout << ". Skipping..." << endl;
            n++;
            continue;
          }
          istringstream iss_word(v.at(0));
          if (!(iss_word >> currentlyTested)) {
            cout << "Major error detected at line " << n+1;
            cout << ". Skipping line." << endl;
            n++;
            continue;
          } else if (!iss_word.eof()) {
            cout << "Minor error detected at line " << n+1
              << ". Using approximate input." << endl;
          }
          varVector.push_back(currentlyTested);
        }
        n++;
      }

      cout << "\n";
      cout << "I read a signal with an index of " << index <<
        " and duration of " << varVector.size() << " :D" << endl;

      signalArray = new double[varVector.size()];
      for (int i = 0; i < varVector.size(); i++) {
        signalArray[i] = varVector.at(i);
        cout << signalArray[i] << endl;
      }

      length = varVector.size();
      return true;
    }

    bool exportSignalToFile(string filename) {
      ofstream file(filename.c_str());
      if (!file.is_open()) {
        cout << "Output could not be written. Maybe check your inputs?" << endl;
        return false;
      } else {
        for (int i = 0; i < length; i++) {
          file << fixed << setprecision(6) << round(signalArray[i] / 0.000001) * 0.000001 << endl;
        }
        file.close();
      }
      return true;
    };

    int start() {
      return index;
    }
    int end() {
      return index + length - 1;
    }
    int duration() {
      return length;
    }
    double* data() {
      return signalArray;
    }
};

engg151Signal normalizedXCorr(engg151Signal signalX, engg151Signal signalY) {
  int lowerBound = signalX.start() - signalY.end();
  int upperBound = signalX.end() - signalY.start();
  cout << upperBound - lowerBound + 1 << endl;
  double* correlatedSignal = new double[upperBound - lowerBound + 1];

  double xSum = 0;
  int xCount = 0;
  for (int i = 0; i < signalX.duration(); i++) {
    xSum += (signalX.data())[i];
    xCount++;
  }
  double xAve = xSum / xCount;

  double* adjustedXSignal = new double[signalX.duration()];
  for (int i = 0; i < signalX.duration(); i++) {
    adjustedXSignal[i] = signalX.data()[i] - xAve;
  }

  double ySum = 0;
  int yCount = 0;
  for (int i = 0; i < signalY.duration(); i++) {
    ySum += (signalY.data())[i];
    yCount++;
  }
  double yAve = ySum / yCount;

  double* adjustedYSignal = new double[signalY.duration()];
  for (int i = 0; i < signalY.duration(); i++) {
    adjustedYSignal[i] = signalY.data()[i] - yAve;
  }

  double xx = 0;
  double yy = 0;
  for (int i = 0; i < sizeof(adjustedXSignal); i++) {
    xx += adjustedXSignal[i] * adjustedXSignal[i];
  }
  for (int i = 0; i < sizeof(adjustedYSignal); i++) {
    yy += adjustedYSignal[i] * adjustedYSignal[i];
  }

  int i = 0;
  for (int l = lowerBound; l <= upperBound; l++) {
    double sumproduct = 0;
    int lowerProductBound = min(signalX.start(), signalY.start() + l);
    int upperProductBound = max(signalX.end(), signalY.end() + l);

    for (int j = lowerProductBound; j <= upperProductBound; j++) {
      double xSignalValue = 0;
      double ySignalValue = 0;
      if (j < signalX.start() || j > signalX.end()) {
        xSignalValue = 0;
      } else {
        xSignalValue = adjustedXSignal[j - signalX.start()];
      }
      if (j < signalY.start() + l || j > signalY.end() + l) {
        ySignalValue = 0;
      } else {
        ySignalValue = adjustedYSignal[j - l - signalY.start()];
      }
      sumproduct += xSignalValue * ySignalValue;
    }
    correlatedSignal[i] = sumproduct / (sqrt(xx * yy));
    i++;
  }

  delete[] adjustedXSignal;
  delete[] adjustedYSignal;

cout << "\n" ;
cout << "===OVERVIEW OF CALCULATED DATA===" << endl;
cout << "x sum = " << xSum << endl;
cout << "y sum = " << ySum << endl;
cout << "x mean = " << xAve << endl;
cout << "y mean = " << yAve << endl;

  engg151Signal normalizedCorrsignal = engg151Signal(correlatedSignal, lowerBound, upperBound - lowerBound + 1);
  return normalizedCorrsignal;
}

int main(int argc, char* argv[]) {
  int i;
  for (i = 0; i < argc; i++) {
    cout << "Argument " << i << " = " << argv[i] << endl;
  }
  string inputFile1, inputFile2, outputFile;

  if (i > 1) {
    inputFile1 = argv[1];
    inputFile2 = argv[2];
    outputFile = argv[3];
  }

  outputFile = "namaste.txt";
  inputFile1 = "x_rawn.text";
  engg151Signal signal1, signal2;

  signal1.importSignalFromFile(inputFile1);
  inputFile2 = "y_rawn.text";
  signal2.importSignalFromFile(inputFile2);
  engg151Signal result = normalizedXCorr(signal1, signal2);

  cout << "\n";
  cout << "===OUTPUT OVERVIEW===" << endl;
  cout << "Your results have an index of " << result.start() <<
    " and duration of " << result.duration() << " :D" << endl;
  cout << "The normalized cross-correlation data are as follows: " << endl;
  for (int i = 0; i < result.length; i++) {
    cout << result.data()[i] << " " << endl;
  }
  cout << endl;

  result.exportSignalToFile(outputFile);
}
