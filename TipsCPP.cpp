//1. Define your own hash function, and used in unordered_set or unordered_map
#include<functional> //std::hash<T> 
#include<unordered_set>
#include<utility>

//The following lambda function won't work as a hash function!
/*
auto hasher = [](pair<char, int>& p){
	return hash<string>()(to_string(p.second)+ '_' + p.first);
};
*/
struct Hash{
     size_t operator()(const pair<int, int>& p) const{
         //hash<long long>() create a hash() variable with default constructor
         //then we call this varibale(val) to do the actual hash.
         return std::hash<long long>()(((long long)p.first << 32) ^ ((long long)p.second));
     }
};
std::unordered_set<std::pair<int, int>, Hash> uSet;


//1.1 for_each function along with lambda function:
auto toLowerC = [](char& c){ c = tolower(c); };
for_each(s.begin(), s.end(), toLowerC);

//1.2 3 ways to define customized comparator for priority_queue
//a. lambda function:
auto myComp = [](pair<int, pair<int, int>>& v1, pair<int, pair<int, int>>& v2){
	return v1.first > v2.first;
};
priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, decltype(myComp)> pq(myComp);
//b. standard templater library std::greater / std::less
priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
//c. customized functor
struct myComp{
	bool operator()(pair<int, pair<int, int>>& v1, pair<int, pair<int, int>>& v2){
		return v1.first > v2.first;
	}
};
priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, myComp> pq;

//2. std::priority_queue is the max queue, which the top element is the largest



//3. partial_sort(it, it+k, it.end, comp) can sort the array from [0..k]
//Time complexity: O(nlogn)
partial_sort(points.begin(), points.begin() + K, points.end(), 
[](vector<int>& a, vector<int>& b){ return a[0] * a[0] + a[1] * a[1] < b[0] * b[0] + b[1] * b[1];} );



//4. nth_element() is the equivalent to the above
//Time complexity: O(n)
auto comp = [](vector<int>& a, vector<int>& b){
            return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
        };
//quick select algorithm!
nth_element(points.begin(), points.begin()+K, points.end(), comp);



//5. Implementation of quick select
class Solution {
    //p0 will be the pivot point
    bool farther(vector<int>& p0, vector<int>& p1){
        return p0[0]*p0[0] + p0[1]*p0[1] < p1[0]*p1[0] + p1[1]*p1[1];
    }
    bool closer(vector<int>& p0, vector<int>& p1){
        return p0[0]*p0[0] + p0[1]*p0[1] > p1[0]*p1[0] + p1[1]*p1[1];
    } 
    //put elements smaller than p[index] before p[index], greater than 
    int partition(vector<vector<int>>& p, int l, int r){
        int index = l;
        l = l + 1;
        while(l <= r){
            if(farther(p[index], p[l]) && closer(p[index], p[r])){
                swap(p[l], p[r]);
                l++; r--;
                continue;
            }
            if(!farther(p[index], p[l])){
                l++;
            }
            if(!closer(p[index], p[r])){
                r--;
            }
        }
        swap(p[index], p[r]);
        return r;
    }
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        int l = 0, r = points.size() - 1;
        if(k > r) return points;
        while(l < r){
            int index = partition(points, l, r);
            //we find kth element
            if(index == k-1) break;
            //we have less than k elements in the left
            if(index < k-1)
                l = index + 1;
            else
                r = index;
        }
        return vector<vector<int>>(points.begin(), points.begin() + k);
    }
};



//6. Implementation of merge sort algorithm
class Solution {
private:
    void mergeSort(vector<int>& nums, int l, int r){
        if(l >= r) return;
        int mid = l + (r - l) / 2;
        mergeSort(nums, l, mid);
        mergeSort(nums, mid+1, r);
        merge(nums, l, mid, r);
    }
    void merge(vector<int>& nums, int l, int mid, int r){
        vector<int> res(r - l + 1, 0);
        int i = l, j = mid+1, ret = 0;
        while(i <= mid && j <= r){
            if(nums[i] <= nums[j]){
                res[ret++] = nums[i++];
            }else
                res[ret++] = nums[j++];
        }
        while(i <= mid) res[ret++] = nums[i++];
        while(j <= r) res[ret++] = nums[j++];
        i = 0, ret = 0;
        //should be < r-l+1
        for(; i < r - l + 1; ++i){
            nums[i + l] = res[ret++];
        }
    }
public:
    vector<int> sortArray(vector<int>& nums) {
        int len = nums.size();
        if(len <= 1) return nums;
        mergeSort(nums, 0, len-1);
        return nums;
    }
};



//7. Implementation of quick sort algorithm!
class Solution {
private:
    void quickSort(vector<int>& nums, int left, int right){
        int index = left;
        int l = left + 1, r = right;
        //Make sure we will have the return condition here!
        if(l > r) return;
        while(l <= r){
            if(nums[l] > nums[index] && nums[r] < nums[index]){
                swap(nums[l], nums[r]);
                l++; r--;
            }
            
            if(nums[l] <= nums[index])
                l++;
            
            if(nums[r] >= nums[index])
                r--;
        }
        swap(nums[index], nums[r]);
        //we already sort r
        int mid = r;
        quickSort(nums, left, mid-1);
        quickSort(nums, mid+1, right);
    }
public:
    vector<int> sortArray(vector<int>& nums) {
        int len = nums.size();
        int l = 0, r = len - 1;
        quickSort(nums, l, r);
        return nums;
    }
};


//8. Implementation of Segment tree data structure!
//Good ref: 
//https://www.topcoder.com/community/competitive-programming/tutorials/range-minimum-query-and-lowest-common-ancestor/
// C++ program to show segment tree operations like construction, query 
// and update 
public class NumArray {
class segmentNode{
    int start;
    int end;
    int sum;
    segmentNode left;
    segmentNode right;
    public segmentNode(int start,int end){
        this.start = start;
        this.end = end;
        this.left = null;
        this.right = null;
        this.sum = 0;
    }
}

segmentNode node;
int[] nums;
public NumArray(int[] nums) {
    this.nums = new int[nums.length];
    for(int i=0;i<nums.length;i++)
        this.nums[i]=nums[i];
    this.node = buildtree(nums,0,nums.length-1);
}

void update(int i, int val) {
    int diff = nums[i]-val;
    nums[i]=val;
    if(node!=null)
        nupdate(node,i,diff);
}

public int sumRange(int i, int j) {
    return node==null?0:sum(node,i,j);
}

public segmentNode buildtree(int[] nums,int start,int end){
    if(start>end)   return null;
    segmentNode root = new segmentNode(start,end);
    if(start == end)
        root.sum = nums[start];
    else{
        int mid = start + (end-start)/2;
        root.left = buildtree(nums,start,mid);
        root.right = buildtree(nums,mid+1,end);
        root.sum = root.left.sum+root.right.sum;
    }
    return root;
}
public int sum(segmentNode root, int start, int end){
    if(root==null||start>root.end || end<root.start)    return 0;
    if(start<=root.start && end>=root.end)  return root.sum;
    int left = sum(root.left,start,end);
    int right = sum(root.right,start,end);
    return left+right;
}
public void nupdate(segmentNode root, int index,int diff){
    if(root==null||index>root.end||index<root.start)  return;
    if(index>=root.start && index<=root.end)    root.sum-=diff;
    nupdate(root.left,index,diff);
    nupdate(root.right,index,diff);
}


//My implementation!
class NumArray {
private:
    int* m_segTree;
    int m_len;
    void buildTree(vector<int>& nums){
        //Calculate leaves
        for(int i = m_len, j = 0; i < 2 * m_len; ++i, ++j){
            m_segTree[i] = nums[j];
        }
        for(int i = m_len-1; i >= 0; --i){
            m_segTree[i] = m_segTree[2*i] + m_segTree[2*i+1];
        }
    }
public:
    NumArray(vector<int>& nums) {
        m_len = nums.size();
        if(m_len > 0){
            m_segTree = new int[2 * m_len];
            buildTree(nums);
        }
    }
    
