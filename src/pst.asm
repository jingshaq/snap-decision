;RCX、RDX、R8 和 R9 
[BITS 64]
section .text


global EntryPointLoad
extern printf
extern ExitProcess
extern GetModuleHandleA
extern GetProcAddress
EntryPointLoad:

	push        rdi
    push        rsi
    push        rbp
    push        rbx
    ; Alloc space on the stack for the function pointers and the kernel base
    ; sub         rsp, 0x200 + 8
	


	call _getKernel32AddressTable
	; mov         rax, r14

	; mov QWORD [ss:rsp + 0x200], rax

	;  mov rdx, QWORD [rsp + 0x200]

	; add rsp, 0x200+8
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

	
	FindWindowA	db	"FindWindowA", 0		

	User32_DLL_NAME  db "User32.dll",0
	; User32_DLL_NAME  db "Kernel32.dll",0
	error_str  db "error_str",0



_getKernel32AddressTable:



	push        rdi
    push        rsi
    push        rbp
    push        rbx



	; mov 	QWORD [ss:rsp + 120], r13	; store HANDLE hFile onto the stack for later use
	; mov 	QWORD [ss:rsp + 112], r14	; store WIN32_FIND_DATAA struct onto the stack for later use
	

	call 	_get_Kernel32_Handle		; get Kernel32.dll base address
	mov 	r12, rax			; *** r12: base addr kernel32.dll ***
	mov 	rcx, rax

	call 	_get_function_export		; get GetProcAddress address
	mov 	r13, rax			; *** r13: GetProcAddress ***
	
	lea rdx, [rel k32LocalAlloc]
	call r13

	mov 	r15, rax			; *** r15: store LocalAlloc import for later use ***

	xor 	rcx, rcx
	mov 	rdx, rcx
	add 	rcx, 0x0040
	add 	rdx, 112 
	call 	rax				; LocalAlloc(LPTR, 112)


	mov 	r14, rax			; *** r14: address Kernel32AddressTable (heap memory allocation where all winapi will be stored) ***
	mov 	rax, r15
	call 	_prepareStoreAddress		; store LocalAlloc address in Kernel32AddressTable
	

	mov 	rcx, r12
	lea 	rdx, [rel  k32LocalFree]
	call 	r13				; GetProcAddress("LocalFree")
	call 	_prepareStoreAddress		; store LocalFree address


	mov 	rcx, r12
	lea 	rdx, [rel  k32ExitProcess]
	call 	r13				; get ExitProcess
	call 	_prepareStoreAddress


	mov 	rcx, r12
	lea 	rdx, [rel  k32CreateFileA]
	call 	r13				; get CreateFileA
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32GetFileSize]
	call 	r13				; get GetFileSize
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32ReadFile]
	call 	r13				; get ReadFile
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32WriteFile]
	call 	r13				; get WriteFile
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32CloseHandle]
	call 	r13				; get CloseHandle
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32FindFirstFileA]
	call 	r13				; get FindFirstFileA
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32FindNextFileA]
	call 	r13				; get FindNextFileA
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32SetCurrentDirectoryA]
	call 	r13				; get SetCurrentDirectoryA
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32GetCurrentDirectory]
	call 	r13				; get GetCurrentDirectoryA
	call 	_prepareStoreAddress

	mov 	rcx, r12
	lea 	rdx, [rel  k32GetModuleHandleA]
	call 	r13				; get GetModuleHandleA

	call 	_prepareStoreAddress
	
	lea 	rcx, [rel  User32_DLL_NAME]
	mov rax,[r14+96]
	call rax
	test rax,rax
	jz _returnError
	
	
	; mov rax,r14

	; mov rcx, r14
	; mov rcx,[rcx]
	; xor rdx, rdx 
	; add rdx, 0x0040
	; xor r8, r8
	; add r8, 100
	; call rcx

	; mov rax, [r14]
	; mov 	rcx, 0x0040
	; mov 	rdx, 104 
	; call 	rax

	; mov rcx,rax
	; mov rax, [r14 + 8]
	; call rax

	

	pop         rbx
    pop         rbp
    pop         rsi
    pop         rdi
    ret


_prepareStoreAddress:
	mov 	r9, rax
	xor 	r8, r8
	mov 	rdx, r8
	mov 	rcx, r14

_storeAddress:
	cmp 	QWORD [ds:rcx + rdx], r8
	jne 	_storeAddress_continueIncrement
	mov 	QWORD [ds:rcx + rdx], r9
	mov 	rax, rdx
	ret

_storeAddress_continueIncrement:
	add 	rdx, 8
	jmp 	_storeAddress


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


_loop:
	
	mov 	r9d, [r8]
	lea 	r9, [rcx + r9]				; pointer to function name
	cmp 	r10, [r9]
	jnz 	_adjust_loop
	cmp 	r11, [r9 + 7]
	jnz 	_adjust_loop

	neg 	rdx
	mov 	r10d, [rax + 18h]
	lea 	rdx, [r10 + rdx]
	mov 	r10d, [rax + 24h]
	lea 	r10, [rcx + r10]
	movzx 	rdx, WORD [r10 + rdx * 2]
	mov 	r10d, [rax + 1Ch]   			; AddressOfFunctions
	lea 	r10, [rcx + r10]

	mov 	r10d, [r10 + rdx * 4]			; r10 = offset of possible func addr

; Check for forwarded function
	mov 	edx, [rax + 0]				; rdx = VirtualAddress
	cmp 	r10, rdx
	jb 		_returnError

	mov 	r11d, [rax + 4]				; r11 = Size
	add 	r11, rdx
	cmp 	r10, r11

	mov 	r11d, [rax + 4]				; r11 = Size
	add 	r11, rdx
	cmp 	r10, r11
	jae 	_returnError

	lea		rax, [rcx + r10]        	; Got our func addr!
	ret


_adjust_loop:
	
	add 	r8, 4
	dec 	rdx
	jnz 	_loop
	

_returnError:
	lea rcx ,[rel error_str]
	call printf
	xor 	rax, rax 
	ret

