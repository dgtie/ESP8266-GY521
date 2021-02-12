/*
#include <stdio.h>

int encrypt(char *in, char *out);

char response[] =
"HTTP/1.1 101 Switching Protocols\r\n"
"Upgrade: websocket\r\n"
"Connection: Upgrade\r\n"
"Sec-WebSocket-Accept: S3PPLMbITXAQ9KYGZZHZRBk+XOO=\r\n" "\r\n";
//"Sec-WebSocket-Protocol: chat\r\n\r\n";
//97 = s3pPL...

int main() {
  char test[] = "dGhlIHNhbXBsZSBub25jZQ==";
  encrypt(test, &response[97]);
  printf("%s\n%d\n", response, (int)sizeof(response));
  return 0;
}
//*/

#include "sha1.h"

static char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
              "abcdefghijklmnopqrstuvwxyz"
              "0123456789+/";

static void print4(unsigned u, char *p) {
  p[0] = code[(u >> 18) & 63];
  p[1] = code[(u >> 12) & 63];
  p[2] = code[(u >> 6) & 63];
  p[3] = code[u & 63];
}

static void print3(unsigned u, char *p) {
  p[0] = code[(u >> 18) & 63];
  p[1] = code[(u >> 12) & 63];
  p[2] = code[(u >> 6) & 63];
  p[3] = '=';
}

static void base64(char *string, uint8_t *md) {
  unsigned h0 = (md[0]<<16) | (md[1]<<8) | md[2];
  unsigned h1 = (md[3]<<16) | (md[4]<<8) | md[5];
  unsigned h2 = (md[6]<<16) | (md[7]<<8) | md[8];
  unsigned h3 = (md[9]<<16) | (md[10]<<8) | md[11];
  unsigned h4 = (md[12]<<16) | (md[13]<<8) | md[14];
  unsigned h5 = (md[15]<<16) | (md[16]<<8) | md[17];
  unsigned h6 = (md[18]<<16) | (md[19]<<8);
  print4(h0, string); print4(h1, &string[4]); print4(h2, &string[8]);
  print4(h3, &string[12]); print4(h4, &string[16]);
  print4(h5, &string[20]); print3(h6, &string[24]);
}

int encrypt(char *in, char *out) {
  uint8_t magic[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  SHA1Context sha;
  uint8_t message_digest[20];
  SHA1Reset(&sha);
  SHA1Input(&sha, (uint8_t*)in, 24);
  SHA1Input(&sha, magic, 36);
  if (SHA1Result(&sha, message_digest)) return 0;	// error
  base64(out, message_digest);
  return 1;
}
