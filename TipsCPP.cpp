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
