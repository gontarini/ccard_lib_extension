NAME = HyperLogLogPlusPlus

CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -g -c -l/usr/lib/libccard-lib.a -l/usr/lib/libccard-lib.so -l/libccard-lib.0.1.a -O2 -std=c++11 


#I/home/pawel/Dokumenty/Sotrender/Interface_ccard_lib/ccard-lib/include/
PREFIX			= /usr
#Edit these lines to correspond with your own directories
LIBRARY_DIR		= $(shell php-config --extension-dir)
PHP_CONFIG_DIR	= /etc/php/7.0/cli/conf.d

LD              = g++
LD_FLAGS        = -Wall -shared -O2
RESULT          = HyperLogLogPlusPlus.so

PHPINIFILE		= HyperLogLogPlusPlus.ini

SOURCES			= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS} -lphpcpp

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}:
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}

install:
		cp -f ${RESULT} ${LIBRARY_DIR}
		cp -f ${PHPINIFILE}	${PHP_CONFIG_DIR}
