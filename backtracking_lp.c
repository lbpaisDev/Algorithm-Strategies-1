#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DEBUG 0

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

//Print Structs
void printP(p pArray){
    printf("\tListener X: %d Y: %d Covered: %d \n", pArray.x, pArray.y, pArray.covered);

}
void printN(n nArray){
    printf("\tPosition X: %d Y: %d\n", nArray.x, nArray.y);

}
void printT(t tArray){
    printf("\tAntenna R: %d C: %d\n", tArray.r, tArray.c);
}

//Print Arrays of structs
void printParr(p *pArray, int len){
    printf("Positions of the listeners:\n");
    int i;
    for(i = 0; i < len; i++){
        printP(pArray[i]);
    }
}
void printNarr(n *nArray, int len){
    printf("Positions of the antennas:\n");
    int i;
    for(i = 0; i < len; i++){
        printN(nArray[i]);
    }
}
void printTarr(t *tArray, int len){
    printf("Types of antennas:\n");
    int i;
    for(i = 0; i < len; i++){
        printT(tArray[i]);
    }
}

//Comparison function for quick sort
int cmpfunc (const void * a, const void * b) {
    t *tA = (t*)a;
    t *tB = (t*)b;
    return ((int)tA->c - (int)tB->c);
}

//Main function
int minimizeCost(){
    int cost = 0;
    int i, j;

    //Get info on listeners
    int np;
    scanf("%d", &np);
    p *pArray = (p*)malloc(np * sizeof(p));
    for(i = 0; i < np; i++){
        scanf("%d %d", &pArray[i].x, &pArray[i].y);
        pArray[i].covered = 0;
    }

    //Get info on antennas
    //Positions
    int nn;
    scanf("%d", &nn);
    n *nArray = (n*)malloc(nn * sizeof(n));
    for(i = 0; i < nn; i++){
        scanf("%d %d", &nArray[i].x, &nArray[i].y);
    }
    //Types
    int nt;
    scanf("%d", &nt);
    t *tArray = (t*)malloc(nt * sizeof(t));
    for(i = 0; i < nt; i++){
        scanf("%d %d", &tArray[i].r, &tArray[i].c);
    }

    //Sort types by cost
    qsort(tArray, nt, sizeof(t), cmpfunc);

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

    if(DEBUG == 1){
        printf("\n\n ----- DEBUG ----- 	\n\n");
        printParr(pArray, np);
        printNarr(nArray, nn);
        printTarr(tArray, nt);
        printf("\n\n\n\n");
    }

    //Main algorithm
    //Start with the cheapest antennas
    int type = 0;
    covered = 0;
    while(type < nt){
        //Pre calculate the radius of each antenna
        double radius = pow(tArray[type].r, 2);
        //For each possible location
        for(i = 0; i < nn; i++){
            //Check how many listeners are covered by the cheapest antennas
            for(j = 0; j < np; j++){
                //Do the math only if a listener isn't already covered
                if(pArray[j].covered == 0){
                    //Determine if current listener is within the radius of the antenna with a the current possible location as it center
                    double computed = pow((nArray[i].x - pArray[j].x), 2) + pow((nArray[i].y - pArray[j].y), 2);

                    if(DEBUG == 1){
                        printf("\n\n ----- DEBUG ----- 	\n\n");
                        printP(pArray[j]);
                        printN(nArray[i]);
                        printT(tArray[type]);
                        printf("\tComputed: %f Radius: %f\n", computed, radius);
                        printf("\tCost: %d\n", cost);
                        printf("\n\n\n\n");
                    }

                    if(computed <= radius){
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
    int finalCost = minimizeCost();
    if(finalCost < 0){
        printf("no solution\n");
    }else{
        printf("%d\n", finalCost);
    }
    return 0;
}
