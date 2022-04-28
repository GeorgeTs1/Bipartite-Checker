
#include "LEDA/graph/graph.h"
#include "LEDA/core/random_source.h"
#include "LEDA/core/p_queue.h"
#include "LEDA/graph/node_partition.h"
#include "LEDA/graph/node_array.h"
#include "LEDA/core/queue.h"
#include "LEDA/system/timer.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <fstream>

using namespace leda;

 
list<node> my_BFS(const graph& G,node s, node_array<int>& dist,
		node_array<edge>& pred,node_array<int>& color)
{

  	queue<node> Q; // Queue for nodes that are needed to explore their adjacent vertices

	node tmp;  // temporary node for computations and comparisons

	node pred_node; //predecessor of node w 

	list<node> visited; // List of all visited nodes

	visited.append(s); // Append the random node s

	dist[s] = 0; // S is in zero level
	
	Q.push(s); // push s into the queue
	
        pred[s] = NULL; // Initialization of predecessor of starting node = NULL
	
	color[s] = 1; // 1 for green 0 for blue

	while(!Q.empty())
	{
		tmp = Q.pop(); 
 
		edge e;

		forall_adj_edges(e,tmp)
		{
			node w = G.opposite(tmp,e);
				
                        if(color[w] == -1) // means not coloured no visited
			{
				visited.push_back(w); // if not visited store it in list
				
				Q.append(w); // Append it to Q

				dist[w] = dist[tmp] + 1; // Set the level of W equal to its predecessor plus 1

				color[w] = dist[w]%2==0 ? 1 : 0; // If W is in even level mark it  green(1),if odd mark it blue 
		 		
				pred[w] = e; // Add the predecessor edge e of W in node_array 
			}
						
		}

			

	}


       

	
  return visited; // Return the list of Visited Nodes from BFS
		

}




/* Recursive Function for Circle Detection in BFS Tree of an undirected Graph 
   It cannot be used in Graphs with many Vertices and Edges due to memory limitations

void  bottom_up_traversal(const graph& G,node_array<edge> pred,node& left,node& right,node& root,list<node>& circle)
{
	
    node left_father,right_father;

    if(left == root || right == root)
    {
		circle.push(root);
		return ;
    }


    circle.push(left);

    circle.push(right);

    left_father = G.source(pred[left]);
  
    right_father = G.source(pred[right]);

    std::cout << "Left: " << left->id() << " Right: " << right->id() << std::endl; 

    bottom_up_traversal(G,pred,left_father,right_father,root,circle);

}

*/

/* Non-Recursive Function for Circle Detection in BFS Tree of an undirected Graph
   applied even in very big scaled Graphs */
void bottom_up_traversal_v2(const graph& G,node_array<edge> pred,node& left,node& right,node& root,list<node>& circle,node_array<int>levels)
{


      int start = levels[left]; // Starting level for bottom up traversal of BFS Tree

      circle.push_back(left); // Add the left Vertice of BFS Tree in circle list

      circle.push_back(right); // Add the right Vertice of BFS Tree in circle list

     for(; start>0; start--) // Bottom Up Loop
     {
	
	if(start == 1) // If it reaches at level 1 just add root (used because root should added only once)
	{
		circle.push_back(root);
		break;
	}	
		
        circle.push_back(G.source(pred[left])); // Add the predecessor vertice of left node at level i
	
	circle.push_back(G.source(pred[right])); // Add the predecessor vertice of right node at level i

	left = G.source(pred[left]); // Update the left vertice as the predecessor of it

	right = G.source(pred[right]); // Update the right vertice as the predecessor of it
     
     }


}	

