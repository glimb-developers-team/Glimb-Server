/*
* Polyakov Daniil
* Mail: arjentix@gmail.com
* Github: Arjentix
* Date: 12.03.19
*/

#include "Server.h"
#include "ClientProcessor.h"
#include "LogPrinter.h"
#include <string>
#include <signal.h>
#include <unistd.h>

/*
* This is the main file of Glimb-Server program.
* It is listening for the client and processing their requests.
*/

/* Server port */
#define PORT 4512

/* If finish will be true, then server will stop his work. */
bool finish = false;

/*
* signal_handler() - hanldet for signal like SIGINT and SIGTERM.
*/
void signal_handler(int sig)
{
	LogPrinter::print("Stopped by system signal");
	finish = true;
}

int main()
{
	int fork_res;
	int return_code = 0;

	fork_res = fork();
	if (fork_res != 0) {	// Parent
		return 0;
	}
	else {			// Child
		/* Closing useless fds */
		close(0);
		close(1);
		close(2);
		try {
			LogPrinter::print("Server started");
			/* Initialization */
			Server server(PORT);
			ClientProcessor client_processor;
			int client_sockfd;

			/* Preparing for signal processing */
			signal(SIGTERM, signal_handler);
			signal(SIGINT, signal_handler);

			/* Start working */
			server.start();
			while (finish != true) {
				client_sockfd = server.get_client();
				if (client_sockfd != -1) {
					client_processor.new_client(client_sockfd);
				}
			}
		}
		catch (const char *error) {
			LogPrinter::print(error);
			return_code = -1;
		}

		LogPrinter::print("Finishing program");
		return return_code;
	}
}
