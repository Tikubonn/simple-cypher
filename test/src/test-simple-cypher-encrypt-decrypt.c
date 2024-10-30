
#include <stddef.h>
#include <stdint.h>
#include <simple-cypher/simple-cypher.h>
#include "test.h"
#include "test-simple-cypher-encrypt-decrypt.h"

static void testcase (){
  uint8_t plaintext[256];
  for (size_t index = 0; index < sizeof(plaintext); index++){
    plaintext[index] = 0;
  }
  simple_cypher cypher;
  simple_cypher_setup(0x123abc, &cypher);
  uint8_t encrypteddata[256];
  simple_cypher_encrypt_decrypt(plaintext, sizeof(plaintext), &cypher, encrypteddata);
  size_t samecount = 0;
  for (size_t index = 0; index < sizeof(encrypteddata); index++){
    samecount += encrypteddata[index] == plaintext[index];
  }
  TEST(samecount < 256, "samecount = %zu", samecount);
  simple_cypher cypher2;
  simple_cypher_setup(0x123abc, &cypher2);
  uint8_t decrypteddata[256];
  simple_cypher_encrypt_decrypt(encrypteddata, sizeof(encrypteddata), &cypher2, decrypteddata);
  for (size_t index = 0; index < sizeof(decrypteddata); index++){
    TEST(decrypteddata[index] == plaintext[index], "%u == %u", decrypteddata[index], plaintext[index]);
  }
}

static void testcase2 (){
  uint8_t plaintext[256];
  for (size_t index = 0; index < sizeof(plaintext); index++){
    plaintext[index] = index;
  }
  simple_cypher cypher;
  simple_cypher_setup(0x123abc, &cypher);
  uint8_t encrypteddata[256];
  simple_cypher_encrypt_decrypt(plaintext, sizeof(plaintext), &cypher, encrypteddata);
  size_t samecount = 0;
  for (size_t index = 0; index < sizeof(encrypteddata); index++){
    samecount += encrypteddata[index] == plaintext[index];
  }
  TEST(samecount < 256, "samecount = %zu", samecount);
  simple_cypher cypher2;
  simple_cypher_setup(0x123abc, &cypher2);
  uint8_t decrypteddata[256];
  simple_cypher_encrypt_decrypt(encrypteddata, sizeof(encrypteddata), &cypher2, decrypteddata);
  for (size_t index = 0; index < sizeof(decrypteddata); index++){
    TEST(decrypteddata[index] == plaintext[index], "%u == %u", decrypteddata[index], plaintext[index]);
  }
}

void test_simple_cypher_encrypt_decrypt (){
  testcase();
  testcase2();
}
