;*****
;libco_x86 : version 0.07 ~byuu (08/15/06)
;cross-platform x86 implementation of libco
;
;context save/restore adheres to c/c++ ABI
;for x86 windows, linux and freebsd
;
;context saves esp+ebp+esi+edi+ebx
;context ignores eax+ecx+edx
;context ignores st([0-7])+xmm[0-15]
;context ignores all else
;*****

section .data

align 4
co_initialized     dd 0
co_active_context  dd 0

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
%define co_jump    @co_jump@4
%define co_call    @co_call@4
%define co_return  @co_return@0
%endif

extern malloc
extern free

global co_init
global co_term
global co_active
global co_create
global co_delete
global co_jump
global co_call
global co_return

;*****
;extern "C" void fastcall co_init();
;*****

align 16
co_init:
    cmp    dword[co_initialized],0          ;only run co_init once
    jne    .end
    inc    dword[co_initialized]

;create context for main thread
    mov    ecx,0                            ;entry point for main thread is not needed
    mov    edx,256                          ;main thread uses default program stack
    call   co_create
    mov    dword[co_active_context],eax
.end
    ret

;*****
;extern "C" void fastcall co_term();
;*****

align 16
co_term:
    ret

;*****
;extern "C" thread_t fastcall co_active();
;return = eax
;*****

align 16
co_active:
    cmp    dword[co_initialized],0          ;make sure co_init has been called
    jne    .next
    call   co_init
.next:
    mov    eax,dword[co_active_context]
    ret

;*****
;extern "C" thread_t fastcall co_create(thread_p coentry, unsigned int heapsize);
;ecx = coentry
;edx = heapsize
;return = eax
;*****

align 16
co_create:
    cmp    dword[co_initialized],0          ;make sure co_init has been called
    jne    .next
    push   ecx
    push   edx
    call   co_init
    pop    edx
    pop    ecx
.next:

;create heap space (stack + register storage)
    add    edx,28                           ;+8(esp+prev_call_context)+4(coentry)+16(stack_align)
    push   ecx
    push   edx

    push   edx
    call   malloc
    add    esp,4

    pop    edx
    pop    ecx

    add    edx,eax                          ;set edx to point to top of stack heap
    and    edx,0xfffffff0                   ;force 16-byte alignment of stack heap

;store thread entry point + registers so that first call to co_jump will go to coentry
    mov    dword[edx-4],ecx                 ;edx=*stack,ecx=coentry
    mov    dword[edx-8],0
    mov    dword[edx-12],0
    mov    dword[edx-16],0
    mov    dword[edx-20],0
    sub    edx,20

;initialize context memory heap
    mov    dword[eax],edx                   ;thread_t[0-3] = stack heap pointer (esp)
    mov    dword[eax+4],0                   ;thread_t[4-7] = (null) pointer to prev_call_context

    ret                                     ;return allocated memory block as thread handle

;*****
;extern "C" void fastcall co_delete(thread_t cothread);
;ecx = cothread
;*****

align 16
co_delete:
    push   ecx
    call   free
    add    esp,4
    ret

;*****
;extern "C" void fastcall co_jump(thread_t cothread);
;ecx = cothread
;*****

align 16
co_jump:
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
;extern "C" void fastcall co_call(thread_t cothread);
;ecx = cothread
;*****

align 16
co_call:
    mov    eax,dword[co_active_context]     ;backup current context
    mov    dword[co_active_context],ecx     ;set new active context
    mov    dword[ecx+4],eax

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
;extern "C" void fastcall co_return();
;*****

align 16
co_return:
    mov    eax,dword[co_active_context]     ;backup current context
    mov    ecx,dword[eax+4]                 ;restore pre-call context
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