    //We will start updating leaf node and then go up until the 
    //root
    void update(int i, int val) {
        int pos = i + m_len;
        m_segTree[pos] = val;
        while(pos > 0){
            int left = pos;
            int right = pos;
            //Not easy to get this part right!
            if(pos % 2 == 0){
                right = pos + 1;
            }else
                left = pos - 1;
            m_segTree[pos/2] = m_segTree[left] + m_segTree[right];
            pos = pos/2;
        }
        
    }
    
    int sumRange(int i, int j) {
        //Get the leaf nodes of the range
        int posL = i + m_len;
        int posR = j + m_len;
        
        int sum = 0;
        while(posL <= posR){
            //left range is located in the right sub tree.
            //We need to include node posL to the sum instead
            //of its parent node
            if(posL % 2 == 1){
                sum += m_segTree[posL];
                posL ++;
            }
            //right range is located in the left sub tree
            //We need to include node posR to the sum instead
            //of its parent node
            if(posR % 2 == 0){
                sum += m_segTree[posR];
                posR --;
            }
            //When L == R, one of the above condition will meet
            //So we are guaranteed to break the loop
            posL /= 2;
            posR /= 2;
        }
        
        return sum;
    }
};


//9. Naive implementation of Trie
class TrieNode{
public:
    bool isEnd;
    vector<TrieNode*> branches;
    TrieNode(bool end = false): isEnd(end){
        branches = vector<TrieNode*>(26, nullptr);
    }
};

class Trie {
private:
    TrieNode* root;
public:
    /** Initialize your data structure here. */
    Trie() {
        root = new TrieNode(false);
    }
    
    ~Trie(){
        destroy(root);
    }
    void destroy(TrieNode* node){
        for(auto c : node->branches){
            if(c != nullptr)
                destroy(c);
        }
        delete node;
    }
    
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        int len = word.size();
        TrieNode* node = root;
        int i = 0;
        for(; i < len; i++){
            if(!node->branches[word[i]-'a']){
                break;
            }
            else{
                node = node->branches[word[i] - 'a'];
                //A very important step which guarantee that when we finishes insertion, 
                //We set the termination to be true
                node->isEnd = (i == len-1)? true : node->isEnd;
            }
        }
        for(; i<len; i++){
            node->branches[word[i] - 'a'] = new TrieNode(i == len-1? true:false);
            node = node->branches[word[i] - 'a'];
        }
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        TrieNode* node = root;
        int len = word.size();
        for(int i = 0; i < len; i++){
            if(!node->branches[word[i] - 'a'])
                return false;
            else{
                node = node->branches[word[i] - 'a'];
            }
        }
        return node->isEnd;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        TrieNode* node = root;
        int len = prefix.size();
        for(int i = 0; i < len; i++){
            if(!node->branches[prefix[i] - 'a'])
                return false;
            else{
                node = node->branches[prefix[i] - 'a'];
            }
        }
        return true;
    }
};



//10. Random number generator
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
/* initialize random seed: */
srand (time(NULL));

/* generate secret number between 1 and 10: */
iSelect = rand() % 10 + 1;



//11. Union Find Data structure to solve cycle detection in graph
// A union by rank and path compression based program to detect cycle in a graph 
#include <stdio.h> 
#include <stdlib.h> 

// a structure to represent an edge in the graph 
struct Edge 
{ 
	int src, dest; 
}; 

// a structure to represent a graph 
struct Graph 
{ 
	// V-> Number of vertices, E-> Number of edges 
	int V, E; 

	// graph is represented as an array of edges 
	struct Edge* edge; 
}; 

struct subset 
{ 
	int parent; 
	int rank; 
}; 

// Creates a graph with V vertices and E edges 
struct Graph* createGraph(int V, int E) 
{ 
	struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) ); 
	graph->V = V; 
	graph->E = E; 

	graph->edge = (struct Edge*) malloc( graph->E * sizeof( struct Edge ) ); 

	return graph; 
} 

// A utility function to find set of an element i 
// (uses path compression technique) 
int find(struct subset subsets[], int i) 
{ 
	// find root and make root as parent of i (path compression) 
	if (subsets[i].parent != i) 
		subsets[i].parent = find(subsets, subsets[i].parent); 

	return subsets[i].parent; 
} 

// A function that does union of two sets of x and y 
// (uses union by rank) 
void Union(struct subset subsets[], int x, int y) 
{ 
	int xroot = find(subsets, x); 
	int yroot = find(subsets, y); 

	// Attach smaller rank tree under root of high rank tree 
	// (Union by Rank) 
	if (subsets[xroot].rank < subsets[yroot].rank) 
		subsets[xroot].parent = yroot; 
	else if (subsets[xroot].rank > subsets[yroot].rank) 
		subsets[yroot].parent = xroot; 

	// If ranks are same, then make one as root and increment 
	// its rank by one 
	else
	{ 
		subsets[yroot].parent = xroot; 
		subsets[xroot].rank++; 
	} 
} 

// The main function to check whether a given graph contains cycle or not 
int isCycle( struct Graph* graph ) 
{ 
	int V = graph->V; 
	int E = graph->E; 

	// Allocate memory for creating V sets 
	struct subset *subsets = 
		(struct subset*) malloc( V * sizeof(struct subset) ); 

	for (int v = 0; v < V; ++v) 
	{ 
		subsets[v].parent = v; 
		subsets[v].rank = 0; 
	} 

	// Iterate through all edges of graph, find sets of both 
	// vertices of every edge, if sets are same, then there is 
	// cycle in graph. 
	for(int e = 0; e < E; ++e) 
	{ 
		int x = find(subsets, graph->edge[e].src); 
		int y = find(subsets, graph->edge[e].dest); 

		if (x == y) 
			return 1; 

		Union(subsets, x, y); 
	} 
	return 0; 
} 

// Driver program to test above functions 
int main() 
{ 
	/* Let us create the following graph 
		0 
		| \ 
		| \ 
		1-----2 */

	int V = 3, E = 3; 
	struct Graph* graph = createGraph(V, E); 

	// add edge 0-1 
	graph->edge[0].src = 0; 
	graph->edge[0].dest = 1; 

	// add edge 1-2 
	graph->edge[1].src = 1; 
	graph->edge[1].dest = 2; 

	// add edge 0-2 
	graph->edge[2].src = 0; 
	graph->edge[2].dest = 2; 

	if (isCycle(graph)) 
		printf( "Graph contains cycle" ); 
	else
		printf( "Graph doesn't contain cycle" ); 

	return 0; 
} 



//12. Dijkstra Algorithm!
//https://www.hackerearth.com/zh/practice/algorithms/graphs/shortest-path-algorithms/tutorial/
/*
Dijkstra's algorithm has many variants but the most common one is to find the 
shortest paths from the source vertex to all other vertices in the graph.

Algorithm Steps:

1. Set all vertices distances = infinity except for the source vertex, set the 
source distance = 0.
2. Push the source vertex in a min-priority queue in the form (distance , vertex)
, as the comparison in the min-priority queue will be according to vertices 
distances.
3. Pop the vertex with the minimum distance from the priority queue (at first 
the popped vertex = source).
4. Update the distances of the connected vertices to the popped vertex in case 
of "current vertex distance + edge weight < next vertex distance", then push 
the vertex with the new distance to the priority queue.
5. If the popped vertex is visited before, just continue without using it.
6. Apply the same algorithm again until the priority queue is empty.

Time Complexity of Dijkstra's Algorithm is O(|V|^2), but with min-priority queue
 it drops down to O(|V| + |E|log|V|).

The following implementation assumes the source node to be 1
*/
#define SIZE 100000 + 1

