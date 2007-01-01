;*****
;libco_x86 : version 0.08 ~byuu (10/21/06)
;cross-platform x86 implementation of libco
;
;context save/restore adheres to c/c++ ABI
;for x86 windows, osx, linux and freebsd
;
;context saves esp+ebp+esi+edi+ebx
;context ignores eax+ecx+edx
;context ignores st([0-7])+xmm[0-15]
;context ignores all else
;*****

section .data

align 4
co_active_context  dd 0
co_primary_context dd 0

section .code

;*****
;linker-specific name decorations
;*****
%ifdef WIN32
%define malloc     _malloc
%define free       _free

%define co_init    @co_init@0
%define co_term    @co_term@0
%define co_active  @co_active@0
%define co_create  @co_create@8
%define co_delete  @co_delete@4
%define co_switch  @co_switch@4
%define co_exit    @co_exit@4
%endif

%ifdef OSX86
%define malloc     _malloc
%define free       _free

%define co_init    _co_init
%define co_term    _co_term
%define co_active  _co_active
%define co_create  _co_create
%define co_delete  _co_delete
%define co_switch  _co_switch
%define co_exit    _co_exit
%endif

extern malloc
extern free

global co_init
global co_term
global co_active
global co_create
global co_delete
global co_switch
global co_exit

;*****
;extern "C" cothread_t fastcall co_init();
;return = eax
;*****

align 16
co_init:
;create context for main cothread
    mov    ecx,0                            ;entry point for main thread is not needed
    mov    edx,512                          ;main cothread uses default program stack
    call   co_create
    mov    dword[co_active_context],eax
    mov    dword[co_primary_context],eax
    ret

;*****
;extern "C" void fastcall co_term();
;*****

align 16
co_term:
    mov    ecx,dword[co_primary_context]
    call   co_delete
    ret

;*****
;extern "C" cothread_t fastcall co_active();
;return = eax
;*****

align 16
co_active:
    mov    eax,dword[co_active_context]
    ret

;*****
;extern "C" cothread_t fastcall co_create(cothread_p coentry, unsigned int heapsize);
;ecx = coentry
;edx = heapsize
;return = eax
;*****

align 16
co_create:
;create heap space (stack + register storage)
    add    edx,512                          ;+4(esp)+4(coentry)+4(colink)+256(stack_align)
    push   ecx
    push   edx

    push   edx
    call   malloc
    add    esp,4

    pop    edx
    pop    ecx

    add    edx,eax                          ;set edx to point to top of stack heap
    and    edx,0xffffff00                   ;force 256-byte alignment of stack heap

;store thread entry point + registers so that first call to co_switch will execute coentry
    mov    dword[edx-4],co_entrypoint       ;edx=*stack
    mov    dword[edx-8],0
    mov    dword[edx-12],0
    mov    dword[edx-16],0
    mov    dword[edx-20],0
    sub    edx,20

;initialize context memory heap
    mov    dword[eax],edx                   ;cothread_t[ 0- 3] = stack heap pointer (esp)
    mov    dword[eax+4],ecx                 ;cothread_t[ 4- 7] = entry point
    mov    ecx,dword[co_active_context]
    mov    dword[eax+8],ecx                 ;cothread_t[ 8-11] = return context

    ret                                     ;return allocated memory block as thread handle

;*****
;extern "C" void fastcall co_delete(cothread_t cothread);
;ecx = cothread
;*****

align 16
co_delete:
    push   ecx
    call   free
    add    esp,4
    ret

;*****
;extern "C" void fastcall co_switch(cothread_t cothread);
;ecx = cothread
;*****

align 16
co_switch:
    mov    eax,dword[co_active_context]     ;backup current context
    mov    dword[co_active_context],ecx     ;set new active context

    push   ebp
    push   esi
    push   edi
    push   ebx
    mov    dword[eax],esp

    mov    esp,dword[ecx]
    pop    ebx
    pop    edi
    pop    esi
    pop    ebp

    ret

;*****
;extern "C" void fastcall co_exit(cothread_t cothread);
;ecx = cothread
;*****

align 16
co_exit:
    cmp    ecx,0
    jne    co_switch

;if cothread is null, switch to context that created current context
    mov    eax,dword[co_active_context]
    mov    ecx,dword[eax+8]
    jmp    co_switch

;*****
;void fastcall co_entrypoint();
;*****

align 16
co_entrypoint:
    mov    eax,dword[co_active_context]
    call   dword[eax+4]
    xor    ecx,ecx
    jmp    co_exit
