;*****
;libco_x86_64 : version 0.10 ~byuu (2007-09-08)
;cross-platform x86-64 implementation of libco
;special thanks to Aaron Giles and Joel Yliluoma for various optimizations
;
;[ABI compatibility]
;- SystemV ( http://refspecs.freestandards.org/elf/x86_64-SysV-psABI.pdf )
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
;rdi    = heapsize
;rsi    = coentry
;return = rax
;*****

align 16
co_create:
;create heap space (stack + register storage)
    add    rdi,512                          ;allocate extra memory for contextual info
    push   rdi
    push   rsi

    call   malloc                           ;rax = malloc(rdi)

    pop    rsi
    pop    rdi

    add    rdi,rax                          ;set rsi to point to top of stack heap
    and    rdi,-16                          ;force 16-byte alignment of stack heap

;store thread entry point + registers, so that first call to co_switch will execute coentry
    mov    qword[rdi-8],rsi                 ;entry point
    mov    qword[rdi-16],0                  ;r15
    mov    qword[rdi-24],0                  ;r14
    mov    qword[rdi-32],0                  ;r13
    mov    qword[rdi-40],0                  ;r12
    mov    qword[rdi-48],0                  ;rbx
    mov    qword[rdi-56],0                  ;rbp
    sub    rdi,56

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
