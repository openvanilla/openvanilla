MODULEID=OVOFRubyBased
SOURCES=OVOFRubyBased
DATA=ov_ruby_filter.rb

include ../Mk/ov.module.mk

RUBY_DIR=/usr/lib/ruby/1.8/universal-darwin8.0/
override CFLAGS=-I$(RUBY_DIR) -F/Library/Frameworks/
override LDFLAGS=-lruby -dl
