// Hugh Angelo C. Sonon
// ENGG 21-B

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>


using namespace std;

// 70 Character Long Line
//12345678901234567890123456789012345678901234567890123456789012345678

int main()
{
  string filepath;
  // For debugging
  // filepath = "C:\\Users\\user\\Downloads\\basic-test-p3-engg21-2023-0.csv";

  while (true)
  {
    
    cout << "Enter the filepath for the XY file" << endl;
    cout << "Example: C:\\Users\\user 1\\Desktop\\test.XY" << endl;
    cout << "Input: ";
    getline(cin, filepath);
    ifstream file(filepath.c_str());
    if (!file.good())
    {
      cout << "That doesn't seem to be a file..." << endl;
      cout << "Take care not to add quotation marks"
        << " and try again" << endl;
    }
    

    // Is only here because of the debug filepath value
    // ifstream file(filepath);

    vector<double> xVarVector;
    vector<double> yVarVector;

    int n = 0;
    for (string line; getline(file, line); )
    {
      istringstream iss(line);
      double a, b;
      if (!(iss >> a >> b))
      {
        cout << "Invalid data detected at line "
          << n + 1 << ". Terminating processing." << endl;
        break;
      }

      xVarVector.push_back(a);
      yVarVector.push_back(b);
      n += 1;
    }
    cout << "Processed: " << n << " Lines" << endl;
    if (n == 0)
    {
      cout << "I couldn't process any lines. Is the entered filepath"
        << " correct?" << endl;
      continue;
    }
    else
    {
      double* xVarArray = new double[n];
      double* yVarArray = new double[n];
      for (int i = 0; i < n; i++)
      {
        xVarArray[i] = xVarVector.at(i);
        yVarArray[i] = yVarVector.at(i);
      }

      double xySum = 0;
      double xSum = 0;
      double ySum = 0;
      double x2Sum = 0;

      for (int i = 0; i < n; i++)
      {
        xySum += xVarArray[i] * yVarArray[i];
        xSum += xVarArray[i];
        ySum += yVarArray[i];
        x2Sum += xVarArray[i] * xVarArray[i];
      }

      double slope;
      double yIntercept;

      slope = (n * xySum) - (xSum * ySum);
      slope = slope / ((x2Sum * n) - (xSum * xSum));

      yIntercept = (ySum - slope * xSum) / n;

      cout << setprecision(15);
      cout << "The line that best fits the data is of the form: "
        << endl << "y = " << slope << "x + " << yIntercept << endl;
    }
    break;
  }
}