vector < pair < int , int > > v [SIZE];   // each vertex has all the connected vertices with the edges weights
int dist [SIZE];
bool vis [SIZE];

void dijkstra(){
    memset(dist, INT_MAX, sizeof(dist));                                            // set the vertices distances as infinity
    memset(vis, false , sizeof(vis));            // set all vertex as unvisited
    dist[1] = 0;
    multiset < pair < int , int > > s;          // multiset do the job as a min-priority queue

    s.insert({0 , 1});                          // insert the source node with distance = 0

    while(!s.empty()){

        pair <int , int> p = *s.begin();        // pop the vertex with the minimum distance
        s.erase(s.begin());

        int x = p.s; int wei = p.f;
        if( vis[x] ) continue;                  // check if the popped vertex is visited before
         vis[x] = true;

        for(int i = 0; i < v[x].size(); i++){
            int e = v[x][i].f; int w = v[x][i].s;
            if(dist[x] + w < dist[e]  ){            // check if the next vertex distance could be minimized
                dist[e] = dist[x] + w;
                s.insert({dist[e],  e} );           // insert the next vertex with the updated distance
            }
        }
    }
}



//13. Bellman Ford Alogirthm
//https://www.youtube.com/watch?v=-mOEd_3gTK0&t=11s
//https://www.hackerearth.com/zh/practice/algorithms/graphs/shortest-path-algorithms/tutorial/
/*
Bellman Ford's Algorithm:
Bellman Ford's algorithm is used to find the shortest paths from the source 
vertex to all other vertices in a weighted graph. It depends on the following 
concept: Shortest path contains at most  edges, because the shortest path 
couldn't have a cycle.

So why shortest path shouldn't have a cycle ?
There is no need to pass a vertex again, because the shortest path to all other 
vertices could be found without the need for a second visit for any vertices.

Algorithm Steps:

1. The outer loop traverses from  0 - n-1.
2. Loop over all edges, check if the next node distance > current node distance + 
edge weight, in this case update the next node distance to "current node 
distance + edge weight".

This algorithm depends on the relaxation principle where the shortest distance 
for all vertices is gradually replaced by more accurate values until eventually 
reaching the optimum solution. In the beginning all vertices have a distance 
of "Infinity", but only the distance of the source vertex = , then update all 
the connected vertices with the new distances (source vertex distance + edge 
weights), then apply the same concept for the new vertices with new distances 
and so on.

A very important application of Bellman Ford is to check if there is a negative
cycle in the graph. When we finished the n-1 iteration, we can do one more
iteration and check whether our shortest path value can still decreasing. If we
find such path, then there must be a negative cycle!

Time Complexity of Bellman Ford algorithm is relatively high O(|V||E|), 
in case |E| = |V|^2 , O(|E|^3) .
*/
    vector <vector<int>> v [2000 + 10];
    int dis [1000 + 10];

    for(int i = 0; i < m + 2; i++){

        v[i].clear();
        dis[i] = 2e9;
    }

   for(int i = 0; i < m; i++){

        scanf("%d%d%d", &from , &next , &weight);

        v[i].push_back(from);
        v[i].push_back(next);
        v[i].push_back(weight);
   }

    dis[0] = 0;
    for(int i = 0; i < n - 1; i++){
        int j = 0;
        while(v[j].size() != 0){

            if(dis[ v[j][0]  ] + v[j][2] < dis[ v[j][1] ] ){
                dis[ v[j][1] ] = dis[ v[j][0]  ] + v[j][2];
            }
            j++;
        }
    }



//14. Floyd–Warshall's Algorithm
//https://www.youtube.com/watch?v=LwJdNfdLF9s
//https://www.hackerearth.com/zh/practice/algorithms/graphs/shortest-path-algorithms/tutorial/
/*
Floyd–Warshall's Algorithm is used to find the shortest paths between between 
all pairs of vertices in a graph, where each edge in the graph has a weight 
which is positive or negative. The biggest advantage of using this algorithm 
is that all the shortest distances between any 2 vertices could be calculated 
in O(|V|^3), where V is the number of vertices in a graph.

The Algorithm Steps:

For a graph with N vertices:

1. Initialize the shortest paths between any 2 vertices with Infinity.
2. Find all pair shortest paths that use 0 intermediate vertices, then find the 
shortest paths that use 1 intermediate vertex and so on.. until using all N
vertices as intermediate nodes.
3. Minimize the shortest paths between any 2 pairs in the previous operation.
4. For any 2 vertices (i, j), one should actually minimize the distances between
this pair using the first K nodes, so the shortest path will be: 
min(dist[i][K] + dist[K][j], dist[i][j]).
dist[i][K] represents the shortest path that only uses the first K vertices,
dist[K][j] represents the shortest path between the pair k, j. As the shortest 
path will be a concatenation of the shortest path from i to K, then from K to j.

Time Complexity of Floyd–Warshall's Algorithm is O(|V|^3).
*/
//path[i][j] record the shortest path between i and j
for(int k = 1; k <= n; k++){
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            dist[i][j] = min( dist[i][j], dist[i][k] + dist[k][j] );
            path[i][j] = path[k][j];
        }
    }
}


//15. Binary Indexed Tree
//https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
/*
Representation
Binary Indexed Tree is represented as an array. Let the array be BITree[]. 
Each node of the Binary Indexed Tree stores the sum of some elements of the 
input array. The size of the Binary Indexed Tree is equal to the size of the 
input array, denoted as n. In the code below, we use a size of n+1 for ease of 
implementation.

Construction
We initialize all the values in BITree[] as 0. Then we call update() for all 
the indexes, the update() operation is discussed below.

Operations:
getSum(x): Returns the sum of the sub-array arr[0,...,x]
// Returns the sum of the sub-array arr[0,...,x] using BITree[0..n], which is 
constructed from arr[0..n-1]
1) Initialize the output sum as 0, the current index as x+1.
2) Do following while the current index is greater than 0.
...a) Add BITree[index] to sum
...b) Go to the parent of BITree[index].  The parent can be obtained by removing
     the last set bit from the current index, i.e., 
     index = index - (index & (-index))
3) Return sum.

BITree[0] is a dummy node.

BITree[y] is the parent of BITree[x], if and only if y can be obtained by 
removing the last set bit from the binary representation of x, that is 
y = x – (x & (-x)).

The child node BITree[x] of the node BITree[y] stores the sum of the elements 
between y(inclusive) and x(exclusive): arr[y,…,x).

update(x, val): Updates the Binary Indexed Tree (BIT) by performing 
arr[index] += val
// Note that the update(x, val) operation will not change arr[]. It only makes 
changes to BITree[]
1) Initialize the current index as x+1.
2) Do the following while the current index is smaller than or equal to n.
...a) Add the val to BITree[index]
...b) Go to parent of BITree[index].  The parent can be obtained by incrementing
     the last set bit of the current index, i.e., 
     index = index + (index & (-index))

The update function needs to make sure that all the BITree nodes which contain 
arr[i] within their ranges being updated. We loop over such nodes in the BITree
 by repeatedly adding the decimal number corresponding to the last set bit of 
 the current index.

 How does Binary Indexed Tree work?
The idea is based on the fact that all positive integers can be represented as 
the sum of powers of 2. For example 19 can be represented as 16 + 2 + 1. Every 
node of the BITree stores the sum of n elements where n is a power of 2. 
For example, in the first diagram above (the diagram for getSum()), the sum of
the first 12 elements can be obtained by the sum of the last 4 elements (from 
9 to 12) plus the sum of 8 elements (from 1 to 8). The number of set bits in 
the binary representation of a number n is O(Logn). Therefore, we traverse 
at-most O(Logn) nodes in both getSum() and update() operations. The time 
complexity of the construction is O(nLogn) as it calls update() for all n 
elements.

*/
// C++ code to demonstrate operations of Binary Index Tree 
#include <iostream> 

