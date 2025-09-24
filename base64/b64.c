#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

static const char *base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const int BUFSIZE = 4096;

void encode_chunk(unsigned char *in, int len, unsigned char *out) {

  //  12345678 -> 00123456
  out[0] = base[in[0] >> 2];

  if (len == 1) {
    // 12345678 -> 0078000
    out[1] = base[(in[0] & 0x3) << 4];
    out[2] = out[3] = '=';
    return;
  }

  //  12345678 abcdefgh -> 00780000 | 0000abcd -> 0078abcd
  out[1] = base[((in[0] & 0x3) << 4) | (in[1] >> 4)];

  if (len == 2) {
    // ABCDEFGH -> 0000EFGH -> 00EFGH00
    out[2] = base[(in[1] & 0xf) << 2];
    out[3] = '=';
    return;
  }

  // abcdefgh ABCDEFGH -> 00efgh00 | 000000AB -> 00efghAB
  out[2] = base[(in[1] & 0xf) << 2 | (in[2] >> 6)];
  // ABCDEFGH -> 00CDEFGH
  out[3] = base[in[2] & 0x3f];
}

void encode_unbuffered() {
  unsigned char in[3];
  unsigned char out[4];
  int len;
  while (len = read(0, in, 3)) {
    encode_chunk(in, len, out);
    write(1, out, 4);
    in[0] = in[1] = in[2] = 0;
  }
}

int encode_buffered() {
  unsigned char in[BUFSIZE];
  unsigned char out[4 * (BUFSIZE / 3 + 1)];
  int len, chunk_len, in_offset, out_offset;

  while (len = read(0, in, BUFSIZE)) {
    if (len == -1) {
      printf("read error: %s", strerror(errno));
      return 1;
    }

    in_offset = out_offset = 0;
    while (in_offset < len) {
      chunk_len = len - in_offset;
      encode_chunk(in + in_offset, chunk_len, out + out_offset);
      in_offset+=3;
      out_offset+=4;
    }
    // TODO: Error handling
    write(1, out, out_offset) == -1)
}

int main() {
  encode_buffered();
  return 0;
}
