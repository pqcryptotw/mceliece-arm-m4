# Classic McEliece implementation for ARM-Cortex M4


## There are 4 directories w.r.t. 4 parameters of Classic McEliece:
- **u32_n3488_t64**: mceliece n3488 t64
- **u32_n4608_t96**: mceliece n4608 t96
- **u32_n6688_t128**: mceliece n6688 t128
- **u32_n8192_t128**: mceliece n8192 t128


- For key-generation with f parameters, please edit the macro //#define _F_PARAM_ in *imple*/src/run_config.h


  All implementations contain a KEM function tester in *imple*/unit-test/test.c .  
  By simply typing  
  'make'  
  in each directory under the Gnu C development environment, it will test
  the functionality and correctness of key-generation, encapsulation, and decapsulation.  



## Instructions for benchmarking on the stm32f4-discovery with **PQM4**

The code is tested and benchmarked on the stm32f4-discovery with PQM4.
The followings are the instructions for testing:


- Download/Install pqm4. https://github.com/mupq/pqm4

- Setup your stm32f4-discovery device (connect USB, setup terminal, etc.)

- Execute 'pyton3 copy_files_to_pqm4.py *path_to_pqm4*' for copying files to **PQM4** directory.  
  It does:  
  1. Copy various testers (in the directory *param*/pqm4tester) to your **PQM4** directory ( assuming in ~/pqm4 ):  
  'cp pqm4tester/* ~/pqm4/mupq/crypto_kem/'  

  2. Create a working directory and copy files :  
    a). Testing key-generation  
    'mkdir ~/pqm4/crypto_kem/mc348864'  
    'cp u32_n3488_t64/src/* ~/pqm4/crypto_kem/mc348864keygen/'  
    'cp pqm4tester/ld_script_for_flash.ld ~/pqm4/ldscripts/crypto_kem_mc348864keygen.ld' <-- loader script for enabling flash disk  
    Note the xxxx in crypto_kem_xxx.ld should be the same as the name of directory pqm4/crypto_kem/xxxx .   

    b). Testing with test keypair  
    'mkdir ~/pqm4/crypto_kem/mc348864'  
    'cp u32_n3488_t64/src/* ~/pqm4/crypto_kem/mc348864/'  
    'cp u32_n3488_t64/testkeypair/* ~/pqm4/crypto_kem/mc348864/'   <-- test key pair


- Generate binary(.bin) file for flashing  wrt various benchmarking scenarios:  
    cwd:   cd ~/pqm4/  
    1. for benchmarking key-generation with flash key storage:  
    'make IMPLEMENTATION_PATH=crypto_kem/mc348864keygen bin/crypto_kem_mc348864keygen_speed0.bin'  
    2. for benchmarking encap/decap with test keypair:  
    'make IMPLEMENTATION_PATH=crypto_kem/mc348864 bin/crypto_kem_mc348864_speed1.bin'  
    3. for benchmarking decode with test keypair:  
    'make IMPLEMENTATION_PATH=crypto_kem/mc8192128 bin/crypto_kem_mc8192128_speed2.bin'  

    4. for stack usage of key-generation with flash key storage:  
    'make IMPLEMENTATION_PATH=crypto_kem/mc348864keygen bin/crypto_kem_mc348864keygen_stack0.bin'  
    5. for stack usage of encap/decap with test keypair:  
    'make IMPLEMENTATION_PATH=crypto_kem/mc348864 bin/crypto_kem_mc348864_stack1.bin'  
    6. for stack usage decode with test keypair:  
    'make IMPLEMENTATION_PATH=crypto_kem/mc8192128 bin/crypto_kem_mc8192128_stack2.bin'  


- Write binary file to stm32f4 and read the results from UART port of the device:  
  'st-flash write bin/crypto_kem_mc348864_speed1.bin 0x8000000'  



## 6 modified testers from **PQM4**
  - There are 6 modified testers, using keypar from the flash, in the directory *pqm4tester* .

  1. speed0.c : benchmarking key-generation.  
     The tester reports 100 cycle counts for key-generation and the average.  
     Applied implementations: **u32_n3488_t64_f** and **u32_n3488_t64** .   
     Since the tester uses flash for storing public key,  
     one has to link with the loader script *pqm4tester/crypto_kem_mc348864.ld* to enable to flash disk for this tester.  

  2. speed1.c : benchmarking encap and decap with testing keypair.  
     The tester reports the cycle counts for decap and  average cycle counts for encap.  
     Applied implementations: **u32_n3488_t64_f** ,  **u32_n3488_t64** and **u32_n4608_t96** .   
      
  3. speed2.c : benchmarking decapsulation with testing keypair.  
     The tester reports the cycle counts for decapsulation. 
     Applied implementations: **u32_n6688_t96** and **u32_n8192_t128** .   

  4. stack0.c : reporting the stack usage for key-generation.  
     Applied implementations: **u32_n3488_t64_f** and **u32_n3488_t64** .   
     Since the tester uses flash for storing public key,  
     one has to link with the loader script *pqm4tester/crypto_kem_mc348864.ld* to enable to flash disk for this tester.  

  5. stack1.c : reporting the stack usage for encap and decap with testing keypair.  
     Applied implementations: **u32_n3488_t64_f** , **u32_n3488_t64** , and **u32_n4608_t96** .   

  6. stack2.c : reporting the stack usage for decapsulation with testing keypair.   
     Applied implementations: **u32_n6688_t96** and **u32_n8192_t128** .   

  - All tester are running on 168 MHz, instead of 20 MHz with original testers from **PQM4**.


