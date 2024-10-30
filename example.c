
#include <stdio.h>
#include <simple-cypher/simple-cypher.h>

#define PLAINTEXT "Hello!"

int main (){

  char encrypted[sizeof(PLAINTEXT)] = {0};
  simple_cypher cypher;
  simple_cypher_setup(0x123, &cypher);
  simple_cypher_encrypt_decrypt(PLAINTEXT, sizeof(PLAINTEXT) -1, &cypher, encrypted);
  printf("encrypted=%s\n", encrypted);
  
  char decrypted[sizeof(PLAINTEXT)] = {0};
  simple_cypher cypher2;
  simple_cypher_setup(0x123, &cypher2);
  simple_cypher_encrypt_decrypt(encrypted, sizeof(encrypted) -1, &cypher2, decrypted);
  printf("decrypted=%s\n", decrypted); 
  
  return 0;

}
