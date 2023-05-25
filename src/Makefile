.PHONY: build test

BUILDCMD:=$(patsubst %, $(MAKE) build -e -C %;, $(MOD))

export
build:
	$(BUILDCMD)
