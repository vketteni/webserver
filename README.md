This project is about writing your own HTTP server.
You will be able to test it with an actual browser.
HTTP is one of the most used protocols on the internet.
Knowing its arcane will be useful, even if you won’t be working on a website.


You must write a HTTP server in C++ 98.
Your executable will be run as follows:

	./webserv [configuration file]


Allowed functions:

	execve, dup, dup2, pipe, strerror, gai_strerror,
	errno, dup, dup2, fork, socketpair, htons, htonl,
	ntohs, ntohl, select, poll, epoll (epoll_create,
	epoll_ctl, epoll_wait), kqueue (kqueue, kevent),
	socket, accept, listen, send, recv, chdir bind,
	connect, getaddrinfo, freeaddrinfo, setsockopt,
	getsockname, getprotobyname, fcntl, close, read,
	write, waitpid, kill, signal, access, stat, open,
	opendir, readdir and closedir
