
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <tgmath.h>
#include <map>
#include <algorithm>

using namespace std;

bool valid(vector<vector <int> > grid, int i, int j);
string gridToString(vector<vector<int> > grid);
int countBlanks(vector<vector <int> > grid);
vector<vector <int> > simpleSolve(vector<vector<int> > grid);
vector<int> markup(vector<vector<int> > grid, vector<int> marks, int coordinates);
map<int, vector<int> > update(vector<vector<int> > grid, map<int, vector <int> > markups, vector<int> emptyCells, int coordinates);
vector<vector <int> > crook(vector<vector<int> > grid);

// the n x n size of the grid
int n;
// the number of blanks remaining in the grid
int blanks;

/*
 * Input file format: first line contains the value of n for the n x n the grid, followed by the sudoku itself,
 * with each value separated by a space, and each row seperated by a new line. blank spaces are represented as 0.
 * Example input file:
 * 4
 * 0 2 0 4
 * 4 1 2 0
 * 0 4 0 0
 * 0 0 4 0
 *
 *
 * Acceptable values of n must have a natural number square root. Program assumes 
 * proper input. For now only 4x4's and 9x9's work properly. 16x16's work theoretically, 
 * but take too long in execution to actually test.
 *
 * This program assumes correct input, including a solvable sudoku. If the sudoku is not solvable, a segfault may occur.
 *
 * Command line format: ./crook <input file name> <algorithm name>
 *  <algorithm name> is either "simple" or "crooks"
 *
 * Example command line: ./crook sudoku.txt simple
 */


int main(int argc, char **argv) {
	string infile = argv[1];
	string alg = argv[2];
	ifstream IN(infile);
	string line;
	if (IN.is_open() && getline(IN, line)) {
		stringstream ss;
		ss << line;
		string temp;
		ss >> temp;
		n = stoi(temp);
		vector<vector<int> > grid;
		for(int i = 0; i < n; i++) {
			vector<int> row;
			getline(IN, line);
			ss.clear();
			ss << line;
			for (int j = 0; j < n; j++) {
				ss >> temp;
				row.push_back(stoi(temp));
			}
			grid.push_back(row);
		}
		clock_t begin = clock();
		blanks = countBlanks(grid);
	    if(alg == "simple") grid = simpleSolve(grid);
		if(alg == "crooks") grid = crook(grid);
		cout << gridToString(grid);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Time to solve: " << to_string(elapsed_secs) << endl;
	}
	IN.close();
}

/*
 * determines if a grid violates rules of a sudoku puzzle after updating a value
 * will return false if there is a duplicate number in the given row, column, or box.
 */ 

bool valid(vector<vector <int> > grid, int i, int j) {
	int root = sqrt(n);
	int val = grid[i][j];
	for (int k = 0; k < n; k++) {
		if (grid[i][k] == val && k != j) return false; // check each value in the row
		if (grid[k][j] == val && k != i) return false; // check each value in the column
	}
	int boxRowStart = i/root * root; // find first row in box
	int boxColStart = j/root * root; // find first row in column
	for (int k = boxRowStart; k < boxRowStart + root; k++) {
		for (int h = boxColStart; h < boxColStart + root; h++) {
			if (grid[k][h] == val && !(k == i && h == j)) { // check each value in box
				return false;
			}
		}
	}
	return true;
}

/*
 * converts the grid to a string for easy printing
 */

string gridToString(vector<vector< int> > grid) {
	string retval = "";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if ((j % ((int)sqrt(n)) == 0) && j != 0) retval += "|";
			retval += to_string(grid[i][j]) + " ";
		}
		if (((i+1) % ((int)sqrt(n) ) == 0) && i != n-1) {
			retval += "\n";
			for (int j = 0; j < n + sqrt(n) - 1; j++) {
				retval += "— ";
			}
		}
		retval += '\n';
	}
	return retval;
}

/*
 * used to initialize the value of blanks
 */

int countBlanks(vector<vector <int> > grid) {
	int blanks = 0;
	for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if (grid[i][j] == 0) blanks++;
		}
	}
	return blanks;
}

/*
 * simple brute force algorithm. recursively tries every possible combination
 */

vector<vector <int> > simpleSolve(vector<vector<int> > grid) {
	if (blanks == 0) return grid;
	for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if (grid[i][j] == 0) { 
				for (int k = 1; k <= n; k++) {
					grid[i][j] = k;
					if(valid(grid, i, j)) {
						blanks--;
						int before = blanks;
						grid = simpleSolve(grid);
						if (blanks != before || blanks == 0) return grid;
						else blanks++;
						if (k == n) {
							grid[i][j] = 0;
							return grid;
						}
					} else if (k == n) {
						grid[i][j] = 0;
						return grid;
					}
				}
			}
		}
	}
	return grid;
}

/*
 * the big bad wolf
 */

