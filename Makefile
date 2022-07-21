all:
	python3 codec.py ./ ./
	gcc -Os my_codec.c -o codec

clean:
	rm codec decode_gen_* encode_gen_* opcode_* opnd_decode_funcs.h opnd_encode_funcs.h
