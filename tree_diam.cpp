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
	cout<<endl<<"Value of n : "<<n<<endl;
	int** adj = new int* [n];
	for(i = 0; i < n; i++)
		adj[i] = new int [n];
	i = j = 0;
	input_file.open("Adj_Mat.txt");
	if(input_file.is_open()) {
		while(getline(input_file, line)){
			stringstream stx(line);
			while(getline(stx, word, ' ')){
				if(j == n && i <= (n - 2)) {
					i++;
					j = 0;
				}
				adj[i][j] = stoi(word);
				
				j++;
			}
		}
	}
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			cout<<adj[i][j]<<"  ";
		}
		cout<<"\n";
	}
	input_file.close();
	return adj;
}

 int bfs(int root, int n, int** adj, int* temp)
 {
	 int i, j;
	 vector<int> Queue;
	 vector<int> dist_from_root;
	 vector<int>::iterator it;
	 for (i = 0; i < n; i++)
		 dist_from_root.push_back(9999);
	 dist_from_root[root] = 0;
	 Queue.push_back(root);
	 while (!Queue.empty())
	 {
		 i = *(Queue.begin());
		 Queue.erase(Queue.begin());
		 for (j = 0; j < n; j++)
		 {
			 if (adj[i][j] > 0)
			 {
				 if (dist_from_root[j] == 9999)
				 {
					 Queue.push_back(j);
					 dist_from_root[j] = dist_from_root[i] + adj[i][j];
				 }
			 }
		 }
	 }
	 *temp = i;
	 return dist_from_root[i];
 }

 int main() 
 {
	 int n, diam1 = 0, diam2 = 0, temp_vt1, temp_vt2;
	 int** adj;
	 ifstream input_file;
	 input_file.open("Adj_Mat.txt");
	 if (input_file.is_open()) {
		 n = calc_n(input_file);
		 adj = create_adj(n, input_file);
	 }
	 else
		 cout << "File can't be opened";
	 diam1 = bfs(0, n, adj, &temp_vt1);
	 diam2 = bfs(temp_vt1, n, adj, &temp_vt2);
	 cout << "Diameter of the tree is : " << diam1 + abs(diam2 - diam1);
 }