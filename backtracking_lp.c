#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//DEBUG
#define DEBUG 0

//Run time
clock_t start, end;

//Structures
//Listeners positions on the map
//coordinates x,y
typedef struct listenersPlace{
    int x,y;
    int covered;
}p;
//Possible positions for antennas
//coordinates x,y
typedef struct antennasPlace{
    int x,y;
}n;
//Types of antennas
//r-radius | c-cost
typedef struct typesAntennas{
    int r,c;
}t;

//Inicialize lists
//Listeners
int np;
p *pArray;
//Antennas
int nn;
n *nArray;
//Types
int nt;
t *tArray;

//Print Structs
//Listeners
void printP(p pArray){
    printf("\tListener X: %d Y: %d Covered: %d \n", pArray.x, pArray.y, pArray.covered);

}
//Antennas
void printN(n nArray){
    printf("\tPosition X: %d Y: %d\n", nArray.x, nArray.y);

}
//Types
void printT(t tArray){
    printf("\tAntenna R: %d C: %d\n", tArray.r, tArray.c);
}

//Print Arrays of structs
//Listeners
void printParr(p *pArray, int len){
    printf("Positions of the listeners:\n");
    int i;
    for(i = 0; i < len; i++){
        printP(pArray[i]);
    }
}
//Antennas
void printNarr(n *nArray, int len){
    printf("Positions of the antennas:\n");
    int i;
    for(i = 0; i < len; i++){
        printN(nArray[i]);
    }
}
//Types
void printTarr(t *tArray, int len){
    printf("Types of antennas:\n");
    int i;
    for(i = 0; i < len; i++){
        printT(tArray[i]);
    }
}

//Comparison function for quick sort
//First in order of x then y for listenners
int cmpfuncList(const void * a, const void * b) {
    p *tA = (p*)a;
    p *tB = (p*)b;
    if(((int)tA->x - (int)tB->x) == 0){
        return (int)tA->y - (int)tB->y;
    }
    return ((int)tA->x - (int)tB->x);
}
//First in order of x then y for antennas
int cmpfuncAnt(const void * a, const void * b) {
    n *tA = (n*)a;
    n *tB = (n*)b;
    if(((int)tA->x - (int)tB->x) == 0){
        return (int)tA->y - (int)tB->y;
    }
    return ((int)tA->x - (int)tB->x);
}
//Cost for types of antennas
int cmpfuncTypes(const void * a, const void * b) {
    t *tA = (t*)a;
    t *tB = (t*)b;
    return ((int)tA->c - (int)tB->c);
}

//Get problem info
void getData(){
    int cost = 0;
    int i, j;

    //Get info on listeners
    scanf("%d", &np);
    pArray = (p*)malloc(np * sizeof(p));
    for(i = 0; i < np; i++){
        scanf("%d %d", &pArray[i].x, &pArray[i].y);
        pArray[i].covered = 0;
    }

    //Get info on antennas
    //Positions
    scanf("%d", &nn);
    nArray = (n*)malloc(nn * sizeof(n));
    for(i = 0; i < nn; i++){
        scanf("%d %d", &nArray[i].x, &nArray[i].y);
    }
    //Types
    scanf("%d", &nt);
    tArray = (t*)malloc(nt * sizeof(t));
    for(i = 0; i < nt; i++){
        scanf("%d %d", &tArray[i].r, &tArray[i].c);
    }
}

//Cost is the var to be optimized
int cost = 0;

//Check covered listenners
int covered = 0;

//Main algorithm
int minimizeCost(){
    //Iterators
    int i,j;

    //Check if the solution is possible
    int covered = 0;
    for(i = 0; i < nn; i++){
        for(j = 0; j < np; j++){
            if(((pow(nArray[i].x - pArray[j].x, 2) + pow(nArray[i].y - pArray[j].y, 2)) < pow(tArray[nt-1].r, 2)) && pArray[j].covered == 0) {
                covered++;
                pArray[j].covered = 1;
            }
        }
    }
    if(covered < np){
        return -1;
    }

    //Reset covered values
    for(i = 0; i < np; i++){
        pArray[i].covered = 0;
    }

     //Start with the cheapest antennas
    int type = 0;
    covered = 0;
    while(type < nt){
        //For each possible location
        for(i = 0; i < nn; i++){
            //Check how many listeners are covered by the cheapest antennas
            for(j = 0; j < np; j++){
                //Do the math only if a listener isn't already covered
                if(pArray[j].covered == 0){
                
                    if(DEBUG == 1){
                        printf("\n\n ----- DEBUG ----- 	\n\n");
                        printP(pArray[j]);
                        printN(nArray[i]);
                        printT(tArray[type]);
                        printf("\tCost: %d\n", cost);
                        printf("\n\n\n\n");
                    }

                    if(pArray[j].x <= (nArray[i].x + tArray[type].r) || pArray[j].x <= (nArray[i].x - tArray[type].r) || pArray[j].x <= (nArray[i].y + tArray[type].r) || pArray[j].x <= (nArray[i].y - tArray[type].r)){
                        covered++;
                        pArray[j].covered = 1;
                        //If it is it means we can use it and its optimal because its the cheapest
                        cost += tArray[type].c;
                    }
                }
            }
            //Shortcut in case an antenna with the current location covers all the listeners
            if(covered == np){
                return cost;
            }
            covered = 0;
        }
        type++;
    }
    return cost;
}

int main() {
    //Get initial info for the problem
    getData();

    //Measure runtime
    start = clock();

    //Sort lists
    qsort(pArray, np, sizeof(p), cmpfuncList);
    qsort(nArray, nn, sizeof(n), cmpfuncAnt);
    qsort(tArray, nt, sizeof(t), cmpfuncTypes);

    if(DEBUG == 1){
        printParr(pArray, np);
        printNarr(nArray, nn);
        printTarr(tArray, nt);
    }

    //Final cost
    int finalCost = minimizeCost();
    if(finalCost < 0){
        printf("no solution\n");
    }else{
        printf("%d\n", finalCost);
    }
    
    //Measure runtime
    end = clock();
    if(DEBUG == 1){
        printf("\n---Runtime: %f\n", ((double)(end - start)/CLOCKS_PER_SEC));
    }
    return 0;
}
