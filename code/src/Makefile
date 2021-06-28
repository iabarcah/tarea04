TARGET=./sumaParalela

CXX=/usr/bin/mpicxx

DIR_OBJ=objs
SOURCES=$(wildcard *.cc)
OBJECTS=$(patsubst %.cc,$(DIR_OBJ)/%.o,$(SOURCES))

INTRINSICS=

BOOST_INCLUDE=-I/usr/include/
BOOST_LIB_DIR=-L/usr/lib/x86_64-linux-gnu
BOOST_LIBS=-lboost_mpi -lboost_serialization -lboost_system -lboost_filesystem

LDFLAGS =-L/usr/local/lib $(BOOST_LIB_DIR)
LDLIBS = -lm -lpthread $(BOOST_LIBS)

INCLUDES =-I. -I./include $(BOOST_INCLUDE)

CPPFLAGS =-std=c++17  -Wno-deprecated $(INTRINSICS) 


all: $(TARGET)
	@echo Made [ $? ] OK :\)
	
$(TARGET): $(OBJECTS) 
	@echo Linking [$@]
	@eval $(CXX) $(LDFLAGS) $^ $(LDLIBS)  -o $@ 
	
$(DIR_OBJ)/%.o: %.cc
	@echo Compiling [$@]
	@mkdir -p $(DIR_OBJ)
	@eval $(CXX) -c $(INCLUDES) $(CPPFLAGS)  $< -o $@
	
clean:
	@$(RM) core *~ \#*\# *.bak *.BAK *\%
	@$(RM) -f .dummy 
	@$(RM) -rf $(DIR_OBJ)

distclean:
	@$(RM) $(TARGET)







