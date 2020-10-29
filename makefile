
CC      = gcc
CFLAGS  =  -W"switch" -W"no-deprecated-declarations" -W"empty-body" -W"conversion" -W"return-type" -W"parentheses" -W"no-format" -W"uninitialized" -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -fpermissive -frtti -fno-omit-frame-pointer -std=c11


CXX          = g++
LDFLAGS      = -DIL_STD -Wswitch -Wno-deprecated-declarations -Wempty-body -Wconversion -Wreturn-type -Wparentheses -Wno-pointer-sign -Wno-format -Wuninitialized -Wunreachable-code -Wunused-function -Wunused-value -Wunused-variable -std=c++11 -Wall -fno-strict-aliasing -I /opt/ibm/ILOG/CPLEX_Studio128/concert/include -I /opt/ibm/ILOG/CPLEX_Studio128/cplex/include -g2 -gdwarf-2 

LIBS 	    = -fPIC -lilocplex -lconcert -lcplex -lm -lpthread -ldl -fopenmp -L /opt/ibm/ILOG/CPLEX_Studio128/concert/lib/x86-64_linux/static_pic -L /opt/ibm/ILOG/CPLEX_Studio128/cplex/lib/x86-64_linux/static_pic



$(OUT): $(OBJFILES)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

OBJSC = GlassCorte.o Glass.o GlassDefect.o GlassItem.o GlassNodeB.o GlassNode.o GlassOriginals.o GlassPlate.o GlassRectangle.o GlassRsol.o GlassStack.o GlassStructures.o GlassCPlex.o bouknap.o

OBJSF  = 
OBJS= $(OBJSC) $(OBJSF)

default: ModelGlass

all: $(CPROGS)

clean:	
	rm -f core  
	rm -f salida* 
	rm -f sal* 
	rm -f *.o  
	rm -f *.exe  

ModelGlass :   main.o $(OBJS)
	$(CXX) $(LDFLAGS) -o ModelGLASS.exe main.o $(OBJS) $(LIBS)

.f.o:
	$(F77) $(FFLAGS) -c $<

.cpp.o:
	$(CXX) -c $(LDFLAGS) -c $(LIBS) $<

.c.o:
	$(CC) -c $(CFLAGS) -c $(LIBS) $<


