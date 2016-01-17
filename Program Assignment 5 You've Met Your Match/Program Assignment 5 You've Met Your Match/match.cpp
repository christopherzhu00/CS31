#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
using namespace std;

const int MAX_WORD_LENGTH = 20;
const int MAX_DOCUMENT_CHAR_LENGTH = 200;

void makeLowerCase(char s[]);
void rotateChar(char a[], int size);
bool viableWord(char a[][MAX_WORD_LENGTH + 1], int n);
void rotateLeftDistance(int a[], int n, int pos);
void rotateLeftCString(char a[][MAX_WORD_LENGTH + 1], int n, int pos);
int standardizeRules(int distance[],
	char word1[][MAX_WORD_LENGTH + 1],
	char word2[][MAX_WORD_LENGTH + 1],
	int nRules);
void rotateEverything(int distance[],
	char word1[][MAX_WORD_LENGTH + 1],
	char word2[][MAX_WORD_LENGTH + 1],
	int nRules, int pos);
int determineQuality(const int distance[],
	const char word1[][MAX_WORD_LENGTH + 1],
	const char word2[][MAX_WORD_LENGTH + 1],
	int nRules,
	const char document[]);

int main()
{

}

int standardizeRules(int distance[],
	char word1[][MAX_WORD_LENGTH + 1],
	char word2[][MAX_WORD_LENGTH + 1],
	int nRules)
{
	if (nRules < 0)														// no rules to standardize, number of standardized rules will be 0
		return 0;
	for (int k = 0; k < nRules; k++)									// go through the arrays
	{
		if (!viableWord(word1, k) || !viableWord(word2, k))				// check to see if word has no special characters besides letters
		{
			rotateEverything(distance, word1, word2, nRules, k);		// put it at the end since we do not want to look at it
			k--;														// factor in that we push everything back one space
			nRules--;													// don't want to factor the one we just moved to the end anymore
			continue;
		}
		if (distance[k] <= 0)											// disregard set if distance is not positive
		{
			rotateEverything(distance, word1, word2, nRules, k);
			k--;
			nRules--;
			continue;
		}
		makeLowerCase(word1[k]);										// change all letters to lowercase in case there were uppercase letters
		makeLowerCase(word2[k]);
		for (int i = k + 1; i < nRules; i++)								// go through rest of the array to check for repeats
		{
			makeLowerCase(word1[i]);									// want to make sure we compare when every letter is lowercase
			makeLowerCase(word2[i]);
			if ((strcmp(word1[k], word1[i]) == 0 & strcmp(word2[k], word2[i]) == 0) ||
				(strcmp(word1[k], word2[i]) == 0 & strcmp(word2[k], word1[i]) == 0))
			{
				if (distance[k] >= distance[i])							// move the lesser distance to the end
				{
					rotateEverything(distance, word1, word2, nRules, i);
					k--;												// factor in that we push everything back one space
					nRules--;											// don't want to factor the one we just moved to the end anymore
					continue;
				}
				else
				{
					rotateEverything(distance, word1, word2, nRules, k);		// move the other one if the first one has smaller distance value
					k--;
					nRules--;
					continue;
				}
			}
		}
	}
	return nRules;													// return the number of rules in standard form
}

// combines 3 functions to rotate the corresponding arrays at the same time
void rotateEverything(int distance[],
	char word1[][MAX_WORD_LENGTH + 1],
	char word2[][MAX_WORD_LENGTH + 1],
	int nRules, int pos)
{
	rotateLeftCString(word1, nRules, pos);		
	rotateLeftCString(word2, nRules, pos);
	rotateLeftDistance(distance, nRules, pos);
}
void rotateLeftDistance(int a[], int n, int pos)
{
	int cya = a[pos];						// creates placeholder
	for (int k = pos; k < n - 1; k++)		// shifts values down one
	{
		a[k] = a[k + 1];
	}
	a[n - 1] = cya;							// move original value to end
}
void rotateLeftCString(char a[][MAX_WORD_LENGTH + 1], int n, int pos)
{
	char cya[MAX_WORD_LENGTH + 1];
	strcpy(cya, a[pos]);				// create placeholder
	for (int k = pos; k < n - 1; k++)	// loop for looking at array of values that need to be changed
	{
		strcpy(a[k], a[k + 1]);			// shifts value down one
	}
	strcpy(a[n - 1], cya);				// fill ending value with first value;
}

