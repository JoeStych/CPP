#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>
#include <sys/types.h>
#include <errno.h>

#define PORT 25568
#define SERVERPORTSTR "25568"
#define SERVERIP "199.17.28.80"
#define SERVERNAME "ahscentos"
