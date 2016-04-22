#include <iostream>
#include <cstdio>

using namespace std;

int main()
{

    char pbch[24] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};

    FILE *I = fopen("pbch","w");
    unsigned int r = fwrite(pbch, sizeof(char), 24, I);
    fclose(I);

    char dci[28] = {1,1,1,0,0,1,0,1,0,0,1,1,0,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1};
    FILE *I1 = fopen("dci","w");

    r = fwrite(dci, sizeof(char), 28, I1);
    fclose(I1);

    cout << "Hello World!" << endl;
    return 0;
}

