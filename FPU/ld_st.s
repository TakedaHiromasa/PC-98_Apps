	#.arch i286+287,jumps
	.code16
	.att_syntax prefix
#NO_APP
	.section	.fartext.f.ld_st.c.64770$,"ax",@progbits
	.global	main
	.type	main, @function
main:
	enterw	$10-2,	$0
	movw	$28836,	-4(%bp)
	movw	$16285,	-2(%bp)
	movw	$0,	-8(%bp)
	movw	$0,	-6(%bp)
#APP
;# 7 "ld_st.c" 1
	finit
;# 0 "" 2
#NO_APP
	movw	$0,	%ax
	movw	%ax,	%ax
	movw	%ax,	%ax
	leavew
	pushw	%ss
	popw	%ds
	lret
	.size	main, .-main
	.ident	"GCC: (GNU) 6.3.0"
