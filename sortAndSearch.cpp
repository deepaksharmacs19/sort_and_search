#include <random> // for srand, rand
#include <iostream>
#include <iomanip>

using namespace std;

const int START{1000};
const int BIGSIZE{64000};
int bigArray[BIGSIZE]{};
const int testSizes[]{START*1, START*2, START*4, START*8, START*16, START*32, BIGSIZE, 0}; // 0 is a sentinel

const int TARGET{-1}; // TARGET, number to search for.

int binarysearch(int array[],int target, int arraylength){
  int currentindex = arraylength/2;
  int leftindex = 0;
  int rightindex = arraylength - 1;
  while (true){
    cout << currentindex<< endl;
    if(currentindex < 0 || currentindex >= arraylength){
      return -1;
    }
    int value = array[currentindex];
    if(value == target){
      return currentindex;
    }
    else if(value < target){
      leftindex = currentindex + 1;
    }
    else if(value > target){
      rightindex = currentindex;
    }
    currentindex = (leftindex + rightindex) / 2;
    if(leftindex >= rightindex){
      return -1;
    }
  }
}

void insertionsort(int array[],int arraylength, bool verbose){
  for(int i = 1;i < arraylength; i++){
    int key = array[i];
    int previousindex = i - 1;
    while(previousindex >= 0 && key < array[previousindex]){
      array[previousindex + 1] = array[previousindex];
      previousindex --;
    }
    array[previousindex + 1] = key;
  }
}

void show_array(int array[], int size, const string &msg="") { // displays every element in array
  cout<<msg;
  for (int i{}; i<size; ++i)
    cout << setw(2) << array[i]; // assumes small numbers; no line breaks / endl are output
  cout << endl;
}

int unorderedCount(int array[], int size) {
  int unsorted_adjacent_pairs{};
  for (int i=0; i<(size-1); ++i)
    if (array[i]>array[i+1]) ++unsorted_adjacent_pairs;
  return unsorted_adjacent_pairs;
}

bool verifySorted(int array[], int size) { // Useful to verify that array is really sorted!
  for (int i=0; i<(size-1); ++i)
    if (array[i]>array[i+1]) return false; // return false as soon as unordered pair detected
  return true;
}

const int AlgorithmNameMaxWidth{16}; // maximum column width to display algorithm names
const int TestSizeWidth{12};         // maximum column width to display time to run algorithms
const int DurationWidth{TestSizeWidth};


const string SELECTION_SORT_NAME{"selection sort"};
void selection_sort_cryptic(int[], int); // from internet, not called
void selection_sort_text(int[], int);    // from our textbook, not called
void selection_sort(int[], int, bool);   // annotated, as expected for this lab, see below

const string INSERTIONSORT {"insertion sort"};
void my_sort_1(int[], int, bool);    // expected for this lab <modify code>

const string SORT2_NAME {"bubble sort"};
void my_sort_2(int[], int, bool);    // expected for this lab <modify code>

bool linear_search_array(int [], int, int, int&); // provided
void binary_search_array(int [], int, int, int&); // expected for this lab <modify code>


float testSortAlgorithm1x(void sortAlgorithm(int [], int, bool),
                          int array[], int arraySize, bool verbose=false) {

  for (int index = 0; index < arraySize; index++)
    array[index] = rand();  // initialize array with random values
  int startTime = clock();  // get the start time, in milliseconds
  sortAlgorithm(array, arraySize, verbose);  // ALGORITHM UNDER TEST
  int stopTime = clock();   // get the stop time, in milliseconds
  float duration = stopTime - startTime;
  
  int unordered_pairs = unorderedCount(array, arraySize);
  if (unordered_pairs != 0)
    return -unordered_pairs; // return a negative count of unordered pairs to indicate sort failure
  else
    return duration;
}

void testSortAlgorithmNx(void sortAlgorithm(int [], int, bool), string sortName,
                         int array[], int arraySize, bool verbose=false) {
  cout << endl << setw (AlgorithmNameMaxWidth) << left << sortName;
  for (int testCount=0; (testSizes[testCount] && testSizes[testCount] <= arraySize); ++testCount)
    cout << setw(DurationWidth) << right << testSortAlgorithm1x(sortAlgorithm, array, testSizes[testCount]);
}
                        

