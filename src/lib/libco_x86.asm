;*****
;libco_x86 : version 0.10 ~byuu (2007-04-18)
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

;*****
;linker-specific name decorations
;*****

%ifdef WIN32
%define malloc     _malloc
%define free       _free

%define co_active  @co_active@0
%define co_create  @co_create@12
%define co_delete  @co_delete@4
%define co_switch  @co_switch@4
%endif

%ifdef OSX86
%define malloc     _malloc
%define free       _free

%define co_active  _co_active
%define co_create  _co_create
%define co_delete  _co_delete
%define co_switch  _co_switch
%endif

bits 32

section .bss

align 4
co_primary_buffer resb 512

section .data

align 4
co_active_context dd co_primary_buffer

section .text

extern malloc
extern free

global co_active
global co_create
global co_delete
global co_switch

;*****
;extern "C" cothread_t fastcall co_active();
;return = eax
;*****

align 16
co_active:
    mov    eax,[co_active_context]
    ret

;*****
;extern "C" cothread_t fastcall co_create(unsigned int heapsize, void (*coentry)(void *data), void *data);
;ecx     = heapsize
;edx     = coentry
;[esp+4] = data
;return  = eax
;*****

align 16
co_create:
;create heap space (stack + register storage)
    add    ecx,512                          ;allocate extra memory for contextual info
    push   ecx
    push   edx

    push   ecx
    call   malloc                           ;eax = malloc(edx)
    add    esp,4

    pop    edx
    pop    ecx

    add    ecx,eax                          ;set edx to point to top of stack heap
    and    ecx,-16                          ;force 16-byte alignment of stack heap

;store thread entry point + registers, so that first call to co_switch will execute coentry
    mov    dword[ecx-4],co_entrypoint       ;entry point
    mov    dword[ecx-8],0                   ;ebp
    mov    dword[ecx-12],0                  ;esi
    mov    dword[ecx-16],0                  ;edi
    mov    dword[ecx-20],0                  ;ebx
    sub    ecx,20

;initialize context memory heap and return
    mov    [eax],ecx                        ;cothread_t[ 0- 3] = stack heap pointer (esp)
    mov    [eax+4],edx                      ;cothread_t[ 4- 7] = coentry
    mov    ecx,[esp+4]
    mov    [eax+8],ecx                      ;cothread_t[ 8-11] = data
    ret    4                                ;return allocated memory block as thread handle

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
;void fastcall co_entrypoint();
;*****

align 16
co_entrypoint:
    mov    ebp,esp
.loop
    mov    eax,[co_active_context]
    mov    ecx,[eax+8]                      ;fastcall
    push   ecx                              ;stdcall
    call   [eax+4]
    mov    esp,ebp                          ;cdecl
    jmp    .loop
