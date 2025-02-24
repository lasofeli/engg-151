// Mayabason, Brix
// Tusi, Gena
// Sonon, Hugh
// ENGG 151.01 - Project 1

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "engg151signal.h"

using namespace std;

engg151Signal normalizedXCorr (
  engg151Signal signalX,
  engg151Signal signalY)
{
  signalX.processSignal();
  signalY.processSignal();

  cout << "Raw signals are processed. Calculating cross-correlation."
    << endl;
  
  int lowerBound = signalX.start() - signalY.end();
  int upperBound = signalX.end() - signalY.start();

  double* correlatedSignal = new double[upperBound - lowerBound + 1];

  int i = 0;
  for (int l = lowerBound; l <= upperBound; l++)
  {
    // Conducts the summing of products
    double sumproduct = 0;
    
    int lowerProductBound = min(signalX.start(), signalY.start() + l);
    int upperProductBound = max(signalX.end(), signalY.end() + l);

    for (int j = lowerProductBound; j <= upperProductBound; j++)
    { 
      double xSignalValue = 0;
      double ySignalValue = 0;
      if (j < signalX.start() || j > signalX.end())
      {
        xSignalValue = 0;
      }
      else
      {
        xSignalValue = (signalX.data())[j - signalX.start()];
      }
      if (j < signalY.start() + l || j > signalY.end() + l)
      {
        ySignalValue = 0;
      }
      else
      {
        ySignalValue = (signalY.data())[j - l - signalY.start()];
      }
      
      sumproduct += xSignalValue * ySignalValue;
    }
    correlatedSignal[i] = sumproduct /
      (sqrt(signalX.autoCorrelation() * signalY.autoCorrelation()));
    i++;
  }

  engg151Signal normalizedCorrsignal = engg151Signal(
    correlatedSignal,
    lowerBound,
    upperBound - lowerBound + 1);

  return normalizedCorrsignal;
}

int main(int argc, char * argv[])
{
  int i;
  string inputFile1, inputFile2, outputFile;

  if (i > 1)
  {
    inputFile1 = argv[1];
    inputFile2 = argv[2];
    outputFile = argv[3];
  }
  else
  {
    cout << "Invalid number of inputs detected. You should provide"
      << " [xdata] [ydata] [output file]." << endl;
    return EXIT_FAILURE;
  }

  engg151Signal signal1, signal2;

  if(!signal1.importSignalFromFile(inputFile1))
  {
    return EXIT_FAILURE;
  }
  cout << "Signal of length " << signal1.duration()
    << " and index " << signal1.start() << " read "
    << "from " << inputFile1 << endl;
  if(!signal2.importSignalFromFile(inputFile2))
  {
    return EXIT_FAILURE;
  }
  cout << "Signal of length " << signal2.duration()
      << " and index " << signal2.start() << " read "
      << "from " << inputFile2 << endl;

  engg151Signal result = normalizedXCorr(signal1, signal2);

  if(!result.exportSignalToFile(outputFile))
  {
    return EXIT_FAILURE;
  }

  cout << "Crosscorrelated signal of length "
      << result.duration() << " and index " << result.start()
      << " has been written to " << outputFile << endl;
  cout << "Program done!" << endl;
}
