OUTPUT   = metaweather
CC       = g++
CXXFLAGS = -std=c++1z -Wall -Wextra -pedantic -Wno-unused-variable -Wno-reorder
LDFLAGS  = -lcurl

OPTIM    = -Ofast
SRCDIR   = .
OBJDIR   = obj
BUILDDIR = build

SOURCES  = metaweather.cpp
OBJ      = $(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))

.PHONY: all clean

all: $(BUILDDIR)/$(OUTPUT)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $< $(CXXFLAGS) $(OPTIM) -o $@
    
$(BUILDDIR)/$(OUTPUT): $(OBJ)
	@echo :: Compiling $(OUTPUT)...
	$(CC) $^ $(LDFLAGS) -o $@
	@echo :: done.

clean:
	rm $(OBJDIR)/*.o $(BUILDDIR)/$(OUTPUT)

