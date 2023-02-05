/*
 * DSA assignment 3 BS20-02 Klyushev Vsevolod
 * Links on CodeForces:
 * 1st task: https://codeforces.com/group/C71Rz4W66e/contest/325457/submission/114259298
 * 2nd task: https://codeforces.com/group/C71Rz4W66e/contest/325457/submission/114259234
 * 3rd task: https://codeforces.com/group/C71Rz4W66e/contest/325457/submission/114259191
 * (1st year, spring semester)
 * Year - 2021
 *
 * This code contains solutions for all 3 tasks. In program there are 2 different input functions and 2 different
 * initializations of matrix, more information would be provided later.
 */
#include <iostream>
#include <algorithm>
#include "vector"
#include "map"
#include "unordered_map"
using namespace std;

//class of vertex objects;
template <typename V>
class Vertex {
public:
    V name;//value of vertex object.
    int color;//color of vertex, which is used to find cycles in task 2.2.
    Vertex<V>* prev;//previous vertex, which is used to find cycles in task 2.2.
    int val;//variable in order to check the length of path in Dijkstra algorithm in task 2.3.
    int band;//variable in order to store the bandwidth of a path in task 2.3.
    bool used;//variable in order to check visited vertices in Dijkstra algorithm in task 2.3.
    Vertex(V n) {//constructor for vertex class.
        name=n;
        color=0;
        prev= nullptr;
        val=INT32_MAX;
        band=INT32_MAX;
        used= false;
    }
};

//class of edge objects;
template <typename V, typename E>
class Edge {
public:
    Vertex<V>* from;//pointer to vertex from which edge is outgoing.
    Vertex<V>* to;//pointer to vertex to which edge is ingoing.
    E weight;//weight of edge or another value that edge may store.
    E bandwidth;//bandwidth of edge for task 2.3.
    Edge(Vertex<V>& f,Vertex<V>& t, E w ) {//constructor for edge class for tasks 2.1, 2.2.
        from=&f;
        to=&t;
        weight=w;
        bandwidth=0;
    }
    Edge(Vertex<V>& f,Vertex<V>& t, E w, E b ) {//constructor for edge class for task 2.3.
        from=&f;
        to=&t;
        weight=w;
        bandwidth=b;
    }
    bool operator > (Edge& e) {//redefinition of operator '>' in order to use it in compare structure.
        return (this->weight > e.weight);
    }
    bool operator < (Edge& e) {//redefinition of operator '<' in order to use it in compare structure.
        return (this->weight < e.weight);
    }
};

struct compare {//compare structure
    bool operator()(Edge<int, int>* a, Edge<int, int>* b) {
        return *a > *b;
    }
};

//-----description of Graph ADT-----------------------------------------------------------------------------------------
/*
 * This is an interface of Sorted List. Since there is no such words as abstract & interface I used virtual function
 * an overloaded them later.
 */
template <typename V, typename E>
class GraphADT {
public:
    virtual Vertex<V>& addVertex(V value)=0;// — add a vertex with value value to the graph
                                            //and return reference to the created vertex object;
    virtual void removeVertex(Vertex<V>& v)=0;// — remove a vertex, provided a reference to a
                                              //vertex object;
    virtual Edge<V,E>& addEdge(V from, V to, E weight)=0;// - add a (directed) edge between
                                                         //from and to vertices with weight weight,
                                                         //return reference to the created edge object;
    virtual Edge<V,E>& addEdge(V from, V to, E weight, E bound)=0;//another add edge function in order
                                                                  //to use it in solution of task 2.3;
    virtual void removeEdge(Edge<V,E>& e)=0;// — remove an edge, given a reference to an edge object;
    virtual vector<Edge<V,E>*> edgesFrom(Vertex<V>& v)=0;// — return a collection or edge objects that are going
                                                         //from vertex v;
    virtual vector<Edge<V,E>*> edgesTo(Vertex<V>& v)=0;// — return a collection or edge objects that are going
                                                       //into vertex v;
    virtual Vertex<V>& findVertex(V value)=0;// — find any vertex with the specified value;
    virtual Edge<V,E>& findEdge(V from_value, V to_value)=0;// — find any edge with specified values in the source and target vertices.
    virtual bool hasEdge(Vertex<V>& v, Vertex<V>& u)=0;// — determine whether there exists a directed edge
                                                       //from v to u;
};