using namespace std; 

/*		 n --> No. of elements present in input array. 
	BITree[0..n] --> Array that represents Binary Indexed Tree. 
	arr[0..n-1] --> Input array for which prefix sum is evaluated. */

// Returns sum of arr[0..index]. This function assumes 
// that the array is preprocessed and partial sums of 
// array elements are stored in BITree[]. 
int getSum(int BITree[], int index) 
{ 
	int sum = 0; // Iniialize result 

	// index in BITree[] is 1 more than the index in arr[] 
	index = index + 1; 

	// Traverse ancestors of BITree[index] 
	while (index>0) 
	{ 
		// Add current element of BITree to sum 
		sum += BITree[index]; 

		// Move index to parent node in getSum View 
		index -= index & (-index); 
	} 
	return sum; 
} 

// Updates a node in Binary Index Tree (BITree) at given index 
// in BITree. The given value 'val' is added to BITree[i] and 
// all of its ancestors in tree. 
void updateBIT(int BITree[], int n, int index, int val) 
{ 
	// index in BITree[] is 1 more than the index in arr[] 
	index = index + 1; 

	// Traverse all ancestors and add 'val' 
	while (index <= n) 
	{ 
	// Add 'val' to current node of BI Tree 
	BITree[index] += val; 

	// Actually get next node of BIT, we need to update all the node contains
    // involves the value from index position
	index += index & (-index); 
	} 
} 

// Constructs and returns a Binary Indexed Tree for given 
// array of size n. 
int *constructBITree(int arr[], int n) 
{ 
	// Create and initialize BITree[] as 0 
	int *BITree = new int[n+1]; 
	for (int i=1; i<=n; i++) 
		BITree[i] = 0; 

	// Store the actual values in BITree[] using update() 
	for (int i=0; i<n; i++) 
		updateBIT(BITree, n, i, arr[i]); 

	// Uncomment below lines to see contents of BITree[] 
	//for (int i=1; i<=n; i++) 
	//	 cout << BITree[i] << " "; 

	return BITree; 
} 


// Driver program to test above functions 
int main() 
{ 
	int freq[] = {2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9}; 
	int n = sizeof(freq)/sizeof(freq[0]); 
	int *BITree = constructBITree(freq, n); 
	cout << "Sum of elements in arr[0..5] is "
		<< getSum(BITree, 5); 

	// Let use test the update operation 
	freq[3] += 6; 
	updateBIT(BITree, n, 3, 6); //Update BIT for above change in arr[] 

	cout << "\nSum of elements in arr[0..5] after update is "
		<< getSum(BITree, 5); 

	return 0; 
} 


//16. AVL Tree (Insertion & Deletion)
/*
AVL tree is a self-balancing Binary Search Tree (BST) where the difference 
between heights of left and right subtrees cannot be more than one for all 
nodes.

Insertion
To make sure that the given tree remains AVL after every insertion, we must 
augment the standard BST insert operation to perform some re-balancing. 
Following are two basic operations that can be performed to re-balance a BST 
without violating the BST property (keys(left) < key(root) < keys(right)).
1) Left Rotation
2) Right Rotation

Steps to follow for insertion
Let the newly inserted node be w
1) Perform standard BST insert for w.
2) Starting from w, travel up and find the first unbalanced node. Let z be the 
first unbalanced node, y be the child of z that comes on the path from w to z 
and x be the grandchild of z that comes on the path from w to z.
3) Re-balance the tree by performing appropriate rotations on the subtree 
rooted with z. There can be 4 possible cases that needs to be handled as x, y 
and z can be arranged in 4 ways. Following are the possible 4 arrangements:
    a) y is left child of z and x is left child of y (Left Left Case)
    b) y is left child of z and x is right child of y (Left Right Case)
    c) y is right child of z and x is right child of y (Right Right Case)
    d) y is right child of z and x is left child of y (Right Left Case)

Following are the operations to be performed in above mentioned 4 cases. In all 
of the cases, we only need to re-balance the subtree rooted with z and the 
complete tree becomes balanced as the height of subtree (After appropriate 
rotations) rooted with z becomes same as it was before insertion. 

a) Left Left Case
T1, T2, T3 and T4 are subtrees.
         z                                      y 
        / \                                   /   \
       y   T4      Right Rotate (z)          x      z
      / \          - - - - - - - - ->      /  \    /  \ 
     x   T3                               T1  T2  T3  T4
    / \
  T1   T2

b) Left Right Case
     z                               z                           x
    / \                            /   \                        /  \ 
   y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
T1   x                          y    T3                    T1  T2 T3  T4
    / \                        / \
  T2   T3                    T1   T2

c) Right Right Case
  z                                y
 /  \                            /   \ 
T1   y     Left Rotate(z)       z      x
    /  \   - - - - - - - ->    / \    / \
   T2   x                     T1  T2 T3  T4
       / \
     T3  T4

d) Right Left Case
   z                            z                            x
  / \                          / \                          /  \ 
T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
    / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
   x   T4                      T2   y                  T1  T2  T3  T4
  / \                              /  \
T2   T3                           T3   T4

Implementation
Following is the implementation for AVL Tree Insertion. The following 
implementation uses the recursive BST insert to insert a new node. In the 
recursive BST insert, after insertion, we get pointers to all ancestors one 
by one in a bottom-up manner. So we don’t need parent pointer to travel up. 
The recursive code itself travels up and visits all the ancestors of the newly 
inserted node.
1) Perform the normal BST insertion.
2) The current node must be one of the ancestors of the newly inserted node. 
Update the height of the current node.
3) Get the balance factor (left subtree height – right subtree height) of the 
current node.
4) If balance factor is greater than 1, then the current node is unbalanced and 
we are either in Left Left case or left Right case. To check whether it is left 
left case or not, compare the newly inserted key with the key in left subtree 
root.
5) If balance factor is less than -1, then the current node is unbalanced and 
we are either in Right Right case or Right-Left case. To check whether it is 
Right Right case or not, compare the newly inserted key with the key in right 
subtree root.

Steps to follow for deletion.
To make sure that the given tree remains AVL after every deletion, we must 
augment the standard BST delete operation to perform some re-balancing. 
Following are two basic operations that can be performed to re-balance a BST 
without violating the BST property (keys(left) < key(root) < keys(right)).
1) Left Rotation
2) Right Rotation

Let w be the node to be deleted
1) Perform standard BST delete for w.
2) Starting from w, travel up and find the first unbalanced node. Let z be the 
first unbalanced node, y be the larger height child of z, and x be the larger 
height child of y. Note that the definitions of x and y are different from 
insertion here.
3) Re-balance the tree by performing appropriate rotations on the subtree 
rooted with z. There can be 4 possible cases that needs to be handled as x, y 
and z can be arranged in 4 ways. Following are the possible 4 arrangements:
    a) y is left child of z and x is left child of y (Left Left Case)
    b) y is left child of z and x is right child of y (Left Right Case)
    c) y is right child of z and x is right child of y (Right Right Case)
    d) y is right child of z and x is left child of y (Right Left Case)

Like insertion, following are the operations to be performed in above 
mentioned 4 cases. Note that, unlike insertion, fixing the node z won’t fix 
the complete AVL tree. After fixing z, we may have to fix ancestors of z as 
well.

a) Left Left Case
T1, T2, T3 and T4 are subtrees.
         z                                      y 
        / \                                   /   \
       y   T4      Right Rotate (z)          x      z
      / \          - - - - - - - - ->      /  \    /  \ 
     x   T3                               T1  T2  T3  T4
    / \
  T1   T2

b) Left Right Case
     z                               z                           x
    / \                            /   \                        /  \ 
   y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
T1   x                          y    T3                    T1  T2 T3  T4
    / \                        / \
  T2   T3                    T1   T2

c) Right Right Case
  z                                y
 /  \                            /   \ 
T1   y     Left Rotate(z)       z      x
    /  \   - - - - - - - ->    / \    / \
   T2   x                     T1  T2 T3  T4
       / \
     T3  T4

d) Right Left Case
   z                            z                            x
  / \                          / \                          /  \ 
T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
    / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
   x   T4                      T2   y                  T1  T2  T3  T4
  / \                              /  \
T2   T3                           T3   T4

Unlike insertion, in deletion, after we perform a rotation at z, we may have 
to perform a rotation at ancestors of z. Thus, we must continue to trace the 
path until we reach the root.

C implementation
Following is the C implementation for AVL Tree Deletion. The following C 
implementation uses the recursive BST delete as basis. In the recursive BST 
delete, after deletion, we get pointers to all ancestors one by one in bottom 
up manner. So we don’t need parent pointer to travel up. The recursive code 
itself travels up and visits all the ancestors of the deleted node.
1) Perform the normal BST deletion.
2) The current node must be one of the ancestors of the deleted node. Update 
the height of the current node.
3) Get the balance factor (left subtree height – right subtree height) of the 
current node.
4) If balance factor is greater than 1, then the current node is unbalanced 
and we are either in Left Left case or Left Right case. To check whether it is 
Left Left case or Left Right case, get the balance factor of left subtree. 
If balance factor of the left subtree is greater than or equal to 0, then it 
is Left Left case, else Left Right case.
5) If balance factor is less than -1, then the current node is unbalanced and 
we are either in Right Right case or Right Left case. To check whether it is 
Right Right case or Right Left case, get the balance factor of right subtree. 
If the balance factor of the right subtree is smaller than or equal to 0, then 
it is Right Right case, else Right Left case.


*/

