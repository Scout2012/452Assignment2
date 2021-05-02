#include <string>
#include <sys/stat.h>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"

using namespace std;

#define EXPECTED_ARG_COUNT 6
#define AES_BLOCK_BYTE_SIZE 16
#define DES_BLOCK_BYTE_SIZE 8 
#define DES_KEY_SIZE 16

struct CipherType
{
	CipherInterface* interface;	
	char* name;
};

struct UserInput
{
	CipherType* cipher;
	char* key;
	bool* encrypt;
	char* input_file_name;
	char* output_file_name;
	int block_size;
};

void run_encryption(UserInput* input)
{
	FILE* input_fp = fopen(input->input_file_name, "r");
	FILE* output_fp = fopen(input->output_file_name, "w");
	bool endOfFile = false;
	int numRead = -1;
	int size = input->block_size;
	printf("Reading from file \"%s\"\n",input->input_file_name);

	while(!endOfFile)
	{
		unsigned char* buffer = new unsigned char[input->block_size];
		numRead = fread(buffer, 1, size, input_fp);
		if(numRead < size)
		{
			endOfFile = true;
			for(int i = numRead; i < size; i++)
			{
				buffer[i] = 0;
			}
		}

		unsigned char* text;
		if(*(input->encrypt))
		{
			printf("Encrypting input chunk...\n");
			text = input->cipher->interface->encrypt(buffer);
		}
		else
		{
			printf("Decrypting input chunk...\n");
			text = input->cipher->interface->decrypt(buffer);
		}

		printf("Writing output to file \"%s\"\n",input->output_file_name);

		fwrite(text, 1, size, output_fp);
		delete [] buffer; 
	}

	fclose(input_fp);
	fclose(output_fp);
}

unsigned char* get_file_contents(unsigned char* filename)
{
	return filename;
}

// Retrieved from https://stackoverflow.com/a/12774387
bool file_exists (const char* name) {
  struct stat buffer;
  bool exists = stat(name, &buffer) == 0;
  return exists;
}

// Retrieved from https://stackoverflow.com/a/41149146
char* convert_to_lower(char* str)
{
    unsigned char* mystr = (unsigned char *)str;

    while (*mystr) {
        *mystr = tolower(*mystr);
        mystr++;
    }
    return str;
}

CipherType* get_validated_cipher(char* raw_cipher)
{
	if(raw_cipher == nullptr) return nullptr;

	char* lower_raw_cipher = convert_to_lower(raw_cipher);
	// strcmp returns 0 if equal!
	if(strcmp("aes", raw_cipher) && strcmp("des", raw_cipher)) { printf("Invalid Cipher Type \"%s\". Options are AES or DES.\n", raw_cipher); return nullptr; }
	
	CipherType* cipher = new CipherType;
	if(!strcmp("aes", raw_cipher)) { cipher->name = raw_cipher; cipher->interface = new AES(); return cipher; }
	if(!strcmp("des", raw_cipher)) { cipher->name = raw_cipher; cipher->interface = new DES(); return cipher; }

	return nullptr;
}


// What makes a valid key?
// [a-zA-Z0-9] only? Probably
// Key size depending on the cipher
char* get_validated_key(char* raw_key, char* cipher)
{
	if(raw_key == nullptr) { printf("Invalid Key \"%s\"", raw_key); return nullptr; }
	if(cipher == nullptr) { printf("Invalid Cipher \"%s\"", cipher); return nullptr; }

	if(!strcmp("AES", cipher) || !strcmp("DES", cipher))
	{
		if(!strcmp("AES", cipher))
			if(strlen(raw_key) == AES_128_KEY_BYTE_SIZE || strlen(raw_key) == AES_192_KEY_BYTE_SIZE || strlen(raw_key) == AES_256_KEY_BYTE_SIZE) return raw_key;

		if(!strcmp("DES", cipher))
			if(strlen(raw_key) == DES_KEY_SIZE) return raw_key;
		printf("Invalid key size \"%ld\" for cipher \"%s\".\n", strlen(raw_key), cipher);
		return nullptr;
	}

	printf("Invalid cipher type. Valid options are AES or DES\n");
	return nullptr;
}

bool* get_validated_encrypt(char* raw_encrpyt)
{
	if(raw_encrpyt == nullptr) { printf("Invalid Encryption Type \"%s\"", raw_encrpyt); return nullptr; }

	char* lower_encrypt = convert_to_lower(raw_encrpyt);
	if(!strcmp("enc", lower_encrypt)) return new bool(true);
	if(!strcmp("dec", lower_encrypt)) return new bool(false);

	printf("Options are ENC or DEC \"%s\"", raw_encrpyt);
	return nullptr;
}

char* get_validated_input_fn(char* raw_input_name)
{
	// if(raw_input_name == nullptr || !file_exists(raw_input_name)) { printf("Input File Does Not Exist \"%s\"", raw_input_name); return nullptr; }
	return raw_input_name;
}

char* get_validated_output_fn(char* raw_output_name)
{
	// We can create the file if it doesn't exist
	return raw_output_name;

}

UserInput* get_user_input(int argc, char** argv)
{
	UserInput* validated_input = new UserInput;

	if(argc != EXPECTED_ARG_COUNT) { printf("Usage: ./cipher <DES|AES> <KEY> <ENC|DEC> <INPUT_FILE_NAME> <OUTPUT_FILE_NAME>\n"); return nullptr; }

	validated_input->cipher = get_validated_cipher(argv[1]);

	if(validated_input->cipher == nullptr || validated_input->cipher->name == nullptr) { return nullptr; }

	validated_input->key = get_validated_key(argv[2], validated_input->cipher->name);
	validated_input->encrypt = get_validated_encrypt(argv[3]);
	validated_input->input_file_name = get_validated_input_fn(argv[4]);
	validated_input->output_file_name = get_validated_output_fn(argv[5]);

	if(validated_input->key != nullptr && validated_input->encrypt != nullptr &&
	   validated_input->input_file_name != nullptr && validated_input->output_file_name != nullptr) return validated_input;

	return nullptr;
}

char* add_aes_encrypt_flag(char* aes_key, bool encrypt)
{
	char* marked_aes_key = (char*)malloc(17*sizeof(char));
	if(encrypt)
	{
		marked_aes_key[0] = '0';
	}
	else
	{
		marked_aes_key[0] = '1';
	}
	for(int i = 1; i < 17; i++)
	{
		marked_aes_key[i] = aes_key[i-1];
	}
	return marked_aes_key;
}

int main(int argc, char** argv)
{

	UserInput* input = get_user_input(argc, argv);
	if(input == nullptr) { printf("\nInvalid User Input.\nExiting program with exit code -1.\n"); exit(-1); };

	CipherInterface* cipher = input->cipher->interface; 

  /* Error checks */
	if(!cipher)
	{
		fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",	
		__FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}

	if(!strcmp("AES", input->cipher->name))
	{
		input->block_size = sizeof(char)*AES_BLOCK_BYTE_SIZE;
		input->key = add_aes_encrypt_flag(input->key, *(input->encrypt));
	}
	else
	{
		input->block_size = sizeof(char)*DES_BLOCK_BYTE_SIZE;
	}
	
	cipher->setKey((unsigned char*) input->key);

	run_encryption(input);
	delete input;
	return 0;
}
