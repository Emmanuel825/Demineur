/*
 * demin.h
 *
 *  Created on: 15 nov. 2022
 *      Author: emmanuel
 */

#define MINE_VALUE		9
#define GAME_OVER_VAL	-1

#define mBitMask(f,m)		((f)&(m))
#define mBitSet(f,m)		((f)|=(m))
#define mBitClear(f,m)		(~(f)&=(m))
#define mBitToggle(f,m)		((f)^=(m))
#define mIsBitSet(f,m)		(((f)&(m))==(m))
#define mIsBitClear(f,m)	((~(f)&(m))==(m))

#define PLAYED_MASK			0x80000000
#define VALUE_MASK			0x0000000F

#define mIsCellPlayed(c)	mIsBitSet(c, PLAYED_MASK)
#define mCellValue(c)		mBitMask(c, VALUE_MASK)
#define mSetPlayed(c)		mBitSet(c, PLAYED_MASK)

typedef struct s_demin t_demin;

t_demin *DeminNew(int nbRow, int nbCol, int minesPercent);
int DeminRun(t_demin *pDemin,int x, int y);
void DeminDel(t_demin *pDemin);
int DeminGetCell(t_demin *pDemin, int cell);
int DeminGetNbMines(t_demin *pDemin);
