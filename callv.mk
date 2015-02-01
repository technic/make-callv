SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

-load $(SELF_DIR)/callv.so

$(SELF_DIR)/callv.so: $(SELF_DIR)/callv.c
	gcc -std=c99 -shared -fPIC -o $@ $<
