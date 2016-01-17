#include <iostream>
#include <string>
#include <cctype>
using namespace std;

char translateNote(int octave, char noteLetter, char accidentalSign);
int translateTune(string tune, string& instructions, int& badBeat);
bool isTuneWellFormed(string tune);
bool withinBounds(int x, string tune);
bool checkForAccidental(int x, string tune);
bool checkForDigit(int x, string tune);
bool playable(string tune);
int badBeatValue(string tune);
string getSingleNote(string tune, int& counter);
string getMultiNote(string tune, int& counter);




int main()
{
	cerr << "*" << endl;
	string instrs;
	int badb;
	cerr << translateTune("G3B3DD5//G/A/A3B/C5/B3D5//G//G//CE5//C5/D5/E5/F#5/B3G5//G//G/", instrs, badb) << endl;
	cerr << instrs << endl;


}

bool isTuneWellFormed(string tune)
{
	if (tune[tune.size() - 1] != '/')			// checks to make sure it ends in a '/'
		return false;
	for (int k = 0; k < tune.size(); k++)			// goes through entire string
	{
		if (k == (tune.size() - 1))
		{
			return true;
		}
		else if (tune[k] >= 'A' && tune[k] <= 'G')		// checks to see if valid character
		{
			if (withinBounds(k + 1, tune))
			{
				if (checkForAccidental(k + 1, tune))
				{
					if (withinBounds(k + 2, tune))
					{
						if (checkForDigit(k + 2, tune))
						{
							k += 2;
							continue;
						}
					}
					k++;
					continue;
				}
				else if (checkForDigit(k + 1, tune))
				{
					k++;
					continue;
				}
			}
			continue;
		}
		else if (tune[k] == '/')
		{
			continue;
		}
		return false;
	}
}

bool withinBounds(int x, string tune)
{
	if (x < tune.size())
	{
		return true;
	}
	return false;
}

bool checkForAccidental(int x, string tune)
{
	if (tune[x] == '#' || tune[x] == 'b')
	{
		return true;
	}
	return false;
}

bool checkForDigit(int x, string tune)
{
	if (tune[x] >= '0' && tune[x] <= '9')
	{
		return true;
	}
	return false;
}



int translateTune(string tune, string& instructions, int& badBeat)
{
	
	if (!isTuneWellFormed(tune))		// returns 1 if not tune is not well formed
	{
		return 1;
	}
	else if (!(playable(tune)))			// changes badBeat to the beat # which doesn't work and returns 2
	{
		badBeat = badBeatValue(tune);
		return 2;
	}
	else
	{

		char note; //create a holder for all notes
		 
		int length = -1;
		bool first = true;
		bool last = false;
		bool chord = false;
		string now;
		for (int k = 0; k < tune.size(); k++)  // goes through the whole string
		{
			int count = 0;
			if (k - 1 == length)					// check to see if end of beat
			{
				for (int x = k; x < tune.size(); x++)		// calculate next beat 
				{
					if (tune[x] == '/')
					{
						now = tune.substr(k, x - k);
						length = x;
						first = true;
						break;
					}
				}
			}
			for (int y = 0; y < now.size(); y++)		// checking for chord
			{
				if (isalpha(now[y]))
				{
					count++;
					continue;
				}
			}
			if (count > 1)
			{
				chord = true;
			}
			else
			{
				chord = false;
			}
			if (tune[k] == '/' && k < tune.size())					// check for '/' and fill appropriately with a space
			{
				instructions += " ";
				continue;
			}
			if (chord)			// do specific instructions for chords
			{
				if (first)		// add bracket for first note of chord
				{
					instructions += "[";
					first = false;
				}

				string placeholder = getMultiNote(tune, k);

				if (tune[k + placeholder.size() - 1] == '/')
				{
					last = true;
					first = true;
				}
				if (placeholder.size() == 1)			// check to see if just letter note
				{
					note = translateNote(4, placeholder[0], ' ');			// translates the note to be 4th octave regular letter note
					instructions += note;									// adds to final instructions
				}
				else if (placeholder.size() == 2)		// checks to see whether letter note + accidental note or letter note + octave number
				{
					if (checkForDigit(1, placeholder))	// letter note + octave number, translates then adds to instructions
					{
						note = translateNote(placeholder[1] - '0', placeholder[0], ' ');
						instructions += note;
					}
					else                               // letter note + accidental note, fills accordingly to instructions and assume from 4th octave
					{
						note = translateNote(4, placeholder[0], placeholder[1]);
						instructions += note;
					}
				}
				else if (placeholder.size() == 3)		// letter note + accidental note + octave number and fills accordingly to instructions
				{
					note = translateNote(placeholder[2] - '0', placeholder[0], placeholder[1]);
					instructions += note;
				}
				if(last)
				{
					instructions += "]";
					last = false;
				}
			}
			else
			{
				string placeholder = getSingleNote(tune, k);		// gets one note and puts it in placeholder (without '/')	
				cout << placeholder << endl;
				if (placeholder.size() == 1)			// check to see if just letter note
				{
					note = translateNote(4, placeholder[0], ' ');			// translates the note to be 4th octave regular letter note
					instructions += note;									// adds to final instructions
				}
				else if (placeholder.size() == 2)		// checks to see whether letter note + accidental note or letter note + octave number
				{
					if (checkForDigit(1, placeholder))	// letter note + octave number, translates then adds to instructions
					{
						note = translateNote(placeholder[1] - '0', placeholder[0], ' ');
						instructions += note;
					}
					else                               // letter note + accidental note, fills accordingly to instructions and assume from 4th octave
					{
						note = translateNote(4, placeholder[0], placeholder[1]);
						instructions += note;
					}
				}
				else if (placeholder.size() == 3)		// letter note + accidental note + octave number and fills accordingly to instructions
				{
					note = translateNote(placeholder[2] - '0', placeholder[0], placeholder[1]);
					instructions += note;
				}
			}
		}
		return 0;
	}
}

