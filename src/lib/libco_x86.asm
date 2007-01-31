;*****
;libco_x86 : version 0.09 ~byuu (2007-01-19)
;cross-platform x86 implementation of libco
;special thanks to Aaron Giles and Joel Yliluoma for various optimizations
;
;[ABI compatibility]
;- visual c++; windows-x86
;- mingw; windows-x86
;- gcc; osx86
;- gcc; linux-x86
;- gcc; freebsd-x86
;
;[nonvolatile registers]
;- esp, ebp, edi, esi, ebx
;
;[volatile registers]
;- eax, ecx, edx
;- st0 - st7
;- xmm0 - xmm15
;*****

bits 32

section .bss

align 4
co_primary_buffer resb 512

section .data

align 4
co_active_context  dd 0
co_primary_context dd 0

section .text

;*****
;linker-specific name decorations
;*****
%ifdef WIN32
%define malloc     _malloc
%define free       _free

%define co_active  @co_active@0
%define co_create  @co_create@8
%define co_delete  @co_delete@4
%define co_switch  @co_switch@4
%define co_exit    @co_exit@4
%define co_init    @co_init@0
%endif

%ifdef OSX86
%define malloc     _malloc
%define free       _free

%define co_active  _co_active
%define co_create  _co_create
%define co_delete  _co_delete
%define co_switch  _co_switch
%define co_exit    _co_exit
%define co_init    _co_init
%endif

extern malloc
extern free

global co_active
global co_create
global co_delete
global co_switch
global co_exit

;*****
;extern "C" cothread_t fastcall co_active();
;return = eax
;*****

align 16
co_active:
    cmp    dword[co_active_context],0
    jnz   .initialized
    jmp    co_init
.initialized
    mov    eax,[co_active_context]
    ret

;*****
;extern "C" cothread_t fastcall co_create(cothread_p coentry, unsigned int heapsize);
;ecx = coentry
;edx = heapsize
;return = eax
;*****

align 16
co_create:
    cmp    dword[co_active_context],0
    jnz   .initialized
    call   co_init
.initialized
;create heap space (stack + register storage)
    add    edx,512                          ;allocate extra memory for contextual info
    push   ecx
    push   edx

    push   edx
    call   malloc                           ;eax = malloc(edx)
    add    esp,4

    pop    edx
    pop    ecx

    add    edx,eax                          ;set edx to point to top of stack heap
    and    edx,-16                          ;force 16-byte alignment of stack heap

;store thread entry point + registers, so that first call to co_switch will execute coentry
    mov    dword[edx-4],co_entrypoint
    mov    dword[edx-8],0                   ;ebp
    mov    dword[edx-12],0                  ;esi
    mov    dword[edx-16],0                  ;edi
    mov    dword[edx-20],0                  ;ebx
    sub    edx,20

;initialize context memory heap
    mov    [eax],edx                        ;cothread_t[ 0- 3] = stack heap pointer (esp)
    mov    [eax+4],ecx                      ;cothread_t[ 4- 7] = entry point (coentry)
    mov    ecx,[co_active_context]
    mov    [eax+8],ecx                      ;cothread_t[ 8-11] = return context

    ret                                     ;return allocated memory block as thread handle

;*****
;extern "C" void fastcall co_delete(cothread_t cothread);
;ecx = cothread
;*****

align 16
co_delete:
    push   ecx
    call   free                             ;free(ecx)
    add    esp,4
    ret

;*****
;extern "C" void fastcall co_switch(cothread_t cothread);
;ecx = cothread
;*****

align 16
co_switch:
    mov    eax,[co_active_context]          ;backup current context
    mov    [co_active_context],ecx          ;set new active context

    push   ebp
    push   esi
    push   edi
    push   ebx
    mov    [eax],esp

    mov    esp,[ecx]
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
    mov    eax,[co_active_context]
    mov    ecx,[eax+8]
    jmp    co_switch

;*****
;void fastcall co_entrypoint();
;*****

align 16
co_entrypoint:
    mov    eax,[co_active_context]
    call   [eax+4]
    xor    ecx,ecx
    jmp    co_exit

;*****
;cothread_t fastcall co_init();
;return = eax
;*****

align 16
co_init:
    mov    eax,co_primary_buffer            ;use pre-allocated memory for primary context
    mov    [co_active_context],eax
    mov    [co_primary_context],eax
    ret
