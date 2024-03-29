define doinsubdirs
$(foreach d,$(1),$(MAKE) -C $(d) $(2) $@;)
endef

SUBDIRS = ut

.PHONY: all check clean googletest

all check clean:
	$(call doinsubdirs,${SUBDIRS})

googletest:
	$(MAKE) -C googletest/make
