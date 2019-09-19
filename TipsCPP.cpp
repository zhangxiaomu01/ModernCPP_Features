//1. Define your own hash function, and used in unordered_set or unordered_map
#include<functional> //std::hash<T> 
#include<unordered_set>
#include<utility>
struct Hash{
     size_t operator()(const pair<int, int>& p) const{
         //hash<long long>() create a hash() variable with default constructor
         //then we call this varibale(val) to do the actual hash.
         return std::hash<long long>()(((long long)p.first << 32) ^ ((long long)p.second));
     }
};
std::unordered_set<std::pair<int, int>, Hash> uSet;



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
iSecret = rand() % 10 + 1;


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



//Dijkstra Algorithm!
https://www.hackerearth.com/zh/practice/algorithms/graphs/shortest-path-algorithms/tutorial/
#define SIZE 100000 + 1

vector < pair < int , int > > v [SIZE];   // each vertex has all the connected vertices with the edges weights
int dist [SIZE];
bool vis [SIZE];

void dijkstra(){
                                                // set the vertices distances as infinity
    memset(vis, false , sizeof vis);            // set all vertex as unvisited
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

