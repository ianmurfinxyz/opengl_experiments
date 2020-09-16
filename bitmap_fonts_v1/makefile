
LDLIBS = -lSDL2 -lGLX_mesa
CXXFLAGS = -g -Wall

cxxsrc = drawing.cpp main.cpp dogica_8px.cpp font.cpp
cxxinc = drawing.h color.h bitmap_fonts.h font.h

goal = demo

objs = $(cxxsrc:.cpp=.o)

$(goal) : $(objs) $(cxxinc)
	$(CXX) $(CXXFLAGS) -o $@ $(objs) $(LDLIBS)

.PHONY: clean
clean:
	rm $(objs) $(goal)
