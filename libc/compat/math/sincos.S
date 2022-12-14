/* Copyright (C) 1999 DJ Delorie, see COPYING.DJ for details */
NaN:
	.long	  0x00000000, 0xFFF80000

.globl _sincos
_sincos:

	/* void sincos(double *cosine, double *sine, double x); */

	movl	16(%esp), %ecx

	movl	4(%esp), %eax		/* Point to cosine. */
	movl	8(%esp), %edx		/* Point to sine. */

	andl	$0x7FF00000, %ecx	/* Examine exponent of x. */
	cmpl	$0x43E00000, %ecx	/* |x| >= 2^63 */
	jae	bigarg

	fldl	12(%esp)
	fsincos
	fstpl	(%eax)			/* cos */
	fstpl	(%edx)			/* sin */
	ret

bigarg:
	cmpl	$0x7FF00000, %ecx	/* x is INF or NaN. */
	jb	finite
	movl	NaN, %ecx		/* Return -NaN */
	movl	%ecx, (%eax)
	movl	%ecx, (%edx)
	movl	NaN+4, %ecx
	movl	%ecx, 4(%eax)
	movl	%ecx, 4(%edx)
	movl	$1, _errno
	ret

finite:
	fld1
	fstpl	(%eax)			/* cos = 1. */
	fldz
	fstpl	(%edx)			/* sin = 0. */
	ret	
