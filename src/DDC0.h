#ifndef _DDC0_H_
#define _DDC0_H_

#include "global.h"

void Audio_OnStart(void);
void Audio_OnStop(void);
void Audio_OnPause(void);
void Audio_OnResume(void);
void Audio_ResetTrack(void);
void Audio_PlayTrack(s32 arg0);
void Audio_SwitchTrack(s32 arg0);
void Audio_StopTrack(s32 arg0);
s32 Audio_WaitDone(s32 arg0);
void Audio_StopAll(void);
void Audio_Disable(void);
void Audio_Enable(void);
void AudioThread_Main(void*);
void Audio_StartThread(void);


#endif // _DDC0_H_
