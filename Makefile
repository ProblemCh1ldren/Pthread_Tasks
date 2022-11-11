all: 
	gcc -Wall -g Task_Without_Pthreads.c -o Task_Without_Pthreads -lm
	gcc -Wall -g Task_With_Pthreads.c -o Task_With_Pthreads -lm
	gcc -Wall -g filecreate.c -o filecreate -lm
	gcc -Wall -g TCP_server.c -o TCP_server -lm
	gcc -Wall -g TCP_cient.c -o TCP_client -lm
