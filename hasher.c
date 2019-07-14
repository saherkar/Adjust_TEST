/*****************************************************************************************************************************************************
 * External Dependency used: libsodium-1.0.17
 *
 * Explaination:
 *
 * 	This is basic implementation of consuming generic hashing using 
 * 	libsodium(https://dl.google.com/android/repository/android-ndk-r18b-linux-x86_64.zip).
 * 	Multiple types supported by libsodium generic hashing are used in this code.
 * 	1) Single Part without Key.
 * 	2) Single Part with Key.
 * 	3) Multipart without Key.
 * 	This is a binary and not an app.
 *
 * Usage:
 *
 * 	Checkout this code from: (https://github.com/saherkar/Adjust_TEST.git)
 * 	To use this library following are the environment variable that need to be set.
 * 		- Export NDK to installed path of android-ndk(android-ndk-r18b is used in this code).
 * 		- Export TOOLCHAIN to directory where the executable will be deployed /root/x86.
 * 		- Export LIBSODIUM to directoru of libsodium(libsodium-1.0.17 is used in this code).
 *
 * 	After setting these, go to checked out folder, run `make` and your binary is ready to use.
 * 	Push the binary on either emulator or your device, attach to shell(emulator/device) using adb(Android tools)
 * 	and execute it.
 *
 * ***************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>

#define MAX_SIZE 256
#define CHUNK_SIZE 32

unsigned char hash[crypto_generichash_BYTES];
unsigned char key[crypto_generichash_KEYBYTES];
crypto_generichash_state state;

int get_string(unsigned char *);
int get_other_string(unsigned char *, unsigned char *);
int start_from = 0;
int total_length = 0;

int main(void)
{
	unsigned char input[MAX_SIZE];
	unsigned char sub_input[CHUNK_SIZE];
	int ch;
	int flag;

	if (sodium_init() < 0) {
        	printf("panic! the library couldn't be initialized, it is not safe to use\n");
		return 1;
	}

	do
	{
		printf("Menu:\n1) Single Part without Key\n2) Single Part with Key\n");
		printf("3) Multipart with Key\n4) Exit.\n");
		printf("Choice:");
		scanf("%d", &ch);
		flag = 0;

		switch(ch)
		{
			case 1:
				if(get_string(input))
				{
					crypto_generichash(hash, sizeof(hash),
							input, ((unsigned long long) strlen((char *)input)),
							NULL, 0);
				}
				else
				{
					flag = 1;
				}
				break;

			case 2:
				if(get_string(input))
				{
					randombytes_buf(key, sizeof(key));
					crypto_generichash(hash, sizeof(hash),
							input, ((unsigned long long) strlen((char *)input)),
							key, sizeof(key));
				}
				else
				{
					flag = 1;
				}
				break;
			
			case 3:
				randombytes_buf(key, sizeof(key));
				crypto_generichash_init(&state, key, sizeof(key), sizeof(hash));

				if(get_string(input))
				{
					crypto_generichash_update(&state, input, ((unsigned long long) strlen((char *)input)));
				}
				else
				{
					while(get_other_string(input, sub_input))
					{
						crypto_generichash_update(&state, sub_input, ((unsigned long long) strlen((char *)sub_input)));
					}
				}
				crypto_generichash_final(&state, hash, sizeof(hash));
				break;

			case 4:
				flag = 1;
				break;

			default:
				flag = 2;
				break;
		}
		if(flag == 0)
		{
			printf("Calculated Hash is: %s\n", hash);
		}
		else if(flag == 1)
		{
			printf("Exiting !!!!\n");
		}
		else
		{
			printf("Invalid Choice !!!!\n");
		}
	}while(ch != 4 && flag != 1);

	return 0;
}

int get_string(unsigned char *input)
{
	
	printf("\nEnter the Input String: ");
	scanf("%s", input);
	if(strlen((char *)input) > MAX_SIZE)
	{
		printf("panic! buffer overflow\n");
		return 0;
	}
	else if(strlen((char *)input) > CHUNK_SIZE)
	{
		total_length = strlen((char *)input);
		printf("Size is greater than (%d)"
		       "If option 3 is selected then code will proceed otherwise...\n", CHUNK_SIZE);
		return 0;
	}

	return 1;
}

int get_other_string(unsigned char *input, unsigned char *sub_input)
{
	int i;

	if(total_length > CHUNK_SIZE)
	{
		memmove(sub_input, input + start_from, CHUNK_SIZE);
		start_from += CHUNK_SIZE;
		total_length -= CHUNK_SIZE;
		return 1;
	}
	else
	{
		memmove(sub_input, input + start_from, total_length);
		total_length = 0;
		start_from = 0;
		return 0;
	}
}
