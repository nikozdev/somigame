# basic

NAME?=somigame
VNUM?=0xa0a0a2
TYPE?=RUN

# files

## suf-fix

SRCSUF:=cxx
HDRSUF:=hxx
OBJSUF:=obj
BINSUF:=bin
MANSUF:=man

## source

### directories

FSDLOC?=.

SRCFSD:=$(FSDLOC)/src
HDRFSD:=$(FSDLOC)/src
OBJFSD:=$(FSDLOC)/obj

BINFSD:=$(FSDLOC)/bin
MANFSD:=$(FSDLOC)/man

RSCFSD:=$(FSDLOC)/rsc

### lists

SRCFSL:=$(wildcard $(SRCFSD)/*.$(SRCSUF))
HDRFSL:=$(wildcard $(HDRFSD)/*.$(HDRSUF))
OBJFSL:=$(patsubst $(SRCFSD)/%.$(SRCSUF),$(OBJFSD)/%.$(OBJSUF),$(SRCFSL))

BINFSL:=$(BINFSD)/$(NAME).$(BINSUF)
MANFSL:=$(wildcard $(MANFSD)/*.$(MANSUF))

RSCFSL:= $(wildcard $(RSCFSD)/*.*) $(wildcard $(RSCFSD)/*/*.*)

## target

### locations

FTDLOC?=$(HOME)/.local

### directories

BINFTD:=$(FTDLOC)/bin
MANFTD:=$(FTDLOC)/share/man/man1

### lists

BINFTL:=$(patsubst $(BINFSD)/%.$(BINSUF),$(BINFTD)/%,$(BINFSL))
MANFTL:=$(patsubst $(MANFSD)/%,$(MANFTD)/%,$(MANFSL))

# build

## compiler

CMAKER?= $(shell which g++) -c -o
CFLAGS+= -std=c++20
CFLAGS+= -O0 -g
CFLAGS+= -Wno-initializer-overrides
CFLAGS+= -D_NAME=$(NAME) -D_NAME_STR=\"$(NAME)\"
CFLAGS+= -D_VNUM=$(VNUM) -D_VNUM_STR=\"$(VNUM)\"
CFLAGS+= -D_TYPE_$(TYPE) -D_TYPE_STR=\"$(TYPE)\"
CFLAGS+= $(shell pkg-config --cflags opengl gl glu glut)

## linker

LMAKER?= $(shell which g++) -o
LFLAGS+= $(shell pkg-config --libs opengl gl glu glut)

## libraries