//Used for single note beats since they are separated by a '/'
string getSingleNote(string tune, int& counter)
{
	for (int k = counter; k < tune.size(); k++)
	{
		if (isalpha(tune[k]))			// checks to see if letter
		{
			if (withinBounds(k + 1, tune))				// check if next character exists
			{
				if (checkForAccidental(k + 1, tune))	
				{
					if(withinBounds(k + 2, tune))
					{
						if (checkForDigit(k + 2, tune))		
						{
							counter += 3;					// skips to next note
							return tune.substr(k, 3);
						}
						else
						{
							counter += 2;					// skips to next note
							return tune.substr(k, 2);
						}
					}
				}
				else if (checkForDigit(k + 1, tune))
				{
					counter += 2;							// skips to next note
					return tune.substr(k, 2);
				}
			}
			counter++;										// skips to next note
			return tune.substr(k, 1);
		}
	}
}

//Used for chords since notes are next to each other instead of after a '/'
string getMultiNote(string tune, int& counter)
{
	for (int k = counter; k < tune.size(); k++)
	{
		if (isalpha(tune[k]))
		{
			if (withinBounds(k + 1, tune))
			{
				if (checkForAccidental(k + 1, tune))
				{
					if (withinBounds(k + 2, tune))
					{
						if (checkForDigit(k + 2, tune))
						{
							counter += 2;
							return tune.substr(k, 3);
						}
						else
						{
							counter += 1;
							return tune.substr(k, 2);
						}
					}
				}
				else if (checkForDigit(k + 1, tune))
				{
					counter += 1;
					return tune.substr(k, 2);
				}
			}
			return tune.substr(k, 1);
		}
	}
}

