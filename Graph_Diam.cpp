#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;
int calc_n(ifstream& file)
{
	int n = 0;
	string line, word;
	if (file.is_open()) {
		while (getline(file, line))
		{
			stringstream str(line);
			while (getline(str, word, ' ')) {
				n++;
				//cout << word << "\t";
			}
		}
	}
	else
		cout << "File can't be opened";
	file.close();
	return sqrt(n);
}
int** create_adj(int n, ifstream& input_file)
{
	int i, j;
	string line, word;
	cout << endl << "Value of n : " << n << endl;
	int** adj = new int* [n];
	for (i = 0; i < n; i++)
		adj[i] = new int[n];
	i = j = 0;
	input_file.open("Adj_Graph.txt");
	if (input_file.is_open()) {
		while (getline(input_file, line)) {
			stringstream stx(line);
			while (getline(stx, word, ' ')) {
				if (j == n && i <= (n - 2)) {
					i++;
					j = 0;
				}
				//cout << endl << word << endl;
				adj[i][j] = stoi(word);
				j++;
			}
		}
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			cout << adj[i][j] << "  ";
		}
		cout << "\n";
	}
	input_file.close();
	return adj;
}

int floyd_warshall(int n, int** adj)
{
	int i, j, k, max_elem;
	for (k = 0; k < n; k++)
	{
		max_elem = 0;
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (adj[i][j] > (adj[i][k] + adj[k][j]))
					adj[i][j] = adj[i][k] + adj[k][j];
				if (max_elem < adj[i][j])
					max_elem = adj[i][j];
			}
		}
	}
	return max_elem;
}

int main()
{
	int n, diam = 0;
	int** adj;
	ifstream input_file;
	input_file.open("Adj_Graph.txt");
	if (input_file.is_open()) {
		n = calc_n(input_file);
		adj = create_adj(n, input_file);
	}
	else
		cout << "File can't be opened";
	diam = floyd_warshall(n, adj);
	cout << "Diameter of the graph is : " << diam;
}