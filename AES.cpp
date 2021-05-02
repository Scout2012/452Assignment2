#include "AES.h"

/**
 * Sets the key to use
 * @param key - the first byte of this represents whether
 * to encrypt or to decrypt. 00 means encrypt and any other
 * value to decrypt.  Then come the bytes of the 128-bit key
 * (should be 16 of them).
 * @return - True if the key is valid and False otherwise
 */
bool AES::setKey(const unsigned char* keyArray)
{
	int keyErrorCode = -1;
	
	// TODO: AES implementation of openssl cares about whether
	// you are encrypting or decrypting when setting the key.
	// That is, when encrypting you use function AES_set_encrypt_key(...)
	// and when decrypting AES_set_decrypt_key(...).
	//
	// One way to solve this problem is to pass in a 17 byte key, where
	// the first byte is used to indicate whether we are encrypting or
	// decrypting. E.g., if the first byte is 0, then use AES_set_encrypt_key(...).
	// Otherwise, use AES_set_decrypt_key(...).  The rest of the bytes in the
	// array indicate the 16 bytes of the 128-bit AES key.
	//
	// Both functions return 0 on success and other values on faliure.
	// For documentation, please see https://boringssl.googlesource.com/boringssl/+/2623/include/openssl/aes.h
	// and aes.cpp example provided with the assignment.
	bool encrypt = keyArray[0] == '0' ? true : false;
	unsigned char* key = (unsigned char*)malloc(16*sizeof(unsigned char));
	for(int i = 1; i < 17; i++)
	{
		key[i-1] = keyArray[i];
	}
	if(encrypt)
	{
		if ((keyErrorCode = AES_set_encrypt_key(key, KEY_SIZE, &(this->enc_key))) != 0)
		{
			fprintf(stderr, "\nkey error %d\n", keyErrorCode);
			return false;
		}
		return true;
	}
	else
	{
		if ((keyErrorCode = AES_set_decrypt_key(key, KEY_SIZE, &(this->dec_key))) != 0)
		{
			fprintf(stderr, "\nkey error %d\n", keyErrorCode);
			return false;
		}
		return true;
	}
}

/**	
 * Encrypts a plaintext string
 * @param plaintext - the plaintext string
 * @return - the encrypted ciphertext string
 */
unsigned char* AES::encrypt(const unsigned char* plainText)
{
	int padding_needed = 0;
	//TODO: 1. Dynamically allocate a block to store the ciphertext.
	unsigned char* cipherText = new unsigned char[strlen((char*) plainText)];
	//	2. Use AES_ecb_encrypt(...) to encrypt the text (please see the URL in setKey(...)
	//	and the aes.cpp example provided.
	AES_ecb_encrypt(plainText,cipherText, &(this->enc_key), AES_ENCRYPT);
	// 	3. Return the pointer to the ciphertext
	return cipherText;	
}

/**
 * Decrypts a string of ciphertext
 * @param cipherText - the ciphertext
 * @return - the plaintext
 */
unsigned char* AES::decrypt(const unsigned char* cipherText)
{
	
	//TODO: 1. Dynamically allocate a block to store the plaintext.
	unsigned char* plainText = new unsigned char[strlen((char*) cipherText)];
	//	2. Use AES_ecb_encrypt(...) to decrypt the text (please see the URL in setKey(...)
	//	and the aes.cpp example provided.
	AES_ecb_encrypt(cipherText, plainText, &(this->dec_key), AES_DECRYPT);
	// 	3. Return the pointer to the plaintext
	return plainText;
}
