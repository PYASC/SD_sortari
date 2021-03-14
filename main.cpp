#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <fstream>
#include <ctime>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;
ifstream f("nr.in");
ofstream g("timers.out");
bool test_sortare(vector <int> &v) {

    for(long long int i=0; i<v.size()-1; ++i)
        if(v[i]>v[i+1])
            return false;

    return true;
}

vector <int> nrs, nrss;
long long int T, nr_rep, N, NMax;

void sortq(vector < int> &v) {

    if(v.size()<=1) {
        return ;
    }


    long long int n=v.size()-1, i, j;
    long long int x1=v[0], x2=v[n/2], x3=v[n];
    if(x1<x2) {
        x1^=x2;
        x2^=x1;
        x1^=x2;
    }
    if(x1<x3) {
        x1^=x3;
        x3^=x1;
        x1^=x3;
    }
    if(x2<x3) {
        x2^=x3;
        x3^=x2;
        x2^=x3;
    }
    //x1 = max ; x3= min -> alegem x2 ca pivot:
    vector <int> v1, v2;
    long long int c=0;
    for(i=0; i<=n; ++i) {
        if(v[i]==x2)
            ++c;
        else if(v[i]<x2)
            v1.push_back(v[i]);
        else
            v2.push_back(v[i]);
    }
    sortq(v1);
    sortq(v2);

    for(j=0; j<v1.size(); ++j)
        v[j]=v1[j];
    for(i=0; i<c ; ++i)
        v[j+i]=x2;
    j+=i;
    for(i=0; i<v2.size(); ++i)
        v[j+i]=v2[i];
    v1.clear();
    v2.clear();
    return;
}


void mergesort(vector <int> &v, long long int l, long long int r) {
    if(r-l>1) {
        long long int mij = (l+r)/2;
        mergesort(v, l, mij);
        mergesort(v, mij + 1, r);

        long long int in1=l,in2=mij + 1;
        vector <int> temp;
        while( in1<= mij && in2 <= r) {
            if(v[in1]<v[in2]) {
                temp.push_back(v[in1]);
                ++in1;
            } else {
                temp.push_back(v[in2]);
                ++in2;
            }
        }
        while( in1<=mij) {
            temp.push_back(v[in1]);
            ++in1;
        }
        while( in2<=r) {
            temp.push_back(v[in2]);
            ++in2;
        }
        for(in1=0; in1<temp.size(); ++in1)
            v[l+in1] = temp[in1];
        temp.clear();
    } else {
        if(v[l]>v[r]) {
            v[l]^=v[r];
            v[r]^=v[l];
            v[l]^=v[r];
        }
    }

}


void radix_2p(vector <int> &v, int base, int nrc) {
    long long int b=base-1;
    long long int i,j,k,c, p=1;
    vector < int> buckets[base];
    for(i=0; i<nrc; ++i) {
        c=0;
        for(j=0; j<v.size(); ++j) {
            buckets[ ((long long int)(v[j]/p) ) & b].push_back(v[j]);
        }
        for( j=0; j<base; ++j) {
            for(k=0; k<buckets[j].size(); ++k) {
                v[c]=buckets[j][k];
                ++c;
            }
            buckets[j].clear();
        }
        p*=base;
    }

}

void bubblesort(vector < int> &v) {
    bool k=false;
    long long int i,j;
    while(!k) {
        k=true;
        for(i=0; i<v.size() -1 ; ++i) {
            if(v[i]>v[i+1]) {
                k=false;
                v[i]^=v[i+1];
                v[i+1]^=v[i];
                v[i]^=v[i+1];
            }
        }
    }

}

void countsort(vector <int> &v) {
    vector <int> cs(NMax+3,0);
    long long int i;
    for(i=0; i<v.size(); ++i) {
        ++cs[v[i]];
    }
    long long int c=0;
    for(i=0; i<=NMax; ++i)
        while(cs[i]) {
            v[c]=i;
            ++c;
            --cs[i];
        }
    cs.clear();
}


void gen_nrs() {
    long long int i,x;
    // Genereaza N numere cu valori de pana la 2^30
    for(i=0; i<N; ++i) {
        x = ( (rand()&32767) <<15 + (rand()&32767)) % NMax;
        nrs.push_back(x);
        nrss.push_back(x);
    }

}
void afisare(vector <int> &v) {

    for(long long int i=0; i<v.size(); ++i) {
        cout<<v[i]<<" ";
    }
}


int main() {

    f>>T>>nr_rep;
    long long int i,j, k;
    for(i=1; i<=T; ++i) {
        g<<"Test "<<i<<'\n';
        f>>N>>NMax;
        if(N>1000000 || NMax > 1000000)
            g<<"I have my limits\n";
        else {
            for(j=1; j<=nr_rep; ++j) {
                gen_nrs();

                //quick sort
                auto start = high_resolution_clock::now();
                sortq(nrs);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(stop - start);
                g << "Quick sort: " << duration.count() << " milliseconds" << '\t';

                //merge sort
                for(k=0; k<nrss.size(); ++k)
                    nrs[k]=nrss[k];
                start = high_resolution_clock::now();
                mergesort(nrs, 0, nrs.size()-1);
                stop = high_resolution_clock::now();
                duration = duration_cast<milliseconds>(stop - start);
                g << "Merge sort: " << duration.count() << " milliseconds" << '\t';

                // bubble sort
                if(N>=1000000 && NMax!=1)
                    g<<"Bubble sort: Too many numbers; can't wait that long \t";
                else {
                    for(k=0; k<nrss.size(); ++k)
                        nrs[k]=nrss[k];
                    start = high_resolution_clock::now();
                    bubblesort(nrs);
                    stop = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(stop - start);
                    g << "Bubble sort: " << duration.count() << " milliseconds" << '\t';
                }

                // radidx base16
                for(k=0; k<nrss.size(); ++k)
                    nrs[k]=nrss[k];
                start = high_resolution_clock::now();
                radix_2p(nrs, 16, (int) ceil(log2(NMax)/4) );
                stop = high_resolution_clock::now();
                duration = duration_cast<milliseconds>(stop - start);
                g << "Radix_16 sort: " << duration.count() << " milliseconds" << '\t';

                // radix base8
                for(k=0; k<nrss.size(); ++k)
                    nrs[k]=nrss[k];
                start = high_resolution_clock::now();
                radix_2p(nrs, 8, (int) ceil(log2(NMax)/3) );
                stop = high_resolution_clock::now();
                duration = duration_cast<milliseconds>(stop - start);
                g << "Radix_8 sort: " << duration.count() << " milliseconds" << '\t';

                //count sort
                if(NMax>=1000000)
                    g<<"Count sort: Don't have enough memory\t";
                else {
                    for(k=0; k<nrss.size(); ++k)
                        nrs[k]=nrss[k];

                    start = high_resolution_clock::now();
                    countsort(nrs);
                    stop = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(stop - start);
                    g << "Count sort: " << duration.count() << " milliseconds" << '\t';
                }


                start = high_resolution_clock::now();
                sort(nrss.begin(), nrss.end());
                stop = high_resolution_clock::now();
                duration = duration_cast<milliseconds>(stop - start);
                g << "Introsort: " << duration.count() << " milliseconds" << '\t';

                g << '\n';
                nrs.clear();
                nrss.clear();
            }
        }
    }
    //https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
    //mt19937_64 qwe;
    //cout<<RAND_MAX;
    //RAND_MAX = 32767 in my case -> 2^15 -1
    return 0;
}




