/*
 *   httpoui.c
 *
 *   alternative code in http client form to download the OUI db
 *   from the IEEE server.
 *
 *   Orestes Leal Rodriguez, 2018
 */
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#define CHK_SIZ 65536

int main() {

  const char *ieee_srv = "standards-oui.ieee.org";
  const char *http_request = "GET /oui.txt HTTP/1.1\r\nHost: standards-oui.ieee.org\r\nConnection: close\r\n\r\n";
  FILE *http_stream;
  int read;

  struct addrinfo ai, *res;  /* info is returned in 'res', 'ai' is for config */
  int aires;
  int s;
  char buf[CHK_SIZ];    /*  buffer to receive the http response  */


  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket(2)");
    return -1;
  }
 
  bzero(&ai, sizeof ai);

  ai.ai_family = AF_INET;
  ai.ai_socktype = SOCK_STREAM;
  ai.ai_protocol = IPPROTO_TCP;
  ai.ai_flags = AI_ALL | AI_V4MAPPED;

  if ((aires = getaddrinfo(ieee_srv, "http", &ai, &res)) != 0) {
    perror("getaddrinfo(3)");
  }

  if (connect(s, (struct sockaddr *)res->ai_addr, sizeof (struct sockaddr_in)) < 0) {
    perror("connect(2):");
    return -1;
  }

  if ((http_stream = fdopen(s, "w+")) == NULL) {
     perror("fdopen(3)");
     return -1;
  }

  FILE *oui = fopen("oui.txt", "w");

  fprintf(http_stream, "%s", http_request);  /* ~ send  http request  */

  /* 
    XXX: fix the gathering of data and skip the http1.1 header at the top
         also filter by "hex" lines, this can be tricky since this is
         not sed(1) or awk(1) that work on one line at a time

         IDEA: read the chunks into buffers, can be a linked list of buffers
         and for each buffer test that if that line that doesn't contains the
         "(hex)" string is removed. this si for mimic the behavior of filtering
         done by the gawk(1) code inside the networking part in oui.sh
  */

  /* 
   *  grab the response 64K chunks at a time 
   */
  while (1) {
    read = fread(&buf, 1, CHK_SIZ, http_stream);
    if (read == 0) break;
    fwrite(&buf, read, 1, oui); 
  }
  
  fclose(oui);
  close(s); 
  freeaddrinfo(res);
  fclose(http_stream);

  return 0;
}