float testLinearSearch(int array[], int arraySize, int retry=1000) {
  bool found = false; // true if TARGET is found in array
  int foundAt = -1;    // index in array where TARGET was found
  int startTime = clock();
  for (int repeat = 0; repeat < retry; ++repeat) // repeat test 1000 times to increase duration
    found = linear_search_array(array, arraySize, TARGET, foundAt); // ALGORITHM UNDER TEST
  int stopTime = clock();
  float duration = stopTime - startTime;
  return duration/retry; // divide duration by 1000 to get time for single search
}

float testBinarySearch(int array[], int arraySize, int retry=1000) {

  return 0.0;
}

void testAlgorithms(int array[], int arraySize, bool verbose=false) {
  cout << setw (AlgorithmNameMaxWidth) << left << "Algorithm";
  for (int testCount=0; testSizes[testCount]; ++testCount)
    cout << setw(TestSizeWidth) << right << testSizes[testCount];
  cout << endl << string(AlgorithmNameMaxWidth, '=');
  for (int testCount=0; testSizes[testCount]; ++testCount)
    cout << setw(DurationWidth) << right << " =======";

  
  testSortAlgorithmNx(selection_sort, SELECTION_SORT_NAME, array, arraySize); // provided
  testSortAlgorithmNx(my_sort_1, INSERTIONSORT, array, arraySize);
  testSortAlgorithmNx(my_sort_2, SORT2_NAME, array, arraySize);

  cout<<"\n--------"; // separator between sort algorithms and search algorithms

  cout << endl << setw (AlgorithmNameMaxWidth) << left << "linear search";
  for (int testCount=0; (testSizes[testCount] && testSizes[testCount] <= arraySize); ++testCount)
    cout << setw(DurationWidth) << right << testLinearSearch(array, testSizes[testCount]);

  cout << endl << setw (AlgorithmNameMaxWidth) << left << "binary search";
  for (int testCount=0; (testSizes[testCount] && testSizes[testCount] <= arraySize); ++testCount)
    cout << setw(DurationWidth) << right << testBinarySearch(array, testSizes[testCount]);
  cout << endl;
}


void testSortOnSmallArray(void sortAlgorithm(int [], int, bool), string sortName) {
  int smallArray[] {7, 9, 3, 1, 8, 6, 2}; // for testing purposes

  const int SMALLSIZE = sizeof(smallArray)/sizeof(smallArray[0]);
  
  show_array(smallArray, SMALLSIZE, sortName + " start: smallArray is: ");
  sortAlgorithm(smallArray, SMALLSIZE, true); // true means verbose, show details
  show_array(smallArray, SMALLSIZE, sortName + " stop:  smallArray is: ");
  cout << ((verifySorted(smallArray, SMALLSIZE)) ?
    "Verified: smallArray is sorted.\n\n" :
    "Oops!!!: smallArray is NOT sorted.\n\n");
}

int main () {
  srand(time(0));  // seed the random number generator only once.


  cout << "Test sorting algorithms on small array:\n\n";
  testSortOnSmallArray(selection_sort, SELECTION_SORT_NAME);
  testSortOnSmallArray(my_sort_1, INSERTIONSORT);
  testSortOnSmallArray(my_sort_2, SORT2_NAME);
  float duration = 0.0; // time in milliseconds
  duration = testSortAlgorithm1x(selection_sort, bigArray, BIGSIZE);
  cout << fixed << setprecision(2);
  cout << "\nSelection sort on bigArray took: "
       << setw(7) << duration << " milliseconds." << endl;

  duration = testLinearSearch(bigArray, BIGSIZE);
  cout << "Linear search  of bigArray took: "
       << setw(7) << duration << " milliseconds.\n\n";

  testAlgorithms(bigArray, BIGSIZE);
  int array []={0,1,3,4,5,6,7,8};
  return 0;
} // end of main

void my_sort_1(int array[], int size, bool verbose=false) {

int offset=1;
int num=0;
int inneroffset=0;

while(offset<size) {
inneroffset=offset-1;
num=array[offset];
if (verbose) {
cout<<"On the "<<offset<<" iteration the smallest between 0 and "<<inneroffset<<" will be on the top"<<endl;
}
while (inneroffset>=0 && array[inneroffset]>num)
{
if(verbose) {
cout<<"in the place of array["<<inneroffset<<"], array["<<inneroffset+1<<"] is placed"<<endl;
}
array[inneroffset+1]=array[inneroffset];
inneroffset--;
}
if(verbose) {
cout<<"Then "<<num<<" is placed in array["<<inneroffset+1<<"]"<<endl;
}
array[inneroffset+1]=num;
if(verbose) {
cout<<"After placing the array will be : "<<endl;
show_array(array,size);
cout<<endl;
}
offset++;
}
}


