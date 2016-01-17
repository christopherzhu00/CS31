#include <iostream>
using namespace std;
int main()
{
	int arr[12] = { 1, 3, 5, 0, 7, 2, 0, 4, 4, 0, 8, 8 };
	int count = 0;
	for (int i = 0; i < 11; i++) {
		if (arr[i] = arr[i + 1])
			count++;
		else
			count--;
	}
	cout << count << endl;
}