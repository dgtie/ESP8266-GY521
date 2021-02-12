/*
#include <stdio.h>

void websocket_init();
char *websocket(char *b, int *len);

char string1[] = 
"GET / HTTP/1.1\r\n"
"Host: 192.168.50.236:3333\r\n"
"Connection: Upgrade\r\n"
"Pragma: no-cache\r\n"
"Cache-Control: no-cache\r\n"
"User-Agent: Mozilla/";

char string2[] = 
"5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36\r\n"
"Upgrade: websocket\r\n"
"Origin:"; 

char string3[] = 
"file://\r\n"
"Sec-WebSocket-Version: 13\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\n"
"Sec-WebSocket-";

char string4[] = 
"Key: PFiYMqZQZkDWyogNwtWRNQ==\r\n"
"Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n\r\n";

char string5[] = "hi hi\r\n";

int main() {
  char *ptr;
  websocket_init();
  int len;
  len = sizeof(string1)-1;
  ptr = websocket(string1, &len);
  if (ptr) printf("string1: %s\n", ptr);
  len = sizeof(string2)-1;
  ptr = websocket(string2, &len);
  if (ptr) printf("string2: %s\n", ptr);
  len = sizeof(string3)-1;
  ptr = websocket(string3, &len);
  if (ptr) printf("string3: %s\n", ptr);
  len = sizeof(string4)-1;
  ptr = websocket(string4, &len);
  if (ptr) { ptr[len] = 0; printf("string4: %s\n", ptr); }
  len = sizeof(string5)-1;
  ptr = websocket(string5, &len);
  if (ptr) { ptr[len] = 0; printf("string5: %s\n", ptr); }
}
//*/

void search(const char *p);
int found(void);	// 0 = found; -1 = not found
const char *scan(char c);
int encrypt(char*, char*);

static char *ptr;

static char response[] =
"HTTP/1.1 101 Switching Protocols\r\n"
"Upgrade: websocket\r\n"
"Connection: Upgrade\r\n"
"Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n\r\n";
//"Sec-WebSocket-Protocol: chat\r\n\r\n";
//97 = s3pPL...

static int connected;

static char mpu_data[16] = { 0x82, 14, 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };

static void handshake(char *b, int len) {
  int i = 0;
  if (found() == -1)
    for (i = 0; i < len; i++)
      if (!scan(b[i])) { i++; ptr = &response[97]; break; }
  if (found() == 0) {
    while (i < len) {
      *ptr = b[i++];
      if (*ptr == '\r') {
        *ptr = 0;
        encrypt(&response[97], &response[97]);
        connected = 1;
        break;
      } else ptr++;
    }
  }
}

static char *unmask(char *p) {
  int mask = 0, len = p[1] & 127;
  char *q;
  if (*p != (char)0x81) return 0;
  if (p[1] & 0x80) {
    q = &p[6];
    p = &p[2];
    for (int i = 0; i < len; i++) {
      q[i] ^= p[mask++];
      mask &= 3;
    }
  } else {
    q = &p[2];
    q[len] = 0;
  }
  return q;
}

void websocket_init() {
  search("Sec-WebSocket-Key: ");
  connected = 0;
}

char *websocket(char *b, int *len) {
  if (connected) {
    unmask(b);
    b[4] = b[0];
    b[5] = b[1] & 127;
//    *len = b[5] ? b[5] + 2 : 0;	// return 0 to disconnect
//    return &b[4];
    *len = b[5] ? 16 : 0;	// return 0 to disconnect
    return mpu_data;
  } else {
    handshake(b, *len);
    *len = sizeof(response) - 1;
    return connected ? response : 0;
  }
}

void save_mpu_data(char *d) {
  for (int i = 0; i < 14; i++) mpu_data[i+2] = d[i];
}
