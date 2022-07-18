#include "functions.h"

int bitwise_xor(int value){
    return value ^ KEY;
}

char *xor_encrypt(char c){
    int encrypted = bitwise_xor(c);
    char* encrypted_bits = malloc(sizeof(char) * 8);  // 7 digits + terminating char
    encrypted_bits[7] = '\0';  // terminating char
    for(int i = 6; i >= 0; i--) {
        if(pow(2, i) <= encrypted) {
            encrypted_bits[6-i] = '1';
            encrypted -= pow(2, i);
        } else
            encrypted_bits[6-i] = '0';
    }
    return encrypted_bits;
}

char xor_decrypt(char *s){
    int encrypted = 0;
    for(int i = 0; i < 7; i++) {
        if(s[i] == '1') {
            encrypted += pow(2, 6-i);
        }
    }
    return bitwise_xor(encrypted);
}

char *gen_code(char *msg){
    int msg_index = 0;  // char index in {@msg}
    int bi_index = 0;  // encrypted binary string of char index
    char* msg_en = xor_encrypt(msg[msg_index]);

    char* sc = malloc(sizeof(char) * 257);
    sc[256] = '\0';
    for(int row = 0; row < 16; row++) {
        for(int col = 0; col < 16; col++) {
            int index = 16 * row + col;

            // aligning binaries
            if(row < 5) {  // first two aligning boxes
                if(col == 0 || col == 4 || col == 11 || col == 15) {  // column space
                    sc[index] = '1';
                    continue;
                }
                else if((row == 0 || row == 4) && ((col > 0 && col < 4) || (col > 11 && col < 15))) {  // remaining row space
                    sc[index] = '1';
                    continue;
                }
                else if(index == 34 || index == 45) {  // centre dots
                    sc[index] = '1';
                    continue;
                }
                else if((col > 0 && col < 4) || (col > 11 && col < 15)) {  // remaining space in squares
                    sc[index] = '0';
                    continue;
                }
            } else if(row > 10) {  // third aligning box
                if(col == 0 || col == 4) {  // column space
                    sc[index] = '1';
                    continue;
                }
                else if((row == 11 || row == 15) && (col > 0 && col < 4)) {  // remaining row space
                    sc[index] = '1';
                    continue;
                }
                else if(index == 210) {  // centre dots
                    sc[index] = '1';
                    continue;
                }
                else if(index == 255) {  // last index
                    sc[index] = '1';
                    continue;
                }
                else if(col > 0 && col < 4) {  // remaining space in squares
                    sc[index] = '0';
                    continue;
                }
            }

            // storing message
            if(msg_index != -1)
                sc[index] = msg_en[bi_index++];
            else
                sc[index] = '0';

            // check indexing of message
            if(bi_index == 7) {  // finishes inputting binary
                msg_index++;
                free(msg_en);
                if(msg_index == strlen(msg) + 1) {  // checks if message char reaches the end
                    msg_index = -1;
                    bi_index = -1;
                } else {
                    bi_index = 0;  // reset binary counter
                    msg_en = xor_encrypt(msg[msg_index]);  // gets new binary string from new char
                }
            }
        }
    }
    if(msg_index != -1) free(msg_en);
    return sc;
}

char *read_code(char *code){
    char* result_string = (char*)malloc(sizeof(char)*26);
    char* working;
    int result_index = 0;
    int working_index = 0;

    for(int row = 0; row < 16; row++) {
        for(int col = 0; col < 16; col++) {
            int index = 16 * row + col;

            if(row < 5) {  // first two aligning boxes
                if(col == 0 || col == 4 || col == 11 || col == 15) {  // column space
                    //code[index] = '1';
                    continue;
                }
                else if((row == 0 || row == 4) && ((col > 0 && col < 4) || (col > 11 && col < 15))) {  // remaining row space
                    //code[index] = '1';
                    continue;
                }
                else if(index == 34 || index == 45) {  // centre dots
                    //code[index] = '1';
                    continue;
                }
                else if((col > 0 && col < 4) || (col > 11 && col < 15)) {  // remaining space in squares
                    //code[index] = '0';
                    continue;
                }
            } else if(row > 10) {  // third aligning box
                if(col == 0 || col == 4) {  // column space
                    //code[index] = '1';
                    continue;
                }
                else if((row == 11 || row == 15) && (col > 0 && col < 4)) {  // remaining row space
                    //code[index] = '1';
                    continue;
                }
                else if(index == 210) {  // centre dots
                    //code[index] = '1';
                    continue;
                }
                else if(index == 255) {  // last index
                    //code[index] = '1';
                    continue;
                }
                else if(col > 0 && col < 4) {  // remaining space in squares
                    //code[index] = '0';
                    continue;
                }
            }
			
            if (working_index == 0){
                working = (char*)malloc(sizeof(char)*8);
                working[working_index] = code[index];
				//printf("\n%c\n", code[index]);
                working_index++;
            }
			else{
                working[working_index] = code[index];
                working_index++;
            }

            if (working_index == 7){
                working[working_index] = '\0';
                result_string[result_index] = xor_decrypt(working);
				//printf("\n%c\n", result_string[result_index]);
                free(working);
                working_index = 0;
                result_index++;
            }

        }
    }
	free(working);
    return result_string;
}

