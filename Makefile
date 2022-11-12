OSNAME = Serve

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
ASMC=nasm
CC = clang++

INCDIR=headers
CFLAGS = -I$(INCDIR) -Werror -Ofast -fshort-wchar -mno-red-zone
ASMFLAGS = -I$(INCDIR)
LDFLAGS = -static -Bsymbolic

SRCDIR := source
OBJDIR := build/OBJ
BUILDDIR = build/BIN
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%_cpp.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

$(OBJDIR)/%_cpp.o:$(SRCDIR)/%.cpp
	@echo ##$^ -> $@
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@


$(BUILDDIR)/$(OSNAME): $(OBJS)
	clang++ $(LDFLAGS) -o $@ $^

.PHONY: clean run compile setup push
setup:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(SRCDIR)
	@mkdir -p $(OBJDIR)

clean:
	@rm -frv $(OBJDIR)/* $(BUILDDIR)/*


compile: $(BUILDDIR)/$(OSNAME)

push: clean compile
	git add .
	git commit -am "$(shell date)" 
	git push "https://loganer%40vivaldi.net:$(shell cat ../git_token.txt)@github.com/Tarcaxoxide/project_serve.git"

run: clean compile
	./$(BUILDDIR)/$(OSNAME) ${URL}
	
