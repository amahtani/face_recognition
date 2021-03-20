EXEC=face

OPENCV = `pkg-config opencv --cflags --libs`
CFLAGS = -Wall

all: $(EXEC)

face : face.cpp
	g++ face.cpp $(CFLAGS) -o face -g3 $(OPENCV)

	

clean :
	rm -f *.o $(EXEC)

rmproper : 

	rm -f $(EXEC) *.o 