vector<vector <int> > crook(vector<vector<int> > grid) {
	map<int, vector<int> > markups;
	vector<int> emptyCells;
	vector<int> fullMarkup;
	// initial markups
	for (int i = 1; i <= n; i++) {
		fullMarkup.push_back(i);
	}
	for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if (grid[i][j] == 0) {
				int coordinates = i * n + j;
				emptyCells.push_back(coordinates);
				pair<int, vector<int> > cell = make_pair(coordinates, fullMarkup);
				markups.insert(cell);
			}
		}
	}
	fullMarkup.clear();
	for(int i = 0; i < emptyCells.size(); i++) {
		int coordinates = emptyCells[i];
		vector<int> currentMarkup = markups[coordinates];
		markups[coordinates] = markup(grid, currentMarkup, coordinates);
		if(markups[coordinates].size() == 1) {
			int col = coordinates % n;
			int row = (coordinates - col) / n;
			grid[row][col] = markups[coordinates][0];
			emptyCells.erase(emptyCells.begin() + i);
			i = -1;
		}
	}
	if(emptyCells.size() == 0) return grid;
	// time for some preemptive set hunting
	for(int i = 0; i < emptyCells.size(); i++) {
		int baseCoordinates = emptyCells[i];
		vector<int> marks = markups[baseCoordinates];
		int col = baseCoordinates % n;
		int row = (baseCoordinates - col) / n;
		int m = marks.size();
		vector<int> PESet;
		for (int j = 0; j < n; j++) {
			int tempCoordinates = (row * n) + j;
			if(count(emptyCells.begin(), emptyCells.end(), tempCoordinates) == 1 && marks == markups[tempCoordinates]) {
				m--;
				PESet.push_back(tempCoordinates);
			}
		}
		if (m == 0) {
			for (int j = 0; j < n; j++) {
				int tempCoordinates = (row * n) + j;
				if (count(emptyCells.begin(), emptyCells.end(), tempCoordinates) == 1 && count(PESet.begin(), PESet.end(), tempCoordinates) == 0) {
					for (int k = 0; k < marks.size(); k++) {
						for (int h = 0; h < markups[tempCoordinates].size(); h++) {
							if (marks[k] == markups[tempCoordinates][h]) {
								markups[tempCoordinates].erase(markups[tempCoordinates].begin() + h);
								h--;
								i = -1;
							}
						}
					}
				}
			}
		}
		m = marks.size();
		PESet.clear();
		for (int j = 0; j < n; j++) {
			int tempCoordinates = (j * n) + col;
			if(count(emptyCells.begin(), emptyCells.end(), tempCoordinates) == 1 && marks == markups[tempCoordinates]) {
				m--;
				PESet.push_back(tempCoordinates);
			}
		}
		if (m == 0) {
			for (int j = 0; j < n; j++) {
				int tempCoordinates = (j * n) + col;
				if (count(emptyCells.begin(), emptyCells.end(), tempCoordinates) == 1 && count(PESet.begin(), PESet.end(), tempCoordinates) == 0) {
					for (int k = 0; k < marks.size(); k++) {
						for (int h = 0; h < markups[tempCoordinates].size(); h++) {
							if (marks[k] == markups[tempCoordinates][h]) {
								markups[tempCoordinates].erase(markups[tempCoordinates].begin() + h);
								h--;
								i = -1;
							}
						}
					}
				}
			}
		}
		m = marks.size();
		PESet.clear();
		int root = sqrt(n);
		int boxRowStart = row/root * root; // find first row in box
		int boxColStart = col/root * root; // find first row in column
		for (int k = boxRowStart; k < boxRowStart + root; k++) {
			for (int h = boxColStart; h < boxColStart + root; h++) {
				int tempCoordinates = (k * n) + h;
				if(count(emptyCells.begin(), emptyCells.end(), tempCoordinates) == 1 && marks == markups[tempCoordinates]) {
					m--;
					PESet.push_back(tempCoordinates);
				}
			}
		}
		if (m == 0) {
			for (int k = boxRowStart; k < boxRowStart + root; k++) {
				for (int h = boxColStart; h < boxColStart + root; h++) {
					int tempCoordinates = (k * n) + h;
					if (count(emptyCells.begin(), emptyCells.end(), tempCoordinates) == 1 && count(PESet.begin(), PESet.end(), tempCoordinates) == 0) {
						for (int q = 0; q < marks.size(); q++) {
							for (int x = 0; x < markups[tempCoordinates].size(); x++) {
								if (marks[q] == markups[tempCoordinates][x]) {
									markups[tempCoordinates].erase(markups[tempCoordinates].begin() + x);
									x--;
									i = -1;
								}
							}
						}
					}
				}
			}
		}
		if (marks.size() == 1) {
			grid[row][col] = marks[0];
			emptyCells.erase(emptyCells.begin() + i);
			i = -1;
		}
	}
	if(emptyCells.size() != 0) {
		int coordinates = emptyCells[0];
		int col = coordinates % n;
		int row = (coordinates - col) / n;
		for(int i = 0; i < markups[coordinates].size(); i++) {
			grid[row][col] = markups[coordinates][i];
			vector<vector <int> > testGrid = crook(grid);
			if(countBlanks(testGrid) == 0) return testGrid;
		}

	}
	emptyCells.clear();
	markups.clear();
	return grid;
}

/*
 * Given a set of coordinates of a cell and the old markup list, this will return the updated markup for the given cell
 */

vector<int> markup(vector<vector<int> > grid, vector<int> marks, int coordinates) {
	int j = coordinates % n;
	int i = (coordinates - j) / n;
	for(int h = 0; h < marks.size(); h++) {
		int val = marks[h];
		bool found = false;
		for (int k = 0; k < n && !found; k++) {
			if ((grid[i][k] == val && k != j) || (grid[k][j] == val && k != i)) {
				marks.erase(marks.begin() + h);
				h--;
				found = true;
			}
		}
		if(!found) {
			int root = sqrt(n);
			int boxRowStart = i/root * root; // find first row in box
			int boxColStart = j/root * root; // find first row in column
			for (int k = boxRowStart; k < boxRowStart + root; k++) {
				for (int q = boxColStart; q < boxColStart + root && !found; q++) {
					if (grid[k][q] == val && !(k == i && q == j)) { // check each value in box
						marks.erase(marks.begin() + h);
						h--;
						found = true;
					}
				}
			}
		}
	}
	return marks;
}