bool playable(string tune)
{
	for (int k = 0; k < tune.size(); k++)
	{
		if (tune[k] == 'C')
		{
			if (withinBounds(k + 1, tune))
			{
				if (tune[k + 1] == 'b')
				{
					if (withinBounds(k + 2, tune))
					{
						if (tune[k + 2] == '6')
						{
							k += 2;
							continue;
						}
					}
				}
				else if (tune[k + 1] == '6')
				{
					k += 1;
					continue;
				}
			}
		}
		else if (tune[k] == 'B')
		{
			if (withinBounds(k + 1, tune))
			{
				if (tune[k + 1] == '#')
				{
					if (withinBounds(k + 2, tune))
					{
						if (tune[k + 2] == '1')
						{
							k += 2;
							continue;
						}
					}
				}
			}
		}
		if (tune[k] == 'C')
		{
			if (withinBounds(k + 1, tune))
			{
				if (tune[k + 1] == 'b')
				{
					if (withinBounds(k + 2, tune))
					{
						if (tune[k + 2] == '2')
						{
							return false;
						}
					}
				}
			}
		}
		if (isdigit(tune[k]))
		{
			if (tune[k] < '2' || tune[k] > '5')
			{
				return false;
			}
		}
	}
	return true;
}
int badBeatValue(string tune)
{
	int beatNumber = 1;
	for (int k = 0; k < tune.size(); k++)
	{
		if (tune[k] == 'C')
		{
			if (withinBounds(k + 1, tune))
			{
				if (tune[k + 1] == 'b')
				{
					if (withinBounds(k + 2, tune))
					{
						if (tune[k + 2] == '6')
						{
							k += 2;
							continue;
						}
					}
				}
				else if (tune[k + 1] == '6')
				{
					k += 1;
					continue;
				}
			}
		}
		else if (tune[k] == 'B')
		{
			if (withinBounds(k + 1, tune))
			{
				if (tune[k + 1] == '#')
				{
					if (withinBounds(k + 2, tune))
					{
						if (tune[k + 2] == '1')
						{
							k += 2;
							continue;
						}
					}
				}
			}
		}
		if (tune[k] == 'C')
		{
			if (withinBounds(k + 1, tune))
			{
				if (tune[k + 1] == 'b')
				{
					if (withinBounds(k + 2, tune))
					{
						if (tune[k + 2] == '2')
						{
							return beatNumber;
						}
					}
				}
			}
		}
		if (isdigit(tune[k]))
		{
			if (tune[k] < '2' || tune[k] > '5')
			{
				return beatNumber;
			}
		}
		if (tune[k] == '/')
		{
			beatNumber++;
		}
	}
	return beatNumber;
}


//*************************************
//  translateNote
//*************************************

// Given an octave number, a note letter, and an accidental sign, return
// the character that the indicated note translates to if it is playable.
// Return a space character if it is not playable.
//
// First parameter:   the octave number (the integer 4 is the number of the
//                    octave that starts with middle C, for example).
// Second parameter:  an upper case note letter, 'A' through 'G'
// Third parameter:   '#', 'b', or ' ' (meaning no accidental sign)
//
// Examples:  translateNote(4, 'A', ' ') returns 'Q'
//            translateNote(4, 'A', '#') returns '%'
//            translateNote(4, 'H', ' ') returns ' '

char translateNote(int octave, char noteLetter, char accidentalSign)
{
	// This check is here solely to report a common CS 31 student error.
	if (octave > 9)
	{
		cerr << "********** translateNote was called with first argument = "
			<< octave << endl;
	}

	// Convert Cb, C, C#/Db, D, D#/Eb, ..., B, B#
	//      to -1, 0,   1,   2,   3, ...,  11, 12

	int note;
	switch (noteLetter)
	{
	case 'C':  note = 0; break;
	case 'D':  note = 2; break;
	case 'E':  note = 4; break;
	case 'F':  note = 5; break;
	case 'G':  note = 7; break;
	case 'A':  note = 9; break;
	case 'B':  note = 11; break;
	default:   return ' ';
	}
	switch (accidentalSign)
	{
	case '#':  note++; break;
	case 'b':  note--; break;
	case ' ':  break;
	default:   return ' ';
	}

	// Convert ..., A#1, B1, C2, C#2, D2, ... to
	//         ..., -2,  -1, 0,   1,  2, ...

	int sequenceNumber = 12 * (octave - 2) + note;

	string keymap = "Z1X2CV3B4N5M,6.7/A8S9D0FG!H@JK#L$Q%WE^R&TY*U(I)OP";
	if (sequenceNumber < 0 || sequenceNumber >= keymap.size())
		return ' ';
	return keymap[sequenceNumber];
}