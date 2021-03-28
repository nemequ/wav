EMSDK = /opt/emscripten

CC := $(EMSDK)/upstream/emscripten/emcc
CXX := $(EMSDK)/upstream/emscripten/em++
WASM2WAT := /home/nemequ/local/src/wabt/build/wasm2wat
CFLAGS := -Weverything -Wno-missing-prototypes -Werror -O1 -msimd128 # -munimplemented-simd128
CXXFLAGS := $(CFLAGS)

NULL =

COMPILE_TESTS = \
	load \
	loada \
	loadu \
	store \
	storea \
	storeu \
	splat \
	make \
	const \
	extract_lane \
	replace_lane \
	shuffle \
	swizzle \
	add \
	sub \
	mul \
	div \
	neg \
	sqrt \
	ceil \
	floor \
	trunc \
	nearest \
	extend_low \
	promote_low \
	extend_high \
	narrow \
	demote \
	extmul_low \
	extmul_high \
	extadd_pairwise \
	dot \
	add_sat \
	sub_sat \
	q15mulr_sat \
	min \
	max \
	pmin \
	pmax \
	avgr \
	abs \
	shl \
	shr \
	and \
	or \
	xor \
	not \
	andnot \
	bitselect \
	blend \
	popcount \
	eq \
	ne \
	lt \
	le \
	gt \
	ge \
	any_true \
	all_true \
	load_zero \
	load_splat \
	loadu_splat \
	load_lane \
	load_extend \
	$(NULL)

TARGETS = \
	$(COMPILE_TESTS:=.wasm) \
	$(COMPILE_TESTS:=-cpp.wasm) \
	$(COMPILE_TESTS:=.wat) \
	$(COMPILE_TESTS:=-cpp.wat) \
	$(NULL)

all: $(TARGETS) .gitignore

clean:
	rm -f $(TARGETS)

%.cpp: %.c
	cp -af "$<" "$@"

%-cpp.wasm: %.cpp ../wav.h
	$(CXX) $(CFLAGS) --no-entry -c -o $@ $<

%.wasm: %.c ../wav.h
	$(CC) $(CFLAGS) --no-entry -c -o $@ $<

%.wat: %.wasm
	$(WASM2WAT) --enable-simd -o $@ $<
