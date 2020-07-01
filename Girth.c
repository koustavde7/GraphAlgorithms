/*Kruskal's Algorithm to find out MST from a given graph in the form 
 of adjacency matrix.
 
 * Author : Koustav De
 * Date : 30.07.2018 - 01.08.2018
 */

#include<stdio.h>
#include<stdlib.h>

/*Declaring the vertex and edge structures*/
struct node{
    int id_old;
    int id_new;
};
struct edge{
    int id_of_node[2];		
    int weight;
    int assoc;    //For checking the association of this edge in the final MST 
};

/*Contains records of the cycles present*/
struct cycles{
    int length;
    struct cycles *next;
};

void sort_edges(struct edge [], int , int , int ); //Declaring Prototype of sort_edges function
void merge(struct edge [], int , int , int, int ); //Declaring Prototype of merge sort function
void Kruskals_Algorithm_to_find_MST(struct node [], struct edge [], int , int ); //Kruskal's Algorithm function prototype
int bfs(int , int , int no_of_nodes, int adj[no_of_nodes][no_of_nodes], int Bool_MST[no_of_nodes][no_of_nodes], int q[], int visited[], int dist_from_parent[], int); //BFS Algorithm Prototype
void calculate_girth(int no_of_nodes, int adj[no_of_nodes][no_of_nodes], struct node [], struct edge [], int ); //calculate_girth function prototype

int main(void)
{
	
	int no_of_nodes, i, j, k, edge_count=0, flag = 1;
	printf("Enter no of nodes : ");
	scanf("%d",&no_of_nodes);
	int adjc[no_of_nodes][no_of_nodes];    //adjc[][] is an adjacency matrix to hold the initial graph information
	struct node n[no_of_nodes];
	struct edge e[no_of_nodes * (no_of_nodes-1) / 2];  //Maximum no of edges must be nC2 i.e. n * (n-1) / 2

	printf("Now enter the adjacency matrix :- \n\n");	
	/* Taking the input */
	for(i=0;i<no_of_nodes;i++)
	{
		for(j=0;j<no_of_nodes;j++)
		{
			scanf("%d",&adjc[i][j]);			
		}
		n[i].id_old = n[i].id_new = 100+i+1;
		printf("\n");
	}
    
    /*Initializing the edges*/
    for(i=0;i<no_of_nodes * (no_of_nodes - 1) / 2; i++)
    {
        e[i].id_of_node[0] = e[i].id_of_node[1] = -9999;
        e[i].weight = 0;
        e[i].assoc = -1;
    }
	/*Now finding the MST using Kruskal's Algorithm */
	
	/*Sorting the edges*/
	/*First populating the edge structure variable*/
    /*Counting no of unique edges*/
	for(i=0;i<no_of_nodes;i++)
	{
		for(j=0; j<no_of_nodes; j++)
		{
			if(adjc[i][j] !=0 ) //Checking in the adjacency matrix if the edge exists or not
			{
				for(k=0;k<edge_count;k++) //Checking for the unique edge
                {
                    if((e[k].id_of_node[0] == n[i].id_old || e[k].id_of_node[0] == n[j].id_old ) && (e[k].id_of_node[1] == n[i].id_old || e[k].id_of_node[1] == n[j].id_old))
                    {
                        flag = 0; //To determine the uniqueness of the current edge in the adjacency matrix
                        break;  //flag = 0 means the current edge is not unique. So, we can stop iterating the loop
                    }
                }
                if(flag)    //The edge from adjacency matrix has not been considered before
                {					
                    e[edge_count].id_of_node[0] = n[i].id_old;
                    e[edge_count].id_of_node[1] = n[j].id_old;
                    e[edge_count].weight = adjc[i][j];
                    edge_count++; //Increasing the no. of edges after every successful iteration
                }
			}
			flag = 1;
		}
	}
	/* Calling sort function to sort the edges by non decreasing order of weight */
    sort_edges(e, 0, edge_count - 1, edge_count);
    
    /*Testing sorted edges*/
    printf("\n\n-------Sorted Edges-------\n\n");
    for(i=0;i<edge_count;i++)
        printf("\n%d - %d : %d\n", e[i].id_of_node[0], e[i].id_of_node[1], e[i].weight);
    
    /*Calling the Kruskal's Algorithm to find out the MST for a given Graph*/
    Kruskals_Algorithm_to_find_MST(n, e, no_of_nodes, edge_count);
    
    /*Printing the MST*/
    printf("\n\n-------Edges present in the MST-------\n\n");
    for(i=0;i<edge_count;i++)
    {
        if(e[i].assoc == 1)
            printf("\n%d - %d : %d\n", e[i].id_of_node[0], e[i].id_of_node[1], e[i].weight);
    }
    
    printf("\n\n-------Edges not present in the MST-------\n\n");
    for(i=0;i<edge_count;i++)
    {
        if(e[i].assoc == -1)
            printf("\n%d - %d : %d\n", e[i].id_of_node[0], e[i].id_of_node[1], e[i].weight);
    }
    calculate_girth(no_of_nodes, adjc, n, e, edge_count);
    return 0;
}

