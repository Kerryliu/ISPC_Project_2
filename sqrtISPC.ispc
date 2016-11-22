task void sqrt(uniform int size, uniform float numArr[], uniform float outArr[]) {
  uniform int span = size/taskCount;
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? size : start+span;
  foreach(i = start ... end) {
    float num = numArr[i];
    float guess = 1;
    do {
      guess = (num/guess + guess)/2.0;
    } while(abs(guess*guess - num) > 0.0001);
    outArr[i] = guess;
  }
}

export void sqrtISPC(uniform int size, uniform float numArr[], uniform float outArr[],
    uniform int numTasks) {
  launch[numTasks] sqrt(size, numArr, outArr);
}
