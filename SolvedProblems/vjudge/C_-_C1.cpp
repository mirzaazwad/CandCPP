#include <bits/stdc++.h>

#define fastio                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL)

#define endl "\n"
#define mx 3000007
#define ll long long
using namespace std;

ll arr[mx];
ll tree[mx * 3];
void init(int node, int b, int e)
{
    if (b == e) {
        tree[node] = arr[b];
        return;
    }
    int Left = node * 2;
    int Right = node * 2 + 1;
    int mid = (b + e) / 2;
    init(Left, b, mid);
    init(Right, mid + 1, e);
    tree[node] = tree[Left] * tree[Right];
}
ll query(int node, int b, int e, int i, int j)
{
    if (i > e || j < b)
        return 0; //বাইরে চলে গিয়েছে
    if (b >= i && e <= j)
        return tree[node]; //রিলেভেন্ট সেগমেন্ট
    int Left = node * 2; //আরো ভাঙতে হবে
    int Right = node * 2 + 1;
    int mid = (b + e) / 2;
    int p1 = query(Left, b, mid, i, j);
    int p2 = query(Right, mid + 1, e, i, j);
    return p1 * p2; //বাম এবং ডান পাশের যোগফল
}

void update(int node, int b, int e, int i, ll newvalue)
{
    if (i > e || i < b)
        return; //বাইরে চলে গিয়েছে
    if (b >= i && e <= i) { //রিলেভেন্ট সেগমেন্ট
        tree[node] = tree[node]*newvalue;
        return;
    }
    int Left = node * 2; //আরো ভাঙতে হবে
    int Right = node * 2 + 1;
    int mid = (b + e) / 2;
    update(Left, b, mid, i, newvalue);
    update(Right, mid + 1, e, i, newvalue);
    tree[node] = tree[Left] * tree[Right];
}

void solve(){
  int n,q;
  cin>>n>>q;
  int a[n+1];
  for(int i=1;i<=n;i++){
    cin>>a[i];
    arr[i]=1;
  }
  init(1,1,n);
  while(q--){
    int l,r;
    ll m;
    cin>>l>>r>>m;
    update(1,1,n,l,r);
  }
  for(int i=1;i<=n;i++){
    cout<<arr[i]<<" ";
  }
  cout<<endl;

}



int main()
{
    fastio;
    int t;
    cin >> t;
    
    while(t--)
    {
       solve();
    }
    return 0;
}