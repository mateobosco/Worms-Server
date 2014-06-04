#include "main_server.h"
#include "main_client.h"

int main(int argc,char* argv[]){
	bool server = true;
	if (server)	return main_server(argc, argv);
	else 	return mainCliente(argc, argv);
}
