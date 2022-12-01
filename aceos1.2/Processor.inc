;---------------------Ace OS source Code-----------------------
;		Created by Sam on 11-Aug-2002(samuelhard@yahoo.com)
; This file is a header file for Processor management routines
;--------------------------------------------------------------
%ifndef PROCESSOR_INC
%define PROCESSOR_INC

EXTERN _DetectProcessor
EXTERN _ExceptionHandler
EXTERN _SetIDT
EXTERN _GetIDT
EXTERN _SetExceptionHandlers


EXTERN __CPU_ProcessorID
EXTERN __CPU_VendorID
EXTERN __CPU_Signature
EXTERN __CPU_Features_EBX
EXTERN __CPU_Features_EDX
EXTERN __CPU_Features_ECX


%endif