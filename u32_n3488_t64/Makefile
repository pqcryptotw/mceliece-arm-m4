
SUPERCOP=../supercop
SCRIPTS=./scripts
DATA=./data

#############################

CC= gcc
CFLAGS = -O3 -Wall -Wextra -march=native -fno-omit-frame-pointer #-g -pg -fsanitize=address
INCLUDE = -I./src -I./extrafiles -I./testkeypair -I./stm32f4flashtool -I$(DATA) -I$(SUPERCOP)
LDFLAGS = -lssl -lcrypto #-lasan #-lkeccak -lsupercop


SS= python
QQ = ../tools/qhasm/qhasm-amd64avx


#ARMCC = arm-none-eabi-gcc-9.2.1
ARMCC = arm-none-eabi-gcc
ARMARCH = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARMCFLAG = -O3 -I./extrafiles -I./src -Wall -Wextra -Wimplicit-function-declaration -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes -Wundef -Wshadow


#############################

SRC:= $(wildcard src/*.c )
SOBJS = $(SRC:.c=.o)
OBJS = $(SOBJS:src/%=%)


#############################

OBJS += test_keypair.o

#############################

#OBJS += transpose-m4speed.o
#OBJS += transpose_16x16.o
#OBJS += keypair_lib-m4speed.o
#OBJS += keypaircontrolbit-m4speed.o
#OBJS += encapdecap2-m4test.o
#OBJS += bm-m4-test.o
#OBJS += encap-components-m4test.o
#OBJS += gf-radix16-m4test.o
#OBJS += bs_gfmul-m4test.o

#OBJS += gfmul-radix16-m4test.o
#OBJS += gf2matmul-m4test.o

#OBJS += matmulvslumul-m4test.o
#OBJS += transpose32x32-m4test.o

#OBJS += test_keypair.o 

#OBJS+= keygen-m4speed.o
#OBJS+= gf2mat-m4test.o
#OBJS+= decrypt-m4speed.o
#OBJS+= randombytes.o

#############################

all: test.run


speed.run: speed
	./speed

speed: speed.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^ $(LDFLAGS)

test.run: test
	./test

test: test.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^  $(LDFLAGS)

%-test: unit-test/%-test.c $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^ -lssl  $(LDFLAGS)

%.o: src/%.c 
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

%.o: experiment-sort/%.c 
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

%.o: stm32f4flashtool/%.c 
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

%.o: testkeypair/%.c 
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

%.o: unit-test/%.c 
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ -c $< 

%.o: m4speed/%.c 
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDE) -c $< 

%.o: $(SUPERCOP)/%.c 
	$(CC) $(CFLAGS) $(INCLUDE) -c $< 

%.S: src/%.c
	$(ARMCC) $(ARMARCH) $(ARMCFLAG) -S -c $< -o $@


#############################

clean:
	-rm *.o 
	-rm *.q
	-rm test
	-rm speed
	-rm *-test
	#-rm *.s 
