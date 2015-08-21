R_HOME := $(shell R RHOME)

## include headers and libraries for R
RCPPFLAGS := $(shell $(R_HOME)/bin/R CMD config --cppflags)
RLDFLAGS := $(shell $(R_HOME)/bin/R CMD config --ldflags)

## include headers and libraries for Rcpp interface classes
RCPPINCL := $(shell echo 'Rcpp:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)
RCPPLIBS := $(shell echo 'Rcpp:::LdFlags()'  | $(R_HOME)/bin/R --vanilla --slave)
FB_BIO_LIB = ./finite-difference-biosensor-library

%.o : %.cpp
	PKG_CPPFLAGS="$(RCPPFLAGS) $(RCPPINCL) -I$(FB_BIO_LIB)/" PKG_LIBS="$(RLDFLAGS) $(RCPPLIBS)" R CMD SHLIB $<

cpp_sources := $(wildcard *.cpp)
cpp_sharedlibs := $(patsubst %.cpp,%.o,$(cpp_sources))



main: shared_libs
	g++ -shared -o testing.so $(wildcard $(FB_BIO_LIB)/*.o) $(wildcard ./*.o) -L/usr/lib/R/lib -lR -L/usr/lib/R/lib -lR

shared_libs: $(cpp_sharedlibs)

clean:
	rm testing.so testing.o $(FB_BIO_LIB)/utils.o $(FB_BIO_LIB)/explicit_calculator.o

