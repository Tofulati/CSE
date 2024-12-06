#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Milestone 1
int32_t is_ascii(char str[]) {
    int i = 0;
    while (str[i] != 0) {
        if ((unsigned char)str[i] > 127) {
            return 0;
        }

        i++;
    }
    return 1;
}

int32_t capitalize_ascii(char str[]) {
    int i = 0, counter = 0;
    while ((unsigned char)str[i] != 0) {
        if ((unsigned char)str[i] >= 97 && (unsigned char)str[i] < 123) {
            str[i] -= 32;
            counter++;
        } 

        i++;
    }

    return counter;
}

// Milestone 2
bool contbyte(unsigned char byte) {
    return (byte & 0xC0) == 0x80;
}

int32_t width_from_start_byte(char start_byte) {
    if (((unsigned char)start_byte & 0x80) == 0x00) {
        return 1;
    }  else if (((unsigned char)start_byte & 0xE0) == 0xC0) {
        return 2;
    }  else if (((unsigned char)start_byte & 0xF0) == 0xE0) {
        return 3;
    }  else if (((unsigned char)start_byte & 0xF8) == 0xF0) {
        return 4;
    }   else {
        return -1;
    }
}

int32_t utf8_strlen(char str[]) {
    int length = 0;
    int byte_index = 0;

    while(str[byte_index] != 0) {
        if (((unsigned char)str[byte_index] & 0x80) == 0) {
            length++;
            byte_index++;
        }  else if (((unsigned char)str[byte_index] & 0xE0) == 0xC0) {
            length++;
            byte_index += 2;
        }  else if (((unsigned char)str[byte_index] & 0xF0) == 0xE0) {
            length++;
            byte_index += 3;
        }  else if (((unsigned char)str[byte_index] & 0xF8) == 0xF0) {
            length++;
            byte_index += 4;
        }   else {
            return -1;
        }
    }

    return length;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {
    int byte_length = 0;
    int str_index = 0;
    
    for (int i = 0; i < cpi; i++) {
        if (((unsigned char)str[str_index] & 0x80) == 0) {
            byte_length++;
            str_index++;
        }  else if (((unsigned char)str[str_index] & 0xE0) == 0xC0) {
            byte_length += 2;
            str_index += 2;
        }  else if (((unsigned char)str[str_index] & 0xF0) == 0xE0) {
            byte_length += 3;
            str_index +=3;
        }  else if (((unsigned char)str[str_index] & 0xF8) == 0xF0) {
            byte_length += 4;
            str_index += 4;
        }
    }
    return byte_length;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    if (cpi_start >= cpi_end || cpi_start < 0 || cpi_end < 0) {
        result[0] = '\0';
        return;
    }

    int byte_index = codepoint_index_to_byte_index(str, cpi_start), cpi_index = cpi_start, res_index = 0;

    while(str[byte_index] != '\0') {
        int char_len = 0;

        if (((unsigned char)str[byte_index] & 0x80) == 0) {
            char_len = 1;
        }  else if (((unsigned char)str[byte_index] & 0xE0) == 0xC0) {
            char_len = 2;
        }  else if (((unsigned char)str[byte_index] & 0xF0) == 0xE0) {
            char_len = 3;
        }  else if (((unsigned char)str[byte_index] & 0xF8) == 0xF0) {
            char_len = 4;
        } else {
            byte_index++;
            continue;
        }

        if (cpi_index >= cpi_start && cpi_index < cpi_end) {
            for (int i = 0; i < char_len; ++i) {
                result[res_index++] = str[byte_index + i];
            }
        }

        byte_index += char_len;
        cpi_index++;

        if (cpi_index >= cpi_end) {
            break;
        }
    }

    result[res_index] = '\0';
}

// Milestone 3
int32_t codepoint_at(char str[], int32_t cpi) {
    int codepoint_index = 0;
    int byte_index = 0;

    while(str[byte_index] != 0) {
        if (codepoint_index == cpi) {
            int32_t codepoint = 0;
            int char_len;

            if ((unsigned char)str[byte_index] < 0b10000000) {
                codepoint = str[byte_index];
                char_len = 1;
            } else if ((unsigned char)str[byte_index] < 0b11100000) {
                codepoint = str[byte_index] & 0b00011111;
                char_len = 2;
            }  else if ((unsigned char)str[byte_index] < 0b11110000) {
                codepoint = str[byte_index] & 0b00001111;
                char_len = 3;
            }  else if ((unsigned char)str[byte_index] < 0b11111000) {
                codepoint = str[byte_index] & 0b00000111;
                char_len = 4;
            }   

            for (int i = 1; i < char_len; i++) {
                codepoint = (codepoint << 6) | (str[byte_index + i] & 0b00111111);
            }
            
            return codepoint;
        }

        if((unsigned char)str[byte_index] > 127) {
            while(contbyte(str[byte_index + 1])) {
                byte_index++;
            }
        }

        byte_index++;
        codepoint_index++;
    }

    return -1;
}

bool is_animal_emoji(int32_t codepoint) {
    if ((codepoint >= 0x1F400 && codepoint <= 0x1F43F) || (codepoint >= 0x1F980 && codepoint <= 0x1F9AE)) {
        return true;
    }
    return false;
}

char is_animal_emoji_at(char str[], int32_t cpi) {
    return is_animal_emoji(codepoint_at(str, cpi));
}

void next_utf8_char(char str[], int32_t cpi, char result[]) {
    int32_t codepoint = codepoint_at(str, cpi);

    if (codepoint == -1) {
        result[0] = '\0';
        return;
    }

    codepoint++;

    if (codepoint > 0x10FFFF) {
        result[0] = '\0';
        return;
    }
    
    if (codepoint <= 0x7F) { //ascii
        result[0] = (char)codepoint;
        result[1] = '\0';
    } else if (codepoint <= 0x7FF) { //two byte special
        result[0] = 0b11000000 | (codepoint >> 6);
        result[1] = 0b10000000 | (codepoint & 0b00111111);
        result[2] = '\0';
    } else if (codepoint <= 0xFFFF) { //three byte special
        result[0] = 0b11100000 | (codepoint >> 12);
        result[1] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
        result[2] = 0b10000000 | (codepoint & 0b00111111);
        result[3] = '\0';
    } else { //four byte special
        result[0] = 0b11110000 | (codepoint >> 18);
        result[1] = 0b10000000 | ((codepoint >> 12) & 0b00111111);
        result[2] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
        result[3] = 0b10000000 | (codepoint & 0b00111111);
        result[4] = '\0';
    }
}

void utf8analyzer() {
// enter string
    char str[50];
    printf("Enter a UTF-8 encoded string: ");
    fgets(str, 50, stdin);
// removing extra \n from fgets
    int index = 0;
    while (str[index] != '\0') {
        if (str[index] == '\n' || str[index] == '\r') {
            str[index] = '\0';
            break;
        }
        index++;
    }
    printf("\n");
// seeing validity
    if (is_ascii(str)) {
        printf("Valid ASCII: true\n");
    } else {
        printf("Valid ASCII: false\n");
    }
// copying for upppercase
    char copy[50];
    index = 0;
    while ((unsigned char)str[index] != 0) {
        copy[index] = str[index];
        index++;
    }
    copy[index + 1] = 0;
    capitalize_ascii(copy);
    printf("Uppercased ASCII: %s\n", copy);
// check length in bytes
    index = 0;
    int total = 0;
    while (str[index] != 0) {
        int width = width_from_start_byte(str[index]);
        if (width != -1) { 
            total += width;
        };
        index += width;
    }
    printf("Length in bytes: %d\n", total);
// check number of code points
    printf("Number of code points: %d\n", utf8_strlen(str));
// check bytes per code point
    printf("Bytes per code point: ");
    index = 0;
    while (str[index] != 0) {
        if (width_from_start_byte(str[index]) != -1) {
            printf("%d ", width_from_start_byte(str[index]));
        };
        index += width_from_start_byte(str[index]);
    }
    printf("\n");
// allocates empty char array for substring
    char res[50];
    utf8_substring(str, 0, 6, res);
    printf("Substring of the first 6 code points: \"%s\"\n", res);
// prints code points as decimals
    printf("Code points as decimal numbers: ");
    for (int i = 0; i < utf8_strlen(str); i++) {
        printf("%d ", codepoint_at(str, i));
        if (codepoint_at(str, i) == -1) {
            break;
        }
    }
    printf("\n");
// prints animal emojis in string
    printf("Animal emojis: ");
    for (int i = 0; i < utf8_strlen(str); i++) {
        int bind = codepoint_index_to_byte_index(str, i);
        if (is_animal_emoji_at(str, i)) {
            printf("%c%c%c%c", str[bind], str[bind + 1], str[bind + 2], str[bind + 3]);
        }
    }
    printf("\n");
//prints next char of codepoint
    char result[100];
    int32_t idx = 3;
    next_utf8_char(str, idx, result);
    if (utf8_strlen(str) < 4) {
        printf("Next Character of Codepoint at Index 3:  \n");
    } else {
        printf("Next Character of Codepoint at Index 3: %s\n", result);
    }
}

// Test functions
int main() {
    utf8analyzer();
}