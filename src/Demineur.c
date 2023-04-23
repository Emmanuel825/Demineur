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
#include "app.h"
#define mRGBA(r,g,b,a)	((uint32_t)r<<24|(uint32_t)g<<16|(uint32_t)b<<8|(uint32_t)a)
int main(int argc, char** argv) {
	t_app *pApp=AppNew("test", APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT, mRGBA(0,0,0,0));
	AppRun(pApp);
	AppDel(pApp);
	return EXIT_SUCCESS;
}

