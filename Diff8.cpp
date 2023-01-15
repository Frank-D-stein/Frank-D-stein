#include <iomanip>
#include <iostream>
#include <climits>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <stdlib.h>

// Dillon Frankenstein
// Lab 8
// 11/18/2022 - 11/30/2022 (used one grace day)

using namespace std;

// class used for cost and link matrix 2D vector
class matrix {
	public:
	  vector<int> & operator[](int i) { return M[i]; }
	  void set_size(int, int);

	  vector< vector<int> > M;
};

// sets the size of the matrix
void matrix::set_size(int i , int j) {
	M.resize(i);
	for (int k = 0; k < i; k++) {
		M[k].resize(j);
	}
}

class text_align {
  public:
	text_align() :  DIAG(1), VERT(2), HORZ(4) { }

	void load_data1(string &);
	void load_data2(string &);

	void compute_lcs();
	void print_diff();
	
  private:
	void print_edit(int, int, int, int, int, int);
	void print_diff(int,int);
	
	matrix cost;		// edit costs
	matrix link;		// edit operations
	vector<char> trace; // solution
	
    const int DIAG;		// = 1
	const int VERT;		// = 2
	const int HORZ;		// = 4
	
	vector<string> text1;
    vector<string> text2;
};


void text_align::load_data1(string &original) {
	ifstream in; in.open(original);
	string str;

	if (!in) { cout << "Invalid file 1 passed.\n"; exit(0); }

	// adds lines to text1,
	text1.push_back("e");
	while (getline(in, str)) {
		text1.push_back(str);
	}

	in.close();
}

void text_align::load_data2(string &modified) {
	ifstream in; in.open(modified);
	string str;

	if (!in) { cout << "Invalid file 2 passed.\n"; exit(0); }

	// adds lines to text2 
	text2.push_back("e");
	while (getline(in, str)) {
		text2.push_back(str);
	}

	in.close();
}

// basically the print_allignemnt code
void text_align::compute_lcs() {
	string str1, str2;
	link.set_size(text1.size()+1, text2.size()+1);
	cost.set_size(text1.size()+1, text2.size()+1);
	
	link[0][0] = 0;
	cost[0][0] = 0;

	for (int i = 1; i <= (int)text1.size(); i++) {
		cost[i][0] = cost[i-1][0] + 1;
		link[i][0] = VERT;
	}

	for (int i = 1; i <= (int)text2.size(); i++) {
		cost[0][i] = cost[0][i-1] + 1;
		link[0][i] = HORZ;

	}

	for (int i = 1; i <= (int)text1.size(); i++) {
		for (int j = 1; j <= (int)text2.size(); j++) {
			cost[i][j] = INT_MAX;

			if (text1[i] == text2[j]) {
				cost[i][j] = cost[i-1][j-1];
				link[i][j] = DIAG;
			}

			int delcost = cost[i-1][j] + 1;

			if (delcost < cost[i][j]) {
				cost[i][j] = delcost;
				link[i][j] = VERT;
			}

			int inscost = cost[i][j-1] + 1;

			if (inscost < cost[i][j]) {
				cost[i][j] = inscost;
				link[i][j] = HORZ;
			}
		}
	}
}	

// wrapper function to call the actual priavte print_diff
void text_align::print_diff() {
	// print_diff call
	print_diff(text1.size(), text2.size());
}

void text_align::print_edit(int numI, int numD, int s1, int s2, int end1, int end2) {
	// delete if statement
	if (numI == 0 && numD > 0) {
		// prints first chunk
		if (s1 == end1) {
			cout << end1;
		} else {
			cout << s1 << "," << end1;
		}

		// then the d
		cout << "d";
		// the second chunk
		if (s2 == end2) {
			cout << end2 << '\n';
		} else {
			cout << s2 << "," << end2 << '\n';
		}

		// the actual lines associated witht the indeces
		for (int i = s1; i <=end1; i++) {
			cout << "< " << text1[i] << "\n";
		}
		
		for (int i = s2; i <= end2; i++) {
			cout << "> " << text2[i] << "\n";
		}

	} 
	// insert if statement
	if (numD == 0 && numI > 0) {
		// prints the first chunk
		if (s1 == end1) {
			cout << end1;
		} else {
			cout << s1 << "," << end1;
		}

		// then the a
		cout << "a";
		// the second chunk
		if (s2 == end2) {
			cout << end2 << '\n';
		} else {
			cout << s2 << "," << end2 << '\n';
		}

		// the actual lines associated with the indeces
		for (int i = s2; i <= end2; i++) {
			cout << "< " << text2[i] << "\n";
		}

	}
	// delete and insert if statement
	if (numD > 0 && numI > 0) {
		// prints the first indeces
		if (s1 == end1) {
			cout << end1;
		} else {
			cout << s1 << "," << end1;
		}

		// then the c
		cout << "c";
		// then the second chunk
		if (s2 == end2) {
			cout << end2 << "\n";
		} else {
			cout << s2 << "," << end2 << "\n";
		}

		// then the actual lines associated with the indeces
		for (int i = s1; i <= end1; i++) {
			cout << "< " << text1[i] << "\n";
		}
		cout << "---\n";

		for (int i = s2; i <= end2; i++) {
			cout << "> " << text2[i] << "\n";
		}

	}
}

void text_align::print_diff(int i, int j) {
	int numInsert = 0, numDelete = 0, start1 = 1, start2 = 1, end1= 0, end2 = 0;

	// this works for most cases but sometime It will list too many indeces and I am not sure wh, I am guessing I am
	// handling the start and end of the text chunk wrong
	if (i == 0 && j == 0) {
			for (int k = 0, i=(int)trace.size(); 0<=i; i--, k++) {
				if (trace[i] == DIAG) {
					// calls print edit in the event of a match
					print_edit(numInsert, numDelete, start1, start2, end1, end2);
		
					// resets or increases all the values
					end1++;
					end2++;
					if (start1 != end1) {
						start1 = end1+1;
					}
					if (start2 != end2) {
						start2 = end2+1;
					}
					numDelete = 0;
					numInsert = 0;

				} else
				// in case of a delete
				if (trace[i] == VERT) {
					// number of deletes is increased as well as the chunk part
					numDelete++; 
					if (numDelete == 1) start1 = end1+1;
					end1++;
				} else
					// in case of a insert
				if (trace[i] == HORZ) {
					// number of inserts is increased as well as the chunk part
					numInsert++; 	
					if (numInsert == 1) start2 = end2+1;
					end2++;
				} 
		}
		return;
	}

	// filling the trace vector with the appropriate move
	if (link[i][j] & DIAG) {
		trace.push_back(DIAG);
		print_diff(i-1, j-1);
	} 
	if (link[i][j] & VERT) {
		trace.push_back(VERT);
		print_diff(i-1, j);
	} 
	if (link[i][j] & HORZ) {
		trace.push_back(HORZ);
		print_diff(i, j-1);
	}
}

int main(int argc, char *argv[]) {
	string original, modified;
	
	// command line parsing
	if (argc == 3) {
		for (int i = 0; i < argc-1; i++) {
			string option = argv[i];
			if (option.compare(option.size()-3, 3, "txt") == 0) {
				original = argv[i];
				modified = argv[i+1];
			}
		}
	} else {
		cout << "usage: ./Diff8 original.txt modified.txt \n"; exit(0);
	}
 
	cout << "\n";

	text_align align;  // instantiate "LCS based diff" object

	// functions
	align.load_data1(original);
	align.load_data2(modified);
	
	align.compute_lcs();
	align.print_diff();

	return 0;
}
