#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"

#define BEEP PEout(0)

void BEEP_Init(void);
void BEEP_ON(void);
void BEEP_OFF(void);
void BUZZER_Play(u16 frq);
void MUSIC_Play(u8 *music,u8 *time,u16 length);
void MUSIC1_Play(void);
#endif
