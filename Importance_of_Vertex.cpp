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
	input_file.open("No_of_Shortest_Paths.txt");
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

int find_min(vector<int> &v, vector<bool> &visited)
{
	int min = INT_MAX, curr_min_index = 0, i = 0;
	vector<int>::iterator nodes_itr = v.begin();
	vector<bool>::iterator visited_itr = visited.begin();
	for (; nodes_itr != v.end(); nodes_itr++, visited_itr++, i++)
	{
		if (*visited_itr == false && min > *nodes_itr)
		{
			min = *nodes_itr;
			curr_min_index = i;
		}
	}
	//cout << endl << "Found Min : " << curr_min_index << endl;
	return curr_min_index;
}

bool end_or_continue(vector<bool> visited)
{
	bool flag = false;
	vector<bool>::iterator itr = visited.begin();
	while (itr != visited.end())
	{
		if (*itr == false)
		{
			flag = true;
			break;
		}
		itr++;
	}
	//cout << endl << "End or Continue Called" <<endl;
	return flag;
}

void modified_dijkstra(int n, int** adj, int source, int destination, vector<int> &dist_from_source, vector<bool> &visited, vector<int> &times_updated, vector<int> *path)
{
	int i, j, s, d, curr_indx;
	while (end_or_continue(visited))
	{
		curr_indx = find_min(dist_from_source, visited);
		for (j = 0; j < n; j++)
		{
			if (j != curr_indx && adj[curr_indx][j] != 0 && ((dist_from_source[curr_indx] + adj[curr_indx][j]) <= dist_from_source[j]))
			{
				if ((dist_from_source[curr_indx] + adj[curr_indx][j]) == dist_from_source[j])
				{
					times_updated[j] += times_updated[curr_indx];
					path[j].insert(path[j].end(), path[curr_indx].begin(), path[curr_indx].end());
					path[j].push_back(j);
					//times_updated[j] += 1;
				}
				else if ((dist_from_source[curr_indx] + adj[curr_indx][j]) < dist_from_source[j])
				{
					times_updated[j] = times_updated[curr_indx];
					path[j].erase(path[j].begin(), path[j].end());
					path[j].insert(path[j].begin(), path[curr_indx].begin(), path[curr_indx].end());
					path[j].push_back(j);
				}
				
				dist_from_source[j] = dist_from_source[curr_indx] + adj[curr_indx][j];
			}
		}
		visited[curr_indx] = true;
	}
}

int main()
{
	int n, i, source, destination;
	float betweenness_centrality = 0.0f;
	int** adj;
	ifstream input_file;
	input_file.open("No_of_Shortest_Paths.txt");
	if (input_file.is_open()) {
		n = calc_n(input_file);
		adj = create_adj(n, input_file);
	}
	else
		cout << "File can't be opened";

	if (n > 0)
	{
		cout << "No of nodes : " << n << ", so source and destination must be within this limit." << endl;
		cout << "Enter Source Node index : ";
		cin >> source;
		cout << endl << "Now Enter Destination Node index : ";
		cin >> destination;
		vector<int> dist_from_source;
		vector<bool> visited;
		vector<int> times_updated;
		vector<int> No_Sp_Psd_Thr(n, 0);
		vector<int> *path = new vector<int> [n];
		for (i = 0; i < n; i++)
		{
			visited.push_back(false);
			dist_from_source.push_back(INT_MAX);
		}
		dist_from_source[source] = 0;
		for (i = 0; i < n; i++)
		{
			if (i == source) {
				times_updated.push_back(1);
				path[source].push_back(source);
			}
			else
			{
				times_updated.push_back(0);
			}
		}

		modified_dijkstra(n, adj, source, destination, dist_from_source, visited, times_updated, path);

		/*for (i = 0; i < n; i++)
			cout << endl << dist_from_source[i] << endl;*/

		cout << endl << "Total No. of Shortest Paths from " << source + 1 << " th node to " << destination + 1 << " th node is : " << times_updated[destination] << endl;

		////Loop to print The full path from source to that node by different paths
		//for (i = 0; i < n; i++)
		//{
		//	for (vector<int>::iterator it = path[i].begin(); it != path[i].end(); it++)
		//	{
		//		cout << "  " << *it << "  ";
		//	}
		//	cout << endl;
		//}

		vector<int>::iterator it = path[destination].begin();
		while (it != path[destination].end())
		{
			No_Sp_Psd_Thr[*it]++;
			it++;
		}

		cout << endl << "Cetrality of vertices :- " << endl;
		it = No_Sp_Psd_Thr.begin();
		i = 0;
		while (it != No_Sp_Psd_Thr.end())
		{
			cout<<"Vertex ["<<i<<"] = "<<*it<<endl;
			if (i != source && i != destination)
			{
				betweenness_centrality += ((float)*it / (float)times_updated[destination]);
			}
			it++;
			i++;
		}
		cout << endl << "Betweenness Centrality of vertives : " << betweenness_centrality << endl;
	}
}