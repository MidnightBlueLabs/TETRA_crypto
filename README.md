# TETRA Crypto

## Dependencies

* libsodium-dev

## Build

Command:
```bash
~/TETRA_crypto$ make
```

Output:
```bash
gcc -Wall -O3 -g -c gen_ks.c -o gen_ks.o
gcc -o gen_ks gen_ks.o -ltetracrypto -L.
gcc -Wall -O3 -g -c gen_ta61.c -o gen_ta61.o
gcc -o gen_ta61 gen_ta61.o -ltetracrypto -L.
```

## Test

Command:
```bash
~/TETRA_crypto$ ./tests
```

Output:
```bash
Testing transform_80_to_120_alt...                 [ OK ]
Testing transform_88_to_120...                     [ OK ]
Testing transform_80_to_128...                     [ OK ]
Testing transform_120_to_88...                     [ OK ]
Testing transform_80_to_128_alt...                 [ OK ]
Testing transform_identity...                      [ OK ]
Testing ta11 (aka ta41)...                         [ OK ]
Testing ta21...                                    [ OK ]
Testing ta31...                                    [ OK ]
Testing ta32...                                    [ OK ]
Testing ta51...                                    [ OK ]
Testing ta52...                                    [ OK ]
Testing ta61...                                    [ OK ]
Testing ta61_inv...                                [ OK ]
Testing ta71...                                    [ OK ]
Testing ta81...                                    [ OK ]
Testing ta82...                                    [ OK ]
Testing ta91...                                    [ OK ]
Testing ta92...                                    [ OK ]
Testing tb4...                                     [ OK ]
Testing tb5...                                     [ OK ]
Testing tb6...                                     [ OK ]
Testing tb7...                                     [ OK ]
Testing HURDLE_encrypt...                          [ OK ]
Testing HURDLE_decrypt...                          [ OK ]
Testing TEA1...                                    [ OK ]
Testing TEA2...                                    [ OK ]
Testing TEA3...                                    [ OK ]
```
