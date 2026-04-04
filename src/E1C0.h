#ifndef _E1C0_H_
#define _E1C0_H_

#include "global.h"

typedef struct GBSecondaryCtx {
  /* 0x0000 */ OSThread thread;
  /* 0x01B0 */ char unk01B0[0x2030];
} GBSecondaryCtx; // size = 0x21E0

void GBTower_Start(GBTowerState* arg0);
OSMesg* GBTower_WaitResult(void);
void GBTower_Teardown(GBTowerState* arg0);

#endif // _E1C0_H_
