all: decode_gen_v80.h
	g++ -Os my_codec.c -o codec -fpermissive

decode_gen_v80.h:
	python3 codec.py ./ ./

clean:
	rm codec decode_gen_* encode_gen_* opcode_* opnd_decode_funcs.h opnd_encode_funcs.h
