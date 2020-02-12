.syntax unified
.arm
.section .text

.extern errno


@ int umask(int mask)
@  returns old mask or -1
.global umask
.type umask,%function
umask:
    push    {fp, lr}                                   @ set up our stack frame
    add     fp, sp, #0                                 @ adjust the frame pointer
    push    {r0}                                       @ mask
    mov     lr, sp                                     @ pointer to indirect args is kept in lr
    swi     #60                                        @ 60 is umask
    bcc     1f                                         @ syscalls return carry-clear for success and carry-set for error
    ldr     r2, =errno                                 @ load up the errno address
    str     r0, [r2]                                   @ stash the returned errno in the error case
    mov     r0, #-1                                    @ syscalls return -1 for error
1:  sub     sp, r11, #0                                @ restore our stack pointer to point to our stack frame
    pop     {fp, pc}                                   @ restore caller frame and return
.size umask, . - umask
