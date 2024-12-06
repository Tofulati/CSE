#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <assert.h>

const int testing = 1;

// Milestone 1
unsigned char hex_byte_helper(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else {
		return 0;
	}
}

uint8_t hex_to_byte(unsigned char h1, unsigned char h2) {
	return (hex_byte_helper(h1) << 4) | hex_byte_helper(h2);
}

void hexstr_to_hash(char hexstr[], unsigned char hash[32]) {
	int i = 0;
	for (i = 0; i < 32; i++) {
	       hash[i] =  hex_to_byte(hexstr[2 * i],hexstr[2 * i + 1]);
	}	
}


// Milestone 2
int8_t check_password(char password[], unsigned char given_hash[32]) {
	unsigned char hashed_pass[32];

	SHA256((unsigned char *)password, strlen(password), hashed_pass);

	int i = 0; 
	 for (i = 0; i < 32; i++) {
		if (hashed_pass[i] != given_hash[i]) {
			return 0;
		}
	}
	return 1;
}

// Milestone 3
int8_t crack_password(char password[], unsigned char given_hash[]) {
	if (check_password(password, given_hash)) {
		return 1;
	}

	int len = strlen(password);
	int i = 0;
	for (i = 0; i < len; i++) {
		char og_char = password[i];
		if (password[i] >= 'a' && password[i] <= 'z') {
			password[i] = password[i] - 32;
			if (check_password(password, given_hash)) {
				return 1;
			}
			password[i] = og_char;
		} else if (password[i] >= 'A' && password[i] <= 'Z') {
                        password[i] = password[i] + 32;
                        if (check_password(password, given_hash)) {
                                return 1;
                        }
			password[i] = og_char;
              	}

		if (password[i] >= '0' && password[i] <= '9') {
			int rep = 0;
			for (rep = 0; rep <= 9; rep++) {
				if (rep != (password[i] - '0')) {
					password[i] = '0' + rep;
					if (check_password(password, given_hash)) {
						return 1;
					}
					password[i] = og_char;
				}
			}
		}
	}
	return 0;
}


// testing
void test_hex_to_byte() {
	assert(hex_to_byte('c', '8') == 200);
  	assert(hex_to_byte('0', '3') == 3);
	assert(hex_to_byte('0', 'a') == 10);
	assert(hex_to_byte('1', '0') == 16);
	assert(hex_to_byte('A', 'F') == 0xAF);
	assert(hex_to_byte('F', '0') == 0xF0);
	assert(hex_to_byte('1', 'F') == 0x1F);
	assert(hex_to_byte('C', '3') == 0xC3);
	assert(hex_to_byte('0', '8') == 0x08);
	assert(hex_to_byte('F', 'F') == 0xFF);	
}

void test_hexstr_to_hash() {
  	char hexstr[64] = "a2c3b02cb22af83d6d1ead1d4e18d916599be7c2ef2f017169327df1f7c844fd";
  	unsigned char hash[32];
  	hexstr_to_hash(hexstr, hash);
	//int i = 0;
	//for(i = 0; i < 32; i += 1) {
	//	printf("%02x ", hash[i]);
	//}
  	assert(hash[0] == 0xa2);
  	assert(hash[31] == 0xfd);
}

void test_check_password() {
	char hash_as_hexstr[] = "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8"; // SHA256 hash for "password"
        unsigned char given_hash[32];
        hexstr_to_hash(hash_as_hexstr, given_hash);                                                           	assert(check_password("password", given_hash) == 1);
        assert(check_password("wrongpass", given_hash) == 0);
}

void test_crack_password() {
	char password[] = "paSsword";
	char hash_as_hexstr[] = "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8"; // SHA256 hash of "password"
	unsigned char given_hash[32];
	hexstr_to_hash(hash_as_hexstr, given_hash);
	int8_t match = crack_password(password, given_hash);
	assert(match == 1);
	assert(password[2] == 's'); // the uppercase 'S' has been lowercased
}

int main(int argc, char** argv) {
	unsigned char hash[32];
  	hexstr_to_hash(argv[1], hash);

	int num_pass = 32;
	int pass_len = 32;
	char passwords[num_pass][pass_len];
	int curr_pass = 0;

	while (fgets(passwords[curr_pass], pass_len, stdin) != NULL) {
		int len = strlen(passwords[curr_pass]);
		if (len > 0 && passwords[curr_pass][len - 1] == '\n') { 
			passwords[curr_pass][len - 1] = '\0';
		}
		curr_pass++;
	}

	int i = 0;
	int found = 0;
	for (i = 0; i < curr_pass; i++) {
		if (crack_password(passwords[i], hash) == 1) {
			printf("Found password: SHA256(%s) = ", passwords[i]);
			int j = 0;
            		for (j = 0; j < 32; j++) {
                		printf("%02x", hash[j]);
            		}
            		printf("\n");
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Did not find a matching password\n");
	}

	// unit testing
	if(testing) {
               test_hex_to_byte();
               test_hexstr_to_hash();
                test_check_password();
                test_crack_password();
        }
}
	
