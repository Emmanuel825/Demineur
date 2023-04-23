/*
 * app.h
 *
 *  Created on: 14 nov. 2022
 *      Author: emmanuel
 */

#define APP_WINDOW_WIDTH		1000
#define APP_WINDOW_HEIGHT		1000
#define APP_DEMIN_NB_COL		10
#define APP_DEMIN_NB_ROW		10
#define APP_DEMIN_SCENE_PADDING	((APP_WINDOW_HEIGHT*10)/100)
#define APP_DEMIN_CELL_SPACE	10
#define APP_DEMIN_CELL_WIDTH	(((APP_WINDOW_WIDTH*80)/100)/APP_DEMIN_NB_COL)-APP_DEMIN_CELL_SPACE
#define APP_DEMIN_CELL_HEIGHT	(((APP_WINDOW_HEIGHT*80)/100)/APP_DEMIN_NB_ROW)-APP_DEMIN_CELL_SPACE


typedef struct s_app t_app;

t_app *AppNew(char*title, int width, int height, int backgroundColor);
int AppRun(t_app *pApp);
void AppDel(t_app *pApp);
