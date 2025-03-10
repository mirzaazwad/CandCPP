# Segment Tree
## Why segment tree?

Let's say we want a range of values that we need to apply operations on, for example, we need the sum of elements from index 1 to index 3 of
```
  arr[] = {4,-9,3,7,1,0,2};
```
We can use a prefix sum:
See more about prefix sums in the following link: [Click here to view notes on prefix sum](https://github.com/mirzaazwad/Competitive-Programming/tree/main/CategoryWisePersonalNotesTutorialsAndProblemsSolved/Data%20Structures/RangeQueries/Prefix_Array)

But let's say now we have queries of upto 10<sup>5</sup> and an array of range upto 10<sup>5</sup>. Each query gives us indices forSo using prefix sum no longer works because of the risk of getting a time limited exceeded in comepetitive programming platforms. So now you need a data structure called segment tree.

Prerequisites for understanding segment tree:
* Recursion
* Merge Sort
* Divide and Conquer

The essence of Divide and Conquer is that it breaks a problem into subproblems and then it finds the results for these subproblems and then you merge these results to compute the result for the bigger problems.

So essentially now we are going to implement a tree with the following properties:
* The idea is creating a binary tree.
* We implement it by breaking the array into 2 halves, basically a divide and conquer approach similar to merge sort. We compute the sum of those halves and utilise that to compute the sum of the segment in O(logn).

So we keep dividing the array into smaller and smaller portions.

![Array Split First Split](https://github.com/mirzaazwad/Competitive-Programming/blob/main/CategoryWisePersonalNotesTutorialsAndProblemsSolved/Data%20Structures/RangeQueries/SegmentTree/ArraySplitFirst.png)

Now if you precompute the sums for the left half and the right half and store it, you can easily compute the sum for the larger array. I suppose we can understand intuitively that the halves have to be further split till we reach an array of size 1. we do know the sum of the 1-sized portion which is the value itself. Using this we can compute the sum for the larger array.

![DivideSum](https://github.com/mirzaazwad/Competitive-Programming/blob/main/CategoryWisePersonalNotesTutorialsAndProblemsSolved/Data%20Structures/RangeQueries/SegmentTree/DivideSum.png)

Even though the image may seem complex but the idea is pretty simple. We break the array into smaller portions and then we find the value for the smallest portion which is essentially the leaf node of the tree. Since we do know the sum for the leaf nodes then we can use that to compute the sum for the bigger arrays by the sum of the nodes to the left or to the right. 

Now if we assume that all the segments in the picture are nodes then we can get a binary tree. Every node contains the sum of that particular portion or halve. This is the essence of a segment tree. Now the question may arise as to how we obtain the sum from index i to j since we are breaking the entire array and then finally we get the sum of the entire array.

Now let's observe the array differently, in a different tree like structure:

![Segment Tree Picture](https://github.com/mirzaazwad/Competitive-Programming/blob/main/CategoryWisePersonalNotesTutorialsAndProblemsSolved/Data%20Structures/RangeQueries/SegmentTree/TreeBuild.png)

Observe that its basically the same as the array tree observed earlier but this time we only showed the segments or the ranges. For instance the 3rd node shows the sum of range from 5 to 7. The numbering of the nodes is essential. The root node is 1 and the node to the left of 1 is:
> 1*2=2

And the node to the right is:

> 1*2+1=3

Which means that if root is say x, then the left node is 2x and the right node is 2x+1. Now first let's see how this structure is built.


## Initialize the segment tree
```cpp
#define MAX 100001
int arr[MAX];
int tree[MAX*3];
 void init(int node,int l,int r){
 {
  if(l==r)
    {
    tree[node]=arr[l];
    return;
   }
   int left=node*2;
   int right=left+1;
   int mid=(l+r)/2;
   init(left,l,mid);
   init(right,mid+1,r);
   tree[node] = tree[l] + tree[r];
  }
```
Then based on the level order indexing we initialise the tree array.

```cpp
  // N*3+1 where N is the length of the array being converted to segment tree.
  int tree[N*3+1];
  // 8 5 3 -5 10 1 2 4 -9 3 7 1 0
```

tree array is used to store the tree or basically the values of the tree. But, 
#### Why is the tree array 3 times the size of the input array?
The reason for this is that for n nodes, we should have approximately number of nodes, x= 2<sup>ceil(log2(n))+1</sup>-1 nodes. Given our range is at max 10<sup>5</sup> so total number of nodes divided by max range gives us x/10<sup>5</sup> = 2.6 and a ceil value for that is 3 that essentially tells us why we take 3 times the original array for safety provided that the range is upto 10<sup>5</sup>. In some places, you might see 4 times multiplication for more safety in taking an upper bound as 10<sup>5</sup> x 4 = 4 x 10<sup>5</sup> is still less than 10<sup>6</sup> which is acceptable for most competitive programming sites.


Now back to init function,
init function generates the tree from arr. The parameters for init are node, l and r.  Node indicates the current node and l,r indicates the current range. At the beginning we traverse from node 1 and the range from 1 to 7.(Observe the picture of the tree given above). 

Now if l==r is true then we have reached the last node or the leaf node and so the value would be the value stored in the array, so we return that value. Else we split the array into 2 halves. The left node would have an index node*2 and the right node would have an index node*2+1. And the array would be split at the middle.
The first step of the idea:

```cpp
int mid=(l+r)/2;
```

As per the mid we break the range into equal halves. Now if we recursively call init for the two sides, we would the sum for the left and right portions. Assuming we computed that, the sum for the current node would be the sum of left and the right nodes. If you have trouble understanding this logic, try to dry run the code on pen and paper.

## Query a Segment Tree

Now let's say we need a query function that can give us the sum for the range from i to j. Let's assume that i=2 and j=6. **Then the aim should be to be obtain the result as the sum of the yellow nodes.**

![QuerySum](https://github.com/mirzaazwad/Competitive-Programming/blob/main/CategoryWisePersonalNotesTutorialsAndProblemsSolved/Data%20Structures/RangeQueries/SegmentTree/QuerySum.png)

So the yellow nodes are the **RELEVANT NODES**, the rest are **EXTRA NODES**. The main purpose of our query function is to find the sum of the relevant nodes. The code would be similar to the init function with some added conditions. Say you are at a node such that it contains the sum from b to e. How do you understand that this is a **RELEVANT NODE**?

There can be three cases:
A(b>=i and e<=j): The current segment is completely a part of i-j segment.
B(i>e or j<b): The current segment is completely outside the i-j segment, so its not necessary to consider this segment.
C(not(A or B)): Basically a part of it is within i-j segment and a part of it is outside i-j segment.

This can be graphically represented as :
![SegmentMatch](https://github.com/mirzaazwad/Competitive-Programming/blob/main/CategoryWisePersonalNotesTutorialsAndProblemsSolved/Data%20Structures/RangeQueries/SegmentTree/SegmentMatch.png)

```cpp
int query(int node, int b, int e, int i, int j)
{
  if(i>e || j<b){
    return 0;//goes outside Case B
  }
  if(b>=i && e<=j){
    return tree[node];//stays inside Case A
   }
   int left=node*2;
   int right=node*2+1;
   int mid=(b+e)/2;
   int p1=query(left,b,mid,i,j);
   int p2=query(right,mid+1,e,i,j);
   return p1+p2;
}
```
The query function works in a similar way to the init function. Here i,j is basically the required range and b,e is the range of the current node. 



## Update Function in Segment Tree

Lastly, we have the update operation. The main reason we made the tree instead of using prefix sum. Let's say you are asked to update index 3 with a value of 10. This means that we update the value for the node that has the 3-3 range. Basically the **yellow node in the image given below**. If the node is updated, the values of the **blue nodes will also be updated**. These blue nodes exist in the path to the yellow node. The remaining nodes won't be affected because the node 3 is outside the range of these nodes.

![SegmentTreePath](https://github.com/mirzaazwad/Competitive-Programming/blob/main/CategoryWisePersonalNotesTutorialsAndProblemsSolved/Data%20Structures/RangeQueries/SegmentTree/SegmentTreePath.png)
```cpp
void update(int node, int l, int r, int i, int newval)
{
  if(i>right || i<left){
    return;
  }
  if(left>=i && right<=i){
    tree[node]=newval;
    return;
  }
  int left=2*node;
  int right=left+1;
  int mid=(l+r)/2;
  update(left,l,mid,i,newval);
  update(right,mid+1,r,i,newVal);
  tree[node]=tree[left]+tree[right];
}
```
The i<sup>th</sup> node would be updated and the extra segments would be **pruned** or ignored at the very beginning. In the relevant segments we have inserted the new sum. Now the condition l==r would also be valid since we are always updating the lead of nodes. Instead of calling init you can also make the tree by calling update on each node.

Due to the array being split into 2, the height of the tree is at max log(n). So every query and update can be performed in log(n) complexity. Segment tree should only be used if we can combine two smaller segment to make a larger segment. Given the least value of the left side and the highest value of the right side, the value of the root node can also be computed with ease. An important idea has been ignored. Let's say we have been asked to update indices from i to j instead of just a specific index. If we update every leaf node seperately it would be O(nlogn) that would give you a TLE. That is where we introduce the idea of Lazy Propagation.

## Some Problems that do not involve Lazy Propagation
* [Array Queries LightOJ](https://lightoj.com/problem/array-queries)
* [MULTQ3 SPOJ](https://www.spoj.com/problems/MULTQ3/)
* [11235 UVA Frequent Values](https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=134&page=show_problem&problem=2176)
* [Curious Robin Hood LightOJ](https://lightoj.com/problem/curious-robin-hood)

## Lazy Propagation Technique(Not in CSE 4303)

The most elegant function of segment trees comes from the lazy propagation technique. This deals with a range of indices being updated at once. This is a common technique used in most segment tree problems. 

Suppose you have an array given of size n and you have to perform queries from i to j index and find the sum of these indices. And for the update operation you have been asked to update the range from i to j by an addition of x.

For instance, if the array is:
```
{4, 1, 2, 3, 9, 8, 7}
```
We are asked to update i=3 to j=5 by an addition of 2 then the array becomes:
```
{4,1,2+2,3+2,9+2,8,7}
```
In this case, from i=3 to j=4 that sum of the values would be 2+2+3+2=9 instead of 2+3=5 as it was earlier. In the earlier cases we just performed update on one index but now the issue of a range of values being updated starts. Note that in a tree the leaf node is the bottom-most nodes and any nodes above the leaf node is an internal node.  So what are the contents of the leaf nodes in segment trees. It basically contains the original value of some index. In a segment tree, the internal nodes have the merged results of the leaf nodes. In a segment tree, the range of a node is the indices that have been merged to produce the result. Such as the range for node 3 is 5 to 7 as per the image given below. Just like in the image of the tree above, the node 10 contains the value for index 3, and node 2 contains the merged sum value for 1,2,3 and 4. The efficient approach would be store that from node 2 onwards all the nodes would have x added to it. ** When we see that for a node, it is completely within the range dictated by i and j, we basically save a propagation value for all the nodes below it instead of traversing down the branches and updating them. For a node the __PROPAGATION VALUE IS THE VALUE ADDED TO ALL THE INDICES WITHIN THE RANGE OF THAT NODE__ . See the image below to understand how it is saved.

![PropagateSegment](https://github.com/mirzaazwad/Competitive-Programming/blob/main/CategoryWisePersonalNotesTutorialsAndProblemsSolved/Data%20Structures/RangeQueries/SegmentTree/PropagateSegment.png)

```cpp
struct info {
    long long prop, sum;
} tree[mx * 3]; //Besides sum the extra values being added below that node is stored in prop
void update(int node, int b, int e, int i, int j, long long x)
{
    if (i > e || j < b)
        return;
    if (b >= i && e <= j) Range of node is within range of update
    {
        tree[node].sum += ((e - b + 1) * x); //There are e-b+1 nodes below, so e-b+1 times x would be added for this range
        tree[node].prop += x; //Nodes below that would have x added to it
        return;
    }
    int Left = node * 2;
    int Right = (node * 2) + 1;
    int mid = (b + e) / 2;
    update(Left, b, mid, i, j, x);
    update(Right, mid + 1, e, i, j, x);
    tree[node].sum = tree[Left].sum + tree[Right].sum + (e - b + 1) * tree[node].prop;
    //Backtracking to update the nodes in the path
    //Even besides left and right side sum extra values below the node would also be added
}
```


```cpp
int query(int node, int b, int e, int i, int j, int carry = 0)
{
    if (i > e || j < b)
        return 0;

    if (b >= i and e <= j)
        return tree[node].sum + carry * (e - b + 1); //The extra values to be added to the range would also be added to the sum

    int Left = node << 1;
    int Right = (node << 1) + 1;
    int mid = (b + e) >> 1;

    int p1 = query(Left, b, mid, i, j, carry + tree[node].prop); propagate value is being propagated by carry
    int p2 = query(Right, mid + 1, e, i, j, carry + tree[node].prop);

    return p1 + p2;
}
```
## Some Problems that involve Lazy Propagation

* [LightOJ Segment Tree Section](https://lightoj.com/problems/category/segment-tree)
* [HORRIBLE-Horrible Queries SPOJ](https://www.spoj.com/problems/HORRIBLE/)
* [LITE-Light Switching SPOJ](https://www.spoj.com/problems/LITE/)



## References

### Bangla Tutorial for Segment Tree:
* [Segment Tree Part 1 Basics Bangla](http://www.shafaetsplanet.com/?p=1557)
* [Segment Tree Part 2 Lazy Propagation Bangla](https://www.shafaetsplanet.com/?p=1591)
### English Tutorials:
* [Segment Tree CP Algorithms](https://cp-algorithms.com/data_structures/segment_tree.html)
* [Segment Tree Codeforces](https://codeforces.com/blog/entry/15890)
### Tutorial Videos:
* [Segment Tree Section](https://www.youtube.com/channel/UCozCCU3b1HmcmCf2gLN_7HA/videos)

## Interesting topics to explore
* Femwick Tree/BIT
* DSU(Disjoint Set Union)
* Sparse Matrix
* Range Max Query and Range Min Query(RMQ)
* Online and Offline Queries
* Understanding which query structure to use and when







