.PHONY: build
BUILDCMD=$(patsubst %, $(MAKE) build -e -C %;, $(MOD))
export
build:
	$(MAKE) -C neuron
	$(MAKE) -C edge
	$(MAKE) -C layer
	$(MAKE) -C network
