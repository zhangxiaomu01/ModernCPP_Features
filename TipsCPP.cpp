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

