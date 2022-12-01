%include "Common.inc"
%include "VGAText.inc"
%include "Processor.inc"

GLOBAL _JumpToTask
GLOBAL DEBUG
GLOBAL _LoadKernelTSS
EXTERN _KernelTSS
EXTERN _KernelTSSSelector
[SECTION .text]
[BITS 32]
;this function receives a task selector and jumps to the task
;Input : Task Selector
;Output: None
_LoadKernelTSS:
	ltr [_KernelTSSSelector]
	ret
DEBUG:
_JumpToTask:
	prologue 4
	
	call dword far [ebp+4]
	
	epilogue
	ret