// C++ program to delete a node from AVL Tree 
#include<bits/stdc++.h> 
using namespace std; 

// An AVL tree node 
class Node 
{ 
	public: 
	int key; 
	Node *left; 
	Node *right; 
	int height; 
}; 

// A utility function to get maximum 
// of two integers 
int max(int a, int b); 

// A utility function to get height 
// of the tree 
int height(Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->height; 
} 

// A utility function to get maximum 
// of two integers 
int max(int a, int b) 
{ 
	return (a > b)? a : b; 
} 

/* Helper function that allocates a 
new node with the given key and 
NULL left and right pointers. */
Node* newNode(int key) 
{ 
	Node* node = new Node(); 
	node->key = key; 
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; // new node is initially 
					// added at leaf 
	return(node); 
} 

// A utility function to right 
// rotate subtree rooted with y 
// See the diagram given above. 
Node *rightRotate(Node *y) 
{ 
	Node *x = y->left; 
	Node *T2 = x->right; 

	// Perform rotation 
	x->right = y; 
	y->left = T2; 

	// Update heights 
	y->height = max(height(y->left), 
					height(y->right)) + 1; 
	x->height = max(height(x->left), 
					height(x->right)) + 1; 

	// Return new root 
	return x; 
} 

// A utility function to left 
// rotate subtree rooted with x 
// See the diagram given above. 
Node *leftRotate(Node *x) 
{ 
	Node *y = x->right; 
	Node *T2 = y->left; 

	// Perform rotation 
	y->left = x; 
	x->right = T2; 

	// Update heights 
	x->height = max(height(x->left), 
					height(x->right)) + 1; 
	y->height = max(height(y->left), 
					height(y->right)) + 1; 

	// Return new root 
	return y; 
} 

// Get Balance factor of node N 
int getBalance(Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return height(N->left) - 
		height(N->right); 
} 

