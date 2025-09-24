#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Base64 alphabet
static const char *base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Input buffer size
#define BUFSIZE 4096

// Groups of 3 bytes translate to groups of 4 bytes.
#define IN_CHUNK_SIZE 3
#define OUT_CHUNK_SIZE 4

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
  while (len = read(STDIN_FILENO, in, 3)) {
    encode_chunk(in, len, out);
    write(1, out, 4);
    in[0] = in[1] = in[2] = 0;
  }
}

void err_exit(char *msg) {
  printf("ERROR: %s: %s", msg, strerror(errno));
  exit(1);
}

void encode_buffered() {
  unsigned char in[BUFSIZE];
  // 3*k + r input bytes give 4*(k +1) output bytes
  unsigned char out[OUT_CHUNK_SIZE * (BUFSIZE / IN_CHUNK_SIZE + 1)];
  ssize_t r_len;
  int in_len, rem, in_offset, out_offset;

  while (true) {
    r_len = read(STDIN_FILENO, in + rem, BUFSIZE - rem);

    // Read error
    if (r_len == -1)
      err_exit("read");

    // Input buffer length is what remains from the previous round,
    // plus what has just been read.
    if ((in_len = rem + r_len) == 0)
      break;

    in_offset = out_offset = 0;
    rem = in_len;
    while (rem > 2) {
      encode_chunk(in + in_offset, 3, out + out_offset);
      in_offset += 3;
      out_offset += 4;
      rem -= 3;
    }

    // Only encode the last incomplete chunk if input is exhausted
    if ((r_len == 0) && (rem > 0))
      encode_chunk(in + in_offset, rem, out + out_offset);

    // TODO: Error handling
    write(STDOUT_FILENO, out, out_offset);

    // Place trailing partial chunk at the beginning of in
    for (int i = 0; i < rem; i++)
      in[i] = in[in_offset + i];
  }
}

int main() {
  encode_buffered();
  return 0;
}
