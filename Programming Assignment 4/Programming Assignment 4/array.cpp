#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int rotateRight(string a[], int n, int pos);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);



int main()
{

}


int appendToAll(string a[], int n, string value)
{
	if (n < 0)						//returns -1 if given negative array size
		return -1;
	for (int k = 0; k < n; k++)		//goes through each value in the array that is interesting and appends "!!!"
		a[k] += value;
	return n;						
}

int lookup(const string a[], int n, string target)
{
	for (int k = 0; k < n; k++)		//goes through each value in the array that is interesting
	{
		if (a[k] == target)			//checks to see if value in array matches target
			return k;
	}
	return -1;						// return -1 if target is not found
}

int positionOfMax(const string a[], int n)
{
	if (n <= 0)						// return -1 if array has no elements or given negatrive array size
		return -1;
	string largest;					// create a string to hold the string that is >= every string in the array
	for (int k = 0; k < n; k++)
	{
		if (a[k] > largest)			// changes the value of largest to any viable contenders
			largest = a[k];
	}
	return lookup(a, n, largest);	// calls lookup function in order to return the location of largest
}

int rotateLeft(string a[], int n, int pos)
{
	if (n <= 0)
		return -1;
	string cya = a[pos];			// create a string to hold original value at a[pos]
	for (int k = pos; k < n - 1; k++)	// loop for looking at array of values that need to be changed
	{
		a[k] = a[k + 1];			// shift values of the array to the left
	}
	a[n - 1] = cya;					// fill ending value with first value;
	return pos;
}


int rotateRight(string a[], int n, int pos)
{
	if (n <= 0)						
		return -1;
	string cya = a[pos];			// creates a placeholder for the string at position pos
	for (int k = pos; k > 0; k--)	// shifts values behind pos up one spot
	{
		a[k] = a[k - 1];
	}
	a[0] = cya;						// set first spot in array as the moved string
	return pos;
}

int flip(string a[], int n)
{
	if (n < 0)
		return -1;
	int opposite = n - 1;				// variable for opposite side of array of interesting strings
	for (int k = 0; k < n / 2; k++)		// goes halfway since since we swap the strings at the ends
	{
		string holder = a[k];			// create original in order to perform a swap
		a[k] = a[opposite];				// swaps string on left with string on right
		a[opposite] = holder;			
		opposite--;						// 	decrement opposite to change swapping locations	
	}
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 <= 0 || n2 <= 0)
		return -1;
	int size;							// find the smaller number between n1 and n2
	if (n1 < n2)
		size = n1;
	else 
		size = n2;
	for (int k = 0; k <= size; k++)		// go through until the max size is reached
	{
		if (a1[k] == a2[k])				// check to see if strings in both arrays are equal
		{
			if (k == size)				// return the position if array runs out
			{
				return k;
			}
			continue;					// check the next pair of strings
		}
		return k;
	}
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0)						// check for invalid array sizes
		return -1;
	if (n1 == 0 && n2 == 0)						// sequence of 0 elements is subsequence of any sequence including one with no elements
		return 0;
	else if (n2 == 0)							// sequence of 0 elements will return 0 since it is subsequence of any sequence
		return 0;
	for (int k = 0; k < n1; k++)				// go through first array
	{
		for (int j = 0; j < n2; j++)			// goes through second array to check if match
		{
			if (a1[k + j] == a2[j])
			{
				if (j == n2 - 1)				// return value if all n2 elements of a2 appear in a1 consecutively and in same order
				{
					return k;
				}
				continue;						// check the next string in second array
			}
			else
				break;							// no match results in check next spot in first array for beginning of subsequence
		}
	}
	return -1;									// no match in entire string results in -1
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	for (int k = 0; k < n1; k++)			// goes through first array
	{
		for (int j = 0; j < n2; j++)		// goes through second array
		{
			if (a2[j] == a1[k])				// check if any of the strings in the second array match a string in the first array
				return k;
		}
	}
	return -1;
}


int separate(string a[], int n, string separator)
{
	if (n < 0)
		return -1;
	bool done = false;							// create a boolean to determine when to stop
	for (int k = 0; k < n; k++)					// go through array
	{
		if (a[k] > separator)					// check to see if > than separator
		{
			for (int z = k; z < n; z++)			//go through rest of list to make sure there are more that are < than separator
			{
				if (z == n - 1 && a[z] > separator)	// means that all the rest of the strings in the array are > than separator
				{
					done = true;				
					break;
				}
				if (a[z] > separator)			// if not > than separator, we can still continue to separate so break from this loop
				{
					continue;
				}
				else
					break;
			}
			if (done)							// check to make sure we can stop so we don't infinitely rotateLeft
				break;
			int greaterThan = rotateLeft(a, n, k);	// Eliminate the item at position k by copying all elements after it one place to the left. Put the item that was thus eliminated into the last position of the array.
			k--;								// used to check the value that would have been next in the array
		}
	}
	for (int i = 0; i < n; i++)					// used to find first element after rearrangement that is not < separator
	{
		if (a[i] >= separator)
			return i;
	}
	return n;									// return n if there is no such element that is < than separator
}
