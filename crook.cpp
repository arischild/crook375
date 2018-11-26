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

using namespace std;

vector<vector <int> > solve(vector<vector<int> > grid, int n);

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
		grid = solve(grid, n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				OUT << grid[i][j] << " ";
			}
			OUT << endl;
		}
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		OUT << "Time to solve: " << to_string(elapsed_secs) << endl;
	}
	OUT.close();
	IN.close();
}

vector<vector <int> > solve(vector<vector<int> > grid, int n) {
	return grid;
}
