



	EXTERN gp_selfos_cur_task
	
	EXTERN get_next_TCB

	EXPORT OSTest

	EXPORT PendSV_Handler

SCB_ICSR	    EQU     0xE000ED04                             
SCB_SHP14		EQU     0xE000ED22                              
PENDSV_PRI		EQU           0xFF                             
PENDSV_SET		EQU     0x10000000        
PENDSV_CLR		EQU		0x800000

    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
    REQUIRE8
    PRESERVE8

PendSV_Handler

	ldr r0,=SCB_ICSR
	ldr r1,=PENDSV_CLR	;//清除pendsv的触发
	str r1,[r0]

	;bx lr

	mrs r0,psp
	cbnz r0,content_save
	
	mrs r1,control	;//设置psp和msp
	orr r1,r1,#0x02
	msr control,r1
	
content_save

	mrs r0,psp
	cbz r0,content_load


	mrs r0,psp		;//read psp addr



	subs r0,r0,#4
	str r11,[r0]

	subs r0,r0,#4
	str r10,[r0]

	subs r0,r0,#4
	str r9,[r0]

	subs r0,r0,#4
	str r8,[r0]

	subs r0,r0,#4
	str r7,[r0]

	subs r0,r0,#4
	str r6,[r0]

	subs r0,r0,#4
	str r5,[r0]

	subs r0,r0,#4
	str r4,[r0]



	ldr r2,=gp_selfos_cur_task
	ldr r1,[r2]		;//获取到当前任务放栈地址的空间地址

	str r0,[r1]		;//save new stack addr

;/*在中断中,lr代表的不是返回地址.由于调用c函数,在进入子函数后,lr会发生变化,变为当前的位置.即使从子函数退出后,也不会变回来.因此在进入子函数前,lr需要保存一下*/
;//	mov r0,lr
content_load
	push {lr}
	ldr r1,=get_next_TCB
	blx r1
;//	mov lr,r0
	pop {lr}
;//content_load:

	ldr r0,=gp_selfos_cur_task
	ldr r0,[r0]
	ldr r0,[r0]		;//get the cur task stack

	ldr r4,[r0]
	add r0,r0,#4
	
	ldr r5,[r0]
	add r0,r0,#4

	ldr r6,[r0]
	add r0,r0,#4

	ldr r7,[r0]
	add r0,r0,#4

	ldr r8,[r0]
	add r0,r0,#4

	ldr r9,[r0]
	add r0,r0,#4

	ldr r10,[r0]
	add r0,r0,#4

	ldr r11,[r0]
	add r0,r0,#4



	msr psp,r0
	orr lr,lr,#0x04

	cpsie i

	
	bx lr

	nop



OSTest
	nop
	nop
	add r0,r0,#1
	bx lr

	end






