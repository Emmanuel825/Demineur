/*
 * demin.c
 *
 *  Created on: 15 nov. 2022
 *      Author: emmanuel
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "demin.h"

struct s_demin{
	int nbRow;
	int nbCol;
	int nbMines;
	int *scene;
};
int _DeminDiscover(int *scene, int nbRow, int nbCol, int x, int y);
t_demin *DeminNew(int nbRow, int nbCol, int minesPercent){
	srand((unsigned int)time(NULL));
	int k, n, nbMinesSave;
	t_demin *pDemin=(t_demin*)malloc(sizeof(t_demin));
	pDemin->scene=(int*)malloc(sizeof(int)*(nbCol*nbRow));
	pDemin->nbCol=nbCol;
	pDemin->nbRow=nbRow;
	pDemin->nbMines=nbMinesSave=(nbRow*nbCol*minesPercent)/100;
	for(k=0;k<nbRow;k++){
		for(n=0;n<nbCol;n++){
			pDemin->scene[k*nbCol+n]=0;
		}
	}
	do{
		do{
			k=rand()%nbRow;
			n=rand()%nbCol;
		}while(pDemin->scene[k*nbCol+n]==MINE_VALUE);
		pDemin->scene[k*nbCol+n]=MINE_VALUE;
		for(int i=-1;i<=1;i++){
			for(int l=-1;l<=1;l++){
				if((k+i>=0)&&(k+i<nbRow)&&(n+l>=0)&&(n+l<nbCol)&&(pDemin->scene[(k+i)*nbCol+(n+l)])!=MINE_VALUE){
					pDemin->scene[(k+i)*nbCol+(n+l)]++;
				}
			}
		}
	}while(--pDemin->nbMines);
	pDemin->nbMines=nbMinesSave;
	return pDemin;
}

void DeminDel(t_demin *pDemin){
	free(pDemin->scene);
	free(pDemin);
}

int _DeminDiscover(int *scene, int nbRow, int nbCol, int x, int y){
	int k, l, nbDiscoverCells=0;
	if(mIsCellPlayed(scene[x*nbCol+y]))return 0;
	mSetPlayed(scene[x*nbCol+y]);
	nbDiscoverCells++;
	if(mCellValue(scene[x*nbCol+y])==MINE_VALUE)return -1;
	if(mCellValue(scene[x*nbCol+y])!=0)return 1;
	for(k=-1;k<=1;k++){
		for(l=-1;l<=1;l++){
			if((k+x>=0)&&(k+x<nbRow)&&(l+y>=0)&&(l+y<nbCol)&&(scene[(k+x)*nbCol+(l+y)]!=MINE_VALUE)){
				nbDiscoverCells+=_DeminDiscover(scene, nbRow, nbCol, k+x, l+y);
			}
		}
	}
	return nbDiscoverCells;
}

int DeminRun(t_demin *pDemin, int x, int y){
	int res;
	//while((quit<((pDemin->nbCol*pDemin->nbRow)-pDemin->nbMines))&&res!=-1){
		res=_DeminDiscover(pDemin->scene, pDemin->nbRow, pDemin->nbCol, x,y);
		return res;
	//}
	/*if(res==-1){
		printf("perdu !");
	}
	else {
		printf("gagne !");
	}*/
}

int DeminGetCell(t_demin *pDemin, int cell){
	return pDemin->scene[cell];
}

int DeminGetNbMines(t_demin *pDemin){
	return (pDemin->nbCol*pDemin->nbRow)-pDemin->nbMines;
}