//Bipartite Checker for Undirected Graphs complexity O(n)
bool my_bipar_checker(const graph& G, list<node>& V1, list<node>& V2)
   {

        edge e;

        node v,v1,v2; 

       
        int num_nodes = G.number_of_nodes(); // Number of nodes of Graph

        int num_edges = G.number_of_edges(); // Number of edges of Graph

        std::ofstream myfile; // For storing the odd circle of Graphs

        myfile.open("circle.txt",std::ios::app | std::ios::out);

	node_array<int> color(G,-1);  // For marking purposes
       
        node_array<int> levels(G,-1); // For matching nodes with their correlated levels

        node_array<edge> pred(G,e); // For storing the predecessor edges of nodes

        node s = G.choose_node(); // Choose a random vertice of Graph G

        my_BFS(G,s,levels,pred,color); // BFS for Graph G

       //std::cout << "I started from node " << s->id() << std::endl; // For Visualization Purposes

        list<node> odd_circle; // An odd length circle list

   
        bool bipart = true; // Variable for Bipartite checking


      
	/* For display Purposes 
	std::cout << " Levels " << std::endl; // Display of Nodes in each Level

	forall_nodes(v,G)
 	{

             std::cout << v->id() << " - " << levels[v] << std::endl;

  	}   
  
        std::cout << "Colors" << std::endl; // Display of Colors of each Vertice
         
       forall_nodes(v,G)
        {

             std::cout << v->id() << " - " << color[v] << std::endl;

        }


        std::cout << " Predecessor edges " << std::endl; //Display of Predecessor Vertice of Each Vertice


         forall_nodes(v,G)
         {
   		 if(v!=s)
    		  std::cout<<v->id()<<" -> "<< (G.source(pred[v]))->id() << std::endl;
    		else
    		  std::cout << s->id() << " -> " << s->id() << std::endl;
  	 }*/	

    


	   forall_edges(e,G) // Loop for checking Edges that has same coloured vertices 
               {
			v1 = G.source(e); // The source of edge e
 
                        v2 = G.target(e); // The target of edge e

			if( color[v1] == color[v2]   ) // Edge with the same colored vertices
                         {
				bipart= false;
			       
				break;
                               
                         }

               }
	

           

    

     if(bipart==true) // If Graph G is Bipartite just create two sets of Vertices with  union(V1,V2) == V && intersection(V1,V2) == empty set
     {
		forall_nodes(v,G)
                {
			if(color[v] == 1)
                        {
				V1.append(v);
                        }
			else
                       {
				V2.append(v);
                       }

                }

     }
    else
    {

	bottom_up_traversal_v2(G,pred,v1,v2,s,odd_circle,levels); // Traverse BFS Tree from edge that has same coloured extremes upwards to the root

        myfile << "Graph with vertices: "<< num_nodes << " and edges: " << num_edges << " has an odd circle of length: " << odd_circle.size() << " and consists of vertices: " << std::endl; //Write to file circle.txt the odd length circle for each non bipartite graph 

	
	forall(v,odd_circle) // Store Vertices in circle.txt  that form an Odd-Length Circle
        {
		
 
           myfile << v->id() << ",";	         

	}

         myfile << std::endl;

         myfile.close();
	
    }


	
    return bipart;
        
       
        


   }




void Graph1(graph& G,int n,int m,int cnt) // Nested-Squares Undirected Graph Maker in Recursive Way
{

	node v[4],t;


	if(n==0) // If we form the last square we terminate the recursion
	{
		return;	
	}

	for(int i=0; i<4; i++) // Create the 4 nodes
        {
		v[i] = G.new_node();		
	}


	// We create the square from Vertices Undirected
	
	G.new_edge(v[0],v[1]); /*Connect the nodes to form a square*/
        G.new_edge(v[0],v[2]);
	G.new_edge(v[1],v[3]);
	G.new_edge(v[2],v[3]);


	G.new_edge(v[1],v[0]); /*Connect the nodes to form a square*/
        G.new_edge(v[2],v[0]);
        G.new_edge(v[3],v[1]);
        G.new_edge(v[3],v[2]);
	
		
	if(cnt!=0) // If we've formed the outer square connect it with the inner one
	{	
	
		
	     int l=0;
		
		forall_nodes(t,G)
		{

			if(l==4)break;			
			
			G.new_edge(t,v[l]);
			G.new_edge(v[l],t);
			l++;
		}
		
		
	}

  cnt +=1;

  Graph1(G,n-4,m-8,cnt); // Recursive Call of Function for the outer Squares

}



