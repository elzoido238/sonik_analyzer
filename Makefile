############# MACROS ##############
RC     := root-config
ifeq ($(shell which $(RC) 2>&1 | sed -ne "s@.*/$(RC)@$(RC)@p"),$(RC))
	MKARCH := $(wildcard $(shell $(RC) --etcdir)/Makefile.arch)
endif
ifneq ($(MKARCH),)
	include $(MKARCH)
else
	ROOTSYS = $(shell $(RC) --prefix)
	include $(ROOTSYS)/etc/Makefile.arch
endif

FPIC		 = -fPIC
ROOFITLIBS 	 = -lRooFit -lRooFitCore -lMinuit -lDragon
INCLUDE		+= -I$(PWD)/include -I$(DRAGONSYS)/src/utils

CXXFLAGS 	+= $(DEBUG) $(FPIC) $(INCLUDE) -v
LDFLAGS 	+= -L$(PWD) -L$(ROOTSYS)/lib -L$(DRAGONSYS)/lib
LDFLAGS 	+= $(EXPLLINKLIBS) $(ROOFITLIBS)

BUILD		:= $(PWD)/build
LDIR		:= $(PWD)/lib
CINT		:= $(PWD)/cint
SDIR		:= $(PWD)/src
HDIR		:= $(PWD)/include

SRCEXT		:= cxx
SRCS      	:= $(shell find $(SDIR) -type f -name *.$(SRCEXT))
OBJS      	:= $(patsubst $(SDIR)/%, $(BUILD)/%, $(SRCS:.$(SRCEXT)=.o))
HDRS      	:= $(patsubst $(SDIR)/%, $(HDIR)/%, $(SRCS:.$(SRCEXT)=.hxx))

LINKDEF		:= $(CINT)/LinkDef.h

SHLIB     	:= $(LDIR)/libSonik.so

DICT      	:= SonikDict.cxx
DICTH     	:= $(patsubst %.cxx, %.h, $(DICT))
DICTO     	:= $(DICT:.$(SrcSuf)=.$(ObjSuf))

DICT      	:= $(addprefix $(CINT)/,$(DICT))
DICTH     	:= $(addprefix $(CINT)/,$(DICTH))
DICTO     	:= $(addprefix $(BUILD)/,$(DICTO))

############# TARGETS #############
all: $(OBJS) $(SHLIB)

$(BUILD)/%.o: $(SDIR)/%.cxx
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DICTO): $(DICT)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS): | $(BUILD)

$(BUILD):
	mkdir $(BUILD)

$(LDIR):
	mkdir $(LDIR)

######## CINT DICTIONARY ##########
$(DICT): $(HDRS) $(LINKDEF)
	@echo "Generating dictionary $@..."
	$(ROOTCINT) -f $@ -c $(CXXFLAGS) -p $^

$(SHLIB): $(DICTO) $(OBJS) | $(LDIR)
ifeq ($(PLATFORM),macosx)
	$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@ $(GLIBS)
else
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt) $@
endif

.PHONY: clean

distclean: clean
	@rm -f $(SHLIB) $(BUILD)/* $(DICT) $(DICTO) $(DICTH)

clean:
	@rm -f $(SHLIB) $(BUILD)/* $(DICT) $(DICTO) $(DICTH)

######## DOXYGEN ##########
doc::
	cd doc; doxygen Doxyfile; cd ..
