;*****
;libco_x86_64 : version 0.09 ~byuu (2007-01-19)
;cross-platform x86-64 implementation of libco
;special thanks to Aaron Giles and Joel Yliluoma for various optimizations
;
;[ABI compatibility]
;- SystemV ( http://refspecs.freestandards.org/elf/x86_64-SysV-psABI.pdf )
;- gcc; linux-x86-64
;- gcc; freebsd-x86-64
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
co_active_context  dq 0
co_primary_context dq 0

section .text

extern malloc
extern free

global co_active
global co_create
global co_delete
global co_switch
global co_exit

;*****
;extern "C" cothread_t co_active();
;return = rax
;*****

align 16
co_active:
    cmp    qword[co_active_context wrt rip],0
    jnz   .initialized
    jmp    co_init
.initialized
    mov    rax,[co_active_context wrt rip]
    ret

;*****
;extern "C" cothread_t co_create(cothread_p coentry, unsigned int heapsize);
;rdi = coentry
;rsi = heapsize
;return = rax
;*****

align 16
co_create:
    cmp    qword[co_active_context wrt rip],0
    jnz   .initialized
    call   co_init
.initialized
;create heap space (stack + register storage)
    add    rsi,512                          ;allocate extra memory for contextual info
    push   rdi
    push   rsi

    mov    rdi,rsi
    call   malloc                           ;rax = malloc(rdi)

    pop    rsi
    pop    rdi

    add    rsi,rax                          ;set rsi to point to top of stack heap
    and    rsi,-16                          ;force 16-byte alignment of stack heap

;store thread entry point + registers, so that first call to co_switch will execute coentry
    mov    qword[rsi-8],co_entrypoint
    mov    qword[rsi-16],0                  ;r15
    mov    qword[rsi-24],0                  ;r14
    mov    qword[rsi-32],0                  ;r13
    mov    qword[rsi-40],0                  ;r12
    mov    qword[rsi-48],0                  ;rbx
    mov    qword[rsi-56],0                  ;rbp
    mov    qword[rsi-64],0                  ;rsp
    sub    rsi,64

;initialize context memory heap
    mov    [rax],rsi                        ;cothread_t[ 0- 7] = stack heap pointer (rsp)
    mov    [rax+8],rdi                      ;cothread_t[ 8-15] = entry point (coentry)
    mov    rdi,[co_active_context wrt rip]
    mov    [rax+16],rdi                     ;cothread_t[16-23] = return context

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

;*****
;extern "C" void co_exit(cothread_t cothread);
;rdi = cothread
;*****

align 16
co_exit:
    cmp    rdi,0
    jne    co_switch

;if cothread is null, switch to context that created current context
    mov    rax,[co_active_context wrt rip]
    mov    rdi,[rax+16]
    jmp    co_switch

;*****
;void co_entrypoint();
;*****

align 16
co_entrypoint:
    mov    rax,[co_active_context wrt rip]
    call   [rax+8]
    xor    rdi,rdi
    jmp    co_exit

;*****
;cothread_t co_init();
;return = rax
;*****

align 16
co_init:
    mov    rax,co_primary_buffer            ;use pre-allocated memory for primary context
    mov    [co_active_context wrt rip],rax
    mov    [co_primary_context wrt rip],rax
    ret