void Graph2(graph&G,int n,int m) // Ring Graph Maker
{
	
	node v[n]; 

	for(int i=0; i<n; i++) // First the creation of nodes and the addition to Graph
	{
		v[i] = G.new_node();
							
	}


	for(int j=0; j<m; j++) // Connect the vertices in order to form a circle
	{

		if(j==m-1) // If we reach the last Vertice connect it to the starting one
		{ 
			G.new_edge(v[m-1],v[0]);
			G.new_edge(v[0],v[m-1]);
			break;
		}
		
		G.new_edge(v[j],v[j+1]);
		G.new_edge(v[j+1],v[j]);
	
		
	}

}

	
void Graph3(graph& G,int k,int l) // 4-Level Synthetic Graph of K vertices and 2*K Edges
{
 
	graph G_levels[4]; // Array of Graphs for 4 Level Represantation

	node v[4][k]; // 2D Array of Nodes for storing the nodes of each level

	node random_v;

	node random_v1,random_v2,random_v3,random_v4;
	
  	
        for(int i=0; i<4; i++) // Creating the nodes of each level and store them 
	  {
	
		for(int j=0; j<k; j++)
		{
			
			v[i][j]=G_levels[i].new_node();

		}
		

	  }
	 
		
	 for(int i=0; i<4; i++)
	 {
		
		// Chart Union of 4-Levels into G 
		G.join(G_levels[i]);
		
	 }
		   	 

	   for(int i=0; i<3; i++) // Create the first k edges from vertices in succesive levels
	   {

		for(int j=0; j<k; j++)
		{
			
			G.new_edge(v[i][j],v[i+1][j]);
			G.new_edge(v[i+1][j],v[i][j]);			

		}

	   }


	std::srand(time(0)); // Seed Initialization
	
	for(int i=0; i<3; i++) // Connect the rest of k edges by picking a random vertice u from level i and connect it to all the vertices of level i+1
        {
		
		random_v = v[i][rand()%k]; // Random Vertice from each level 

		for(int j=0; j<k; j++)
		{
			G.new_edge(random_v,v[i+1][j]);
			G.new_edge(v[i+1][j],random_v);

		}

	}


	// Random Edges from Level 1 to Level 3 and from Level 2 to Level 4
	
	random_v1 = v[0][rand()%k];
	
	random_v3 = v[2][rand()%k];

	G.new_edge(random_v1,random_v3);
	
	G.new_edge(random_v3,random_v1);

	random_v2 = v[1][rand()%k];

        random_v4 = v[3][rand()%k];
		   
        G.new_edge(random_v2,random_v4);

	G.new_edge(random_v4,random_v2);


   
}	


