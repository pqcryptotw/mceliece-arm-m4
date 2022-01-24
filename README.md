# Classic McEliece implementation for ARM-Cortex M4

This is an implementation of Classic McEliece based on the CHES 2021 paper
``Classic McEliece on the ARM Cortex-M4'' (https://tches.iacr.org/index.php/TCHES/article/view/8970).

## Authors

Ming-Shing Chen (IIS, Academia Sinica) and Tung Chou (CITI, Academia Sinica)

## Licence

All implementations are in the public domain.


## Contents

- **pqm4-projects**: Projects for PQM4(https://github.com/mupq/pqm4).  
 [Note] The implementation generates the same KATs as the R3 submisions in https://classic.mceliece.org/ .  

- **experiments-pk-in-flash-stm32l4r5**: Experiments for storing the public key in the flash memory on STM32L4R5.  


## Results:

We produce the following results with the PQM4. (version: 2691b4915 Nov/29/2021 and arm-none-eabi-gcc (15:9-2019-q4-0ubuntu2) 9.2.1)


### Stack usage:

Benchamrked on the mps2-an386 simulator with the mupq/crypto_kem/stack.c in the PQM4.  

| Parameter        | keypair | encaps | decaps  |
| :---             |   ---:  |  ---:  |    ---: |
| mceliece348864f  | 170880  | 1420   | 18500   |
| mceliece348864   | 170880  | 1420   | 18500   |
| mceliece460896f  | 400056  | 2004   | 34956   |
| mceliece460896   | 397624  | 2004   | 34956   |
| mceliece6688128f | 581896  | 3996   | 35716   |
| mceliece6688128  | 578640  | 3996   | 35716   |
| mceliece6960119f | 510340  | 3948   | 35764   |
| mceliece6960119  | 510276  | 3948   | 35764   |
| mceliece8192128f | 588736  | 4116   | 36100   |
| mceliece8192128  | 585480  | 4116   | 36100   |

These numbers are the measurements of stack usage of related functions. It does not contain the key pair storage provided by the caller functions.  

### Speed


Benchamrked on the STM32L4R5 board with the code mupq/crypto_kem/speed.c or mupq/crypto_kem/pkinflashspeed.c .  
CPU Clock: 16,000,000  
Public keys of mceliece460896f, mceliece6688128f, and mceliece8192128f are stored in the flash memory.   

| Parameter        | keypair cycles|keypair (real time) | encaps cycles | decaps cycles |
| :--              |            --:| --:                |       ---:    |          ---: |
| mceliece348864f  | 1289567934    | (80.60sec)         | 520301        | 2409014       |
| mceliece460896f  | 4216263186    |(263.52sec)         |        919916 |       5787440 |
| mceliece6688128f | 7435609623    |(464.73sec)         |       2123336 |       6696367 |
| mceliece6960119f | 9532764749    |(595.80sec)         |       3876811 |       6420361 |
| mceliece8192128f | 8473271737    |(529.58sec)         |      3445655  |       6762145 |



CPU Clock: 120,000,000  
Public keys of mceliece460896f, mceliece6688128f, and mceliece8192128f are stored in the flash memory.   

| Parameter        | keypair cycles | keypair (real time) | encaps cycles | decaps cycles |
| :--              |---------------:|--------------------:|       ---:    |          ---: |
| mceliece348864f  |  1298845375    |     (10.82sec)      |        627088 |      4213438  |
| mceliece460896f  |  5109945366    |     (42.58sec)      |       1484238 |     10315592  |
| mceliece6688128f |  9210630170    |     (76.76sec)      |       2931480 |     11327907  |
| mceliece6960119f | 11313040532    |     (94.28sec)      |       4589508 |     11024607  |
| mceliece8192128f | 10773637011    |     (89.78sec)      |       4545305 |     11400390  |

The **non-f** parameters use reject sampling for key generation. It consumes about 1.X times clock cycles comparing to
the -f parameters. The tow parameters use the same encap andn decap functions.  


## Instructions for benchmarking

Common instructions:


- Download/Install pqm4. https://github.com/mupq/pqm4  
```
git clone --recursive https://github.com/mupq/pqm4.git ~/pqm4
```


### Benchmarking stack usage with the mps2-an386 simulator.

- Copy files to the pqm4 directory.  
```
cp -r pqm4-projects/crypto_kem  ~/pqm4/
```
  [Caution] For users do not use pqm4: Since the code relies on the predefined macro **PQM4** on pqm4 for conditional compilation,
  please add the **PQM4** macro in src/run_config.h to enable the M4 assembly code.  


- Build projects for mps2-an386 platform:  
```
make PLATFORM=mps2-an386 IMPLEMENTATION_PATH=crypto_kem/mceliece348864f/ches2021
```

- Run the tester with the simulator:  
```
qemu-system-arm -M mps2-an386 -nographic -semihosting -kernel elf/crypto_kem_mceliece348864f_ches2021_test.elf
```

-  Benchmarking the stack usage:  
```
qemu-system-arm -M mps2-an386 -nographic -semihosting -kernel elf/crypto_kem_mceliece348864f_ches2021_stack.elf
```


### Benchmarking speed on the stm32l4r5 board.

- Build projects for the stm32l4r5 platform:  
```
make PLATFORM=nucleo-l4r5zi IMPLEMENTATION_PATH=crypto_kem/mceliece348864f/ches2021
```

- Connect the device to the host pc and connect to its terminal with the script hostside/host_unidirectional.py  
  [Caution] One might have to change the device file name (/dev/ttyXXXX) in the script to connect to correct device.
```
python3  hostside/host_unidirectional.py
```

- Write the binary to the board and run the test:  
```
st-flash write bin/crypto_kem_mceliece348864f_ches2021_test.bin 0x8000000
```


### Benchmarking speed with storing public key on the stm32l4r5 board.

- Copy files to the pqm4 directory.
```
cp -r experiments-pk-in-flash-stm32l4r5/crypto_kem ~/pqm4/
cp -r experiments-pk-in-flash-stm32l4r5/mk         ~/pqm4/
cp -r experiments-pk-in-flash-stm32l4r5/mupq       ~/pqm4/
cp -r experiments-pk-in-flash-stm32l4r5/common     ~/pqm4/
cp -r experiments-pk-in-flash-stm32l4r5/ldscripts  ~/pqm4/
```

- Build projects for the stm32l4r5 platform:  
```
make PLATFORM=nucleo-l4r5zi IMPLEMENTATION_PATH=crypto_kem/mceliece460896f/pkinflash
```

- Connect the device to the host pc and connect to its terminal with the script hostside/host_unidirectional.py  
  [Caution] One might have to change the device file name (/dev/ttyXXXX) in the script to connect to correct device.
```
python3  hostside/host_unidirectional.py
```

- Write the binary to the board and run the test:  
```
st-flash write bin/crypto_kem_mceliece460896f_ches2021_flashinpkspeed.bin 0x8000000
```
