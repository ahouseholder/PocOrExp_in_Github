source: http://www.securityfocus.com/bid/54816/info

Calligra is prone to a remote buffer-overflow vulnerability.

An attacker can exploit this issue to execute arbitrary code in the context of the user running the affected application. Failed exploit attempts will likely result in denial-of-service conditions.

Calligra 2.4.3 and KOffice 2.3.3 are vulnerable; other versions may also be affected. 

bool STD::read( U16 baseSize, U16 totalSize, OLEStreamReader* stream, bool
preservePos )
...
grupxLen = totalSize - ( stream->tell() - startOffset );
grupx = new U8[ grupxLen ];
int offset = 0;
for ( U8 i = 0; i < cupx; ++i) {
U16 cbUPX = stream->readU16(); // size of the next UPX
stream->seek( -2, G_SEEK_CUR ); // rewind the "lookahead"
cbUPX += 2; // ...and correct the size
for ( U16 j = 0; j < cbUPX; ++j ) {
grupx[ offset + j ] = stream->readU8(); // read the whole UPX
}