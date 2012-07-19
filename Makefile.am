ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

bin_PROGRAMS = bornholdt

bornholdt_SOURCES = \
	src/bornholdt.cpp \
	src/BornholdtApp.cpp \
	src/model/BornholdtModel.cpp \
	src/EvolutionController.cpp	\
	src/BornholdtOptions.cpp \
	src/model/GraphStateReader.cpp \
	src/model/GraphStateWriter.cpp

bornholdt_CXXFLAGS = $(AM_CXXFLAGS) $(ODEINT_CXXFLAGS) $(BOOST_CXXFLAGS) $(DEPS_CFLAGS)
bornholdt_LDFLAGS = $(DEPS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LDFLAGS)
bornholdt_LDADD = $(DEPS_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)

if DEBUGMODE
else
bornholdt_CXXFLAGS += -DNDEBUG
endif