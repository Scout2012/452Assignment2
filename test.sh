#!/bin/bash

rm TestFiles/output/* -rf

make

executable=cipher
test_input_file_folder=TestFiles
test_output_file_folder=TestFiles/output

aes_valid_key='00112233445566778899aabbccddeeff'
des_valid_key='0123456789abcdef'

for file in ${test_input_file_folder}/*.txt; do
	output=${test_output_file_folder}/$(basename ${file})
	./cipher DES ${des_valid_key} ENC ${file} ${output}.des_enc.txt
	./cipher DES ${des_valid_key} DEC ${output}.des_enc.txt ${output}.des_dec.txt

	./cipher AES ${aes_valid_key} ENC ${file} ${output}.aes_enc.txt
	./cipher AES ${aes_valid_key} DEC ${output}.aes_enc.txt ${output}.aes_dec.txt
	done
