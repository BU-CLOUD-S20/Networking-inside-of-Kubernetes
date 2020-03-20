// warm remind: gcc hash4String.cpp -lstdc++ -o hash4String

#include <iostream>   
#include <string> 
#include <typeinfo>  // check variable's type


#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define F 37 /* also prime, F means FIrst */

// have hashing functions involving primes
void hash(std::string longStr) {

  int seed = 131; 
  
  unsigned temp = F;
  
  for(int i = 0; i < longStr.length(); i++) {
    temp = (temp * A) + B;
  }
  
  auto ans = temp % C;

  std::cout << ans << std::endl;
}

int main() {
  std::string longStr = "abcdefghijklmnopqrstuvwxyz111";

  hash(longStr);

  return 0;
}