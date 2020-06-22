#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>


using namespace std;

bool isinvec(vector<int> vec, int value) {

	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == value)
			return true;
	}
	return false;

};
vector<int>  vector_indexing(vector<int> vec, vector<int> inds) {

	vector<int> vals;
	for(int i=0;i<inds.size();i++){
	
		vals.push_back(vec[inds[i]]);
	}
	return vals;
}

vector<int> find_smaller_than(vector<int> vec, int val) {
	vector<int> inds;
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] < val) {
			inds.push_back(i);
		}
	}
	return inds;
}

vector<int> find_greater_than(vector<int> vec, int val) {
	vector<int> inds;
	for (int i = 0; i < vec.size(); i++) {
if (vec[i] > val) {
	inds.push_back(i);
}
	}
	return inds;
}
vector<int> fetch_greater_than(vector<int> vec, int val) {
	vector<int> vals;
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] > val) {
			vals.push_back(vec[i]);
		}
	}
	return vals;
}

vector<int> find_in_vec(vector<int> vec, int val) {
	vector<int> inds;
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == val) {
			inds.push_back(i);
		}
	}
	return inds;
}

int count_in_vec(vector<int> vec, int val) { //count number of occurences of a certain value in a vector
	int num_occ = 0;
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == val) {
			num_occ += 1;
		}
	}

	return num_occ;
}

int bin2dec(vector<bool> bs) //inverse of bitget assuming msb first
{

	int n = 0;
	int size = bs.size();
	for (int i = 0; i < size; i++)
	{
		n = n + ((int)bs[i]) * pow(2, (size - i - 1));
	}

	return n;
}

vector<bool> bitget(int n, int nb)
{
	vector<bool> binarray = {};
	for (int i = nb - 1; i >= 0; i--) {
		binarray.push_back((n >> i) & 1);
	}
	return binarray;
}

int max_of_vec(vector<int> invec) {
	double max_val = -numeric_limits<double>::infinity();//inf;
	for (int i = 0; i < invec.size(); i++) {
		int curr = invec[i];
		if (curr > max_val) {
			max_val = curr;
		}
	}
	return max_val;

}
int argmin_of_vec(vector<int> invec) {
	int argmin_val = -1;
	double min_val = numeric_limits<double>::infinity();//inf;
	for (int i = 0; i < invec.size(); i++) {
		int curr = invec[i];
		if (curr < min_val) {
			min_val = curr;
			argmin_val = i;
		}
	}
	return argmin_val;

}
int min_of_vec(vector<int> invec) {
	double min_val = numeric_limits<double>::infinity();//inf;
	for (int i = 0; i < invec.size(); i++) {
		int curr = invec[i];
		if (curr < min_val) {
			min_val = curr;
		}
	}
	return min_val;

}

vector<int> vec_diff(vector<int> bigvec, vector<int> smvec) {
	vector<int> diffvec = {};

	for (int i = 0; i < bigvec.size(); i++) {

		if (!isinvec(smvec, bigvec[i])) {

			diffvec.push_back(bigvec[i]);
		}
	}

	return diffvec;
}

vector<int> unique_elts(vector<int> vec) {
	vector<int> uniques;
	for (int i = 0; i < vec.size(); i++) {
		int curr_val = vec[i];
		if (count_in_vec(uniques, curr_val) == 0){

			uniques.push_back(curr_val);
			}
		
	}

	return uniques;
}
//
template <typename iob>
void display_2dvec(vector<vector<iob>> invec) {
	cout << "\n";
	for (unsigned int i = 0; i < invec.size(); i++) {
		for (unsigned int j = 0; j < invec[i].size(); j++) {
			cout << invec[i][j] << ",";
		}
		cout << "\n";
	}
}
//
template <typename intorbool>
 void display_1dvec(vector<intorbool> invec) {
	 cout << "\n";
	for (int j = 0; j < invec.size(); j++) {
		cout << invec[j] << ",";
	}
	cout << "\n";
}

 int sub2ind1(int x,int z,int sz_z){
	 int flat_i =  x* (sz_z)+z;
	 return flat_i;
	 }
vector<int> sub2ind(int sz_x, int sz_z,  vector<int> xvals, vector<int> zvals) {

	int x;
	int z;
	vector<int> flat_is;
	for (int i = 0; i < xvals.size(); i++) {
	    x = xvals[i];
		z = zvals[i];
		flat_is.push_back( sub2ind1(x,z,sz_z));

	}

	return flat_is;
}

vector<bool> const1d_bool(int size,bool zeroorone) {
	//int size = 5;
	vector<bool> output_vec = {};
	for (int i = 0; i < size; i++) {	
		output_vec.push_back(zeroorone);
	}
	return output_vec;
}
//
//
template <typename iob>
vector<vector<iob>> read_csv(string filename) {
	// Reads a CSV file into a vector of <string, vector<int>> pairs where
	// each pair represents <column name, column values>

	// Create a vector to store the result
	vector < vector<iob>> result;
	//	vector<int> result_row;
		// Create an input filestream
	ifstream myFile;

	myFile.open(filename);
	// Make sure the file is open
	if (!myFile.is_open()) {
		cout << filename << ":file not open \n";
	};

	// Helper vars
	string line;
	int val;
	int rowIdx = 0;

	// Read data, line by line

	while (std::getline(myFile, line))
	{
		//cout << line;
		// Create a stringstream of the current line
		std::stringstream ss(line);

		// Keep track of the current column index
		int colIdx = 0;
		//	result_row = {};
			// Extract each integer
		vector<iob> newRow;
		while (ss >> val) {

			// Add the current integer to the 'colIdx' column's values vector
		//	cout << val;
			//	cout << ',';
		//	result_row.push_back(val);
			newRow.push_back(val);
			// If the next token is a comma, ignore it and move on
			if (ss.peek() == ',') ss.ignore();
			else colIdx++;

			// Increment the column index
			//colIdx++;
		}
		//	result[rowIdx].assign(result_row);
		result.push_back(newRow);
		rowIdx++;
		//	cout << '\n';

	}

	// Close file
	myFile.close();
	//cout << result.size();
	return result;
}
//
//
template <typename iob>
void write_csv(vector<vector<iob>> arr, string filename) {


	std::ofstream myfile;//{ filename};

	//if (arr.size()>0){
	myfile.open(filename);
	if (arr.size() > 0) {
	for (unsigned int i = 0; i < arr.size(); i++) {

			for (unsigned int j = 0; j < (arr[i].size() - 1); j++) {
				myfile << arr[i][j];
				myfile << ',';

			}

		myfile << arr[i][arr[i].size() - 1];
		myfile << "\n";
	}
	}
	myfile.close();
	//}
}
void make_empty_file(string filename) {
	std::ofstream outfile(filename);
}
