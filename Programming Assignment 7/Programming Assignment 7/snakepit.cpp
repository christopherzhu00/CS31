// snakepit.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Pit::display going.  That gives you
// more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

#include <cassert>

#define CHECKTYPE(f, t) { (void)(t)(f); }


///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the pit
const int MAXCOLS = 40;             // max number of columns in the pit
const int MAXSNAKES = 180;          // max number of snakes allowed

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Pit;  // This is needed to let the compiler know that Pit is a
// type name, since it's mentioned in the Snake declaration.

class Snake
{
public:
	// Constructor
	Snake(Pit* pp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;

	// Mutators
	void move();

private:
	Pit* m_pit;
	int  m_row;
	int  m_col;
};

class Player
{
public:
	// Constructor
	Player(Pit *pp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	int  age() const;
	bool isDead() const;

	// Mutators
	void   stand();
	void   move(int dir);
	void   setDead();

private:
	Pit*  m_pit;
	int   m_row;
	int   m_col;
	int   m_age;
	bool  m_dead;
};

class Pit
{
public:
	// Constructor/destructor
	Pit(int nRows, int nCols);
	~Pit();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     snakeCount() const;
	int     numberOfSnakesAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	bool   addSnake(int r, int c);
	bool   addPlayer(int r, int c);
	bool   destroyOneSnake(int r, int c);
	bool   moveSnakes();

private:
	int     m_rows;
	int     m_cols;
	Player* m_player;
	Snake*  m_snakes[MAXSNAKES];
	int     m_nSnakes;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nSnakes);
	~Game();

