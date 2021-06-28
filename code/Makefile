SRCDIR=src
default: all

all: 
	@for i in $(SRCDIR); do \
	echo "make all in $$i..."; \
	($(MAKE) -C $$i); done

clean:
	@for i in $(SRCDIR); do \
	echo "make clean in $$i..."; \
	($(MAKE) -C $$i clean); done

distclean:
	@for i in $(SRCDIR); do \
	echo "make clean && make distclean in $$i..."; \
	($(MAKE) -C $$i clean && $(MAKE) -C $$i distclean); done


.PHONY: all clean distclean

