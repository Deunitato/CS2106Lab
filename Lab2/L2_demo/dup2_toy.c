#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	printf("STDIN_FILENO: %d\n", STDIN_FILENO);
	printf("STDOUT_FILENO: %d\n", STDOUT_FILENO);
	printf("STDERR_FILENO: %d\n", STDERR_FILENO);

	// use the file descriptor directly
	printf("Enter some content (less than 10 characters):\n");
	char str[100];
	// the line below reads the content you entered from console (aka via std input) into str
	int num = read(0, str, 10);
	// the line below writes the content in str into the console (aka via std output)
	write(1, str, num);

	// the line below creates a file descriptor that points to file.txt; O_WRONLY means this file descriptor can only be used to read from the file, not to write to the file
	int fd = open("file.txt", O_RDONLY);
	// the line below lets STDIN_FILENO file descriptor point to the file that fd points to (aka, file.txt)
	printf("running scanf before redirect std input to file.txt\nPlease enter a number:\n");
	scanf("%d", &num);
	printf("num: %d\n", num);

	dup2(fd, STDIN_FILENO);
	printf("running scanf after redirect standard input to file.txt\nPlease enter a number:\n");
	// notice how the scanf below doesn't block program from continueing to run, unlike the previous scanf that blocks the program until you enter some number
	scanf("%d", &num);
	// the number printed below actually comes from file.txt; if you modify the number in file.txt, the following will change accordingly
	printf("num: %d\n", num);
	return 0;
}
