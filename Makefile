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

FPIC  		 = -fPIC
ROOFITLIBS 	 = -lRooFit -lRooFitCore -lMinuit

CXXFLAGS 	+= $(DEBUG) -v
CXXFLAGS 	+= $(FPIC)
LDFLAGS 	+= -L$(PWD) -L$(ROOTSYS)/lib
LDFLAGS 	+= $(EXPLLINKLIBS) $(ROOFITLIBS)

LIBDIR		:= lib
CINTDIR		:= cint
OBJDIR		:= obj

DICT      	:= SonikFitDict.$(SrcSuf)
DICT		:= $(addprefix $(CINTDIR)/,$(DICT))
DICTH     	:= $(DICT:.$(SrcSuf)=.h)
DICTH		:= $(addprefix $(CINTDIR)/,$(DICTH))
DICTO     	:= $(DICT:.$(SrcSuf)=.$(ObjSuf))
DICTO		:= $(addprefix $(OBJDIR)/,$(DICT))

SRCS      	:= $(wildcard *.$(SrcSuf))

HDRS      	:= $(SRCS:.$(SrcSuf)=.h) $(CINTDIR)/LinkDef.h
HDRS      	:= $(filter-out $(DICTH),$(HDRS))

SHLIB     	:= libSonikFit.$(DllSuf)
DYLIB     	:= libSonikFit.dylib

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
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt) $@

dylib: $(DICTO) $(OBJS) $(SHLIB)
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $(DYLIB)

distclean: clean
	@rm -f $(SHLIB) $(DYLIB) $(OBJS) $(DICT) $(DICTO) $(DICTH)

clean:
	@rm -f $(SHLIB) $(DYLIB) $(OBJS) $(DICT) $(DICTO) $(DICTH)