Node* insert(Node* node, int key) 
{ 
	/* 1. Perform the normal BST rotation */
	if (node == NULL) 
		return(newNode(key)); 

	if (key < node->key) 
		node->left = insert(node->left, key); 
	else if (key > node->key) 
		node->right = insert(node->right, key); 
	else // Equal keys not allowed 
		return node; 

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left), 
						height(node->right)); 

	/* 3. Get the balance factor of this 
		ancestor node to check whether 
		this node became unbalanced */
	int balance = getBalance(node); 

	// If this node becomes unbalanced, 
	// then there are 4 cases 

	// Left Left Case 
	if (balance > 1 && key < node->left->key) 
		return rightRotate(node); 

	// Right Right Case 
	if (balance < -1 && key > node->right->key) 
		return leftRotate(node); 

	// Left Right Case 
	if (balance > 1 && key > node->left->key) 
	{ 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	// Right Left Case 
	if (balance < -1 && key < node->right->key) 
	{ 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 

	/* return the (unchanged) node pointer */
	return node; 
} 

/* Given a non-empty binary search tree, 
return the node with minimum key value 
found in that tree. Note that the entire 
tree does not need to be searched. */
Node * minValueNode(Node* node) 
{ 
	Node* current = node; 

	/* loop down to find the leftmost leaf */
	while (current->left != NULL) 
		current = current->left; 

	return current; 
} 

// Recursive function to delete a node 
// with given key from subtree with 
// given root. It returns root of the 
// modified subtree. 
Node* deleteNode(Node* root, int key) 
{ 
	
	// STEP 1: PERFORM STANDARD BST DELETE 
	if (root == NULL) 
		return root; 

	// If the key to be deleted is smaller 
	// than the root's key, then it lies 
	// in left subtree 
	if ( key < root->key ) 
		root->left = deleteNode(root->left, key); 

	// If the key to be deleted is greater 
	// than the root's key, then it lies 
	// in right subtree 
	else if( key > root->key ) 
		root->right = deleteNode(root->right, key); 

	// if key is same as root's key, then 
	// This is the node to be deleted 
	else
	{ 
		// node with only one child or no child 
		if( (root->left == NULL) || 
			(root->right == NULL) ) 
		{ 
			Node *temp = root->left ? 
						root->left : 
						root->right; 

			// No child case 
			if (temp == NULL) 
			{ 
				temp = root; 
				root = NULL; 
			} 
			else // One child case 
			*root = *temp; // Copy the contents of 
						// the non-empty child 
			free(temp); 
		} 
		else
		{ 
			// node with two children: Get the inorder 
			// successor (smallest in the right subtree) 
			Node* temp = minValueNode(root->right); 

			// Copy the inorder successor's 
			// data to this node 
			root->key = temp->key; 

			// Delete the inorder successor 
			root->right = deleteNode(root->right, 
									temp->key); 
		} 
	} 

	// If the tree had only one node 
	// then return 
	if (root == NULL) 
	return root; 

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
	root->height = 1 + max(height(root->left), 
						height(root->right)); 

	// STEP 3: GET THE BALANCE FACTOR OF 
	// THIS NODE (to check whether this 
	// node became unbalanced) 
	int balance = getBalance(root); 

	// If this node becomes unbalanced, 
	// then there are 4 cases 

	// Left Left Case 
	if (balance > 1 && 
		getBalance(root->left) >= 0) 
		return rightRotate(root); 

	// Left Right Case 
	if (balance > 1 && 
		getBalance(root->left) < 0) 
	{ 
		root->left = leftRotate(root->left); 
		return rightRotate(root); 
	} 

	// Right Right Case 
	if (balance < -1 && 
		getBalance(root->right) <= 0) 
		return leftRotate(root); 

	// Right Left Case 
	if (balance < -1 && 
		getBalance(root->right) > 0) 
	{ 
		root->right = rightRotate(root->right); 
		return leftRotate(root); 
	} 

	return root; 
} 

// A utility function to print preorder 
// traversal of the tree. 
// The function also prints height 
// of every node 
void preOrder(Node *root) 
{ 
	if(root != NULL) 
	{ 
		cout << root->key << " "; 
		preOrder(root->left); 
		preOrder(root->right); 
	} 
} 

// Driver Code 
int main() 
{ 
Node *root = NULL; 

	/* Constructing tree given in 
	the above figure */
	root = insert(root, 9); 
	root = insert(root, 5); 
	root = insert(root, 10); 
	root = insert(root, 0); 
	root = insert(root, 6); 
	root = insert(root, 11); 
	root = insert(root, -1); 
	root = insert(root, 1); 
	root = insert(root, 2); 

	/* The constructed AVL Tree would be 
			9 
		/ \ 
		1 10 
		/ \ \ 
	0 5 11 
	/ / \ 
	-1 2 6 
	*/

	cout << "Preorder traversal of the "
			"constructed AVL tree is \n"; 
	preOrder(root); 

	root = deleteNode(root, 10); 

	/* The AVL Tree after deletion of 10 
			1 
		/ \ 
		0 9 
		/ / \ 
	-1 5	 11 
		/ \ 
		2 6 
	*/

	cout << "\nPreorder traversal after"
		<< " deletion of 10 \n"; 
	preOrder(root); 

	return 0; 
} 



// 17. Minimum Spanning Tree (Kruskal's Algorithm)
/*
What is Minimum Spanning Tree?
Given a connected and undirected graph, a spanning tree of that graph is a 
subgraph that is a tree and connects all the vertices together. A single graph 
can have many different spanning trees. A minimum spanning tree (MST) or 
minimum weight spanning tree for a weighted, connected and undirected graph 
is a spanning tree with weight less than or equal to the weight of every other 
spanning tree. The weight of a spanning tree is the sum of weights given to 
each edge of the spanning tree.

How many edges does a minimum spanning tree has?
A minimum spanning tree has (|V| – 1) edges where V is the number of vertices in 
the given graph.

Below are the steps for finding MST using Kruskal’s algorithm
1. Sort all the edges in non-decreasing order of their weight.
2. Pick the smallest edge. Check if it forms a cycle with the spanning tree 
formed so far. If cycle is not formed, include this edge. Else, discard it.
The step#2 uses Union-Find algorithm to detect cycle.
3. Repeat step#2 until there are (V-1) edges in the spanning tree.

Time Complexity: O(ElogE) or O(ElogV). Sorting of edges takes O(ELogE) time. 
After sorting, we iterate through all edges and apply find-union algorithm. 
The find and union operations can take atmost O(LogV) time. So overall 
complexity is O(ELogE + ELogV) time. The value of E can be atmost O(V2), 
so O(LogV) are O(LogE) same. Therefore, overall time complexity is O(ElogE) 
or O(ElogV).

*/
// C++ program for Kruskal's algorithm to find Minimum Spanning Tree 
// of a given connected, undirected and weighted graph 
using namespace std; 

// a structure to represent a weighted edge in graph 
class Edge 
{ 
	public: 
	int src, dest, weight; 
}; 

// a structure to represent a connected, undirected 
// and weighted graph 
class Graph 
{ 
	public: 
	// V-> Number of vertices, E-> Number of edges 
	int V, E; 

	// graph is represented as an array of edges. 
	// Since the graph is undirected, the edge 
	// from src to dest is also edge from dest 
	// to src. Both are counted as 1 edge here. 
	Edge* edge; 
}; 

// Creates a graph with V vertices and E edges 
Graph* createGraph(int V, int E) 
{ 
	Graph* graph = new Graph; 
	graph->V = V; 
	graph->E = E; 

	graph->edge = new Edge[E]; 

	return graph; 
} 

// A structure to represent a subset for union-find 
class subset 
{ 
	public: 
	int parent; 
	int rank; 
}; 

// A utility function to find set of an element i 
// (uses path compression technique) 
int find(subset subsets[], int i) 
{ 
	// find root and make root as parent of i 
	// (path compression) 
	if (subsets[i].parent != i) 
		subsets[i].parent = find(subsets, subsets[i].parent); 

	return subsets[i].parent; 
} 

// A function that does union of two sets of x and y 
// (uses union by rank) 
void Union(subset subsets[], int x, int y) 
{ 
	int xroot = find(subsets, x); 
	int yroot = find(subsets, y); 

	// Attach smaller rank tree under root of high 
	// rank tree (Union by Rank) 
	if (subsets[xroot].rank < subsets[yroot].rank) 
		subsets[xroot].parent = yroot; 
	else if (subsets[xroot].rank > subsets[yroot].rank) 
		subsets[yroot].parent = xroot; 

	// If ranks are same, then make one as root and 
	// increment its rank by one 
	else
	{ 
		subsets[yroot].parent = xroot; 
		subsets[xroot].rank++; 
	} 
} 

// Compare two edges according to their weights. 
// Used in qsort() for sorting an array of edges 
int myComp(const void* a, const void* b) 
{ 
	Edge* a1 = (Edge*)a; 
	Edge* b1 = (Edge*)b; 
	return a1->weight > b1->weight; 
} 

// The main function to construct MST using Kruskal's algorithm 
void KruskalMST(Graph* graph) 
{ 
	int V = graph->V; 
	Edge result[V]; // Tnis will store the resultant MST 
	int e = 0; // An index variable, used for result[] 
	int i = 0; // An index variable, used for sorted edges 

	// Step 1: Sort all the edges in non-decreasing 
	// order of their weight. If we are not allowed to 
	// change the given graph, we can create a copy of 
	// array of edges 
	qsort(graph->edge, graph->E, sizeof(graph->edge[0]), myComp); 

	// Allocate memory for creating V ssubsets 
	subset *subsets = new subset[( V * sizeof(subset) )]; 

	// Create V subsets with single elements 
	for (int v = 0; v < V; ++v) 
	{ 
		subsets[v].parent = v; 
		subsets[v].rank = 0; 
	} 

	// Number of edges to be taken is equal to V-1 
	while (e < V - 1 && i < graph->E) 
	{ 
		// Step 2: Pick the smallest edge. And increment 
		// the index for next iteration 
		Edge next_edge = graph->edge[i++]; 

		int x = find(subsets, next_edge.src); 
		int y = find(subsets, next_edge.dest); 

		// If including this edge does't cause cycle, 
		// include it in result and increment the index 
		// of result for next edge 
		if (x != y) 
		{ 
			result[e++] = next_edge; 
			Union(subsets, x, y); 
		} 
		// Else discard the next_edge 
	} 

	// print the contents of result[] to display the 
	// built MST 
	cout<<"Following are the edges in the constructed MST\n"; 
	for (i = 0; i < e; ++i) 
		cout<<result[i].src<<" -- "<<result[i].dest<<" == "<<result[i].weight<<endl; 
	return; 
} 

// Driver code 
int main() 
{ 
	/* Let us create following weighted graph 
			10 
		0--------1 
		| \ | 
	6| 5\ |15 
		| \ | 
		2--------3 
			4 */
	int V = 4; // Number of vertices in graph 
	int E = 5; // Number of edges in graph 
	Graph* graph = createGraph(V, E); 


	// add edge 0-1 
	graph->edge[0].src = 0; 
	graph->edge[0].dest = 1; 
	graph->edge[0].weight = 10; 

	// add edge 0-2 
	graph->edge[1].src = 0; 
	graph->edge[1].dest = 2; 
	graph->edge[1].weight = 6; 

	// add edge 0-3 
	graph->edge[2].src = 0; 
	graph->edge[2].dest = 3; 
	graph->edge[2].weight = 5; 

	// add edge 1-3 
	graph->edge[3].src = 1; 
	graph->edge[3].dest = 3; 
	graph->edge[3].weight = 15; 

	// add edge 2-3 
	graph->edge[4].src = 2; 
	graph->edge[4].dest = 3; 
	graph->edge[4].weight = 4; 

	KruskalMST(graph); 

	return 0; 
} 


// 18. Binary Heap
/*
A Binary Heap is a Binary Tree with following properties.
1) It’s a complete tree (All levels are completely filled except possibly the 
last level and the last level has all keys as left as possible). This property 
of Binary Heap makes them suitable to be stored in an array.

2) A Binary Heap is either Min Heap or Max Heap. In a Min Binary Heap, the key 
at root must be minimum among all keys present in Binary Heap. The same 
property must be recursively true for all nodes in Binary Tree. Max Binary 
Heap is similar to MinHeap.

How is Binary Heap represented?
A Binary Heap is a Complete Binary Tree. A binary heap is typically represented as an array.

The root element will be at Arr[0].
Below table shows indexes of other nodes for the ith node, i.e., Arr[i]:
Arr[(i-1)/2]	Returns the parent node
Arr[(2*i)+1]	Returns the left child node
Arr[(2*i)+2]	Returns the right child node
The traversal method use to achieve Array representation is Level Order.

Operations on Min Heap:
1) getMini(): It returns the root element of Min Heap. Time Complexity of this 
operation is O(1).

2) extractMin(): Removes the minimum element from MinHeap. Time Complexity of 
this Operation is O(Logn) as this operation needs to maintain the heap 
property (by calling heapify()) after removing root.

3) decreaseKey(): Decreases value of key. The time complexity of this 
operation is O(Logn). If the decreases key value of a node is greater than 
the parent of the node, then we don’t need to do anything. Otherwise, we need 
to traverse up to fix the violated heap property.

4) insert(): Inserting a new key takes O(Logn) time. We add a new key at the 
end of the tree. IF new key is greater than its parent, then we don’t need to 
do anything. Otherwise, we need to traverse up to fix the violated heap 
property.

5) delete(): Deleting a key also takes O(Logn) time. We replace the key to be 
deleted with minum infinite by calling decreaseKey(). After decreaseKey(), the 
minus infinite value must reach root, so we call extractMin() to remove the key.

*/
// A C++ program to demonstrate common Binary Heap Operations 
#include<iostream> 
#include<climits> 
using namespace std; 

// Prototype of a utility function to swap two integers 
void swap(int *x, int *y); 

// A class for Min Heap 
class MinHeap 
{ 
	int *harr; // pointer to array of elements in heap 
	int capacity; // maximum possible size of min heap 
	int heap_size; // Current number of elements in min heap 
public: 
	// Constructor 
	MinHeap(int capacity); 

	// to heapify a subtree with the root at given index 
	void MinHeapify(int ); 

	int parent(int i) { return (i-1)/2; } 

	// to get index of left child of node at index i 
	int left(int i) { return (2*i + 1); } 

	// to get index of right child of node at index i 
	int right(int i) { return (2*i + 2); } 

	// to extract the root which is the minimum element 
	int extractMin(); 

	// Decreases key value of key at index i to new_val 
	void decreaseKey(int i, int new_val); 

	// Returns the minimum key (key at root) from min heap 
	int getMin() { return harr[0]; } 

	// Deletes a key stored at index i 
	void deleteKey(int i); 

	// Inserts a new key 'k' 
	void insertKey(int k); 
}; 

// Constructor: Builds a heap from a given array a[] of given size 
MinHeap::MinHeap(int cap) 
{ 
	heap_size = 0; 
	capacity = cap; 
	harr = new int[cap]; 
} 

// Inserts a new key 'k' 
void MinHeap::insertKey(int k) 
{ 
	if (heap_size == capacity) 
	{ 
		cout << "\nOverflow: Could not insertKey\n"; 
		return; 
	} 

	// First insert the new key at the end 
	heap_size++; 
	int i = heap_size - 1; 
	harr[i] = k; 

	// Fix the min heap property if it is violated 
	while (i != 0 && harr[parent(i)] > harr[i]) 
	{ 
        swap(&harr[i], &harr[parent(i)]); 
        i = parent(i); 
	} 
} 

// Decreases value of key at index 'i' to new_val. It is assumed that 
// new_val is smaller than harr[i]. 
void MinHeap::decreaseKey(int i, int new_val) 
{ 
	harr[i] = new_val; 
	while (i != 0 && harr[parent(i)] > harr[i]) 
	{ 
        swap(&harr[i], &harr[parent(i)]); 
        i = parent(i); 
	} 
} 

// Method to remove minimum element (or root) from min heap 
int MinHeap::extractMin() 
{ 
	if (heap_size <= 0) 
		return INT_MAX; 
	if (heap_size == 1) 
	{ 
		heap_size--; 
		return harr[0]; 
	} 

	// Store the minimum value, and remove it from heap 
	int root = harr[0]; 
	harr[0] = harr[heap_size-1]; 
	heap_size--; 
	MinHeapify(0); 

	return root; 
} 


// This function deletes key at index i. It first reduced value to minus 
// infinite, then calls extractMin() 
void MinHeap::deleteKey(int i) 
{ 
	decreaseKey(i, INT_MIN); 
	extractMin(); 
} 

// A recursive method to heapify a subtree with the root at given index 
// This method assumes that the subtrees are already heapified 
void MinHeap::MinHeapify(int i) 
{ 
	int l = left(i); 
	int r = right(i); 
	int smallest = i; 
	if (l < heap_size && harr[l] < harr[i]) 
		smallest = l; 
	if (r < heap_size && harr[r] < harr[smallest]) 
		smallest = r; 
	if (smallest != i) 
	{ 
		swap(&harr[i], &harr[smallest]); 
		MinHeapify(smallest); 
	} 
} 

// A utility function to swap two elements 
void swap(int *x, int *y) 
{ 
	int temp = *x; 
	*x = *y; 
	*y = temp; 
} 

// Driver program to test above functions 
int main() 
{ 
	MinHeap h(11); 
	h.insertKey(3); 
	h.insertKey(2); 
	h.deleteKey(1); 
	h.insertKey(15); 
	h.insertKey(5); 
	h.insertKey(4); 
	h.insertKey(45); 
	cout << h.extractMin() << " "; 
	cout << h.getMin() << " "; 
	h.decreaseKey(2, 1); 
	cout << h.getMin(); 
	return 0; 
} 


// 19. Prim's MST algorithm
/*
Prim’s algorithm is also a Greedy algorithm. It starts with an empty spanning 
tree. The idea is to maintain two sets of vertices. The first set contains the 
vertices already included in the MST, the other set contains the vertices not 
yet included. At every step, it considers all the edges that connect the two 
sets, and picks the minimum weight edge from these edges. After picking the 
edge, it moves the other endpoint of the edge to the set containing MST.

A group of edges that connects two set of vertices in a graph is called cut 
in graph theory. So, at every step of Prim’s algorithm, we find a cut (of two 
sets, one contains the vertices already included in MST and other contains 
rest of the verices), pick the minimum weight edge from the cut and include 
this vertex to MST Set (the set that contains already included vertices).

Algorithm
1) Create a set mstSet that keeps track of vertices already included in MST.
2) Assign a key value to all vertices in the input graph. Initialize all key 
values as INFINITE. Assign key value as 0 for the first vertex so that it is 
picked first.
3) While mstSet doesn’t include all vertices
….a) Pick a vertex u which is not there in mstSet and has minimum key value.
….b) Include u to mstSet.
….c) Update key value of all adjacent vertices of u. To update the key values,
iterate through all adjacent vertices. For every adjacent vertex v, if weight 
of edge u-v is less than the previous key value of v, update the key value as 
weight of u-v。

The idea of using key values is to pick the minimum weight edge from cut. The 
key values are used only for vertices which are not yet included in MST, the 
key value for these vertices indicate the minimum weight edges connecting them
to the set of vertices included in MST.

How to implement the above algorithm?
We use a boolean array mstSet[] to represent the set of vertices included in 
MST. If a value mstSet[v] is true, then vertex v is included in MST, otherwise 
not. Array key[] is used to store key values of all vertices. Another array 
parent[] to store indexes of parent nodes in MST. The parent array is the 
output array which is used to show the constructed MST.

Time Complexity of the above program is O(V^2). If the input graph is 
represented using adjacency list, then the time complexity of Prim’s 
algorithm can be reduced to O(E log V) with the help of binary heap. 
*/
// A C++ program for Prim's Minimum 
// Spanning Tree (MST) algorithm. The program is 
// for adjacency matrix representation of the graph 
#include <bits/stdc++.h> 
using namespace std; 

// Number of vertices in the graph 
#define V 5 

// A utility function to find the vertex with 
// minimum key value, from the set of vertices 
// not yet included in MST 
int minKey(int key[], bool mstSet[]) 
{ 
	// Initialize min value 
	int min = INT_MAX, min_index; 

	for (int v = 0; v < V; v++) 
		if (mstSet[v] == false && key[v] < min) 
			min = key[v], min_index = v; 

	return min_index; 
} 

// A utility function to print the 
// constructed MST stored in parent[] 
void printMST(int parent[], int graph[V][V]) 
{ 
	cout<<"Edge \tWeight\n"; 
	for (int i = 1; i < V; i++) 
		cout<<parent[i]<<" - "<<i<<" \t"<<graph[i][parent[i]]<<" \n"; 
} 

// Function to construct and print MST for 
// a graph represented using adjacency 
// matrix representation 
void primMST(int graph[V][V]) 
{ 
	// Array to store constructed MST 
	int parent[V]; 
	
	// Key values used to pick minimum weight edge in cut 
	int key[V]; 
	
	// To represent set of vertices not yet included in MST 
	bool mstSet[V]; 

	// Initialize all keys as INFINITE 
	for (int i = 0; i < V; i++) 
		key[i] = INT_MAX, mstSet[i] = false; 

	// Always include first 1st vertex in MST. 
	// Make key 0 so that this vertex is picked as first vertex. 
	key[0] = 0; 
	parent[0] = -1; // First node is always root of MST 

	// The MST will have V vertices 
	for (int count = 0; count < V - 1; count++) 
	{ 
		// Pick the minimum key vertex from the 
		// set of vertices not yet included in MST 
		int u = minKey(key, mstSet); 

		// Add the picked vertex to the MST Set 
		mstSet[u] = true; 

		// Update key value and parent index of 
		// the adjacent vertices of the picked vertex. 
		// Consider only those vertices which are not 
		// yet included in MST 
		for (int v = 0; v < V; v++) 

			// graph[u][v] is non zero only for adjacent vertices of m 
			// mstSet[v] is false for vertices not yet included in MST 
			// Update the key only if graph[u][v] is smaller than key[v] 
			if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v]) 
				parent[v] = u, key[v] = graph[u][v]; 
	} 

	// print the constructed MST 
	printMST(parent, graph); 
} 

