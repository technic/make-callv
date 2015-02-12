> LICENSE: GNU GPLv2

# make-callv
Substitute $1,$2,...,$9 symbols like $(call ...) does but doesn't expand other variables like $(value ...) does. Needs make 4 with load objects support. You can still escape $1 with $$1.

### Purpose
When in Makefile you have very similar blocks of code, you may want to define a bunch of code and evaluate it with different paremeters. So you expect to do something like this, just primitive example

```make
define code
compile-$1:
  $(MAKE) -C dir-$1 CC=$2 all

clean-$1:
  $(MAKE) -C dir-$1 clean
endef

$(eval $(call code,foo,gcc))
$(eval $(call code,bar,arm-gcc))
```

Ok, it was simple, let's do somethig more complicated
```make
define code
archive_$1 = $1-$(version_$1).tar.gz
url_$1 = http://mysite.com/$(archive_$1)

$(archive_$1):
  wget $(url_$1)

extract-$1: $(arhive_$1)
  tar -xzf $<
endef

version_foo = 1.0
$(eval $(call code,foo))
version_bar = 2.0
$(eval $(call code,bar))
```
And here problem comes. Variables `archive_$1` and `url_$1` is expanded when you use call function, so instead of `$(url_$1)` and `$(archive_$1)` you will see only whitespace! <br>
You may add `$(info $(call code,foo))` to see what is happening. <br>
Yes you may use $$ escapes or use _subst_ instead of _call_ or don't define new variables, but my sollution is **callv** ! It is more convinient and easy to use. You need to write:
```make
include callv.mk
$(callv code,bar)
```