LIBDIR:=$(FSDLOC)/lib
LIBSET:=$(patsubst $(LIBDIR)/%,%,$(wildcard $(LIBDIR)/*))

# terminal

TERMSU:= $(shell which sudo)
TERMCO:= $(shell which chown) -R $(USER):$(USER)
TERMCM:= $(shell which chmod) -R
TERMCP:= $(shell which cp) -riv
TERMRM:= $(shell which rm) -rfv
TERMMV:= $(shell which mv) -iv
TERMMD:= $(shell which mkdir) -p
TERMDB:= $(shell which gdb)

# rules

## internal

build: build-head $(OBJFSL) $(BINFSL)
build-head:
	$(info "[[build]]")
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib build; done

clean: clean-head
	$(TERMRM) $(OBJFSL) $(BINFSL)
clean-head:
	$(info "[[clean]]")
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib clean; done

## external

setup: setup-head $(BINFTL) $(MANFTL)
setup-head:
	$(info "[[setup]]")
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib setup; done

reset: reset-head
	$(TERMRM) $(BINFTL) $(MANFTL)
reset-head:
	$(info "[[reset]]")
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib reset; done

## addition

again: again-head clean build
again-head:
	$(info "[[again]]")
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib again; done

start: start-head build
	@for bin in ${BINFSL}; do $$bin; done
start-head:
	$(info "[[start]]")
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib start; done

rerun: rerun-head again start
rerun-head:
	$(info "[[rerun]]")
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib rerun; done

debug: debug-head again
	@for bin in ${BINFSL}; do $(TERMDB) $$bin; done
debug-head:
	$(info "[[debug]]")
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib debug; done

print: print-head
	$(info [=[basic]=])
	$(info [NAME]=$(NAME))
	$(info [VNUM]=$(VNUM))
	$(info [TYPE]=$(TYPE))
	$(info [=[files]=])
	$(info [==[suffix]==])
	$(info [SRCSUF]=$(SRCSUF))
	$(info [HDRSUF]=$(HDRSUF))
	$(info [OBJSUF]=$(OBJSUF))
	$(info [BINSUF]=$(BINSUF))
	$(info [MANSUF]=$(MANSUF))
	$(info [==[source]==])
	$(info [===[directories]===])
	$(info [FSDLOC]=$(FSDLOC))
	$(info [SRCFSD]=$(SRCFSD))
	$(info [HDRFSD]=$(HDRFSD))
	$(info [OBJFSD]=$(OBJFSD))
	$(info [BINFSD]=$(BINFSD))
	$(info [MANFSD]=$(MANFSD))
	$(info [RSCFSD]=$(RSCFSD))
	$(info [===[lists]===])
	$(info [SRCFSL]=$(SRCFSL))
	$(info [HDRFSL]=$(HDRFSL))
	$(info [OBJFSL]=$(OBJFSL))
	$(info [BINFSL]=$(BINFSL))
	$(info [MANFSL]=$(MANFSL))
	$(info [RSCFSL]=$(RSCFSL))
	$(info [==[target]==])
	$(info [===[locations]===])
	$(info [FTDLOC]=$(FTDLOC))
	$(info [===[directories]===])
	$(info [BINFTD]=$(BINFTD))
	$(info [MANFTD]=$(MANFTD))
	$(info [===[lists]===])
	$(info [BINFTL]=$(BINFTL))
	$(info [MANFTL]=$(MANFTL))
	$(info [=[build]=])
	$(info [==[compiler]==])
	$(info [CMAKER]=$(CMAKER))
	$(info [CFLAGS]=$(CFLAGS))
	$(info [==[linker]==])
	$(info [LMAKER]=$(LMAKER))
	$(info [LFLAGS]=$(LFLAGS))
	$(info [=[terminal]=])
	$(info [TERMSU]=$(TERMSU))
	$(info [TERMCO]=$(TERMCO))
	$(info [TERMCM]=$(TERMCM))
	$(info [TERMCP]=$(TERMCP))
	$(info [TERMRM]=$(TERMRM))
	$(info [TERMMV]=$(TERMMV))
	$(info [TERMMD]=$(TERMMD))
	$(info [TERMDB]=$(TERMDB))
	$(info [=[libraries]=])
	$(info [LIBDIR]=$(LIBDIR))
	$(info [LIBSET]=$(LIBSET))
	$(info [=[rules]=])
	$(info [build]=link binary file from object code compiled from source code)
	$(info [clean]=remove compiled object code and linked binary file)
	$(info [setup]=copy binary and manual files into the system)
	$(info [reset]=remove binary and manual files from the system)
	$(info [again]=clean and rebuild the project again)
	$(info [start]=build and run the binary file)
	$(info [rerun]=clean, rebuild and run the binary file with the shell)
	$(info [debug]=clean, rebuild and run the binary file with the debugger)
	$(info [print]=write this whole text)
print-head:
	$(info [[print]])
	@for lib in ${LIBSET}; do ${MAKE} -C $(LIBDIR)/$$lib print; done

## source

$(SRCFSD)/%.$(SRCSUF):
	$(info "[source]=$@")

$(HDRFSD)/%.$(HDRSUF):
	$(info "[header]=$@")

$(OBJFSD)/%.$(OBJSUF): $(SRCFSD)/%.$(SRCSUF)
	$(info "[object]=$@")
	$(CMAKER) $@ $^ $(CFLAGS)

$(BINFSD)/%.$(BINSUF): $(OBJFSL)
	$(info "[source-binary]=$@")
	$(LMAKER) $@ $^ $(LFLAGS)

$(MANFSD)/%.$(MANSUF):
	$(info "[source-manual]=$@")

## target

$(BINFTD)/%: %.$(BINSUF)
	$(info "[target-binary]=$@")
	$(TERMCP) $< $@
	$(TERMCO) $@
	$(TERMCM) 744 $@

$(MANFTD)/%.$(MANSUF): %.$(MANSUF)
	$(info "[target-manual]=$@")
	$(TERMCP) $< $@
	$(TERMCM) 644 $@

# endof
