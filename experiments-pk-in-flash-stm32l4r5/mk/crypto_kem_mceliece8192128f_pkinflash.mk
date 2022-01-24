

elf/crypto_kem_mceliece8192128f_pkinflash_pkinflashtest.elf: LDSCRIPT=ldscripts/$(PLATFORM)-flashpk.ld

elf/crypto_kem_mceliece8192128f_pkinflash_pkinflashspeed.elf: LDSCRIPT=ldscripts/$(PLATFORM)-flashpk.ld



tests: elf/crypto_kem_mceliece8192128f_pkinflash_pkinflashspeed.elf elf/crypto_kem_mceliece8192128f_pkinflash_pkinflashtest.elf
tests-bin: bin/crypto_kem_mceliece8192128f_pkinflash_pkinflashspeed.bin  bin/crypto_kem_mceliece8192128f_pkinflash_pkinflashtest.bin

