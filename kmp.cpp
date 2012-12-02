#include <iostream>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

using namespace std;

void computeLPSArray(char *pat, int M, int *lps, int *mismatch);
int max_mismatch = 3; 

void KMPSearch(char *pat, char *txt)
{
    int M = strlen(pat);
    int N = strlen(txt);
 
    // create lps[] that will hold the longest prefix suffix values for pattern
    int *lps = (int *)malloc(sizeof(int)*M);
    int *mismatch = (int *)malloc(sizeof(int)*M);
    int j  = 0;  // index for pat[]
 
    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps, mismatch);
    
    int i = 0;  // index for txt[]
    int m = 0;
    while(i < N)
    {
        m += (txt[i] != pat[j]);
        cout<<i<<" "<<j<<" "<<m<<" "<<txt+i-j<<" "<<pat<<endl;
        if(m<max_mismatch)
        {
            j++;
            i++;
            
            if (j == M || i == N)
            {
                printf("Found pattern at index %d \n", i-j);
                j = lps[j-1];
                break;
            }
        } else {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if(j != 0) {
                j = lps[j-1];
                m = mismatch[j-1];
                cout<<"reset "<<m<<endl;
            } else {
                i = i+1;
                m = 0;
            }
        }
    }
    free(lps); // to avoid memory leak
    return;
}

void computeLPSArray(char *pat, int M, int *lps, int *mismatch)
{
    mismatch[0] = 0;
    int len = 0;  // lenght of the previous longest prefix suffix
    int i;
 
    lps[0] = 0; // lps[0] is always 0
    i = 1;
 
    // the loop calculates lps[i] for i = 1 to M-1
    int m = 0;
    while(i < M)
    {
       m = mismatch[len] + (pat[i] != pat[len]);
       if(i==3) {
        cout<<i<<" "<<len<<endl;
       }
       //cout<<m<<endl;
       if(m<max_mismatch)
       {
         len++;
         lps[i] = len;
         mismatch[i] = m;
         i++;
       }
       else // (pat[i] != pat[len])
       {
         if( len != 0 )
         {
            len = len--;
         }
         else // if (len == 0)
         {
            lps[i] = 0;
            i++;
         }
       }
    }
    for(int i=0; i<M; i++) {
        cout<<lps[i]<<" ";
    }
    cout<<endl;
    for(int i=0; i<M; i++) {
        cout<<mismatch[i]<<" ";
    }
    cout<<endl;
    cout<<mismatch[13]<<endl;
    return;
}
 
// Driver program to test above function
int main()
{
   char *txt = "ABABDABACDABACBABAB";
   char *pat = "ABABA";
   KMPSearch(pat, txt);
   return 0;
}

