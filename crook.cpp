#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <ctime>
#include <tgmath.h>

using namespace std;

vector<vector <int> > simpleSolve(vector<vector<int> > grid, int n);
bool valid(vector<vector <int> > grid, int n);
string gridToString(vector<vector<int> > grid, int n);
int countBlanks(vector<vector <int> > grid, int n);

int blanks;
int lowestBlank = 140;

/*
 * input format: first line contains the value of n for the n x n the grid, followed by the sudoku itself,
 * with each value separated by a space or new line. blank spaces are represented as 0.
 * Example input:
 * 4
 * 0 2 0 4
 * 4 1 2 0
 * 0 4 0 0
 * 0 0 4 0
 *
 * multiple sudoku puzzles can be solved with one input file, seperated by a new line, each
 * with its own value of n.
 *
 * Acceptable values of n must have a natural number for a square root. Program assumes 
 * proper input.
 */

int main(int argc, char **argv) {
	string infile = argv[1];
	string outfile = argv[2];
	ifstream IN(infile);
	ofstream OUT(outfile);
	string line;
	while (IN.is_open() && getline(IN, line)) {
		if (line.empty()) break;
		stringstream ss;
		ss << line;
		string temp;
		ss >> temp;
		int n = stoi(temp);
		vector<vector<int> > grid;
		for(int i = 0; i < n; i++) {
			vector<int> row;
			stringstream ss2;
			string temp1;
			getline(IN, line);
			ss2 << line;
			for (int j = 0; j < n; j++) {
				ss2 >> temp1;
				row.push_back(stoi(temp1));
			}
			grid.push_back(row);
		}
		clock_t begin = clock();
		blanks = countBlanks(grid, n);
		grid = simpleSolve(grid, n);
		cout << gridToString(grid, n);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Time to solve: " << to_string(elapsed_secs) << endl;
	}
	OUT.close();
	IN.close();
}

bool valid(vector<vector <int> > grid, int n) {
	int root = sqrt(n);
	for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if (grid[i][j] != 0) {
				int val = grid[i][j];
				for (int k = 0; k < n; k++) {
					if (grid[i][k] == val && k != j) return false;
					if (grid[k][j] == val && k != i) return false;
				}
				int boxRowStart = i/root * root;
				int boxColStart = j/root * root;
				for (int k = boxRowStart; k < boxRowStart + root; k++) {
					for (int h = boxColStart; h < boxColStart + root; h++) {
						if (grid[k][h] == val && !(k == i && h == j)) {
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

int countBlanks(vector<vector <int> > grid, int n) {
	int blanks = 0;
	for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if (grid[i][j] == 0) blanks++;
		}
	}
	return blanks;
}

vector<vector <int> > simpleSolve(vector<vector<int> > grid, int n) {
	if (blanks == 0) return grid;
	for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if (grid[i][j] == 0) {
				for (int k = 1; k <= n; k++) {
					grid[i][j] = k;
					if(valid(grid,n)) {
						blanks--;
						if (blanks < lowestBlank) lowestBlank = blanks;
						cout << to_string(blanks) << " " << to_string(lowestBlank) << endl;
						int before = blanks;
						grid = simpleSolve(grid,n);
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

string gridToString(vector<vector< int> > grid, int n) {
	string retval = "";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			retval += to_string(grid[i][j]) + " ";
		}
	retval += '\n';
	}
	return retval;
}