char bi_to_hex(char* bi_code) {
    int bi = 8 * bi_code[0] + 4 * bi_code[1] + 2 * bi_code[2] + bi_code[3];
    free(bi_code);

    if(bi < 10) {
        return bi + 48;
    } else {
        return bi + 55;
    }
}

char* hex_to_bi(char hex_code){
	
	char* bi_code = (char*)malloc(sizeof(char)*5);
	bi_code[4] = '\0';
	int sum = 0;
	
	if (hex_code < 'A'){
		sum = hex_code - 48;
	}
		
	if(hex_code >= 'A'){
		sum = hex_code - 55;
	}
	
	if (sum >= 8){
		bi_code[0] = '1';
		sum = sum - 8;
	}
	else{
		bi_code[0] = '0';
	}
	
	if(sum >= 4){
		bi_code[1] = '1';
		sum = sum - 4;
	}
	else{
		bi_code[1] = '0';
	}
	
	if(sum >= 2){
		bi_code[2] = '1';
		sum = sum - 2;
	}
	else{
		bi_code[2] = '0';
	}
	
	if(sum >= 1){
		bi_code[3] = '1';
		sum = sum - 1;
	}
	else{
		bi_code[3] = '0';
	}
	
	return bi_code;
}

char *compress(char *code){
    char* compressed = malloc(sizeof(char) * 65);
    compressed[64] = '\0';
    for(int row = 0; row < 16; row++) {
        for(int col = 0; col < 4; col++) {
            int bi_index = row * 16 + col * 4;
            int hex_index = row * 4 + col;

            char* bi_code = malloc(sizeof(char) * 4);
            for(int i = 0; i < 4; i++) {
                bi_code[i] = code[bi_index + i] - 48;
            }
            compressed[hex_index] = bi_to_hex(bi_code);
        }
    }
    return compressed;
}

char *decompress(char *code){
	char* decompressed = malloc(sizeof(char) * 257);
	
    decompressed[256] = '\0';
	
	for(int row = 0; row < 16; row++) {
        for(int col = 0; col < 4; col++) {
            int bi_index = row * 16 + col * 4;
            int hex_index = row * 4 + col;
			
			char* bi_code;
			bi_code = hex_to_bi(code[hex_index]);
			for (int i = 0; i<4; i++){
				decompressed[bi_index + i] = bi_code[i];
			}
			free(bi_code);
		}
			
	}
	return decompressed;
}

int calc_ld(char *sandy, char *cima){
    if(sandy[0] == '\0' && cima[0] == '\0') return 0;
    else if(sandy[0] == '\0') return strlen(cima);
    else if(cima[0] == '\0') return strlen(sandy);

    char s_new[strlen(sandy)];
    char c_new[strlen(cima)];
    strncpy(s_new, &sandy[1], strlen(sandy) - 1);
    strncpy(c_new, &cima[1], strlen(cima) - 1);
    s_new[strlen(sandy) - 1] = '\0';
    c_new[strlen(cima) - 1] = '\0';
    if(sandy[0] == cima[0]) return calc_ld(s_new, c_new);
    else {
        int v1, v2, v3;
        v1 = calc_ld(s_new, c_new);
        v2 = calc_ld(sandy, c_new);
        v3 = calc_ld(s_new, cima);

        if(v1 <= v2 && v1 <= v3)
            return 1 + v1;
        else if(v1 <= v2)
            return 1 + v3;
        else if(v1 <= v3)
            return 1 + v2;
        else if(v2 <= v3)
            return 1 + v2;
        else
            return 1 + v3;
    }
}