	// Mutators
	void play();

private:
	Pit* m_pit;
};

///////////////////////////////////////////////////////////////////////////
//  Snake implementation
///////////////////////////////////////////////////////////////////////////

Snake::Snake(Pit* pp, int r, int c)
{
	if (pp == nullptr)
	{
		cout << "***** A snake must be in some Pit!" << endl;
		exit(1);
	}
	if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
	{
		cout << "***** Snake created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_pit = pp;
	m_row = r;
	m_col = c;
}

int Snake::row() const
{
	return m_row;
}

int Snake::col() const
{
	return m_col;  
}

void Snake::move()
{
	// Attempt to move in a random direction; if we can't move, don't move
	switch (rand() % 4)
	{
		case UP:
			// Move the snake up one row if possible.
			if (m_row >= 2)
				m_row--;
			break;
		case DOWN:
			// Move the snake down one row if possible.
			if (m_row < (m_pit->rows()))
				m_row++;
			break;
		case LEFT:
			// Move the snake left one column if possible
			if (m_col >= 2)
				m_col--;
			break;
		case RIGHT:
			// Move the snake right one column if possible
			if (m_col < (m_pit->cols()))
				m_col++;
			break;
	}
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Pit* pp, int r, int c)
{
	if (pp == nullptr)
	{
		cout << "***** The player must be in some Pit!" << endl;
		exit(1);
	}
	if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_pit = pp;
	m_row = r;
	m_col = c;
	m_age = 0;
	m_dead = false;
}

int Player::row() const
{
	return m_row;  
}

int Player::col() const
{
	return m_col;  
}

int Player::age() const
{
	return m_age;  
}

void Player::stand()
{
	m_age++;
}

void Player::move(int dir)
{
	m_age++;

	// TODO:  If there's a grid position in the indicated direction that is
	//        adjacent to the player and vacant, move the player there.  If
	//        there's a grid position in the indicated direction that is
	//        adjacent to the player and contains one or more snakes, with
	//        a grid position next to and on the other side of the snake(s),
	//        jump over the snake(s), killing (destroying) it, and landing
	//        at that position (and dying if a snake is there).  In any
	//        other case, don't move.

	switch (dir)
	{
		case LEFT:								// we check left most column
			if (m_col == 1)
			{									// on boundary, do nothing
				stand();
				m_age--;
			}
			else if (m_col == 2)			// the column next to left edge
			{
				if (m_pit->numberOfSnakesAt(m_row, 1) > 0)	// do nothing if snake(s) present
				{
					stand();
					m_age--;
				}
				else
					m_col--;					// otherwise move
			}
			else								// any other column
			{
				if (m_pit->destroyOneSnake(m_row, m_col - 1))	// check for jump
				{
					if (m_pit->numberOfSnakesAt(m_row, m_col - 2) > 0)	// check for jump landing for snakes
					{
						m_col -= 2;								// jump and die
						setDead();
					}
					else
						m_col -= 2;								// just jump
				}
				else
					m_col--;					// move without jumping
			}
			break;
		case RIGHT:								// check right most column
			if (m_col == (m_pit->cols()))	// at edge, do nothing
			{
				stand();
				m_age--;
			}
			else if (m_col == (m_pit->cols() - 1))	// check for column next to right edge
			{
				if (m_pit->numberOfSnakesAt(m_row, m_col + 1) > 0)	// do nothing if snake 
				{
					stand();
					m_age--;
				}
				else
					m_col++;						// move otherwise
			}
			else									// any other column
			{
				if (m_pit->destroyOneSnake(m_row, m_col + 1))	// check for jump
				{
					if (m_pit->numberOfSnakesAt(m_row, m_col + 2) > 0)	// check for landing zone
					{
						m_col += 2;					// jump and die
						setDead();
					}
					else
						m_col += 2;					// just jump
				}
				else m_col++;						// just move
			}
			break;
		case UP:
			if (m_row == 1)							// check first row
			{
				stand();
				m_age--;
			}
			else if (m_row == 2)				// row next to first row
			{
				if (m_pit->numberOfSnakesAt(1, m_col) > 0)	// do nothing if snake
				{
					stand();
					m_age--;
				}
				else
					m_row--;						// move otherwise
			}
			else									// any other row
			{
				if (m_pit->destroyOneSnake(m_row - 1, m_col))	// check for jump
				{
					if (m_pit->numberOfSnakesAt(m_row - 2, m_col) > 0)	// check landing zone
					{
						m_row -= 2;					// jump and die
						setDead();
					}
					else
						m_row -= 2;					// just jump
				}
				else m_row--;						// otherwise just move
			}
			break;
		case DOWN:
			if (m_row == (m_pit->rows()))	// do nothing if at bottom row
			{
				stand();
				m_age--;
			}
			else if (m_row == (m_pit->rows() - 1))		// row above last row
			{
				if (m_pit->numberOfSnakesAt(m_row + 1, m_col) > 0)	// do nothing if snakes block the way
				{
					stand();
					m_age--;
				}
				else
					m_row++;						// move otherwise
			}
			else
			{
				if (m_pit->destroyOneSnake(m_row + 1, m_col) > 0)	// check for jump
				{
					if (m_pit->numberOfSnakesAt(m_row + 2, m_col) > 0)	// check landing zone
					{
						m_row += 2;					// jump and die
						setDead();
					}
					else
						m_row += 2;					// just jump
				}
				else
					m_row++;						// move otherwise
			}
			break;
	}	
}

bool Player::isDead() const
{
	return m_dead;  
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Pit implementations
///////////////////////////////////////////////////////////////////////////

Pit::Pit(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Pit created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nSnakes = 0;
}

Pit::~Pit()
{
	//Delete the player and all remaining dynamically allocated snakes.
	for (int k = 0; k < m_nSnakes; k++)		// delete all snakes
	{
		delete m_snakes[k];
	}
	delete m_player;					// delete the player
}

int Pit::rows() const
{
	return m_rows;  
}

int Pit::cols() const
{
	return m_cols;  
}

Player* Pit::player() const
{
	return m_player;
}

int Pit::snakeCount() const
{
	return m_nSnakes;
}

int Pit::numberOfSnakesAt(int r, int c) const
{
	//  Return the number of snakes at row r, column c.
	int counter = 0;
	for (int k = 0; k < m_nSnakes; k++)			// go through array of snakes and check to see if rows and columns match
	{
		if (m_snakes[k]->row() == r && m_snakes[k]->col() == c)
			counter++;
	}
	return counter;
}

void Pit::display(string msg) const
{
	// Position (row,col) in the pit coordinate system is represented in
	// the array element grid[row-1][col-1]
	char grid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill the grid with dots
	for (r = 0; r < rows(); r++)
		for (c = 0; c < cols(); c++)
			grid[r][c] = '.';

	// Indicate each snake's position
	//  If one snake is at some grid point, set the char to 'S'.
	//        If it's 2 though 8, set it to '2' through '8'.
	//        For 9 or more, set it to '9'.
	


	for (r = 0; r < rows(); r++)				// filling grid
	{
		for (c = 0; c < cols(); c++)
		{
			int amount = numberOfSnakesAt(r + 1, c + 1);	// create int for how many snakes at the position
			if (amount == 1)						// set char to 'S' for 1 snake
				grid[r][c] = 'S';
			else if (amount >= 2 && amount <= 8)	// fill correspondingly to amount of snakes
			{
				switch (amount)
				{
					case 2:
						grid[r][c] = '2';
						break;
					case 3:
						grid[r][c] = '3';
						break;
					case 4:
						grid[r][c] = '4';
						break;
					case 5:
						grid[r][c] = '5';
						break;
					case 6:
						grid[r][c] = '6';
						break;
					case 7:
						grid[r][c] = '7';
						break;
					case 8:
						grid[r][c] = '8';
						break;
				}
			}
			else if (amount >= 9)				// 9 and above snakes get a '9'
				grid[r][c] = '9';
		}
	}
	// Indicate player's position
	if (m_player != nullptr)
	{
		char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
		if (m_player->isDead())
			gridChar = '*';
		else
			gridChar = '@';
	}

	// Draw the grid
	clearScreen();
	for (r = 0; r < rows(); r++)
	{
		for (c = 0; c < cols(); c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

	// Write message, snake, and player info
	cout << endl;
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << snakeCount() << " snakes remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player." << endl;
	else
	{
		if (m_player->age() > 0)
			cout << "The player has lasted " << m_player->age() << " steps." << endl;
		if (m_player->isDead())
			cout << "The player is dead." << endl;
	}
}

bool Pit::addSnake(int r, int c)
{
	// If MAXSNAKES have already been added, return false.  Otherwise,
	// dynamically allocate a new snake at coordinates (r,c).  Save the
	// pointer to the newly allocated snake and return true.

	// TODO:  Implement this
	if (m_nSnakes == MAXSNAKES)			// checks for limit
		return false;  
	m_snakes[m_nSnakes] = new Snake(this, r, c);		// dynamically allocate new snake at (r,c)
	m_nSnakes++;						// increase number of snakes that are in grid
	return true;
}

bool Pit::addPlayer(int r, int c)
{
	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Dynamically allocate a new Player and add it to the pit
	m_player = new Player(this, r, c);
	return true;
}

bool Pit::destroyOneSnake(int r, int c)
{
	// Destroy one snake at row r, column c.  Return true if a snake
	//was destroyed, or false if there were no snakes there.
	if(numberOfSnakesAt(r, c) == 0)			// return false if no snakes
		return false;
	for (int k = 0; k < m_nSnakes; k++)			// check all the snakes
	{
		if (m_snakes[k]->row() == r && m_snakes[k]->col() == c)	// delete the first one and then shrink the array
		{
			delete m_snakes[k];
			m_snakes[k] = m_snakes[m_nSnakes - 1];
			m_nSnakes--;
			return true;
		}
	}

}

bool Pit::moveSnakes()
{
	for (int k = 0; k < m_nSnakes; k++)
	{
		//  Have the k-th snake in the pit make one move.
		//        If that move results in that snake being in the same
		//        position as the player, the player dies.
		m_snakes[k]->move();					// move each snake
		if ((m_snakes[k]->row() == m_player->row()) && (m_snakes[k]->col() == m_player->col()))		// check to see if kills player
			m_player->setDead();
	}

	// return true if the player is still alive, false otherwise
	return !m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nSnakes)
{
	if (nSnakes > MAXSNAKES)
	{
		cout << "***** Trying to create Game with " << nSnakes
			<< " snakes; only " << MAXSNAKES << " are allowed!" << endl;
		exit(1);
	}

	// Create pit
	m_pit = new Pit(rows, cols);

	// Add player
	int rPlayer = 1 + rand() % rows;
	int cPlayer = 1 + rand() % cols;
	m_pit->addPlayer(rPlayer, cPlayer);

	// Populate with snakes
	while (nSnakes > 0)
	{
		int r = 1 + rand() % rows;
		int c = 1 + rand() % cols;
		// Don't put a snake where the player is
		if (r == rPlayer  &&  c == cPlayer)
			continue;
		m_pit->addSnake(r, c);
		nSnakes--;
	}
}

Game::~Game()
{
	delete m_pit;
}

void Game::play()
{
	Player* p = m_pit->player();
	if (p == nullptr)
	{
		m_pit->display("");
		return;
	}
	string msg = "";
	do
	{
		m_pit->display(msg);
		msg = "";
		cout << endl;
		cout << "Move (u/d/l/r//q): ";
		string action;
		getline(cin, action);
		if (action.size() == 0)
			p->stand();
		else
		{
			switch (action[0])
			{
			default:   // if bad move, nobody moves
				cout << '\a' << endl;  // beep
				continue;
			case 'q':
				return;
			case 'u':
			case 'd':
			case 'l':
			case 'r':
				p->move(decodeDirection(action[0]));
				break;
			}
		}
		m_pit->moveSnakes();
	} while (!m_pit->player()->isDead() && m_pit->snakeCount() > 0);
	m_pit->display(msg);
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
	switch (dir)
	{
	case 'u':  return UP;
	case 'd':  return DOWN;
	case 'l':  return LEFT;
	case 'r':  return RIGHT;
	}
	return -1;  // bad argument passed in!
}

bool directionToDeltas(int dir, int& rowDelta, int& colDelta)
{
	switch (dir)
	{
	case UP:     rowDelta = -1; colDelta = 0; break;
	case DOWN:   rowDelta = 1; colDelta = 0; break;
	case LEFT:   rowDelta = 0; colDelta = -1; break;
	case RIGHT:  rowDelta = 0; colDelta = 1; break;
	default:     return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

void thisFunctionWillNeverBeCalled()
{
	// If the student deleted or changed the interfaces to the public
	// functions, this won't compile.  (This uses magic beyond the scope
	// of CS 31.)

	Snake(static_cast<Pit*>(0), 1, 1);
	CHECKTYPE(&Snake::row, int  (Snake::*)() const);
	CHECKTYPE(&Snake::col, int  (Snake::*)() const);
	CHECKTYPE(&Snake::move, void (Snake::*)());

	Player(static_cast<Pit*>(0), 1, 1);
	CHECKTYPE(&Player::row, int  (Player::*)() const);
	CHECKTYPE(&Player::col, int  (Player::*)() const);
	CHECKTYPE(&Player::age, int  (Player::*)() const);
	CHECKTYPE(&Player::isDead, bool (Player::*)() const);
	CHECKTYPE(&Player::stand, void (Player::*)());
	CHECKTYPE(&Player::move, void (Player::*)(int));
	CHECKTYPE(&Player::setDead, void (Player::*)());

	Pit(1, 1);
	CHECKTYPE(&Pit::rows, int     (Pit::*)() const);
	CHECKTYPE(&Pit::cols, int     (Pit::*)() const);
	CHECKTYPE(&Pit::player, Player* (Pit::*)() const);
	CHECKTYPE(&Pit::snakeCount, int     (Pit::*)() const);
	CHECKTYPE(&Pit::numberOfSnakesAt, int     (Pit::*)(int, int) const);
	CHECKTYPE(&Pit::display, void    (Pit::*)(string) const);
	CHECKTYPE(&Pit::addSnake, bool    (Pit::*)(int, int));
	CHECKTYPE(&Pit::addPlayer, bool    (Pit::*)(int, int));
	CHECKTYPE(&Pit::destroyOneSnake, bool    (Pit::*)(int, int));
	CHECKTYPE(&Pit::moveSnakes, bool    (Pit::*)());

	Game(1, 1, 1);
	CHECKTYPE(&Game::play, void (Game::*)());
}

void doBasicTests()
{
	{
		Pit brad(10, 20);
		assert(brad.addPlayer(2, 6));
		Player* pp = brad.player();
		assert(pp->row() == 2 && pp->col() == 6 && !pp->isDead());
		pp->move(UP);
		assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
		pp->move(UP);
		assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
		pp->setDead();
		assert(pp->row() == 1 && pp->col() == 6 && pp->isDead());
	}
	{
		Pit brad(2, 2);
		assert(brad.addPlayer(1, 1));
		assert(brad.addSnake(2, 2));
		Player* pp = brad.player();
		assert(brad.moveSnakes());
		assert(!pp->isDead());
		for (int k = 0; k < 1000 && brad.moveSnakes(); k++)
			;
		assert(pp->isDead());
	}
	{
		Pit brad(1, 5);
		assert(brad.addPlayer(1, 1));
		assert(brad.addSnake(1, 2));
		assert(brad.addSnake(1, 4));
		assert(brad.addSnake(1, 4));
		assert(brad.snakeCount() == 3 && brad.numberOfSnakesAt(1, 4) == 2);
		Player* pp = brad.player();
		pp->move(RIGHT);
		pp->move(RIGHT);
		assert(brad.snakeCount() == 1 && brad.numberOfSnakesAt(1, 4) == 1);
		pp->move(LEFT);
		assert(brad.snakeCount() == 0);
		assert(brad.addSnake(1, 5));
		assert(brad.snakeCount() == 1);
	}
	cout << "Passed all basic tests" << endl;
}

int main()
{
	
	// Initialize the random number generator.  (You don't need to
	// understand how this works.)
	srand(static_cast<unsigned int>(time(0)));

	// Create a game
	// Use this instead to create a mini-game:   
	//Game g(3, 3, 2);
	Game g(9, 10, 40);
	doBasicTests();

	// Play the game
	//g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementations
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <cstring>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif