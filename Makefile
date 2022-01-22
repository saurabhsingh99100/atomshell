bin_dir = bin
obj_dir = obj
src_dir = src
inc_dir = include

Target = hydrogensoc
Target_allcaps = $(shell echo $(Target) | tr  '[:lower:]' '[:upper:]')

CC = riscv64-unknown-elf-gcc
CFLAGS = -mabi=ilp32 -march=rv32i -nostartfiles -ffreestanding -I $(inc_dir) -I $(RVATOM_LIB)/include -T $(RVATOM_LIB)/link/link_hydrogensoc.ld -DTARGET_$(Target_allcaps)

default: $(bin_dir)/atomshell.elf

CSRCS = $(src_dir)/atomshell.c
CHEADERS = $(shell find $(inc_dir) -name '*.h')

$(obj_dir)/atomshell.o: $(CSRCS) $(CHEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

$(bin_dir)/atomshell.elf: $(obj_dir)/atomshell.o
	$(CC) $(CFLAGS) -L ${RVATOM_LIB}  $^ -o $@ -lcatom -Wl,--gc-sections


.PHONY: run
run:
	atomsim --vuart $(RVATOM)/simport $(bin_dir)/atomshell.elf --maxitr 9999999999

.PHONY: copy
copy:
	cp $(RVATOM)/build/init/* $(RVATOM)/boards/spartan6-mini/

.PHONY: clean
clean:
	rm -f $(bin_dir)/*
	rm -f $(obj_dir)/*