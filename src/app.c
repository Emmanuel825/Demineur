/*
 * app.c
 *
 *  Created on: 14 nov. 2022
 *      Author: emmanuel
 */


/*
 ============================================================================
 Name        : ttf.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "demin.h"
#define mRGBA_r(rgba)	((Uint8)(rgba>>24))
#define mRGBA_g(rgba)	((Uint8)(rgba>>16))
#define mRGBA_b(rgba)	((Uint8)(rgba>>8))
#define mRGBA_a(rgba)	((Uint8)(rgba>>0))
struct s_app{
	SDL_Window		*	pWindow;
	SDL_Renderer	*	pRenderer;
	SDL_Surface		*	message;
	SDL_Texture		*	texture;
	SDL_Rect			rect;
	SDL_Color			color;
	SDL_Color			backgroundColor;
	TTF_Font		*	font;
	SDL_Event			event;

	/***************Demin Part******************/
	t_demin *pDemin;
	SDL_Rect cellsRect;
	SDL_Surface *cellsRest;

};

void _AppDraw(t_app* pApp, int cellulesRestantes);
int _AppIsMouseOnCell(t_app *pApp, int x, int y, int *clicRow, int *clicCol);

t_app *AppNew(char*title, int width, int height, int backgroundColor){
	t_app *pApp=(t_app*)malloc(sizeof(t_app));
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		fprintf(stderr, "failed to init SDL : %s\n", SDL_GetError());
		return pApp;
	}
	if(TTF_Init()<0){
		fprintf(stderr,"failed to init TTF : %s\n", TTF_GetError());
		return pApp;
	}
	pApp->pWindow=SDL_CreateWindow(
				title,
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				width,
				height,
				SDL_WINDOW_SHOWN
				);
	if(pApp->pWindow==NULL){
		fprintf(stderr,"failed to create window : %s\n", SDL_GetError());
	}
	pApp->pRenderer=SDL_CreateRenderer(pApp->pWindow, -1, SDL_RENDERER_ACCELERATED);
	if(pApp->pRenderer==NULL){
		fprintf(stderr,"failed to create accelerated renderer : %s\n", SDL_GetError());
		pApp->pRenderer=SDL_CreateRenderer(pApp->pWindow, -1, SDL_RENDERER_SOFTWARE);
		if(pApp->pRenderer==NULL){
			fprintf(stderr,"failed to create accelerated renderer : %s\n", SDL_GetError());
			return pApp;
		}
	}
	pApp->font=TTF_OpenFont("arial.ttf", 80);
	if(pApp->font==NULL){
		fprintf(stderr,"failed to open font : %s\n", TTF_GetError());
		return pApp;
	}
	pApp->pDemin=DeminNew(APP_DEMIN_NB_ROW, APP_DEMIN_NB_COL, 10);
	pApp->backgroundColor=(SDL_Color){
		mRGBA_r(backgroundColor),
		mRGBA_g(backgroundColor),
		mRGBA_b(backgroundColor),
		mRGBA_a(backgroundColor)
	};

	SDL_SetRenderDrawColor(
			pApp->pRenderer,
			pApp->backgroundColor.r,
			pApp->backgroundColor.g,
			pApp->backgroundColor.b,
			pApp->backgroundColor.a
	);
	SDL_RenderClear(pApp->pRenderer);
	SDL_RenderPresent(pApp->pRenderer);
	pApp->rect=(SDL_Rect){APP_DEMIN_SCENE_PADDING,APP_DEMIN_SCENE_PADDING, APP_DEMIN_CELL_WIDTH, APP_DEMIN_CELL_HEIGHT};
	pApp->cellsRect=(SDL_Rect){300,10,400,60};
	pApp->color=(SDL_Color){255,255,255,255};
	return pApp;
}

void AppDel(t_app *pApp){
	if(pApp->pDemin!=NULL)DeminDel(pApp->pDemin);
	if(pApp->texture!=NULL)SDL_DestroyTexture(pApp->texture);
	if(pApp->message!=NULL)SDL_FreeSurface(pApp->message);
	if(pApp->pRenderer!=NULL)SDL_DestroyRenderer(pApp->pRenderer);
	if(pApp->pWindow!=NULL)SDL_DestroyWindow(pApp->pWindow);
	TTF_Quit();
	SDL_Quit();
	free(pApp);
}

