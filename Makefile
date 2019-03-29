# Directories
BUILDDIR = build
BINDIR = bin
SRCDIR = src
INCLUDEDIR = include
LOGDIR = log

# Compiler
CC = g++

# Compiler flags
CFLAGS = -I/usr/include/mysql -I$(INCLUDEDIR)
# Linker flags
LDFLAGS = -pthread -L/usr/lib/mysql -lmysqlclient

all: $(BINDIR)/GlimbServer

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CC) -c $^ -o $@ $(CFLAGS)

$(BINDIR)/GlimbServer: $(BUILDDIR)/main.o $(BUILDDIR)/Server.o $(BUILDDIR)/ClientProcessor.o $(BUILDDIR)/DbConnector.o $(BUILDDIR)/LogPrinter.o
	mkdir -p $(BINDIR)
	mkdir -p $(LOGDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp
$(BUILDDIR)/Server.o: $(SRCDIR)/Server.cpp
$(BUILDDIR)/ClientProcessor.o: $(SRCDIR)/ClientProcessor.cpp
$(BUILDDIR)/DbConnector.o: $(SRCDIR)/DbConnector.cpp
$(BUILDDIR)/LogPrinter.o: $(SRCDIR)/LogPrinter.cpp

clean:
	-rm -r $(BUILDDIR)
