SILENCE = @

COMPONENT_NAME = AIS_TEST

CPP_PLATFORM = Gcc
PROJECT_HOME_DIR = .


SRC_DIRS = \
	.. \
	
TEST_SRC_DIRS = \
	. \


INCLUDE_DIRS = \
	. \
	./include \
	$(CPPUTEST_HOME)/include \
	.. \
	../.. \

CPPUTEST_CFLAGS = -Wall -pedantic 
CPPUTEST_WARNINGFLAGS += -Wno-c++14-compat 

ifeq ($(CPPUTEST_HOME),)
$(info CPPUTEST_HOME not set! See README.txt)
else
include $(CPPUTEST_HOME)/build/MakefileWorker.mk
endif