int AppRun(t_app *pApp){
	//pApp->message=TTF_RenderText_Blended(pApp->font, "test", pApp->color);
	//pApp->texture=SDL_CreateTextureFromSurface(pApp->pRenderer, pApp->message);
	//SDL_RenderCopy(pApp->pRenderer, pApp->texture, NULL, &pApp->rect);
	char title[30];
	printf("%d", 16/30);
	printf("\n%d\n", APP_DEMIN_CELL_WIDTH);
	int quit=0, clicRow=0, clicCol=0, cellsDiscoverd=0, res=0;
	_AppDraw(pApp, DeminGetNbMines(pApp->pDemin));
	while(!quit){
		if(SDL_PollEvent(&pApp->event)){
			switch(pApp->event.type){
			case SDL_QUIT :
				quit=1;
				break;
			case SDL_KEYDOWN:
				switch(pApp->event.key.keysym.sym){
				case SDLK_ESCAPE:
					pApp->event.type=SDL_QUIT;
					SDL_PushEvent(&pApp->event);
					break;
				}

				break;
			case SDL_MOUSEBUTTONDOWN:
				switch(pApp->event.button.button){
				case SDL_BUTTON_LEFT:
					if(_AppIsMouseOnCell(pApp, pApp->event.motion.x, pApp->event.motion.y, &clicRow, &clicCol)){
						sprintf(title,"x : %d, y : %d, OnCell, %d, %d", pApp->event.motion.x, pApp->event.motion.y, clicRow, clicCol);
						res=DeminRun(pApp->pDemin, clicRow, clicCol);
						switch(res){
							case GAME_OVER_VAL:
								quit=1;
								//printf("perdu !");
								break;
							default :
								cellsDiscoverd+=res;
								break;
						}
						if(cellsDiscoverd==DeminGetNbMines(pApp->pDemin))quit=1;
						_AppDraw(pApp, (DeminGetNbMines(pApp->pDemin)-cellsDiscoverd));
					}
					else sprintf(title,"x : %d, y : %d, OffCell", pApp->event.motion.x, pApp->event.motion.y);
					SDL_SetWindowTitle(pApp->pWindow, title);
					break;
				}
				break;
			}
		}
	}
	if(res==GAME_OVER_VAL)printf("perdu !");
	else printf("gagne !");
	return 0;
}

void _AppDraw(t_app* pApp, int cellulesRestantes){
	int cell;
	char val[8], score[30];
	SDL_SetRenderDrawColor(pApp->pRenderer, 0, 0, 0, 0);
	SDL_RenderClear(pApp->pRenderer);

	SDL_SetRenderDrawColor(pApp->pRenderer, 0, 0, 255, 0);
	//SDL_RenderFillRect(pApp->pRenderer, &pApp->cellsRect);
	for(int k=0;k<APP_DEMIN_NB_ROW;k++){
		for(int n=0;n<APP_DEMIN_NB_COL;n++){
			cell=DeminGetCell(pApp->pDemin, (k*APP_DEMIN_NB_COL+n));
			if(mIsCellPlayed(cell)){
				SDL_RenderDrawRect(pApp->pRenderer, &pApp->rect);
				sprintf(val, "%d", mCellValue(cell));
				pApp->message=TTF_RenderText_Blended(pApp->font, val, pApp->color);
				pApp->texture=SDL_CreateTextureFromSurface(pApp->pRenderer, pApp->message);
				SDL_RenderCopy(pApp->pRenderer, pApp->texture, NULL, &pApp->rect);
			}
			else {
				SDL_RenderFillRect(pApp->pRenderer, &pApp->rect);
			}
			//SDL_RenderClear(pApp->pRenderer);

			pApp->rect.x+=(APP_DEMIN_CELL_WIDTH+APP_DEMIN_CELL_SPACE);
		}

		pApp->rect.x=APP_DEMIN_SCENE_PADDING;
		pApp->rect.y+=(APP_DEMIN_CELL_HEIGHT+APP_DEMIN_CELL_SPACE);
	}
	sprintf(score, "cellules restantes : %d", cellulesRestantes);
	pApp->cellsRest=TTF_RenderText_Blended(pApp->font, score, pApp->color);
	pApp->texture=SDL_CreateTextureFromSurface(pApp->pRenderer, pApp->cellsRest);
	SDL_RenderCopy(pApp->pRenderer, pApp->texture, NULL, &pApp->cellsRect);
	pApp->rect=(SDL_Rect){APP_DEMIN_SCENE_PADDING,APP_DEMIN_SCENE_PADDING, APP_DEMIN_CELL_WIDTH, APP_DEMIN_CELL_HEIGHT};
	SDL_RenderPresent(pApp->pRenderer);


}
int _AppIsMouseOnCell(t_app *pApp, int x, int y, int *clicRow, int *clicCol){
	if((((x-APP_DEMIN_SCENE_PADDING)%(APP_DEMIN_CELL_WIDTH+APP_DEMIN_CELL_SPACE))<=APP_DEMIN_CELL_WIDTH)&&((((y-APP_DEMIN_SCENE_PADDING)%(APP_DEMIN_CELL_HEIGHT+APP_DEMIN_CELL_SPACE))<=APP_DEMIN_CELL_HEIGHT))){
		if(((x<APP_DEMIN_SCENE_PADDING) || (x>(APP_DEMIN_SCENE_PADDING+(APP_DEMIN_NB_COL*(APP_DEMIN_CELL_WIDTH+APP_DEMIN_CELL_SPACE)))))||(((y<APP_DEMIN_SCENE_PADDING) || (y>(APP_DEMIN_SCENE_PADDING+(APP_DEMIN_NB_ROW*(APP_DEMIN_CELL_HEIGHT+APP_DEMIN_CELL_SPACE)))))))return 0;
		else {

			*clicRow=(y-APP_DEMIN_SCENE_PADDING)/(APP_DEMIN_CELL_HEIGHT+APP_DEMIN_CELL_SPACE);
			*clicCol=(x-APP_DEMIN_SCENE_PADDING)/(APP_DEMIN_CELL_WIDTH+APP_DEMIN_CELL_SPACE);
			return 1;
		}
	}
	return 0;
}
