APP_BINARY = smallworld
APP_SOURCE = main.cpp
INCLUDES   = nnd.cpp # None. Use -Ipath to add
OPTIONS    = -O2 -std=gnu++98

CXXFLAGS   += $(INCLUDES) $(OPTIONS)
LDFLAGS    += -lm

all: $(APP_BINARY)

$(APP_BINARY): 
_SOURCES = $(APP_SOURCE)
_CPPOBJECTS = $(_SOURCES:.cpp=.o)
_EXECUTABLE=$(APP_BINARY)

$(_EXECUTABLE): $(_CPPOBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(_CPPOBJECTS) -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean realclean

clean: 
	rm -rf $(_CPPOBJECTS)

realclean: clean
	rm -rf $(_EXECUTABLE)_*
