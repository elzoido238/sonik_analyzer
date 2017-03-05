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

SDIR        := src
ODIR        := obj
CINTDIR     := cint
LDIR        := lib

DICT      	:= SonikDict.$(SrcSuf)
DICTH     	:= $(DICT:.$(SrcSuf)=.h)
DICTO     	:= $(DICT:.$(SrcSuf)=.$(ObjSuf))

DICT		:= $(addprefix $(CINTDIR)/,$(DICT))
DICTH		:= $(addprefix $(CINTDIR)/,$(DICTH))
DICTO		:= $(addprefix $(ODIR)/,$(DICTO))

SRCS      	:= $(wildcard *.$(SrcSuf))
SRCS		:= $(addprefix $(SDIR)/,$(SRCS))

HDRS      	:= $(SRCS:.$(SrcSuf)=.h)
HDRS		:= $(addprefix $(SDIR)/,$(HDRS))

SHLIB     	:= $(LDIR)/libSonik.$(DllSuf)
DYLIB     	:= $(LDIR)/libSonik.dylib

OBJS      	:= $(SRCS:.$(SrcSuf)=.$(ObjSuf))
OBJS 		:= $(addprefix $(ODIR)/,$(OBJS))

FPIC  		 = -fPIC
ROOFITLIBS 	 = -lRooStats -lRooFit -lRooFitCore -lMinuit

CXXFLAGS 	+= $(DEBUG)
CXXFLAGS 	+= $(FPIC)
LDFLAGS 	+= -L$(PWD) -L$(ROOTSYS)/lib
LDFLAGS 	+= $(EXPLLINKLIBS) $(ROOFITLIBS)

############# RULES ###############

## Must be last object rule!
$(ODIR)/%.o: $(SRCS) $(DICT)
	$(CXX) $(CXXFLAGS) -c $(OutPutOpt) $@ $<

############# TARGETS #############
.SUFFIXES: .$(SrcSuf) .$(ObjSuf) $(ExeSuf) .$(DllSuf)

all:    $(SHLIB)

$(DICT): $(HDRS) $(CINTDIR)/LinkDef.h
	@echo "Generating dictionary $@..."
	$(ROOTCINT) -f $@ -c $(CXXFLAGS) $^

$(SHLIB): $(DICTO) $(OBJS)
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt) $@

dylib: $(DICTO) $(OBJS) $(SHLIB)
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $(DYLIB)

distclean: clean
	@rm -f $(SHLIB) $(DYLIB) $(OBJS) $(DICT) $(DICTO) $(DICTH)

clean:
	@rm -f $(SHLIB) $(DYLIB) $(OBJS) $(DICT) $(DICTO) $(DICTH)
