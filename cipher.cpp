#include <string>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"

using namespace std;

int main(int argc, char** argv)
{

	//UserInput user = get_user_input(argc, argv);
	CipherInterface* cipher;
	/* Error checks */
	if(!cipher)
	{
		fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",	
		__FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}
	File* fp = fopen(user.input_file_name);
	char buffer[8];

	if(
		//typeid(user.cipher).name()=="DES" 
	)
	{
		cipher = new DES();
	}
	else
	{
		cipher = new AES();
		addEncryptionFlag(user.key, user.encrypt);
	}
	/* Set the encryption key
	 * A valid key comprises 16 hexidecimal
	 * characters. Below is one example.
	 * Your program should take input from
	 * command line.
	 */
	cipher->setKey(user.key);
	/* Perform encryption */
	unsigned char* cipherText = cipher->encrypt((unsigned char*)"hello world");
	
	/* Perform decryption */
	//cipher->decrypt(cipherText);	
	
	return 0;
}

void addEncryptionFlag(char *aes_key, bool encrypt){
	char *marked_aes_key (unsigned char*)malloc(17*sizeof(unsigned char));;
	if(encrypt){
		marked_aes_key[0] = '00';
	}
	else{
		marked_aes_key[0] = '01';
	}
	for(int i = 1; i < 17; i++){
		marked_aes_key[i] = aes_key[i-1];
	}
	aes_key = marked_aes_key;
}