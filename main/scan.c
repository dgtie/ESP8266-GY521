/*
#include <stdio.h>

void search(const char *p);
int found(void);	// 0 = found; -1 = not found
const char *scan(char c);

char string[] =
"GET /chat HTTP/1.1\r\n"
"Host: server.example.com\r\n"
"Upgrade: websocket\r\n"
"Connection: Upgrade\r\n"
"Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
"Sec-WebSocket-Protocol: chat, superchat\r\n"
"Sec-WebSocket-Version: 13\r\n"
"Origin: http://example.com\r\n";

const char string1[] = "Sec-WebSocket-Key: ";

int main() {
  int i;
  search(string1);
  for (i = 0; i < sizeof(string); i++) {
    scan(string[i]);
    if (found() == 0) break;
  }
  if (i < sizeof(string)) printf("found\n");
  else printf("not found\n");
  return 0;
}
//*/


static const char *pattern, *ptr;

static int check(const char *p, const char *q) {
  while (q >= pattern) if (*q-- != *p--) return -1;
  return 0;
}

const char *scan(char c) {
  if (c == *ptr) {
    if (!*++ptr) ptr = 0; // found
  } else {
    const char *q = ptr;
    while (q >= pattern) if (*q-- == c) {
      if (check(ptr, q) == -1) continue;
      ptr = q + 2;
      return ptr;
    }
    ptr = pattern;
  }
  return ptr;
}

void search(const char *p) { pattern = ptr = p; }

int found(void) { return ptr ? -1 : 0; }
