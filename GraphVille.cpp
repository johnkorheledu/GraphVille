/*
  John Korhel
  CSCI-2275 Programming and Data Structures Assignment 7
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

struct vertex {
  string name;
  bool visited;
  int distance;
  string path;
  vector<vertex *> adjacency; //Changed this to pointer, couldn't get it to work otherwise.
  explicit vertex(string n) {
    name = std::move(n);
    visited = false;
    distance = 0;
  }
};

class GraphVille {
  vector<vertex> vertices;

public:
  void addVertex(const string& n);
  void addEdge(const string& n1, const string& n2);
  static void addEdgeUtil(vertex &v1, vertex *v2);
  bool areAdjacent(const string& n1, const string& n2);
  static bool areAdjacentUtil(vertex &vert1, vertex *vert2);
  void printList();
  void knowEach(const string& n1, const string& n2);
  vertex *findUnvisited();
  void printGroup();
  void introduction(const string& n1, const string& n2);
  void introductionUtil();
};

// Function that adds a vertex to the GraphVille graph.
void GraphVille::addVertex(const string& n) { vertices.emplace_back(n); }

// Function that adds an edge to a vertex on the GraphVille graph.
void GraphVille::addEdge(const string& n1, const string& n2) {
  for (vertex &v1 : vertices) {
    if (v1.name == n1) {
      for (vertex &v2 : vertices) {
        if (v2.name == n2) {
          addEdgeUtil(v1, &v2);
          addEdgeUtil(v2, &v1);
          return;
        }
      }
    }
  }
}

// Helper function for adding edges to the GraphVille graph. Checks if the
// vertex structs are not adjacent. If this is the case, add the v2 vertex to
// the v1 adjacency vector.
void GraphVille::addEdgeUtil(vertex &v1, vertex *v2) {
  if (!areAdjacentUtil(v1, v2))
    v1.adjacency.emplace_back(v2);
}
// Function that determines if two strings in the GraphVille graph are adjacent.
// Used later in the knowEach function.
bool GraphVille::areAdjacent(const string& n1, const string& n2) {
  for (vertex &vert1 : vertices) {
    if (vert1.name == n1) {
      for (vertex &vert2 : vertices) {
        if (vert2.name == n2) {
          if (areAdjacentUtil(vert1, &vert2) || areAdjacentUtil(vert2, &vert1))
            return true;
        }
      }
    }
  }
}

// Helper function for the adjacency function.
bool GraphVille::areAdjacentUtil(vertex &vert1, vertex *vert2) {
  for (vertex *v : vert1.adjacency)
    if (v == vert2)
      return true;
  return false;
}

// Prints the list of acquaintances in the GraphVille graph
void GraphVille::printList() {
  for (vertex &v : vertices) {
    cout << v.name << " : ";
    for (vertex *vSub : v.adjacency) {
      cout << vSub->name << " ";
    }
    cout << endl;
  }
}

// Function to determine whether or not two people know eachother in the
// GraphVille graph
void GraphVille::knowEach(const string& n1, const string& n2) {
  if (areAdjacent(n1, n2)) {
    cout << "True" << endl;
    return;
  } else {
    cout << "False" << endl;
    return;
  }
}

// Function that finds and outputs the 'groups' within the graph. Utilizes stack
// data structure.
void GraphVille::printGroup() {
  stack<vertex *> gStack;
  vertex *start;
  int gIt = 1;
  start = findUnvisited();
  while (start) {
    cout << "Group ID: " << gIt << endl;
    start->visited = true;
    gStack.push(start);
    while (!gStack.empty()) {
      vertex *v = gStack.top();
      gStack.pop();
      cout << "- " << v->name << " ";

      for (vertex *vSub : v->adjacency) {
        if (!vSub->visited) {
          vSub->visited = true;
          gStack.push(vSub);
        }
      }

      cout << "\n";
      start = findUnvisited();
    }

    gIt++;
  }
  for(vertex &v : vertices){ //Reset the vertice 'visited' property to false.
    v.visited = false;
  }
}

// Helper function to assist with the printGroup function. Returns the next
// unvisited node in the graph.
vertex *GraphVille::findUnvisited() {
  for (vertex &v : vertices)
    if (!v.visited)
      return &v;
  return nullptr;
}

// Function that determines the shortest path for two people to meet one
// another. Utilizes breadth first traversal with the queue data structure.
void GraphVille::introduction(const string& n1, const string& n2) {

  queue<vertex *> introQ; // Initialize queue
  vertex *start = nullptr; // Initialize starting vertex

  for (vertex &v : vertices) { // Check all vertices to see which matches with the first name passed in
    if (v.name == n1) {
      start = &v; // Set this as the new start
    }
  }
  start->visited = true; //This node is now visited, mark it as so
  start->path = start->name; //Path is equal to the name
  introQ.push(start); //Push this to the queue

  while (!introQ.empty()) { //While the queue is not empty, continue
    vertex *v = introQ.front();
    introQ.pop();
    for (vertex *vSub : v->adjacency) { //Check the adjacency list
      if (!vSub->visited) {
        vSub->distance = v->distance + 1;
        vSub->path = v->path + ", " + vSub->name;
        if (vSub->name == n2) { // If we reach the second name
          cout << "Distance: " << vSub->distance << endl;
          cout << "Path: " << vSub->path << endl;
          introductionUtil(); //Clear
          return;
        }
        vSub->visited = true;
        introQ.push(vSub);
      }
    }
  }
  cout << "No way to introduce them." << endl;
  introductionUtil(); //Clear
}

//Utility function used to reset the vertices to default values.
void GraphVille::introductionUtil() {
  for (vertex &v : vertices) {
    v.visited = false;
    v.distance = 0;
    v.path.clear();
  }
}

int main() {

  GraphVille g;
  ifstream f("people.txt");
  vector<string> n, adjN;
  if (!f) {
    cout << "Cannot open file. " << endl;
    exit(1);
  }
  string w;
  while (getline(f, w)) {
    stringstream ss(w);
    string vert, adj;
    getline(ss, vert, '-');
    getline(ss, adj, '-');
    g.addVertex(vert); //First name is the vertex
    stringstream ss2(adj);

    n.emplace_back(vert);
    adjN.emplace_back(adj);
  }

  for (int i = 0; i < adjN.size(); i++) {
    stringstream ss(adjN[i]);

    while (ss.good()) {
      string s;
      getline(ss, s, ',');

      if (isspace(s[s.size() - 1]))
        s = s.substr(0, s.size() - 1); //Disgusting one liner for handling space cases

      g.addEdge(n[i], s); //Add edge to graph
    }
  }

  while (true) {
    cout << "1. Print list of people and their acquaintances" << endl;
    cout << "2. Print if people know each other" << endl;
    cout << "3. Print groups" << endl;
    cout << "4. Find the least number of introductions required" << endl;
    cout << "5. Quit" << endl;

    int x;
    cin >> x;
    if (x == 1) {
      g.printList();
    }
    if (x == 2) {
      string n1, n2;
      cout << "Enter first name: " << endl;
      cin >> n1;
      cout << "Enter second name: " << endl;
      cin >> n2;
      g.knowEach(n1, n2);
    }
    if (x == 3) {
      g.printGroup();
    }
    if (x == 4) {
      string n1, n2;
      cout << "Enter first name: " << endl;
      cin >> n1;
      cout << "Enter second name: " << endl;
      cin >> n2;
      g.introduction(n1, n2);
    }
    if (x == 5) {
      cout << "Goodbye!" << endl;
      exit(0);
    }
  }
}