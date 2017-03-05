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

ODIR		:= obj

FPIC		 = -fPIC
ROOFITLIBS 	 = -lRooFit -lRooFitCore -lMinuit

CXXFLAGS 	+= $(DEBUG) -v
CXXFLAGS 	+= $(FPIC)
LDFLAGS 	+= -L$(PWD) -L$(ROOTSYS)/lib
LDFLAGS 	+= $(EXPLLINKLIBS) $(ROOFITLIBS)

DICT      	:= SonikFitDict.$(SrcSuf)
DICTH     	:= $(DICT:.$(SrcSuf)=.h)
DICTO     	:= $(DICT:.$(SrcSuf)=.$(ObjSuf))

SRCS      	:= $(wildcard *.$(SrcSuf))

HDRS      	:= $(SRCS:.$(SrcSuf)=.h) LinkDef.h
HDRS      	:= $(filter-out $(DICTH),$(HDRS))

SHLIB     	:= libSonikFit.$(DllSuf)

OBJS      	:= $(SRCS:.$(SrcSuf)=.$(ObjSuf))

############# RULES ###############

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

############# TARGETS #############

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) $(ExeSuf) .$(DllSuf)

all:    $(SHLIB)

$(DICT): $(HDRS)
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