//-----implementation of Graph ADT as Adjacency Matrix Graph------------------------------------------------------------
/*
 * This is the implementation of Graph ADT based on adjacency matrix, but with the following idea:
 *  map1<vertex, map2<vertex, edge>>
 *  map1: key - vertex, value - map2  \\first part of matrix
 *  map2: key - vertex, value - edge  \\second part of matrix
 *  A bit more information:
 *  1)In order to not creating copies of our objects we would work with pointers1)in order to not creating copies of our objects we would work with pointers.
 *  2)access to the special edge with information of starting and ending vertex would take O(1) if there won't be any object,
 *  we'll receive nullptr.
 *  I decided to use such a structure in order to save some time and memory.
 *  However, maps in c++ are based on arrays, so, we may consider such construction also as simplification of
 *  2-dimensional array which store edges with storing an association of every vertex with special index and control of it.
 *  All methods have better or same time complexity as usual Adjacency matrix implementation based on arrays.
 *  However findVertex work with O(n), because of finding an association of value with the object. It happens because of c++
 *  hash-map implementation. (it hashes objects, so 2 vertices with the same name would be considered as different).
 *  Even though, we may improve such structure by using one more map, to store value as a key and pointers on an object (Vertex)
 *  as value. But its improvement may be sound in case that we will need to have better time complexity.
 *  Generally I wanted to use Vertex and Edge classes in my implementation, since they are given, and not base matrix
 *  as just a 2d array of pointers on edges.
 *  However, there may be inconsistencies in my reasoning, but I still think that such implementation may have sense.
 *  For simplicity I'll call such structure a matrix.
 *
 *  This implementation has the following functions:
 *  -addVertex(V value) - using creation of new object in map store value in wrapper class Vertex. Time complexity = O(1);
 *  -removeVertex(Vertex<V> &v) - firstly, we remove from matrix all edges that starts from that vertex by special map
 *  function, then using array iterators we check all the possible positions of pointer to a vertex in our matrix
 *  and assign them with nullptr. Time complexity = O(n^2);
 *  -addEdge(V from, V to, E weight) || addEdge(V from, V to, E weight, E bound) - by using findVertex function with
 *  time complexity O(n) and access to cell of matricx with O(1), has total time complexity O(n);
 *  -removeEdge(Edge<V,E>& e) - by using pointer on starting and ending vertices in edge, we access to matrix cell with
 *  edge and assign its value with nullptr, so time complexity is O(1);
 *  -edgesFrom(Vertex<V>& v) - by using iterator on map, that lies in value part of first part of matrix, we store all
 *  pointer in vector and return it. Time complexity - O(n);
 *  -edgesTo(Vertex<V>& v) - same idea and time complexity as in previous function;
 *  -findVertex(V value) - by using iterator check all values of vertices that are keys in first part of matrix. So, time
 *  complexity is O(n);
 *  -findEdge(V from_value, V to_value) - by using findVertex function O(n) and call to particular cell of matrix O(1),
 *  we receive time complexity O(n);
 *  -hasEdge(Vertex<V>& v, Vertex<V>& u) - by checking if a cell in matrix is nullptr, we can say whether an edge exists
 *  or not. Time complexity is O(1);
 *  =====For task 2.2===================================================================================================
 *  -dfs(Vertex<V> &vertex) - as its name, it is dfs function that helps in determining whether there is cycles, that
 *  starts from particular vertex. So, if it finds any of them, it stores trac, by using pointer "prev" in Vertex class
 *  implementation and global pointers cycle_st and cycle_end.
 *  -isAcyclic() - this function call dfs for vertices until all vertices were considered or cycle was found. After that,
 *  depending on  whether cycle_st pointer is null or not it print the cycle or print that graph is acyclic;
 *  -transpose() - this function collect all edges in one vector and then remove all edges from graph and
 *  add opposite edges in graph by using from and to pointers to vertices from Edge class implementation;
 *  =====For task 2.3===================================================================================================
 *  -dijkstra() - this function use dijkstra algorithm determine the shortest path with information of its bandwidth. As
 *  priority queue we use sorted array.
 */
template <typename V, typename E>
class AdjacencyMatrixGraph : public GraphADT<V,E> {

private://Our matrix based on hash maps. In c++ hash maps = unordered_maps.
    unordered_map< Vertex<V>*, unordered_map< Vertex<V>* , Edge<V,E>* > > matrix;

public:

    Vertex<V>& addVertex(V value) override {//O(1)
        Vertex<V>* vertex = new Vertex<V>(value);
        matrix[vertex]=unordered_map< Vertex<V>* , Edge<V,E>* >();
        return *vertex;
    }

    void removeVertex(Vertex<V> &v) override {//O(n)
        matrix.erase(&v);
        for (auto itRow = matrix.begin(); itRow != matrix.end(); itRow++) {
            for (auto it = itRow->second.begin(); it != itRow->second.end(); ) {
                if ((it->first) == &v) {
                    it = itRow->second.erase(it);
                } else {
                    it++;
                }
            }
        }
    }

