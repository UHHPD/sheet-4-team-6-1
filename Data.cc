#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  string FileName = filename;


  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }

  for (int i = 0; i < size; ++i) {
    double errors;
    file >> errors;
    m_error.push_back(errors);
  }

  // done! close the file
  file.close();

  assertSizes();

};

int Data::checkCompatibility(const Data& in, int n) {
    using namespace std;
    int count = 0;
    for (int i=0; i<in.size(); i++){
        double a = m_data[i] - in.measurement(i);
        double b = 2*sqrt(pow(m_error[i], 2) + pow(in.error(i), 2));
        double c = b - a;

        if (c < 0) {
        count++;
        }
    }
    cout << "number of incompatibilities: " << count << endl;
    if (count > 0) {
        cout << "datasets are not compatible" << endl;
    }
    else {
        cout << "datasets are compatible" << endl;
    }
    return count;
}

double equation3(double x) {
    double f = 0.005 - (0.00001*x) + (0.08*exp(-0.015*x));
    return f;
}

double Data::x2test() {
    double x2 = 0.0;

    for (int i=0; i<size(); i++) {

        x2 += pow(measurement(i)-equation3(binCenter(i)),2)/pow(error(i),2);

    }
    cout << "x2 Test: " << x2/52 << endl;
    return x2/52;
}
/*
int Data::check(int n) {
    int check = 0;

    for (int i=0; i<size(); i++) {
        if (abs(measurement(i)-equation3(binCenter(i)))>= n*error(i)) {
            check += 1;
        }
    }
    cout << "datapoints that differ by more than " << n << " standard deviations: "<< check << endl;

    return check;

}
*/

int Data::check(int n) {
    int check = 0;

    for (int i=0; i<size(); i++) {
        if (abs(measurement(i)-equation3(binCenter(i)))>= n*error(i)) {
            check += 1;
        }
    }
    cout << "datapoints that differ by more than " << n << " standard deviations: "<< check << endl;

    return check;

}

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }
