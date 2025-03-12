/* 
Mayabason, Brix
Tusi, Gena
Sonon, Hugh
ENGG 151.01 - A
Project 2: Fourier Transform
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "engg151signal.h"

/*
TODO:
- Input validation
- DFT resistance to flipped start and end frequencies
- ensure frequency calculator is resistant to non-terminating numbers (e.g., 1/3)
- file export
- add program cmdline reporting 
*/

using namespace std;

double convertToDigital (double analogFreq)
{
  double digFreq = analogFreq * 2 * atan(1) * 4; // atan(1)*4 is for pi
  return digFreq;
}

void computeDFT (
  engg151Signal xData,
  double samplingFreq,
  double startFreq,
  double endFreq,
  int nSteps
)
{
  double stepLength = (endFreq - startFreq) / nSteps;
  double* realPart = new double[nSteps + 1];
  double* imagPart = new double[nSteps + 1];
  double* freqIndex = new double[nSteps + 1];

  freqIndex[0] = convertToDigital(startFreq);
  double freqIter = startFreq;
  for (int i = 0; i < nSteps; i++)
  {
    freqIter += stepLength;
    freqIndex[i + 1] = convertToDigital(freqIter);
  }
  
  for (int i = 0; i <= nSteps; i++)
  {
    realPart[i] = 0;
    imagPart[i] = 0;
    for (int j = 0; j < xData.duration(); j++)
    {
      realPart[i] += (xData.data())[j] * cos(-1 * freqIndex[i] * j / samplingFreq);
      imagPart[i] += (xData.data())[j] * sin(-1 * freqIndex[i] * j / samplingFreq);
    }
  }

  for (int i = -1; i <= nSteps; i++)
  {
    if (i == -1)
    {
      cout << "frequency (Hz)" << "\t" << "real part" << "\t" << "imaginary part" << endl;
    }
    else
    {
      cout << startFreq + stepLength * i << "\t" << realPart[i] << "\t" << imagPart[i] << endl;
    }
  }

  for (int i = -1; i <= nSteps; i++)
  {
    if (i == -1)
    {
      cout << "frequency (Hz)" << "\t" << "magnitude" << "\t" << "phase (degrees)" << endl;
    }
    else
    {
      cout << startFreq + stepLength * i << "\t" << sqrt(pow(realPart[i], 2) + pow(imagPart[i], 2))
        << "\t" << atan(imagPart[i] / realPart[i]) * 180 /  (atan(1) * 4) << endl;
    }
  }
}

int main(int argc, char * argv[])
{
  string inputFile = "x.signal";
  string outputFile = "log.txt";
  double sampling_rate = 32;
  double start_freq = 4;
  double end_freq = 8;
  int steps = 8;
  
  engg151Signal xSignal;

  xSignal.importSignalFromFile(inputFile);
  
  computeDFT(xSignal, sampling_rate, start_freq, end_freq, steps);
  cout << "Program done!" << endl;
}
