#ifndef timer2_H
#define timer2_H

//È±Ê¡ÖÜÆÚ
#define TIMER2_DEFAULT_MS	9

void TIM2_init(void);
void start_tim2(u8 on);
u32 timer2_get_clock(void);
u32 timer2_get_shake(void);
u8 timer2_check_timer(u32 *timer, u32 interval)	;
#endif