    Edge<V, E> &addEdge(V from, V to, E weight) override {//O(n)
        Edge<V,E>* edge = new Edge<V,E>(findVertex(from),findVertex(to),weight);
        matrix[&findVertex(from)][&findVertex(to)]=edge;
        return *edge;
    }

    Edge<V, E> &addEdge(V from, V to, E weight, E bound) override {//addEdge function for task 2.3.
        Edge<V,E>* edge = new Edge<V,E>(findVertex(from),findVertex(to),weight,bound);
        matrix[&findVertex(from)][&findVertex(to)]=edge;
        return *edge;
    }

    void removeEdge(Edge<V, E> &e) override {//O(1)
        matrix[e.from][e.to]=nullptr;
    }

    vector<Edge<V, E> *> edgesFrom(Vertex<V> &v) override {//O(n)
        vector<Edge<V, E> *> vec;
        for (auto it = matrix[&v].begin(); it != matrix[&v].end(); it++) {
            if (it->second != nullptr) {
                vec.push_back(it->second);
            }
        }
        return vec;
    }

    vector<Edge<V, E> *> edgesTo(Vertex<V> &v) override {//O(n)
        vector<Edge<V, E> *> vec;
        for (auto itRow = matrix.begin(); itRow != matrix.end(); itRow++) {
            if ((itRow->second)[&v] != nullptr) {
                vec.push_back((itRow->second)[&v]);
            }
        }
        return vec;
    }

    Vertex<V> &findVertex(V value) override {//O(n)
        for (auto itRow = matrix.begin(); itRow != matrix.end(); itRow++) {
            if (itRow->first->name==value) {
                return *(itRow->first);
            }
        }
        throw -1;
    }

    Edge<V, E> &findEdge(V from_value, V to_value) override {//O(n)
        Vertex<V>* v=&findVertex(from_value);
        Vertex<V>* u=&findVertex(to_value);
        return *matrix[v][u];
    }

    bool hasEdge(Vertex<V> &v, Vertex<V> &u) override {//O(1)
        if (matrix[&v][&u] != nullptr) {
            return true;
        } else {
            return false;
        }
    }

//-----functions for task 2.2.------------------------------------------------------------------------------------------
    Vertex<V>* cycle_end;
    Vertex<V>* cycle_st;
    bool dfs(Vertex<V> &vertex) {
        vertex.color=1;
        vector<Edge<V, E> *> edges = edgesFrom(vertex);
        for (int i=0;i<edges.size();i++) {
            Vertex<V>* to = edges[i]->to;
            if (to->color==0) {
                to->prev=&vertex;
                if (dfs (*to)) {
                    return true;
                }
            } else if (to->color == 1) {
                cycle_end = &vertex;
                cycle_st = to;
                return true;
            }
        }
        vertex.color = 2;
        return false;
    }

    void isAcyclic() {
        cycle_st = nullptr;

        for (auto itRow = matrix.begin(); itRow != matrix.end(); itRow++) {
            if ( dfs(*(itRow->first) ) )
                break;
        }

        if (cycle_st == nullptr)
            cout<<"ACYCLIC"<<endl;
        else {
            int w=0;
            vector<Vertex<V>*> cycle;
            w+=findEdge(cycle_end->name, cycle_st->name).weight;
            cycle.push_back (cycle_st);
            while (cycle_end!=cycle_st) {
                cycle.push_back(cycle_end);
                w+=findEdge(cycle_end->prev->name, cycle_end->name).weight;
                cycle_end=cycle_end->prev;
            }
            reverse (cycle.begin(), cycle.end());
            cout<<w<<' ';
            for (int i=0; i<cycle.size(); i++)
                cout<< cycle[i]->name<<' ';
            cout<<endl;
        }
        for (auto itRow = matrix.begin(); itRow != matrix.end(); itRow++) {
            itRow->first->color=0;
        }
    }

