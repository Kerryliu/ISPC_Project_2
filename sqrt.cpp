#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "sqrtISPC.h"

using namespace std;

static const int size = 50000000;
static const int reps = 5;
void randFillArr(int size, float* arr);
void printArr(int size, float* arr);
void sqrtSeq(int size, float* numArr, float* outArr);
void benchmarkSeq(int size, float* numArr, float* outArr, int reps);
void benchmarkISPC(int size, float* numArr, float* outArr,int numTasks,int reps);

int main() {
  float* inputArray = new float[size];
  float* outputArray = new float[size];
  randFillArr(size, inputArray);

  cout << "Elements: " << size << endl;
  cout << "Repetitions: " << reps << endl;
  benchmarkSeq(size, inputArray, outputArray, reps);
  benchmarkISPC(size, inputArray, outputArray, 1, reps);
  benchmarkISPC(size, inputArray, outputArray, 2, reps);
  benchmarkISPC(size, inputArray, outputArray, 3, reps);
  benchmarkISPC(size, inputArray, outputArray, 4, reps);
  benchmarkISPC(size, inputArray, outputArray, 5, reps);
  benchmarkISPC(size, inputArray, outputArray, 6, reps);
  benchmarkISPC(size, inputArray, outputArray, 7, reps);
  benchmarkISPC(size, inputArray, outputArray, 8, reps);
  //printArr(size, inputArray);
  //printArr(size, outputArray);
}

void randFillArr(int size, float* arr) {
  uniform_real_distribution<double> urd(0, 5);
  random_device randDev;
  default_random_engine randEng(randDev());

  for(int i = 0; i < size; i++) {
    arr[i] = urd(randEng);
  }
}

void printArr(int size, float* arr) {
  for(int i = 0; i < size; i++) {
    cout << arr[i] << endl;
  }
  cout << endl;
}

void sqrtSeq(int size, float* numArr, float* outArr) {
  for(int i = 0; i < size; i++) {
    float num = numArr[i];
    float guess = 1;
    do {
      guess = (num/guess + guess)/2;
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
    int difference = chrono::duration_cast<chrono::microseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "Squential sqrt average time: " << average 
    << " microseconds" << endl;
}

void benchmarkISPC(int size, float* numArr, float* outArr, int numTasks, int reps) {
  vector<int> meow(reps);
  for(int i = 0; i < reps; i++) {
    auto t1 = chrono::steady_clock::now();
    ispc::sqrtISPC(size, numArr, outArr, numTasks);
    auto t2 = chrono::steady_clock::now();
    int difference = chrono::duration_cast<chrono::microseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "ISPC Sqrt with: " << numTasks << " tasks.  Average time: " 
    << average << " microseconds" << endl;
}
