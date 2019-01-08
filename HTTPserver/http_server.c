#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

int main() {

	// create file to read from
	FILE *html_data;
	html_data = fopen("index.html", "r");

	// create string to store data
	char response_data[1024];
	fgets(response_data, 1025, html_data);
	//printf("%s", response_data);

	// response header to say if ok or not + adding to resp
	char http_header[2048] = "HTTP/1.1 200 OK\r\n\r\n";
	strcat(http_header, response_data);
	printf("%s", http_header);

	// create a server socker
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// define address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9000);
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
		printf("Error with bind");
		exit(EXIT_FAILURE);
	}

	if (listen(server_socket, 5) < 0) {
		printf("Error with listen");
		exit(EXIT_FAILURE);
	}

	int client_socket;
	printf("client time\n");
	// infinitely want server to receive requests and respond until told to stop/exit using ctrl+D and so on
	while (1) {
		write(1, "yea\t", 4);
		if ((client_socket = accept(server_socket, NULL, NULL)) < 0) {
			printf("Error with accept");
			exit(EXIT_FAILURE);
		}
		write(1, "accepted\t", 9);
		if ((send(client_socket, http_header, sizeof(http_header), 0)) < 0) {
			printf("Error with send");
			exit(EXIT_FAILURE);
		}
		write(1, "DONE\n", 5);
		usleep(10000);
		close(client_socket);
	}
	//close(server_socket);
	return (0);
}
