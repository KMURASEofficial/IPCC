# ipcc

**Coming soon**
- Version with openssl dependency removed (random number generation)
- Attacker's key search time complexity using backtracking at a specific graph size
- opt(lexical ordering, hashing, bitwise-operation, avx2)

This project manages the IPCC's whitepaper and code. It is currently participating in KpqC (https://www.kpqc.or.kr/competition.html). <br>

**IPCC (first version)**
-------------------------------------------------------
Our first IPCC algorithm is a practical implementation of the Perfect Code Cryptosystem, which is based on graph theory. It is based on the version 2 of the Koblitz algorithm. <br>

**IPCC7 (second version)**
-------------------------------------------------------
While IPCC7 shares the design concept with the first IPCC, it addresses the vulnerabilities of the first one. The algorithm is designed to ensure that the maximum degree of the ciphertext polynomial is 7, hence the number 7 in the name. This is based on version 3 of the Koblitz algorithm. <br>



Test programs
-------------------------------------------------------
**Prerequisites** <br>
Some test programs require OpenSSL. If the OpenSSL header files and/or shared libraries are not located in one of the standard locations on your system, you must specify their location via compiler and linker flags in the environment variable LDFLAGS. <br>
For example, on Mac OS, you can install openssl with the following command: <br>
```
$ brew install openssl
```


**To compile** the test programs on Linux or macOS, go to the `ipcc7/ipcc7_reference_implementation/` directory and run
```
$ make
```
This produces the executables
```
pke
pqc_genkat_pke
```
The parameters of the cryptosystem can be adjusted in "params.h" file. <br>
The executable file `pke` can go through the public key encryption process, <br>
The resulting `pqckat_n.rsp` from the `pqc_genkat_pke` executable can be compared to the `pqckat_n.rsp` file in the `ipcc7/kat/` directory. <br>
**The ciphertext for the integer plaintext `4410` is in the rsp file.**
