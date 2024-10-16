#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>

// gcc md5cycle.c -o md5cycle.bin -lssl -lcrypto -O3
// (sudo apt-get install libssl-dev for evp.h)

//===============================
#define HASH_NAME "md5"
#define HASH_LEN 16
//===============================
//#define HASH_NAME "md4"
//#define HASH_LEN 16
//===============================
//#define HASH_NAME "sha1"
//#define HASH_LEN 20
//===============================
//#define HASH_NAME "sha224"
//#define HASH_LEN 28
//===============================
//#define HASH_NAME "sha256"
//#define HASH_LEN 32
//===============================
//#define HASH_NAME "sha384"
//#define HASH_LEN 48
//===============================
//#define HASH_NAME "sha512"
//#define HASH_LEN 64
//===============================
//#define HASH_NAME "ripemd160"
//#define HASH_LEN 20
//===============================
//#define HASH_NAME "sha3-224"
//#define HASH_LEN 28
//===============================
//#define HASH_NAME "sha3-256"
//#define HASH_LEN 32
//===============================
//#define HASH_NAME "sha3-384"
//#define HASH_LEN 48
//===============================
//#define HASH_NAME "sha3-512"
//#define HASH_LEN 64
//===============================

int hash_hex(unsigned char *binhexout) {
  int i;
  for (i=0 ; i<HASH_LEN ; i++) {
    sprintf((unsigned char *)&binhexout[HASH_LEN + 2*i], "%02x", (unsigned int)binhexout[i]);
  }
}
int hash(char *msg, unsigned char *binhexout, const long msglen, const EVP_MD *digest) {
  EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
  EVP_DigestInit_ex(mdctx, digest, NULL);
  EVP_DigestUpdate(mdctx, &msg[0], msglen);
  int md_len;
  EVP_DigestFinal_ex(mdctx, binhexout, &md_len);
  EVP_MD_CTX_destroy(mdctx);
  EVP_cleanup();
  /*
  if (HASH_LEN != md_len) {
    printf("Recompile with HASH_LEN %i\n", md_len);
    exit(1);
  }
  for (i=0 ; i<md_len ; i++) {
    sprintf((unsigned char *)&binhexout[HASH_LEN + 2*i], "%02x", (unsigned int)binhexout[i]);
  }
  */
}

static inline int nexthash(EVP_MD_CTX *mdctx, unsigned char *md_binhex, unsigned char *input, unsigned char *output, long inputmsglen, long matchlen) {
  EVP_DigestUpdate(mdctx, input, inputmsglen);
  EVP_DigestFinal_ex(mdctx, md_binhex, NULL);
  int j=0, i=0;
  for(i=inputmsglen-matchlen; i<inputmsglen; i++) {
    output[i] = 0x41 + (md_binhex[j] >> 4);
    i++;
    output[i] = 0x41 + (md_binhex[j] & 0xf);
    j++;
  }
  output[inputmsglen] = 0;
}

int main (int argc, char* argv[]) {

  OpenSSL_add_all_digests();
  const EVP_MD *usedigest = EVP_get_digestbyname(HASH_NAME);
  unsigned char *md_binhex = (unsigned char *)calloc(1, 3*HASH_LEN + 1);
  if (argc != 3) {
    printf("This program tests that string is in a md5 hashing cycle where the final numchars are replaced each iteration with the first 16 letters of the uppercase alphabet derived from the hex.\n");
    printf("Usage %s string numchars\n", argv[0]);
    printf("e.g. %s SimonGoaterBPEOPLCGICNMME 14\n", argv[0]);
    exit(0);
  }
  unsigned char *inputmsg = argv[1];
  long inputmsglen = strlen(inputmsg);
  long matchlen = atol(argv[2]);
  long elementcount = 1;
  unsigned char *temp;
  unsigned char *input = calloc(1, inputmsglen + 1);
  unsigned char *output = calloc(1, inputmsglen + 1);
  //unsigned char *outputsfx = output + (inputmsglen - matchlen);
  //unsigned char *inputsfx = input + (inputmsglen - matchlen);
  //unsigned char *inputmsgsfx = inputmsg + (inputmsglen - matchlen);
  memcpy(input, inputmsg, inputmsglen);
  memcpy(output, inputmsg, inputmsglen - matchlen);
  long starttime = time(0);
  EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
  EVP_DigestInit_ex(mdctx, usedigest, NULL);
  nexthash(mdctx, md_binhex, input, output, inputmsglen, matchlen);
  temp = input;
  input = output; 
  output = temp;
  while(strcmp(input, inputmsg) != 0) {
    EVP_DigestInit_ex(mdctx, usedigest, NULL);
    nexthash(mdctx, md_binhex, input, output, inputmsglen, matchlen);
    //temp = inputsfx;
    //inputsfx = outputsfx; 
    //outputsfx = temp;
    temp = input;
    input = output; 
    output = temp;
    //memcpy(inputsfx, outputsfx, matchlen);
    elementcount++;
  }  
  EVP_MD_CTX_destroy(mdctx);
  EVP_cleanup();
  long endtime = time(0);
  //printf("%s\n", output);
  printf("Cycle for %s confirmed with %li elements.\n", inputmsg, elementcount);
  printf("Previous element is = %s \n", temp);
  if (endtime > starttime) printf("md5 Hash Rate = %f MH/s\n", (float)elementcount/(1000000UL*(endtime-starttime)));
}
