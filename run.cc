#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"



// generic function comparing two values of some type T used later for int and
// double
template <class T>

bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

/*
int checkCompatibility(Data in, Data in2, int n) {
    using namespace std;
    int count = 0;
    int count2 = 0;
    for (int i=0; i<in.size(); i++){
        double a = in.measurement(i) - in2.measurement(i);
        double b = 2*sqrt(pow(in.error(i), 2) + pow(in2.error(i), 2));
        double c = b - a;

        if (c < 0) {
        cout << "not compatible " << c << " bin: " << count2 << endl;
        count++;
        count2++;
        }
        else {
        cout << "compatible " << c << " bin: " << count2 << endl;
        count2++;
        }
    }
    cout << "number of incompatibilities: " << count << endl;
    return count;
}
*/

/*
double average(Data in1, Data in2){
    using namespace std;
    if (in1.checkCompatibility(in2, 2) > 0){
        cout << "Datsasets are not compatible! Average is not calculated" << endl;
        return 0;
    }
    else{
        double avSum = 0.0;
        for (int i=0; i<in1.size(); i++){
            double w1 = 1/pow(in1.error(i),2);
            double w2 = 1/pow(in2.error(i),2);
            double yAv = ((w1*in1.measurement(i))+(w2*in2.measurement(i)))/(w1+w2);
            double oAv = sqrt(1/(w1+w2));
            avSum += yAv;
        }
        cout << "Average: " << avSum/in1.size() << endl;
        return avSum/in1.size();
    }
}
*/

Data average(Data in1, Data in2 ){
  using namespace std;
  if (in1.checkCompatibility(in2, 2) > 0){
      cout << "Datsasets are not compatible! Average is not calculated" << endl;
      return in1;
  }
  else{
    int out_size = in1.size();
    vector<double> out_data;
    vector<double> out_bins;
    vector<double> out_errors;
    for (int i=0;i<out_size;i++){
      double w1= 1/(in1.error(i)*in1.error(i));
      double w2= 1/(in2.error(i)*in2.error(i));
      double dataout= (w1*in1.measurement(i)+w2*in2.measurement(i))/(w1+w2);
      double errorout= sqrt(1/(w1+w2));
      out_data.push_back(dataout);
      out_bins.push_back(in1.binLow(i));
      out_errors.push_back(errorout);
    }
    out_bins.push_back(in1.binLow(out_size+1));
  
    ofstream fout("average");
  
    fout << out_size << endl;
  
    for (int i=0;i < out_size + 1; i++){
      fout << out_bins[i] << endl;
    }
  
    for (int i=0;i < out_size ; i++){
      fout << out_data[i] << endl;
    }
  
    for (int i=0;i < out_size ; i++){
      fout << out_errors[i] << endl;
    }
  
    fout.close();
  
    Data outputdata("average");
    return outputdata; 
  } 
}

double equation(double x) {
    double f = 0.005 - (0.00001*x) + (0.08*exp(-0.015*x));
    return f;
}

double x2Compare(){
    using namespace std;

    vector<Data> Dataset;

    string Datenliste[4] = {"exp_A", "exp_B", "exp_C", "exp_D"};

    for (int i=0; i<4; i++)
    {
    Data file(Datenliste[i]);
    Dataset.push_back(file);
    }

    double x2 = 0.0;

    for (int i=0; i<56; i++){
        double ywSum = 0.0;
        double wSum = 0.0;
        for(Data entry : Dataset){
            double wn = 1/pow(entry.error(i),2);
            ywSum += wn*entry.measurement(i);
            wSum += wn;
        }
        double yav = ywSum/wSum;
        double oy2 = 1/wSum;

        x2 += pow(yav-equation(Dataset[0].binCenter(i)),2)/oy2;

    }
    cout << "x2/ndf of yAverage: " << x2/ 52 << endl;

    return x2;

}


int main() {
    using namespace std;

    vector<Data> Dataset;

    string Datenliste[4] = {"exp_A", "exp_B", "exp_C", "exp_D"};

    for (int i=0; i<4; i++)
    {
    Data file(Datenliste[i]);
    Dataset.push_back(file);
    }


    cout << "******************************************************" << endl;
    runTests();
    cout << "******************************************************" << endl;
    // create an object which holds data of experiment A

    Data datA("exp_A");
    Data datB("exp_B");
    Data datC("exp_C");
    Data datD("exp_D");

    //Data& rdatA = datA;
    //Data& rdatB = datB;


    // here is the data from experiment A
    cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
         << endl;
    cout << "measurement of experiment A in bin 27: " << datA.measurement(27)
         << endl;
    cout << "measurement of experiment A in bin 27: " << datA.error(27)
        << endl;
    cout << "******************************************************" << endl;
    cout << "Aufgabe 1c)" << endl;

    cout << "measurement of experiment A in bin 27:" << Dataset[0].measurement(27) << endl;
    cout << "measurement of experiment B in bin 27:" << Dataset[1].measurement(27) << endl;
    cout << "measurement of experiment C in bin 27:" << Dataset[2].measurement(27) << endl;
    cout << "measurement of experiment D in bin 27:" << Dataset[3].measurement(27) << endl;


    double a = Dataset[0].measurement(27) - Dataset[1].measurement(27);
    double b = 2*sqrt(pow(Dataset[0].error(27), 2) + pow(Dataset[1].error(27), 2));
    double c = b- a;

    cout << "bin 27, c: " << c << endl;

    cout << "******************************************************" << endl;
    cout << "Aufgabe 1d)" << endl;

    //checkCompatibility(datA, datD, 2);
    cout << "DatA/DatB" << endl;
    datA.checkCompatibility(datB, 2);
    cout << "DatA/DatC" << endl;
    datA.checkCompatibility(datC, 2);
    cout << "DatA/DatD" << endl;
    datA.checkCompatibility(datD, 2);
    cout << "DatB/DatC" << endl;
    datB.checkCompatibility(datC, 2);
    cout << "DatB/DatD" << endl;
    datB.checkCompatibility(datD, 2);


    cout << "******************************************************" << endl;
    cout << "Aufgabe 1e)" << endl;
    //cout << "DatA/DatD" << endl;

    Data datav = average(datA,datB);
    cout << "measurement of average of a and b in bin 27: " << datav.measurement(27)
       << endl;

    //average(datA, datD);

    cout << "******************************************************" << endl;
    cout << "Aufgabe 2b)" << endl;

    datA.x2test();
    datB.x2test();
    datC.x2test();
    datD.x2test();

    cout << "******************************************************" << endl;
    cout << "Aufgabe 2a)" << endl;

    datA.check(2);
    datB.check(2);
    datC.check(2);
    datD.check(2);

    datA.check(3);
    datB.check(3);
    datC.check(3);
    datD.check(3);

    cout << "******************************************************" << endl;
    cout << "Aufgabe 2c)" << endl;

    x2Compare();

    return 0;
}
