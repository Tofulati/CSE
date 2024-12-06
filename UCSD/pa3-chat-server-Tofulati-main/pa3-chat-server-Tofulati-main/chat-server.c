#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "http-server.h"

#define MAX_CHAT 10000
#define MAX_REACTIONS 10000

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_202_SUCCESS[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

typedef struct Reaction { // create new reaction
	char *user;
    	char *message;
} Reaction;

typedef struct Chat { // create new chat
	uint32_t id;
	char *user;
    	char *message;
    	char timestamp[20];
    	uint32_t num_reactions;
    	Reaction *reactions;
} Chat;

void handle_200(int client_sock, char *path);
void handle_404(int client_sock, char *path);

// define global variables
Chat *chat_arr[MAX_CHAT];
uint32_t curr_id = 0;

uint8_t convert_hex(char c) {                                                                                
       	if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
}

void decode_message(char* message, char* dest) {
        char *p_mes = message;
        char *p_dest = dest;
	
	while (*p_mes) {
        	if (*p_mes == '%' && *(p_mes + 1) && *(p_mes + 2)) {
            		uint8_t f = convert_hex(*(p_mes + 1));
            		uint8_t s = convert_hex(*(p_mes + 2));
            		uint8_t final = (f << 4) | s;

            		if (final == '%') {
                		*p_dest = '%';
            		} else {
                		*p_dest = (unsigned char)final;
            		}
            		p_mes += 3;
        	} else {
            		*p_dest = *p_mes;
            		p_mes++;
        	}
        	p_dest++;
    	}
    *p_dest = '\0';
}

// gets values of user and message and pass into the arrays
void extract_info(char *path, char *user, char *message, char *id) {
	const char* userkey = "user=";
	const char* messagekey = "message=";
	const char* idkey = "id=";

	char decoded_path[100000];
	decode_message(path, decoded_path); 

	const char *userpos = strstr(decoded_path, userkey);
	if (userpos) {
		userpos += strlen(userkey);
		const char *end = strchr(userpos, '&');
		if (end == NULL) {
			end = userpos + strlen(userpos);
		}
		int len = end - userpos;
		snprintf(user, len + 1, "%s", userpos);
		user[len] = '\0';

		char temp[strlen(user) + 1];
		decode_message(user, temp);
		snprintf(user, strlen(temp) + 1, "%s", temp);
	}

	const char *messagepos = strstr(decoded_path, messagekey);
	if (messagepos) {
		messagepos += strlen(messagekey);
		const char *end = strchr(messagepos, '&');
		if (end == NULL) {
                        end = messagepos + strlen(messagepos);
                }
		int len = end - messagepos;
		snprintf(message, len + 1, "%s", messagepos);
		message[len] = '\0';

		char dest[strlen(message)];
		decode_message(message, dest);
		snprintf(message, strlen(dest) + 1, "%s", dest);
	}

	const char *idpos = strstr(decoded_path, idkey);
	if (idpos) {
		idpos += strlen(idkey);
		const char *end = strchr(idpos, '&');
		if (end == NULL) {
			end = idpos + strlen(idpos);
		}
		int len = end - idpos;

		strncpy(id, idpos, len);
		id[len] = '\0';		
	}
}

// get timestamp
void get_timestamp(char* time_str) {
	time_t rawtime;
	time(&rawtime);

	struct tm *timeinfo = localtime(&rawtime);
	strftime(time_str, 23, "%Y-%m-%d %H:%M:%S", timeinfo);
}

uint8_t add_chat(char* username, char* message) {
	if (curr_id >= MAX_CHAT) {
		printf("Exceeded max chats allowed\n");
		return 0;
	}

	Chat *new_chat = (Chat *)malloc(sizeof(Chat));

	new_chat->id = ++curr_id;

	new_chat->user = calloc(strlen(username) + 1, sizeof(char));
	if (new_chat->user) {
		strcpy(new_chat->user, username);
	} else {
		free(new_chat);
		return 0;
	}
	
	new_chat->message = calloc(strlen(message) + 1, sizeof(char));
        if (new_chat->message) {
                strcpy(new_chat->message, message);
        } else {
		free(new_chat->user);
		free(new_chat);
		return 0;
	}

	get_timestamp(new_chat->timestamp);

	new_chat->num_reactions = 0;

	new_chat->reactions = NULL;

	chat_arr[curr_id - 1] = new_chat;

	return 1;
}

uint8_t add_reaction(char* username, char* message, char* id) {
	if (atoi(id) == 0) {
		printf("Chat is empty\n");
		return 0;
	}

	Chat *chat = NULL;
	int i = 0;
	for (i = 0; i < MAX_CHAT; i++) {
		if (chat_arr[i]->id == atoi(id)) {
			chat = chat_arr[i];
			break;
		}
	}

	if (chat == NULL) {
		printf("Chat is empty, no id\n");
		return 0;
	}	
	
	if (chat->num_reactions >= MAX_REACTIONS) {
		printf("Reached max reactions\n");
		return 0;
	}

	if (chat->reactions == NULL) {
		chat->reactions = malloc(sizeof(Reaction) * (chat->num_reactions + 1));
	} else {
		chat->reactions = realloc(chat->reactions, sizeof(Reaction) * (chat->num_reactions + 1));
	}
	if (!chat->reactions) {
        	printf("Failed to allocate memory for reactions\n");
        	return 0;
    	}

	Reaction new_reaction;
	new_reaction.user = calloc(strlen(username) + 1, sizeof(char));
	if (!new_reaction.user) {
		printf("Failed to allocate memory user\n");
		free(chat->reactions);
		chat->reactions = NULL;
		return 0;
	}
	strcpy(new_reaction.user, username);

	new_reaction.message = calloc(strlen(message) + 1, sizeof(char));
	if (!new_reaction.message) {
		printf("Failed to allocate memory message\n");	
		free(new_reaction.user);
		free(chat->reactions);
		chat->reactions = NULL;
		return 0;
	}
	strcpy(new_reaction.message, message);

	chat->reactions[chat->num_reactions] = new_reaction;
	
	chat->num_reactions++;

	return 1;
}

uint8_t edit_chat(char* id, char* message) {
	if (atoi(id) == 0) {
		printf("Chat is empty\n");
		return 0;
	}	

	if (strlen(message) > 255) {
		printf("Message is too long\n");
		return 0;
	}	

	Chat *chat = NULL;
	int i = 0;
	for (i = 0; i < MAX_CHAT; i++) {
		if (chat_arr[i] && chat_arr[i]->id == atoi(id)) {
			chat = chat_arr[i];
			break;
		}
	}	

	if (chat == NULL) {
		printf("Chat with ID isn't found\n");
		return 0;
	}

	if (chat->message) { 
		free(chat->message);
		chat->message = NULL;
	}

	chat->message = malloc(strlen(message) + 1);
	if (!chat->message) {
		printf("Failed to allocate memory\n");
		return 0;
	}

	strcpy(chat->message, message);

	return 1;
}

void respond_with_chats(int client) {
	if (*(chat_arr) == NULL) {
		char error[] = "Empty chat, add chat first.\n";
		write(client, error, strlen(error));
		//printf("%s\n", error);
		return;
	}

	int i = 0; 
	for (i = 0; i < curr_id; i++) {
		char *timestamp = chat_arr[i]->timestamp;
		int id = chat_arr[i]->id;
		char *user = chat_arr[i]->user;
		char *message = chat_arr[i]->message;
	
		
		int format_size = 64 + strlen(timestamp) + strlen(user) + strlen(message) + 10 + 1;
		char formatted_str[format_size];

		snprintf(formatted_str, sizeof(formatted_str), "[#%d %s]%13s: %s", id, timestamp, user, message);

		write(client, formatted_str, strlen(formatted_str));
		write(client, "\n", 1);
		//printf("%s\n", formatted_str);

		if (chat_arr[i]->reactions != NULL) {
			int j;
			for (j = 0; j < chat_arr[i]->num_reactions; j++) {
				char *reaction_user = chat_arr[i]->reactions[j].user;
				char *reaction_message = chat_arr[i]->reactions[j].message;

				if (reaction_user == NULL || reaction_message == NULL) {
					break;
				}
				
				int react_size = 64 + strlen(reaction_user) + strlen(reaction_message) + 33 + 1;
				char react_str[react_size];
				snprintf(react_str, sizeof(react_str), "%*s(%s)  %s", 30, "", reaction_user, reaction_message);
				
				write(client, react_str, strlen(react_str));
				write(client, "\n", 1);
				//printf("%s\n", react_str);
			}
		}
	}	
}

void reset() {
	uint32_t i = 0;
   	for (i = 0; i < MAX_CHAT; i++) {
       		if (chat_arr[i] != NULL) {
            		if (chat_arr[i]->reactions != NULL) {
		    		uint32_t j = 0;
                		for (j = 0; j < chat_arr[i]->num_reactions; j++) {
                    			if (chat_arr[i]->reactions[j].user != NULL) {
                        			free(chat_arr[i]->reactions[j].user);
                        			chat_arr[i]->reactions[j].user = NULL;
                    			}
                    			if (chat_arr[i]->reactions[j].message != NULL) {
                        			free(chat_arr[i]->reactions[j].message);
                        			chat_arr[i]->reactions[j].message = NULL;
                    			}
                		}
                		free(chat_arr[i]->reactions);
                		chat_arr[i]->reactions = NULL;
            		}

            		if (chat_arr[i]->user != NULL) {
                		free(chat_arr[i]->user);
                		chat_arr[i]->user = NULL;
            		}
            		if (chat_arr[i]->message != NULL) {
                		free(chat_arr[i]->message);
                		chat_arr[i]->message = NULL;
            		}

            		free(chat_arr[i]);
            		chat_arr[i] = NULL;
        	}
    	}
    	curr_id = 0;
}


void handle_404(int client_sock, char *path)  {
    	printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    	char response_buff[BUFFER_SIZE];
    	snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    	// snprintf includes a null-terminator
	
	write(client_sock, response_buff, strlen(response_buff));
}

void handle_200(int client_sock, char *path)  {
    	char response_buff[BUFFER_SIZE];
    	snprintf(response_buff, BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\n\r\n");
    	// snprintf includes a null-terminator
	write(client_sock, response_buff, strlen(response_buff));
}

void handle_post(char* request, int client) {
	char path[256];

    	printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    	// Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    	if (sscanf(request, "GET %255s", path) != 1) {
       		printf("Invalid request line\n");
		handle_404(client,path);
        	return;
    	} else {
		char user[15];                                            
		char message[255];                                                              
		char id[100];
                                                                                                     
		memset(user, 0, sizeof(user));
		memset(message, 0, sizeof(message));                                               
		memset(id, 0, sizeof(id));                                                                                    
		if (strcmp(path, "/chats") == 0) { 
			handle_200(client, request);			
			printf("Opened chat\n");
			respond_with_chats(client);                                              
		} else if (strstr(path, "/post?")) {                                    
			printf("Reading post\n");
                	extract_info(path, user, message, id);   

			if (strlen(user) == 0 || strlen(message) == 0) {
				handle_404(client, request);
				return;
			}
				
                	if (strlen(user) > 15 || strlen(message) > 255 || curr_id > 100000) {
				handle_404(client, request);
				return;
			} else {
				handle_200(client, path);	
				add_chat(user, message);
                        	respond_with_chats(client);
			}     
		} else if (strstr(path, "/react?")) {
                	if (curr_id == 0) {
				handle_404(client, request);                                 
                        	return;
                	} else {
                        	extract_info(path, user, message, id);

				if (strlen(user) == 0 || strlen(message) == 0 || strlen(id) == 0) {
					handle_404(client, request);
					return;
				}

                        	if (strlen(user) > 15 || strlen(message) > 15 || atoi(id) > 100 || atoi(id) > curr_id || curr_id > 100) {
                                	handle_404(client, request);
					return;                                                    
				} else {      
					handle_200(client, path);
					printf("Adding reaction\n");
					add_reaction(user, message, id);
					respond_with_chats(client);                                
				}                                                                  
			}
		} else if (strstr(path, "/edit?")) {
			if (curr_id == 0) {                                                                                           handle_404(client, request);
                                return;
                        } else {                                                                                                      extract_info(path, user, message, id);

                                if (strlen(message) == 0 || strlen(id) == 0) {                                           handle_404(client, request);
                                        return;
                                }
                                                                                                                                      if (strlen(message) > 15 || atoi(id) > 100 || atoi(id) > curr_id || curr_id > 100) {
                                        handle_404(client, request);                                                                          return;                                                                                       } else {
                                        handle_200(client, path);                                                                             printf("Adding reaction\n");                                                                          edit_chat(id, message);
                                        respond_with_chats(client);                                                                   }                                                                                             }
		} else if (strcmp(path, "/reset") == 0) {
			handle_200(client, path);
			printf("Reseting server\n");
			reset(); 
			printf("Server resetted\n");
        	} else {                                                                     
			printf("Invalid request line\n");
			return;
        	}
		
		memset(user, 0, sizeof(user));
		memset(message, 0, sizeof(message));                        
		memset(id, 0, sizeof(id));
	}

}

int main(int argc, char* argv[]) {
	int port = 0;
	if (argc >= 2) {
		port = atoi(argv[1]);
	}

	start_server(&handle_post, port);

	reset();

    	return 0;
}


