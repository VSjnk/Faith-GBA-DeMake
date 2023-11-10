/*****************************************************************************\
                 DevKit Advance Main Interrupt Service Routine
\*****************************************************************************/

/* Administrivia:
 *
 * Copyright 2003, Jason Wilkins.  This source code is free for any use except
 * that this copyright notice and the following disclaimers remain intact when
 * the source is distributed.  There are absolutely no restrictions on use of
 * object code generated from this source, but the disclaimers remain in force.
 *
 * THIS CODE WAS NOT MADE IN ASSOCIATION WITH NINTENDO AND DOES NOT MAKE USE OF
 * ANY INTELLECTUAL PROPERTY CLAIMED BY NINTENDO.
 *
 * GAMEBOY ADVANCE IS A TRADEMARK OF NINTENDO.
 *
 * THIS CODE HAS BEEN PROVIDED "AS-IS" WITHOUT A WARRANTY OF ANY KIND, EITHER
 * EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO IMPLIED WARRANTIES OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  THE ENTIRE RISK AS TO THE
 * QUALITY OR PERFORMANCE OF THE CODE IS WITH YOU.
 *
 * IN NO EVENT, UNLESS AGREED TO IN WRITING, WILL ANY COPYRIGHT HOLDER, OR ANY
 * OTHER PARTY, BE HELD LIABLE FOR ANY DAMAGES RESULTING FROM THE USE OR
 * INABILITY TO USE THIS CODE.
 *
 * See $(DEVKITADV)/docs/Credits.txt for a list of contributors.
 *****************************************************************************/
#include "asm-macros.h"
#include "config-isr.h"



	.section    ISR_SECTION, "ax", %progbits

	.align



#if WEAK_USER_INTERRUPT_HANDLER
	.weak USER_INTERRUPT_HANDLER_NAME
#endif

#if FAST_INTERRUPTS
	.extern	USER_INTERRUPT_HANDLER_NAME

    FUNCTION(__interrupt_handler)
        ldr     r0,=USER_INTERRUPT_HANDLER_NAME
        bx      r0
    END(__interrupt_handler)
#else



	.extern	INTERRUPT_TABLE_NAME

    FUNCTION(__interrupt_handler)
	ldr     r3, =0x04000200

#if MULTIPLE_INTERRUPTS
	ldrh    r0, [r3, #0x8]
	mrs     r1, spsr
	stmfd   sp!, {r0, r1, lr}

	mov     r0, #1
	strh    r0, [r3, #0x8]
#endif

	ldr     r1, [r3,#0x0]
        and     r0, r1, r1, lsr #16

#if INTERRUPT_TABLE
        ldr     r2, =INTERRUPT_TABLE_NAME	// r2 = &IntrTable[0]

        ands    r1, r0, #1               // V-Blank Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #2               // H-Blank Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #4               // V Counter Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #8               // Timer 0 Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x10            // Timer 1 Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x20            // Timer 2 Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x40            // Timer 3 Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x80            // Serial Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x100           // DMA0 Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x200           // DMA1 Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x400           // DMA2 Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x800           // DMA3 Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x1000          // Key Interrupt
        bne     .Ljump_intr

        add     r2, r2, #4
        ands    r1, r0, #0x2000		// Cart Interrupt

#if HANDLE_CART_INTERRUPT
        strneb  r1, [r3, #0x84 - 0x200]		// Stop sound if cart removed (REG_SOUNDCNT_X)
   .Lcart_loop:
	bne     .Lcart_loop		// Infinite loop if cart removed
#endif

   .Ljump_intr:
#endif // INTERRUPT_TABLE

#if MULTIPLE_INTERRUPTS
        mrs     r3, cpsr
#if SWITCH_TO_USER_STACK
        bic     r3, r3, #0xdf
        orr     r3, r3, #0x1f
#else
        bic     r3, r3, #0xc0
#endif
        msr     cpsr, r3

#if INTERRUPT_TABLE
	// r1 = interrupt bit
	// r2 = interrupt table pointer

	ldr	r3, =0x04000200
        strh    r1, [r3, #0x2]	// Acknowledge Interrupt

        ldr     r1, [r2]
#else
	// r0 = interrupt flags to pass to interrupt process
	ldr	r1, =USER_INTERRUPT_HANDLER_NAME
#endif

	// r1 = interrupt process

        stmfd   sp!, {lr}
        adr     lr, .Lintr_return

        bx      r1
   .Lintr_return:
        ldmfd   sp!, {lr}

#if !INTERRUPT_TABLE
	ldr	r3, =0x04000200
	strh    r0, [r3, #0x2]	// Acknowledge Interrupt
#endif

        mrs     r3, cpsr
#if SWITCH_TO_USER_STACK
        bic     r3, r3, #0xdf
        orr     r3, r3, #0x92            // Disable IRQ. Enable FIQ. Set CPU mode to IRQ.
#else
        orr     r3, r3, #0x80            // Disable IRQ.
#endif
        msr     cpsr, r3

        ldmfd   sp!, {r0, r1, lr}        // restore spsr, IME, return address}

	ldr	r3, =0x04000200
        strh    r1,  [r3, #0x8]

        msr     spsr, r0

        bx      lr
#else // !MULTIPLE_INTERRUPTS

#if INTERRUPT_TABLE
        strh    r1, [r3, #0x2]	// Acknowledge Interrupt

        ldr     r1, [r2]		// Jump to ISR
        bx      r1
#else
	ldr	r1, =USER_INTERRUPT_HANDLER_NAME
	bx	r1

	ldr	r3, =0x04000200
        strh    r0, [r3, #0x2]	// Acknowledge Interrupt
#endif

#endif

    END(__interrupt_handler)

#endif // !FAST_INTERRUPTS

	.align
    STATIC_OBJECT(__interrupt_literals)
	.pool
    END(__interrupt_literals)






