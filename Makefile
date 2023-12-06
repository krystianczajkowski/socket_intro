server: server.c
	cc server.c -o server
	chmod +x server

client: client.c
	cc client.c -o client
	chmod +x client

winclient: clientwin.c
	gcc clientwin.c - o clientwin -lws2_32