bool viableWord(char a[][MAX_WORD_LENGTH + 1], int pos)
{
	if (strlen(a[pos]) == 0)
		return false;
	for (int k = 0; k < strlen(a[pos]); k++)		// go through length of string
	{
		if (!isalpha(a[pos][k]))					// check to see if each char is a letter
			return false;
	}
	return true;
}

void makeLowerCase(char s[])
{
	for (int k = 0; s[k] != '\0'; k++)		// go through whole string
		s[k] = tolower(s[k]);				// change each value to lower case
}

int determineQuality(const int distance[],
	const char word1[][MAX_WORD_LENGTH + 1],
	const char word2[][MAX_WORD_LENGTH + 1],
	int nRules,
	const char document[])
{
	char update[MAX_DOCUMENT_CHAR_LENGTH + 1];			// create new array to place all viable values
	int counter = 0;									// int value to hold the spot in the new array
	for (int k = 0; document[k] != '\0'; k++)			// go through document
	{
		if (isalpha(document[k]))
		{
			update[counter] = tolower(document[k]);		// make sure all letters are lowercase
			counter++;
		}
		else if (document[k] == ' ')
		{
			if (update[counter - 1] == ' ')			// don't want multiple spaces in between words
				continue;
			else
			{
				update[counter] = ' ';
			}
			counter++;
		}
	}
	update[counter] = '\0';							// need to end the cstring with a null byte
	for (int i = 0; i < strlen(update); i++)		// make the first char in update array as a letter
	{
		rotateChar(update, counter);
		if (isalpha(update[i + 1]))					// leave loop once it reaches the first letter of the first word
			break;
	}

	int wordCounter = 1;							// used since the last word doesn't have a space at the end
	
	for (int p = 0; update[p] != '\0'; p++)			// find out how many words there are
	{
		if (update[p] == ' ')
		{
			wordCounter++;
		}
	}

	int spot = 0;
	char words[MAX_DOCUMENT_CHAR_LENGTH][MAX_DOCUMENT_CHAR_LENGTH + 1];		// create array to hold a list of words
	for (int r = 0; r <= wordCounter; r++)				// go through as many columns as there are words
	{
		for (int c = 0; update[spot] != ' '; c++)		// fill in the columns with the chars for each word
		{
			words[r][c] = update[spot];
			words[r][c + 1] = '\0';						// don't forget null byte to end C string
			spot++;										// shift position to next char in the update array
		}
		spot++;											// skip the ' ' char to the next word
	}


	int matchQuality = 0;
	bool match = false;
	for (int i = 0; i < nRules; i++)					// go through all the nRules
	{
		for (int k = 0; k < wordCounter; k++)				// go through the words array
		{
			if (strcmp(word1[i], words[k]) == 0)	// check to see if first term in nRules matches any terms in the document
			{
				for (int interval = 1; interval <= distance[i]; interval++)		// used to check up and down the words array for match with the second term in nRules
				{
					if (k + interval < wordCounter)								// want to be in bounds of array
					{
						if (strcmp(word2[i], words[k + interval]) == 0)			// checks down the array location
						{
							matchQuality++;
							match = true;
							break;
						}
					}
					else if (k - interval >= 0)									// want to be in bounds of array
					{
						if (strcmp(word2[i], words[k - interval]) == 0)			// checks above the array location
						{
							matchQuality++;
							match = true;
							break;
						}
					}
				}
				if (match)														// don't want to check for any more occurrences for this set
				{
					match = false;												// takes us to next set to examine
					break;
				}
			}
		}
	}
	return matchQuality;
}

void rotateChar(char a[], int size)			// shifts all the chars down one space in the array
{
	for (int i = 0; i < size; i++)
	{
		a[i] = a[i + 1];
	}
}