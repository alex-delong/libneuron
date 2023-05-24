.PHONY: build test

BUILDCMD=$(patsubst %, $(MAKE) build -e -C %;, $(MOD))
TESTCMD=$(patsubst %, $(MAKE) test -e -C %;, $(MOD))

export
build:
	$(BUILDCMD)

test:
	$(TESTCMD)
