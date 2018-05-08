---------------------------------------------------------------------------
;A9汇编编程一个LED闪亮代码，注意代码格式，0与o\l与1一定要区分清楚

.global _start
_start:
    ldr r0,=0x11000c40
    ldr r1,[r0]
    bic r1,#0xf0000000
    orr r1,#0x10000000
    str r1,[r0]
loop:
    ldr r0,=0x11000c44
    ldr r1,[r0]
    orr r1,r1,#0x80
    str r1,[r0]
    bl delay
    bic r1,r1,#0x80
    str r1,[r0]
    bl delay
    b loop
delay:
    ldr r2,=0xfffffff
loop1:
	sub r2,r2,#0x1
	cmp r2,#0x0
	bne loop1
	mov pc,lr
stop:
	b stop
.end
