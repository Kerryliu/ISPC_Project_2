#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "sqrtISPC.h"

using namespace std;

static const int size = 15;
static const int repetitionss = 5;
void randFillArr(int size, float* arr);
void printArr(int size, float* numArr, float* outArr);
void sqrtSeq(int size, float* numArr, float* outArr);
void benchmarkSeq(int size, float* numArr, float* outArr, int reps);
void benchmarkISPC(int size, float* numArr, float* outArr,int numTasks,int reps);

int main() {
  float* inputArray = new float[size];
  float* outputArray = new float[size];
  randFillArr(size, inputArray);

  cout << "Elements: " << size << endl;
  cout << "Repetitions: " << repetitionss << endl;
  benchmarkSeq(size, inputArray, outputArray, repetitionss);
  for(int i = 1; i <=10; i++) {
    benchmarkISPC(size, inputArray, outputArray, i, repetitionss);
  }
  printArr(size, inputArray, outputArray);
}

void randFillArr(int size, float* arr) {
  uniform_real_distribution<double> urd(0, 5);
  random_device randDev;
  default_random_engine randEng(randDev());

  for(int i = 0; i < size; i++) {
    arr[i] = urd(randEng);
  }
}

void printArr(int size, float* numArr, float* outArr) {
  if(size < 50) {
    for(int i = 0; i < size; i++) {
      cout << numArr[i] << "  " << outArr[i] << "  " << sqrt(numArr[i]) << endl;
    }
    cout << endl;
  }
}

void sqrtSeq(int size, float* numArr, float* outArr) {
  for(int i = 0; i < size; i++) {
    float num = numArr[i];
    float guess = 1;
    do {
      guess = (num/guess + guess)/2.0;
    } while(abs(guess*guess - num) > 0.0001);
    outArr[i] = guess;
  }
}

void benchmarkSeq(int size, float* numArr, float* outArr, int reps) {
  vector<int> meow(reps);
  for(int i = 0; i < reps; i++) {
    auto t1 = chrono::steady_clock::now();
    sqrtSeq(size, numArr, outArr);
    auto t2 = chrono::steady_clock::now();
    int difference = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "Sequential sqrt average time: " << average 
    << " milliseconds" << endl;
}

void benchmarkISPC(int size, float* numArr, float* outArr, int numTasks, int reps) {
  vector<int> meow(reps);
  for(int i = 0; i < reps; i++) {
    auto t1 = chrono::steady_clock::now();
    ispc::sqrtISPC(size, numArr, outArr, numTasks);
    auto t2 = chrono::steady_clock::now();
    int difference = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "ISPC Sqrt with: " << numTasks << " tasks.  Average time: " 
    << average << " milliseconds" << endl;
}
