.PHONY: all clean
export ROOT = /home/yongj/p2p
DIRS = $(ROOT)/source/base/src \
       $(ROOT)/source/p2pclient/src \
       $(ROOT)/source/p2pserver/src

RM = rm
RMFLAGS = -fr
RMS = $(ROOT)/build/exes $(ROOT)/build/libs

all:
	@set -e; \
	for dir in $(DIRS); \
	do \
		cd $$dir && $(MAKE); \
	done
	@echo ""
	@echo ":-)Completed"
	@echo ""

clean:
	@set -e; \
	for dir in $(DIRS); \
	do \
		cd $$dir && $(MAKE) clean; \
	done
	@echo ""
	@echo ":-)Completed"
	@echo ""
