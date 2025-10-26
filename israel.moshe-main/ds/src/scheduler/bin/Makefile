TARGET=scheduler
CPPFLAGS=-Iinclude
CFLAGS=-ansi -pedantic-errors -Wall -Wextra
LIBS= oll dll pq
LDLIBS=-l_pq -l_oll -l_dll
LDFLAGS=-Linclude
DEBUG=-g
RELEASE=-DNDEBUG -O3
CC=gcc

.PHONY: _clean release build clean $(LIBS)

SRCS= $(wildcard src/*.c)
OBJS=$(SRCS:%.c=%.o)


all: debug
build: $(TARGET)


$(OBJS): $(SRCS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $(notdir $@)
$(TARGET): $(OBJS) libs
	ar crs -o lib_$@.a $(TARGET).o
	$(CC) $(LDFLAGS) $(notdir $(TEST)) -o $@ $(LDLIBS)


debug: CFLAGS+=$(DEBUG)
debug: build

	
release: CFLAGS+=$(RELEASE)
release: build

libs: $(LIBS)
backup_libs: $(LIBS)
	
	@for l in $^; do \
		$(MAKE) -C ../$$l shared; \
		mv ../$$l/lib_$$l.so ../$(TARGET)/include; \
	done

%.a:
	make release
	ar crs -o lib_$@ $*.o


_clean: clean
	@rm include/*.a 2> /dev/null
clean:
	@rm *.o &> /dev/null
	@rm $(TARGET) &> /dev/null