void my_sort_2(int array[], int size, bool verbose=false) {

int outerSize=size-2;
int offset1=0;
int offset2=0;
int temp=0;

// This loop will run upto size-2 times
while(outerSize>0) {
offset1=0;
offset2=offset1+1;
if(verbose) {
cout<<"The largest value from "<<offset1<<" to "<<offset2<<" will be settle down at the "<<outerSize<< "th index."<<endl;
}
while(offset1<=outerSize) {
// This is the loop that will generate that series 0 1, 1 2, 2 3 ........
if(array[offset1]>array[offset2]) {
if(verbose) {
cout<<"swapping the value of "<<offset1<<" to "<<offset2<<" as the value of "<<offset1<<" is greater than "<<offset2<<endl;
}
// comparing and swapping
temp=array[offset1];
array[offset1]=array[offset2];
array[offset2]=temp;
}

if(verbose) {
cout<<"After swapping the array will be : "<<endl;
show_array(array,size);
cout<<endl;
}

offset1++;
offset2++;
}
outerSize--;
}
}

bool linear_search_array(int array[], int size, int target, int &position) {
  for (int offset = 0; offset < size; ++offset)
    if (array[offset] == target){
	position=offset;
 return true;
}
  position = -1;
  return false;
}

void binary_search_array(int array[], int size, int target, int &position) {
int currentindex = size/2;
  int leftindex = 0;
  int rightindex = size - 1;

  while (leftindex<=rightindex) {
    if(currentindex < 0 || currentindex >= size) {
	position=-1;
      return;
    }
    int value = array[currentindex];
    if(value == target){

      position = currentindex;
	return;
    }
    else if(value < target){
      leftindex = currentindex + 1;
    }
    else if(value > target){
      rightindex = currentindex;
    }
    currentindex = (leftindex + rightindex) / 2;
  }
}

void selection_sort_cryptic(int array[], int size) {
  int i, x, val;
  for (i = 0; i < (size - 1); i++) {
    x = i;
    val = array[i];
    for (int j = i + 1; j < size; j++) {
      if (array[j] < val) {
        val = array[j];
        x = j;
      }
    }
    array[x] = array[i];
    array[i] = val;
  }
}

void selection_sort_text(int array[], int size) {
  int startScan, minIndex, minValue;
  for (startScan = 0; startScan < (size - 1); startScan++) {
    minIndex = startScan;
    minValue = array[startScan];
    for (int index = startScan + 1; index < size; index++) {
      if (array[index] < minValue) {
        minValue = array[index];
        minIndex = index;
      }
    }
    array[minIndex] = array[startScan];
    array[startScan] = minValue;
  }
}

void selection_sort(int array[], int size, bool verbose=false) {
  if (size<=1) return; // no work to do
  int minIndexSoFar = 0, minValueSoFar{array[0]};
  for (int unsortedIndex = 0; unsortedIndex < (size - 1); unsortedIndex++) {
    minIndexSoFar = unsortedIndex;
    minValueSoFar = array[unsortedIndex];
    for (int seekMinIndex = unsortedIndex + 1; seekMinIndex < size; seekMinIndex++) {
     if (array[seekMinIndex] < minValueSoFar) { // found a smaller min value than before, save it for now
        if (verbose) cout << "  prev min in array[" << minIndexSoFar << "]=" << minValueSoFar;
        minValueSoFar = array[seekMinIndex];
        minIndexSoFar = seekMinIndex;
        if (verbose) cout << "; new min in array[" << minIndexSoFar << "]=" << minValueSoFar << endl;
      }
    } 
    if (verbose) {
      cout << "  swap leftwards in array[" << unsortedIndex << "]=" << array[unsortedIndex];
      cout << " with min in array[" << minIndexSoFar << "]=" << array[minIndexSoFar] << endl;
    }

    array[minIndexSoFar] = array[unsortedIndex];
    array[unsortedIndex] = minValueSoFar;

    if (verbose) {
      cout << "After pass #" << unsortedIndex << ", array is: ";
      show_array(array, size);
      cout << endl;
    }
  }
}

/* TEST OUTPUT:

*/