    void transpose() {
        vector<Edge<V, E> *> edges;
        for (auto itRow = matrix.begin(); itRow != matrix.end(); itRow++) {
            vector<Edge<V, E> *> e=edgesFrom(*(itRow->first) ) ;
            for (int i=0; i<e.size();i++) {
                edges.push_back(e[i]);
            }
        }
        for (int i=0; i<edges.size();i++) {
            removeEdge(findEdge(edges[i]->from->name,edges[i]->to->name));
        }
        for (int i=0; i<edges.size();i++) {
            addEdge(edges[i]->to->name,edges[i]->from->name,edges[i]->weight);
        }
    }

//-----function for task 2.3--------------------------------------------------------------------------------------------
/*
 * In this function we would visit all vertices starting from initial, in every iteration of first loop we'll find unvisited
 * vertex with smallest value, mark it as visited and do relaxation on edges, that came from it.
 * After considering of all vertices, we'll check whether we reached end vertex or not. If not - answer "IMPOSSIBLE",
 * otherwise we print all asked information.
 */
    void dijkstra (V startV, V endV) {
        Vertex<V>* temp=&findVertex(startV);
        Vertex<V>* endVertex=&findVertex(endV);
        temp->val=0;

        for (int i = 0; i < matrix.size(); ++i) {
            temp = nullptr;
            for (auto itRow = matrix.begin(); itRow != matrix.end(); itRow++) {
                if (!itRow->first->used && (temp == nullptr || itRow->first->val < temp->val)) {
                    temp = itRow->first;
                }
            }

            temp->used = true;

            vector<Edge<V, E> *> tempVec = edgesFrom(*temp);
            for (int i = 0; i < tempVec.size(); i++) {
                Edge<V, E> *edge = tempVec[i];
                Vertex<V> *vertex = edge->to;

                if (temp->val == INT32_MAX)
                    continue;

                if (temp->val + edge->weight < vertex->val) {
                    vertex->prev = temp;
                    vertex->val = temp->val + edge->weight;
                    vertex->band = min(temp->band, edge->bandwidth);
                }
            }
        }

        if (endVertex->val==INT32_MAX) {
            cout<<"IMPOSSIBLE"<<endl;
        } else {
            temp=endVertex;
            vector<int> outOrder;
            while (temp!= nullptr) {
                outOrder.push_back(temp->name);
                temp=temp->prev;
            }
            reverse(outOrder.begin(),outOrder.end());
            cout<<outOrder.size()<<' ';
            cout << endVertex->val << ' ' << endVertex->band << endl;
            for (int i=0;i<outOrder.size();i++)
                cout<<outOrder[i]<<' ';
        }
    }
};

//=====input functions and main part====================================================================================
//-----input function for tasks 2.1, 2.2--------------------------------------------------------------------------------
//while we can read commands, we will call corresponding functions for our matrix.
void doCommand1_2(AdjacencyMatrixGraph<string ,int>& matrix) {
    string command;
    while (cin>>command) {
        if (command=="ADD_VERTEX") {//Checking whether we already have vertex or not. If have we'll not add it.
            string temp;
            cin>>temp;
            try {
                matrix.findVertex(temp);
            } catch(int a) {
                matrix.addVertex(temp);
            }
        }
        if (command=="REMOVE_VERTEX") {
            string temp;
            cin>>temp;
            matrix.removeVertex(matrix.findVertex(temp));
        }
        if (command=="ADD_EDGE") {
            string from,to;
            int weight;
            cin>>from>>to>>weight;
            matrix.addEdge(from,to,weight);
        }
        if (command=="REMOVE_EDGE") {
            string from,to;
            cin>>from>>to;
            matrix.removeEdge(matrix.findEdge(from,to));
        }
        if (command=="HAS_EDGE") {
            string from,to;
            cin>>from>>to;
            bool a = matrix.hasEdge(matrix.findVertex(from),matrix.findVertex(to));
            if (a==0) {
                cout<<"FALSE"<<endl;
            } else {
                cout<<"TRUE"<<endl;
            }
        }
        if (command=="IS_ACYCLIC") {
            matrix.isAcyclic();
        }
        if (command=="TRANSPOSE") {
            matrix.transpose();
        }
    }
}

//-----input function for tasks 2.3-------------------------------------------------------------------------------------
//standard input and call for corresponding function in matrix (dijkstra)
void doDijkstra(AdjacencyMatrixGraph<int ,int>& matrix) {
    int numVer, numEdge, minBand;
    cin>>numVer;
    for (int i=1; i<=numVer; i++) {
        matrix.addVertex(i);
    }
    cin>>numEdge;
    vector<pair<pair<int,int>,pair<int,int>>> inp;

    for (int i=0; i< numEdge; i++) {
        int from,to,length, band;
        cin>>from>>to>>length>>band;
            inp.emplace_back(make_pair(from,to),make_pair(length,band));
    }
    int first, second;
    cin>>first>>second;
    cin>>minBand;
    for (int i=0; i<inp.size(); i++) {
        if (inp[i].second.second>=minBand) {
            matrix.addEdge(inp[i].first.first,inp[i].first.second,inp[i].second.first,inp[i].second.second);
        }
    }
    matrix.dijkstra(first,second);
}

int main() {//uncomment needed function and matrix initialization and comment other.

    AdjacencyMatrixGraph<string,int> matrixGraph;//matrix for tasks 2.1, 2.2;
    doCommand1_2(matrixGraph);//input function for tasks 2.1, 2.2;

//    AdjacencyMatrixGraph<int,int> matrixGraph;//matrix for task 2.3;
//    doDijkstra(matrixGraph);//input function for task 2.3;

}