/*Function to calculate girth of a graph*/
void calculate_girth(int no_of_nodes, int adj[no_of_nodes][no_of_nodes], struct node n[], struct edge e[], int no_of_edges) 
{
    int dist = 0, i, j, k;
    int Bool_MST[no_of_nodes][no_of_nodes];
    int girth = 0;
    struct cycles *root = NULL;
    int visited[no_of_nodes], dist_from_parent[no_of_nodes], q[no_of_nodes];
    
    /*Creating Boolean Adjacency matrix considering edge associativity in the MST*/
    for(i = 0; i < no_of_nodes; i++)
    {
        for(j = 0; j < no_of_nodes; j++)
        {
            Bool_MST[i][j] = 0;
        }
    }
    
    /*Making association in the Bool_MST*/
    for(i = 0; i < no_of_edges; i++)
    {
        if(e[i].assoc == 1) /*This means adding this particular edge will create a cycle*/
        {
            Bool_MST[e[i].id_of_node[0] - 100 - 1][e[i].id_of_node[1] - 100 - 1] = 1;
            Bool_MST[e[i].id_of_node[1] - 100 - 1][e[i].id_of_node[0] - 100 - 1] = 1;
        }
    }
    
    /*Printing Bool_MST*/
    printf("\n\nPrinting Bool_MST\n\n");
    for(i = 0; i < no_of_nodes; i++)
    {
        for(j = 0; j < no_of_nodes; j++)
        {
            printf("%d ",Bool_MST[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    /*Now calculating distance from root*/
    for(i = 0; i < no_of_edges; i++)
    {
        /*Initializing the passed parameters*/
        for(k = 0; k < no_of_nodes; k++)
        {
            visited[k] = 0;
            dist_from_parent[k] = 0;
            q[k] = 0;
        }
        if(e[i].assoc == -1) /*This means adding this particular edge will create a cycle*/
        {
            int reachable_node = e[i].id_of_node[1] - 100 - 1;
            bfs(e[i].id_of_node[0] - 100 - 1, e[i].id_of_node[1] - 100 - 1, no_of_nodes, adj, Bool_MST, q, visited, dist_from_parent, reachable_node);
            dist = dist_from_parent[reachable_node];
            struct cycles *temp = (struct cycles *)malloc(sizeof(struct cycles));
            printf("\ndist = %d, adj[][] = %d\n",dist, adj[e[i].id_of_node[0] - 100 - 1][e[i].id_of_node[1] - 100 - 1]);
            temp->length = dist + adj[e[i].id_of_node[0] - 100 - 1][e[i].id_of_node[1] - 100 - 1];
            temp->next = NULL;
            printf("\ntemp->length = %d\n", temp->length);
            if(root == NULL)
                root = temp;
            if(girth == 0 || girth > temp->length)
                girth = temp->length;
            else
            {
                struct cycles *p = root;
                while(p->next)
                    p = p->next;
                p->next = temp;
            }
        }
    }
    printf("\nGirth of the Graph is %d\n",girth);
}

/*BFS Algorithm*/
int bfs(int v, int d, int no_of_nodes, int adj[no_of_nodes][no_of_nodes], int Bool_MST[no_of_nodes][no_of_nodes], int q[no_of_nodes], int visited[no_of_nodes], int dist_from_parent[no_of_nodes], int reachable_node) {
    static int f = 0, r = -1;
    visited[v] = 1;
    /*if(v == reachable_node)
        return dist_from_parent[reachable_node];*/
	for (int i = 0; i < no_of_nodes; i++)
	{
	  if(Bool_MST[v][i] && !visited[i]) {
	   q[++r]=i;
       dist_from_parent[i] = dist_from_parent[v] + adj[v][i];
       printf("\nDistance from parent of node v = %d is %d",v,dist_from_parent[v]);
       printf("\nDistance from parent of node i = %d is %d",i,dist_from_parent[i]);
      }
	}
	if(f<=r) {
		visited[q[f]]=1;
		bfs(q[f++], d, no_of_nodes, adj, Bool_MST, q, visited, dist_from_parent, reachable_node);
	}
}

/*Merge Sort Algorithm for sorting the edges by weight starts here*/

void sort_edges(struct edge ed[], int low, int high, int no_of_edges)
{
    int mid;
    if(low < high)
    {
        mid = (low + high) / 2;
        
        // Divide and Conquer
        sort_edges(ed, low, mid, no_of_edges);
        sort_edges(ed, mid + 1, high, no_of_edges);
        
        // Combine
        merge(ed, low, mid, high, no_of_edges);
    }
}

void merge(struct edge ed1[], int l, int m, int h, int n)
{
    int n1, n2, i, j, k;
    n1 = m - l + 1;
    n2 = h - m;
    
    struct edge arr1[n1], arr2[n2];  // Two temporary arrays to hold the two arrays to be merged
    
    for(i = 0; i < n1; i++)
    {
        arr1[i].weight = ed1[l+i].weight;
        arr1[i].assoc = ed1[l+i].assoc;
        arr1[i].id_of_node[0] = ed1[l+i].id_of_node[0];
        arr1[i].id_of_node[1] = ed1[l+i].id_of_node[1];
    }
    for(j = 0; j < n2; j++)
    {
        arr2[j].weight = ed1[m+j+1].weight;
        arr2[j].assoc = ed1[m+j+1].assoc;
        arr2[j].id_of_node[0] = ed1[m+j+1].id_of_node[0];
        arr2[j].id_of_node[1] = ed1[m+j+1].id_of_node[1];
    }
    
    i=j=0;
    k = l;
    while(i < n1 && j < n2)  //process of combining two sorted arrays
    {
        if(arr1[i].weight <= arr2[j].weight)
        {
            ed1[k].weight = arr1[i].weight;
            ed1[k].assoc = arr1[i].assoc;
            ed1[k].id_of_node[0] = arr1[i].id_of_node[0];
            ed1[k].id_of_node[1] = arr1[i].id_of_node[1];
            i++;
        }
        else
        {
            ed1[k].weight = arr2[j].weight;
            ed1[k].assoc = arr2[j].assoc;
            ed1[k].id_of_node[0] = arr2[j].id_of_node[0];
            ed1[k].id_of_node[1] = arr2[j].id_of_node[1];
            j++;
        }
        k++;
    }
    
    /*Copy the remaining elements of arr1[] if there are any*/
    while(i < n1)
    {
        ed1[k].weight = arr1[i].weight;
        ed1[k].assoc = arr1[i].assoc;
        ed1[k].id_of_node[0] = arr1[i].id_of_node[0];
        ed1[k].id_of_node[1] = arr1[i].id_of_node[1];
        i++;
        k++;
    }
    
    /*Copy the remaining elements of arr2[] if there are any*/
    while(j < n2)
    {
        ed1[k].weight = arr2[j].weight;
        ed1[k].assoc = arr2[j].assoc;
        ed1[k].id_of_node[0] = arr2[j].id_of_node[0];
        ed1[k].id_of_node[1] = arr2[j].id_of_node[1];
        j++;
        k++;
    }
}

/*Merge Sort Algorithm Ends here*/

/*Kruskal's Algorithm to find out the minimum spanning tree from a given Graph starts here*/
void Kruskals_Algorithm_to_find_MST(struct node no[], struct edge eg[], int no_of_nodes, int no_of_edges)
{
    int i,j,k,l, vertex_id_modifier = 100 + no_of_nodes + 1; //vertex_id_modifier just modifies the vertex ids
    
    for(i = 0; i < no_of_edges; i++)
    {
        for(j = 0; j < no_of_nodes; j++)        //Identifying the vertex as same as the edges's one end vertex
        {
            if(no[j].id_old == eg[i].id_of_node[0])
                break;
        }
        for(k = 0; k < no_of_nodes; k++)        //Identifying the vertex as same as the edges's other end vertex
        {
            if(no[k].id_old == eg[i].id_of_node[1])
                break;
        }
        if(no[j].id_new != no[k].id_new)        //Finally modifying the id of the vertices
        {
//             /*To be deleted later*/
//             /*Testing if the id_new of the vertices are calculated properly or not*/
//             for(l=0;l<no_of_nodes;l++)
//                 printf("\n%d",no[l].id_new);
//             /*To be deleted later*/
            
            /*Equating the edge's left end vertex with all the other vertices having similar ids*/
            for(l = 0; l < no_of_nodes; l++)
            {
                if(l != j && l != k && no[l].id_new == no[j].id_new)
                    no[l].id_new = vertex_id_modifier;
            }
            
            /*Equating the edge's right end vertex with all the other end vertices having similar ids*/
            for(l = 0; l < no_of_nodes; l++)
            {
                if(l != j && l != k && no[l].id_new == no[k].id_new)
                    no[l].id_new = vertex_id_modifier;
            }
            
            no[j].id_new = no[k].id_new = vertex_id_modifier;   //Now modifying the edge's end vertices ids
            eg[i].assoc = 1;                    //Specifying the association of the edge in the MST
            vertex_id_modifier++;
        }
    }
}
