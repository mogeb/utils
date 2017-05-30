CXXFLAGS += 
LFLAGS += -lpthread
LTCMALLOC_FLAG += -ltcmalloc
LTCMALLOC_MINIMAL_FLAG += -ltcmalloc_minimal

all:
	g++ $(CXXFLAGS) -o main main.cpp $(LFLAGS) $(LTCMALLOC_FLAG)

minimal:
	g++ $(CXXFLAGS) -o main main.cpp $(LFLAGS) $(LTCMALLOC_MINIMAL_FLAG)

clean:
	rm -f main
