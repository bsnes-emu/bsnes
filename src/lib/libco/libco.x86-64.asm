;*****
;libco.x86-64 (2007-12-11)
;author: byuu
;license: public domain
;
;cross-platform x86-64 implementation of libco
;thanks to Aaron Giles and Joel Yliluoma for various optimizations
;thanks to Lucas Newman and Vas Crabb for assistance with OS X support
;
;[ABI compatibility]
;- SystemV ( http://refspecs.freestandards.org/elf/x86_64-SysV-psABI.pdf )
;- gcc; mac os x; x86-64
;- gcc; linux; x86-64
;- gcc; freebsd; x86-64
;
;[nonvolatile registers]
;- rsp, rbp, rbx, r12, r13, r14, r15
;
;[volatile registers]
;- rax, rcx, rdx, r8, r9, r10, r11, rdi, rsi
;- st0 - st7
;- xmm0 - xmm15
;*****

;*****
;linker-specific name decorations
;*****

%ifdef OSX
%define malloc     _malloc
%define free       _free

%define co_active  _co_active
%define co_create  _co_create
%define co_delete  _co_delete
%define co_switch  _co_switch
%endif

bits 64

section .bss

align 8
co_primary_buffer resb 512

section .data

align 8
co_active_context dq co_primary_buffer

section .text

extern malloc
extern free

global co_active
global co_create
global co_delete
global co_switch

;*****
;extern "C" cothread_t co_active();
;return = rax
;*****

align 16
co_active:
    mov    rax,[co_active_context wrt rip]
    ret

;*****
;extern "C" cothread_t co_create(unsigned int heapsize, void (*coentry)());
;rdi = heapsize
;rsi = coentry
;return = rax
;*****

align 16
co_create:
;create heap space (stack + context)
    add    rdi,512                          ;allocate extra memory for contextual info

    push   rdi                              ;backup volatile registers before malloc call
    push   rsi

    sub    rsp,8                            ;SSE 16-byte stack alignment
    call   malloc                           ;rax = malloc(rdi)
    add    rsp,8

    pop    rsi                              ;restore volatile registers
    pop    rdi

    add    rdi,rax                          ;set rdi to point to top of stack heap
    and    rdi,-16                          ;force 16-byte alignment of stack heap

;store thread entry point + registers, so that first call to co_switch will execute coentry
    mov    qword[rdi-8],0                   ;crash if entry point returns
    mov    qword[rdi-16],rsi                ;entry point
    mov    qword[rdi-24],0                  ;r15
    mov    qword[rdi-32],0                  ;r14
    mov    qword[rdi-40],0                  ;r13
    mov    qword[rdi-48],0                  ;r12
    mov    qword[rdi-56],0                  ;rbx
    mov    qword[rdi-64],0                  ;rbp
    sub    rdi,64

;initialize context memory heap and return
    mov    [rax],rdi                        ;*cothread_t = stack heap pointer (rsp)
    ret                                     ;return allocated memory block as thread handle

;*****
;extern "C" void co_delete(cothread_t cothread);
;rdi = cothread
;*****

align 16
co_delete:
    jmp    free                             ;free(rdi)

;*****
;extern "C" void co_switch(cothread_t cothread);
;rdi = cothread
;*****

align 16
co_switch:
    mov    rax,[co_active_context wrt rip]  ;backup current context
    mov    [co_active_context wrt rip],rdi  ;set new active context

    push   rbp
    push   rbx
    push   r12
    push   r13
    push   r14
    push   r15
    mov    [rax],rsp

    mov    rsp,[rdi]
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    pop    rbx
    pop    rbp

    ret
