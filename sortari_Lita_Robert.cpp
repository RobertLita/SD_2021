#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;
ifstream f("numbers_in.txt");
ofstream g("numbers_out.txt");
#define ull unsigned long long
const char sortari[7][40]={"Bubble Sort","Merge Sort","Radix Sort cu baza 16","Bucket Sort","Quick Sort cu pivot random","Quick Sort cu pivot <mediana din 3>","STL Sort"};

void generator(int &n,ull &mx,vector<ull> &v)
{
    ull x;
    v.clear();
    mx=0;
    n=rand()%1000000+100;
    for(int i=0;i<n;i++)
    {
        x=rand()%10000+10000;
        v.push_back(x);
        if(x>mx)mx=x;
    }
}

bool verifica_sortarea(vector<ull> v)
{
    for(int i=0;i<v.size()-1;i++)
        if(v[i]>v[i+1])return 0;
    return 1;
}

bool bubble_sort(vector<ull> &v)
{
    if(v.size()>18000)return 0;
    bool ok=0;
    do
    {
        ok=1;
        for(int i=0;i<v.size()-1;i++)
            if(v[i]>v[i+1])
            {
                swap(v[i],v[i+1]);
                ok=0;
            }
    }
    while(ok==0);
    return 1;
}

bool merge_sort(vector<ull> &v,int st,int dr)
{
    if(v.size()>2400000)return 0;
    if(st<dr)
    {
        int m=(st+dr)/2;
        merge_sort(v,st,m);
        merge_sort(v,m+1,dr);
        vector<ull> c;
        int i=st,j=m+1;
        while(i<=m && j<=dr)
        {
            if(v[i]<v[j])
            {
                c.push_back(v[i]);
                i++;
            }
            else if(v[j]<=v[i])
            {
                c.push_back(v[j]);
                j++;
            }
        }
        for(;i<=m;i++)
            c.push_back(v[i]);
        for(;j<=dr;j++)
            c.push_back(v[j]);
        for(i=st,j=0;i<=dr;i++,j++)
            v[i]=c[j];
    }
    return 1;
}

bool sortare_stl(vector<ull> &v)
{
    if(v.size()>10000000)return 0;
    sort(v.begin(),v.end());
    return 1;
}

int alege_pivot(int st,int dr,int m,vector<ull> &v)
{
    if(m==0)
    {
        return st+rand()%(dr-st+1);
    }
    else
    {
        int mij=(st+dr)/2;
        if(v[dr] < v[st])
            swap(v[st], v[dr]);
        if (v[mij] < v[st])
            swap(v[mij],v[st]);
        if (v[dr] < v[mij])
            swap(v[dr],v[mij]);
        return mij;
    }
}

bool quick_sort(int st,int dr,vector<ull> &v,int mod)
{
    if(v.size()>13500000)return 0;
    if(st>dr)return 1;
    int i,j;
    ull pivot;
    i=st;j=dr;
    pivot=v[alege_pivot(st,dr,mod,v)];
    do
    {
        while(v[i]<pivot)i++;
        while(v[j]>pivot)j--;
        if(i<=j)
        {
            swap(v[i],v[j]);
            i++;
            j--;
        }
    }while(i<=j);
    if(st<j)quick_sort(st,j,v,mod);
    if(i<dr)quick_sort(i,dr,v,mod);
}

void count_sort(vector<ull> &v,int shift)
{
    vector<ull> c(v.size(),0);
    int vf[16]={0};
    for(int i=0;i<v.size();i++)
        vf[(v[i]>>shift)&15]++;
    for(int i=1;i<16;i++)
        vf[i]+=vf[i-1];
    for(int i=v.size()-1;i>=0;i--)
    {
        c[vf[(v[i]>>shift)&15]-1]=v[i];
        vf[(v[i]>>shift)&15]--;
    }
    for(int i=0;i<v.size();i++)
        v[i]=c[i];
}

bool radix_sort(vector<ull> &v,ull mx)
{
    if(v.size()>40000000)return 0;
    for (int shift = 0; (mx >> shift) > 0; shift += 4)
            count_sort(v, shift);
    return 1;
}

bool bucket_sort(vector<ull> &v,ull mx)
{
    if(v.size()>16000000 || mx>100000000000000)return 0;
    int buckets;
    if(v.size()<1000000)buckets = v.size()/100;
    else buckets=10000;
    vector<ull> b[buckets];
    for(int i=0;i<v.size();i++)
    {
        int bi;
        bi = (1ll*v[i] * buckets / (mx + 1) )% buckets;
        b[bi].push_back(v[i]);
    }
    for (int i = 0; i < buckets; i++)
        sort(b[i].begin(), b[i].end());
    v.clear();
    for (int i = 0; i < buckets; i++)
        for (int j = 0; j < b[i].size(); j++)
            v.push_back(b[i][j]);
    return 1;
}

void masoara_timpul(vector<ull> v,int idsort,ull mx=0)
{
    bool s=1;
    idsort--;
    auto time_start=high_resolution_clock::now();
    if(idsort==0)
    {
        s=bubble_sort(v);
    }
    else if(idsort==1)
    {
        s=merge_sort(v,0,v.size()-1);
    }
    else if(idsort==2)
    {
        s=radix_sort(v,mx);
    }
    else if(idsort==3)
    {
        s=bucket_sort(v,mx);
    }
    else if(idsort==4)
    {
        s=quick_sort(0,v.size()-1,v,s);
    }
    else if(idsort==5)
    {
        s=quick_sort(0,v.size()-1,v,s);
    }
    else if(idsort==6)
    {
        s=sortare_stl(v);
    }
    if(s==1)
    {
        bool ok = verifica_sortarea(v);
        if (ok == 1)
        {
            auto time_end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(time_end - time_start);
            g << "Vectorul a fost sortat corect in " << duration.count() << " milisecunde folosind " << sortari[idsort]<< '\n';
        }
        else
            g << "Vectorul nu a fost sortat corect folosing " << sortari[idsort] << '\n';
    }
    else
        g<<"Vectorul poate fi sortat corect folosind "<< sortari[idsort] <<", dar intr-un timp mediu >2,5 secunde\n";
}
int main()
{
    short int t;
    int n;
    ull mx;
    vector<ull> v;
    srand((unsigned)time(NULL));
    f>>t;
    for(int i=1;i<=t;i++)
    {
        generator(n,mx,v);
        g<<"N="<<n<<" MAX="<<mx<<'\n';
        if(verifica_sortarea(v)==0)
        {
            //1 Bubble Sort
            masoara_timpul(v,1);
            //2 Merge Sort
            masoara_timpul(v,2);
            //3 Radix Sort
            masoara_timpul(v,3,mx);
            //4 Bucket Sort
            masoara_timpul(v,4,mx);
            //5 Quick Sort cu pivot random
            masoara_timpul(v,5);
            //6 Quick Sort cu pivot mediana din 3
            masoara_timpul(v,6);
            //7 Sortare din STL
            masoara_timpul(v,7);
            g<<'\n';
        }
        else
        {
            g<<"Vectorul este deja soratat\n";
        }
    }
    return 0;
}
