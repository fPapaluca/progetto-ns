#
# OMNeT++/OMNEST Makefile for progetto-ns
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -KINET4_PROJ=../inet4 -DINET_IMPORT -I. -I$$\(INET4_PROJ\)/src -L$$\(INET4_PROJ\)/src -lINET$$\(D\)
#

# Name of target to be created (-o option)
TARGET = progetto-ns$(D)$(EXE_SUFFIX)
TARGET_DIR = .

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I. -I$(INET4_PROJ)/src

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)$(INET4_PROJ)/src  -lINET$(D)

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cpp, .msg and .sm files
OBJS = \
    $O/cryptopp850/3way.o \
    $O/cryptopp850/adler32.o \
    $O/cryptopp850/algebra.o \
    $O/cryptopp850/algparam.o \
    $O/cryptopp850/allocate.o \
    $O/cryptopp850/arc4.o \
    $O/cryptopp850/aria.o \
    $O/cryptopp850/aria_simd.o \
    $O/cryptopp850/ariatab.o \
    $O/cryptopp850/asn.o \
    $O/cryptopp850/authenc.o \
    $O/cryptopp850/base32.o \
    $O/cryptopp850/base64.o \
    $O/cryptopp850/basecode.o \
    $O/cryptopp850/bfinit.o \
    $O/cryptopp850/blake2.o \
    $O/cryptopp850/blake2b_simd.o \
    $O/cryptopp850/blake2s_simd.o \
    $O/cryptopp850/blowfish.o \
    $O/cryptopp850/blumshub.o \
    $O/cryptopp850/camellia.o \
    $O/cryptopp850/cast.o \
    $O/cryptopp850/casts.o \
    $O/cryptopp850/cbcmac.o \
    $O/cryptopp850/ccm.o \
    $O/cryptopp850/chacha.o \
    $O/cryptopp850/chacha_avx.o \
    $O/cryptopp850/chacha_simd.o \
    $O/cryptopp850/chachapoly.o \
    $O/cryptopp850/cham.o \
    $O/cryptopp850/cham_simd.o \
    $O/cryptopp850/channels.o \
    $O/cryptopp850/cmac.o \
    $O/cryptopp850/cpu.o \
    $O/cryptopp850/crc.o \
    $O/cryptopp850/crc_simd.o \
    $O/cryptopp850/cryptlib.o \
    $O/cryptopp850/darn.o \
    $O/cryptopp850/default.o \
    $O/cryptopp850/des.o \
    $O/cryptopp850/dessp.o \
    $O/cryptopp850/dh.o \
    $O/cryptopp850/dh2.o \
    $O/cryptopp850/dll.o \
    $O/cryptopp850/dlltest.o \
    $O/cryptopp850/donna_32.o \
    $O/cryptopp850/donna_64.o \
    $O/cryptopp850/donna_sse.o \
    $O/cryptopp850/dsa.o \
    $O/cryptopp850/eax.o \
    $O/cryptopp850/ec2n.o \
    $O/cryptopp850/eccrypto.o \
    $O/cryptopp850/ecp.o \
    $O/cryptopp850/elgamal.o \
    $O/cryptopp850/emsa2.o \
    $O/cryptopp850/eprecomp.o \
    $O/cryptopp850/esign.o \
    $O/cryptopp850/files.o \
    $O/cryptopp850/filters.o \
    $O/cryptopp850/fips140.o \
    $O/cryptopp850/fipsalgt.o \
    $O/cryptopp850/fipstest.o \
    $O/cryptopp850/gcm.o \
    $O/cryptopp850/gcm_simd.o \
    $O/cryptopp850/gf256.o \
    $O/cryptopp850/gf2_32.o \
    $O/cryptopp850/gf2n.o \
    $O/cryptopp850/gf2n_simd.o \
    $O/cryptopp850/gfpcrypt.o \
    $O/cryptopp850/gost.o \
    $O/cryptopp850/gzip.o \
    $O/cryptopp850/hc128.o \
    $O/cryptopp850/hc256.o \
    $O/cryptopp850/hex.o \
    $O/cryptopp850/hight.o \
    $O/cryptopp850/hmac.o \
    $O/cryptopp850/hrtimer.o \
    $O/cryptopp850/ida.o \
    $O/cryptopp850/idea.o \
    $O/cryptopp850/integer.o \
    $O/cryptopp850/iterhash.o \
    $O/cryptopp850/kalyna.o \
    $O/cryptopp850/kalynatab.o \
    $O/cryptopp850/keccak.o \
    $O/cryptopp850/keccak_core.o \
    $O/cryptopp850/keccak_simd.o \
    $O/cryptopp850/lea.o \
    $O/cryptopp850/lea_simd.o \
    $O/cryptopp850/luc.o \
    $O/cryptopp850/mars.o \
    $O/cryptopp850/marss.o \
    $O/cryptopp850/md2.o \
    $O/cryptopp850/md4.o \
    $O/cryptopp850/md5.o \
    $O/cryptopp850/misc.o \
    $O/cryptopp850/modes.o \
    $O/cryptopp850/mqueue.o \
    $O/cryptopp850/mqv.o \
    $O/cryptopp850/nbtheory.o \
    $O/cryptopp850/neon_simd.o \
    $O/cryptopp850/oaep.o \
    $O/cryptopp850/osrng.o \
    $O/cryptopp850/padlkrng.o \
    $O/cryptopp850/panama.o \
    $O/cryptopp850/pch.o \
    $O/cryptopp850/pkcspad.o \
    $O/cryptopp850/poly1305.o \
    $O/cryptopp850/polynomi.o \
    $O/cryptopp850/ppc_power7.o \
    $O/cryptopp850/ppc_power8.o \
    $O/cryptopp850/ppc_power9.o \
    $O/cryptopp850/ppc_simd.o \
    $O/cryptopp850/pssr.o \
    $O/cryptopp850/pubkey.o \
    $O/cryptopp850/queue.o \
    $O/cryptopp850/rabbit.o \
    $O/cryptopp850/rabin.o \
    $O/cryptopp850/randpool.o \
    $O/cryptopp850/rc2.o \
    $O/cryptopp850/rc5.o \
    $O/cryptopp850/rc6.o \
    $O/cryptopp850/rdrand.o \
    $O/cryptopp850/rdtables.o \
    $O/cryptopp850/rijndael.o \
    $O/cryptopp850/rijndael_simd.o \
    $O/cryptopp850/ripemd.o \
    $O/cryptopp850/rng.o \
    $O/cryptopp850/rsa.o \
    $O/cryptopp850/rw.o \
    $O/cryptopp850/safer.o \
    $O/cryptopp850/salsa.o \
    $O/cryptopp850/scrypt.o \
    $O/cryptopp850/seal.o \
    $O/cryptopp850/seed.o \
    $O/cryptopp850/serpent.o \
    $O/cryptopp850/sha.o \
    $O/cryptopp850/sha3.o \
    $O/cryptopp850/sha_simd.o \
    $O/cryptopp850/shacal2.o \
    $O/cryptopp850/shacal2_simd.o \
    $O/cryptopp850/shake.o \
    $O/cryptopp850/shark.o \
    $O/cryptopp850/sharkbox.o \
    $O/cryptopp850/simeck.o \
    $O/cryptopp850/simon.o \
    $O/cryptopp850/simon128_simd.o \
    $O/cryptopp850/simple.o \
    $O/cryptopp850/skipjack.o \
    $O/cryptopp850/sm3.o \
    $O/cryptopp850/sm4.o \
    $O/cryptopp850/sm4_simd.o \
    $O/cryptopp850/sosemanuk.o \
    $O/cryptopp850/speck.o \
    $O/cryptopp850/speck128_simd.o \
    $O/cryptopp850/square.o \
    $O/cryptopp850/squaretb.o \
    $O/cryptopp850/sse_simd.o \
    $O/cryptopp850/strciphr.o \
    $O/cryptopp850/tea.o \
    $O/cryptopp850/tftables.o \
    $O/cryptopp850/threefish.o \
    $O/cryptopp850/tiger.o \
    $O/cryptopp850/tigertab.o \
    $O/cryptopp850/ttmac.o \
    $O/cryptopp850/tweetnacl.o \
    $O/cryptopp850/twofish.o \
    $O/cryptopp850/vmac.o \
    $O/cryptopp850/wake.o \
    $O/cryptopp850/whrlpool.o \
    $O/cryptopp850/xed25519.o \
    $O/cryptopp850/xtr.o \
    $O/cryptopp850/xtrcrypt.o \
    $O/cryptopp850/xts.o \
    $O/cryptopp850/zdeflate.o \
    $O/cryptopp850/zinflate.o \
    $O/cryptopp850/zlib.o \
    $O/gpsrbase/GpsrBase.o \
    $O/gpsrbase/GpsrGrayholeSecure.o \
    $O/gpsrbase/GpsrPositionTable.o \
    $O/gpsrbase/GpsrSecure.o \
    $O/gpsrbase/Grayhole.o \
    $O/gpsrbase/Sybil.o \
    $O/gpsrbase/SybilSecure.o \
    $O/gpsrbase/GpsrBase_m.o

# Message files
MSGFILES = \
    gpsrbase/GpsrBase.msg

# SM files
SMFILES =

# Other makefile variables (-K)
INET4_PROJ=../inet4

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(TOOLCHAIN_NAME),clangc2)
LIBS += -Wl,-rpath,$(abspath $(INET4_PROJ)/src)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# inserted from file 'makefrag':
CXXFLAGS += -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -mpclmul -maes -mavx -mavx2 -msha
MSGC:=$(MSGC) --msg6

# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clangc2)
	$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)echo >.tmp$$$$ $(OBJS) $(EXTRA_OBJS) && $(AR) .tmplib$$$$ @.tmp$$$$ && $(CXX) -o $O/$(TARGET) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) .tmplib$$$$ $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS) && rm .tmp$$$$ && rm .tmplib$$$$

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cpp

$O/%.o: %.cpp $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cpp %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cpp -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cpp %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cpp $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET)
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET:%.dll=%.lib)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cpp *_m.h *_sm.cpp *_sm.h)

cleanall:
	$(Q)$(MAKE) -s clean MODE=release
	$(Q)$(MAKE) -s clean MODE=debug
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)
