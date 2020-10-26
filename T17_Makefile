CC          = g++
CC          = gcc
CFLAGS      = -std=c++11 -O4 -DDEBUG


LIBS        =  -fPIC -lilocplex -lconcert -lcplex -lm -lpthread -ldl -fopenmp -lstdc++

OBJSC = GlassCorte.o Glass.o GlassDefect.o GlassItem.o GlassNodeB.o GlassNode.o GlassOriginals.o GlassPlate.o GlassRectangle.o GlassRsol.o GlassStack.o GlassStructures.o GlassCplex.o

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
	$(CC) $(CFLAGS) -o ModelGlass.exe main.o $(OBJS) $(LIBS)

.f.o:
	$(F77) $(FFLAGS) -c $<

.cpp.o:
	$(CC) -c $(CFLAGS) -c  $<

