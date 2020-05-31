# HttpRequest
Only for Windows
Send HTTP request in C and grab the result
if you compile in visual studio code you have to add -lws2_32 library

EXAMPLE---> gcc HttpRequest.c -o HttpRequest -lws2_32

example of use of the program

HttpRequest www.google.com 80 / GET
