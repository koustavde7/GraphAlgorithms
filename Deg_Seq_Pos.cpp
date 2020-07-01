#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>	//contains sort(), find()
#include <sstream>		//Contains stringstream()
using namespace std;

/*Function to check new degree*/
bool check_new_degree(vector<int> v, int index, int* a)
{
	int i, n = v.size(), count = 0;
	for (i = 0; i < n; i++)
	{
		if (a[i] == 1)
			count++;
	}
	if (count == v.at(index))
		return true;
	else
		return false;
}

/*Function that checks for a graphical degree sequence*/
bool check(vector<int> vc){
	int count;
	bool flag = true;
	vector<int>::iterator itr1 = vc.begin(), itr2;
	if(vc.size() >= 2 && *itr1 >= 1 && *itr1 < vc.size()){
		while(*itr1 > 0)
		{
			sort(vc.begin(), vc.end(), greater<int>());
			count = *itr1;
			for (itr2 = itr1 + 1; count != 0; count--, itr2++)
				*itr2 -= 1;
			vc.erase(vc.begin());
			itr1 = vc.begin();
		}
		for (itr1 = vc.begin(); itr1 != vc.end(); itr1++) {
			if (*itr1 < 0) {
				flag = false;
				break;
			}
		}
		return flag;
	}
	else{
		if (*itr1 == 0)
		{
			itr2 = vc.begin();
			while (itr2 != vc.end())
			{
				if (*itr2 != 0)
				{
					flag = false;
					break;
				}
				itr2++;
			}
			return flag;
		}
		else
			return false;
	}
}

/*Function to check finally the integrity of the created adjacency matrix*/
int** check_final_adj(int n, vector<int> v, int** arr)
{
	int i, j, count;
	bool flag;
	vector<int>::iterator itr1, itr2;

	vector<int> deg_not;	//vector for holding those vertices not having intended degrees as given in the degree sequence
	for (i = 0, itr1 = v.begin(); i < n; i++, itr1++)
	{
		count = 0;
		flag = true;
		for (j = 0; j < n ; j++)
		{
			if (arr[i][j] == 1)
				count++;
		}
		if (count != *itr1)
			deg_not.push_back(i);
	}
	
	if (deg_not.size() > 0)
	{
		itr2 = deg_not.begin();
		i = 0; j = i + 1;
		bool ij_flag, i_itr2_flag, j_next_flag;
		while(itr2 != deg_not.end())
		{
			ij_flag = i_itr2_flag = j_next_flag = true;
			if (find(deg_not.begin(), deg_not.end(), i) == deg_not.end())
			{
				if (arr[i][j] != 1)
					ij_flag = false;
				if (arr[i][*itr2] != 0)
					i_itr2_flag = false;
				if (ij_flag && i_itr2_flag && find(deg_not.begin(), deg_not.end(), j) == deg_not.end())
				{
					arr[i][j] = arr[j][i] = 0;
					arr[i][*itr2] = arr[*itr2][i] = 1;

					j_next_flag = check_new_degree(v, *itr2, arr[*itr2]);	//Checking whether degree completed or not

					if (j_next_flag && arr[j][*(itr2 + 1)] == 0)
					{
						arr[j][*(itr2 + 1)] = arr[*(itr2 + 1)][j] = 1;
						itr2 += 2;
					}
					else if(j_next_flag == false && arr[j][*itr2] == 0)
					{
						arr[j][*itr2] = arr[*itr2][j] = 1;
						itr2 += 1;
					}
				}
			}
			if (find(deg_not.begin(), deg_not.end(), i) != deg_not.end())
			{
				i++;
				j++;
			}
			else
				j++;
		}
	}
	return arr;
}

/*Create Adjacency matrix for valid degree sequence*/
int** create_adj_matrix(vector<int> v)
{
	int n = v.size(), i, j, count;
	int** arr = new int*[n];
	vector<int> vt(v);	//Using copy constructor
	vector<int>::iterator itr = v.begin();
	
	/*allocating memory for 2D array*/
	for(i = 0; i < n; i++)
		arr[i] = new int[n];
	
	/*Initialize the adjacency matrix to all 0s*/
	for (i = 0, itr = vt.begin(); i < n; i++)
	{
		count = *itr;
		for (j = i; j < n; j++)
		{
			if (i == j || count <= 0 || vt.at(j) <= 0) {
				arr[i][j] = 0;
				arr[j][i] = 0;
			}
			else if (vt.at(j) > 0 && count > 0)
			{
				arr[i][j] = 1;
				arr[j][i] = 1;
				vt.at(j) -= 1;
				count--;
			}
		}
		itr++;
	}
	arr = check_final_adj(n, v, arr);
	return arr;
}
int main()
{
	int i, j, t, v_size;
	bool flag;
	string str_of_numbers, temp;
	cout<<"Enter the degree sequence : ";
	getline(cin, str_of_numbers);
	stringstream X(str_of_numbers);
	vector<int> v;
	i = 0;
	while(getline(X, temp, ' '))
	{
		t = std::stoi(temp);
		v.push_back(t);
		i++;
	}
	v_size = v.size();
	int** adj;
	vector<int>::iterator itr;
	sort(v.begin(), v.end(), greater<int>());
	flag = check(v); //Calling function 'check' to check for the possibility of graphical degree sequence 
	if (flag)
		cout << "Degree Sequence is graphical" << endl;
	else
	{
		if (v.size() > 0)
			cout << "Degree Sequence is not graphical" << endl;
		else if (v.size() == 0)
			cout << "Trivial Graph of order 0" << endl;
	}

	cout << endl;

	if (flag) {
		adj = create_adj_matrix(v);
		for (i = 0; i < v_size; i++)
		{
			for (j = 0; j < v_size; j++)
			{
				cout << adj[i][j] << "   ";
			}
			cout << endl;
		}
	}
	return 0;
}