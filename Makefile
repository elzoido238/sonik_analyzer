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

ODIR		:= $(PWD)/obj
LDIR		:= $(PWD)/lib
CINT		:= $(PWD)/cint
SDIR		:= $(PWD)/src

FPIC		 = -fPIC
ROOFITLIBS 	 = -lRooFit -lRooFitCore -lMinuit

CXXFLAGS 	+= $(DEBUG) -v
CXXFLAGS 	+= $(FPIC)
LDFLAGS 	+= -L$(PWD) -L$(ROOTSYS)/lib
LDFLAGS 	+= $(EXPLLINKLIBS) $(ROOFITLIBS)

DICT      	:= SonikDict.$(SrcSuf)
DICTH     	:= $(DICT:.$(SrcSuf)=.h)
DICTO     	:= $(DICT:.$(SrcSuf)=.$(ObjSuf))

# DICT      	:= $(addprefix $(CINT)/,$(DICT))
# DICTH     	:= $(addprefix $(CINT)/,$(DICTH))
# DICTO     	:= $(addprefix $(CINT)/,$(DICTO))

SRCS      	:= $(wildcard *.$(SrcSuf))
# SRCS		:= $(addprefix $(SDIR)/,$(SRCS))

HDRS      	:= $(PWD)/SonikFit.h #$(SRCS:.$(SrcSuf)=.h)
HDRS      	:= $(filter-out $(DICTH),$(HDRS))
LINKDEF		:= $(CINT)/LinkDef.h

SHLIB     	:= $(LDIR)/libSonik.$(DllSuf)

OBJS      	:= $(SRCS:.$(SrcSuf)=.$(ObjSuf))

############# RULES ###############

%.$(ObjSuf): %.$(SrcSuf)
	$(CXX) $(CXXFLAGS) -c $(OutPutOpt) $@ $<

############# TARGETS #############

all:    $(SHLIB)

$(DICT): $(HDRS) $(LINKDEF)
	@echo "Generating dictionary $@..."
	$(ROOTCINT) -f $@ -c $(CXXFLAGS) -p $^

$(SHLIB): $(DICTO) $(OBJS)
ifeq ($(PLATFORM),macosx)
	$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@ $(GLIBS)
else
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt) $@
endif

distclean: clean
	@rm -f $(SHLIB) $(OBJS) $(DICT) $(DICTO) $(DICTH)

clean:
	@rm -f $(SHLIB) $(OBJS) $(DICT) $(DICTO) $(DICTH)