int main()
{
    graph G1,G2,G3; // G1 -> nested-squares, G2 - > ring, G3->4-levels 

 
    edge e;  // For iteration purposes

   
    std::ofstream myfile,myfile2; //For writing execution times to a file 
    
    myfile.open("times.txt",std::ios::app | std::ios::out); // File for storing the time measurements for each graph family

    myfile2.open("V1_V2.txt",std::ios::app | std::ios::out); //File for storing the sets V1 and V2 of the bipartite graphs

    clock_t start; // For starting measuring the CPU Time of functions 
  
    clock_t end; // For terminating measurement of CPU Time of functions


    timer a;  // LEDA timer
	
    double elapsed; // Elapsed time of each Function


    list<node> V1; // List V1 of nodes for G1
   
    list<node> V2; // List V2 of nodes for G1

    list<node> V3; // List V3 of nodes for G2

    list<node> V4; // List V4 of nodes for G2

    list<node> V5; // List V5 of nodes for G3

    list<node> V6; // List V6 of nodes for G3
        

    node_array<int> levels1(G1,-1); // Initialization of all levels for G1

    node_array<int> color1(G1,-1); // Color Array for G1

    node_array<edge> pred1(G1,e);  // Predecessor array for G1


    node_array<int> levels2(G2,-1); // Initialization of all levels for G2

    node_array<int> color2(G2,-1); // Color Array for G2

    node_array<edge> pred2(G2,e);  // Predecessor array for G2


    
    node_array<int> levels3(G3,-1); // Initialization of all levels for G3

    node_array<int> color3(G3,-1); // Color Array for G3

    node_array<edge> pred3(G3,e);  // Predecessor array for G3



    node v; // For iteration purposes

    list<node> visited_nodes1; // Visited nodes for BFS in G1

    list<node> visited_nodes2; // Visited nodes for BFS in G2

    list<node> visited_nodes3; // Visited nodes for BFS in G3

   

    Graph1(G1,10000,199996,0); // make the Nested-Squares Graph
     

    Graph2(G2,100001,100001);  // make the Ring Graph


    Graph3(G3,1800,3600); // make the 4-levels Graph

    
   

  bool isBipart1 = false; // variable for the bipar_checker of G1
 
  bool isBipart2 = false; // variable for the bipar_checker of G2
 
  bool isBipart3 = false; // variable for the bipar_checker of G3




  a.start();  

  isBipart1 = my_bipar_checker(G1,V1,V2); // check if G1 is bipartite

  a.stop();
 

  myfile << "G1(100000,199996): "  << a.elapsed_time() << std::endl;
  
  
  a.restart();

  isBipart2 = my_bipar_checker(G2,V3,V4); // check if G2 is bipartite
 
  a.stop();
 

  myfile << "G2(100001,100001): "  << a.elapsed_time() << std::endl;
  
  a.restart();

  isBipart3 = my_bipar_checker(G3,V5,V6); // check if G3 is bipartite

  a.stop();
	
  myfile << "G3(1800,3600): "  <<  a.elapsed_time() << std::endl;

 if(isBipart1 == true)
 {
	myfile2 << "Graph G1 is bipartite" << std::endl;
        myfile2 << "V1: " << std::endl; 
        forall(v,V1)
        {
            myfile2 << v->id() << ",";
        }

        myfile2 << std::endl;

	myfile2 << "V2: " << std::endl;

	forall(v,V2)
	{
	    myfile2 << v->id() << ",";
	}

        myfile2 << std::endl;  

 }

 
 if(isBipart2 == true)
 {
        myfile2 << "Graph G2 is bipartite" << std::endl;
        myfile2 << "V1: " << std::endl;
        forall(v,V3)
        {
            std::cout << v->id() << std::endl;
        }
	
	myfile << std::endl;
	
	myfile2 << "V2: " << std::endl;
	
	forall(v,V4)
        {
	
	   std::cout << v->id() << std::endl;

	}
 }



 if(isBipart3 == true)
 {
        myfile2 << "Graph G3 is bipartite" << std::endl;
        myfile2 << "V1: " << std::endl;
        forall(v,V5)
        {
            myfile2 << v->id() << ",";
        }

        myfile2 << std::endl;

        myfile2 << "V2: " << std::endl;

        forall(v,V6)
        {

           myfile2 << v->id() << ",";

        }
 }



 myfile2.close();
	

 bool LedaisBipart1 = false; // variable for LEDA Bipartite Checker of G1

 bool LedaisBipart2 = false; // variable for LEDA Bipartite Checker of G2

 bool LedaisBipart3 = false; // variable for LEDA Bipartite Checker of G3



 a.restart();

 LedaisBipart1 = Is_Bipartite(G1,V1,V2); // LEDA function for Bipartite Graphs tested in G1

 a.stop();

 myfile << "G1(100000,199996) LEDA: " << a.elapsed_time() << std::endl;
 
 a.restart();

 LedaisBipart2 = Is_Bipartite(G2,V3,V4); // LEDA function for Bipartite Graphs tested in G2
 
 a.stop(); 
 
 myfile << "G2(100001,100001) LEDA: " << a.elapsed_time() <<std::endl;

 a.restart();

 LedaisBipart3 = Is_Bipartite(G3,V5,V6); // LEDA function for Bipartite Graphs tested in G3

 a.stop();

 myfile << "G3(1800,3600) LEDA: " << a.elapsed_time() <<std::endl;

 myfile.close();

// Check if LEDA and my_bipart_checker have the same results

 if(LedaisBipart1 == isBipart1)
 {
	std::cout << "LEDA confirms my function for graph Nested Squares graph" << std::endl;
 }


 if(LedaisBipart2 == isBipart2)
 {
        std::cout << "LEDA confirms my function for graph Ring graph" << std::endl;
 }

 if(LedaisBipart3 == isBipart3)
 {
        std::cout << "LEDA confirms my function for graph 4-Level graph" << std::endl;
 }



return 0;


}