// Driver code 
int main() 
{ 
	/* Let us create the following graph 
		2 3 
	(0)--(1)--(2) 
	| / \ | 
	6| 8/ \5 |7 
	| / \ | 
	(3)-------(4) 
			9	 */
	int graph[V][V] = { { 0, 2, 0, 6, 0 }, 
						{ 2, 0, 3, 8, 5 }, 
						{ 0, 3, 0, 0, 7 }, 
						{ 6, 8, 0, 0, 9 }, 
						{ 0, 5, 7, 9, 0 } }; 

	// Print the solution 
	primMST(graph); 

	return 0; 
} 


//*********************************************************************
// 20. Extract information from string
stringstream ss(data);
vector<int> val;
while(ss){
	string s;
	ss >> s;
	if(!s.empty()){
		int num = stoi(s);
		val.push_back(num);
	}
}



//*********************************************************************
//21. KMP algorithm
// C++ program for implementation of KMP pattern searching 
// algorithm 
void computeLPSArray(char* pat, int M, int* lps); 

// Prints occurrences of txt[] in pat[] 
void KMPSearch(char* pat, char* txt) 
{ 
	int M = strlen(pat); 
	int N = strlen(txt); 

	// create lps[] that will hold the longest prefix suffix 
	// values for pattern 
	int lps[M]; 

	// Preprocess the pattern (calculate lps[] array) 
	computeLPSArray(pat, M, lps); 

	int i = 0; // index for txt[] 
	int j = 0; // index for pat[] 
	while (i < N) { 
		if (pat[j] == txt[i]) { 
			j++; 
			i++; 
		} 

		if (j == M) { 
			printf("Found pattern at index %d ", i - j); 
			j = lps[j - 1]; 
		} 

		// mismatch after j matches 
		else if (i < N && pat[j] != txt[i]) { 
			// Do not match lps[0..lps[j-1]] characters, 
			// they will match anyway 
			if (j != 0) 
				j = lps[j - 1]; 
			else
				i = i + 1; 
		} 
	} 
} 

// Fills lps[] for given patttern pat[0..M-1] 
void computeLPSArray(char* pat, int M, int* lps) 
{ 
	// length of the previous longest prefix suffix 
	int len = 0; 

	lps[0] = 0; // lps[0] is always 0 

	// the loop calculates lps[i] for i = 1 to M-1 
	int i = 1; 
	while (i < M) { 
		if (pat[i] == pat[len]) { 
			len++; 
			lps[i] = len; 
			i++; 
		} 
		else // (pat[i] != pat[len]) 
		{ 
			// This is tricky. Consider the example. 
			// AAACAAAA and i = 7. The idea is similar 
			// to search step. 
			if (len != 0) { 
				len = lps[len - 1]; 

				// Also, note that we do not increment 
				// i here 
			} 
			else // if (len == 0) 
			{ 
				lps[i] = 0; 
				i++; 
			} 
		} 
	} 
} 

// Driver program to test above function 
int main() 
{ 
	char txt[] = "ABABDABACDABABCABAB"; 
	char pat[] = "ABABCABAB"; 
	KMPSearch(pat, txt); 
	return 0; 
} 



