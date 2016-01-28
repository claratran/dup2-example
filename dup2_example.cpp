/**
 * Program demonstrating outputting a child's output to a file (without any
 * work by you) via dup2.
 *
 * man 2 fork
 * man 3 exec
 * man 2 wait
 * man 2 dup2
 * man perror
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int main() {
  string file = "/tmp/child_process_output";

  int cpid = fork();

  if (cpid == -1) {
    perror("not good");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0) {
    // child

    // Open a file to which we can redirect this process' output.
    //  - Make sure you read about what the flags mean, and update them
    //    accordingly depending on what you're trying to do.
    //  - 0644 represents the file permissions to use.
    int fd = open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // In an actual program, you should always check system calls for error.

    // Change STDOUT_FILENO to point to the file. Try running this program with
    // this line both commented out vs not and make sure you understand what's
    // happening.
    dup2(fd, STDOUT_FILENO);

    // Again, you should check for errors...

    // Execute 'ls -l'; all output should get written to the file, with no extra
    // work required.
    execl("/bin/ls", "ls", "-l", 0);

    // Since exec clobbers the currently running program (in this case, with the
    // 'ls' command), the only way we reach this point is if the execl command
    // failed. Print an error.
    perror("execl");
    exit(EXIT_FAILURE);
  } else {
    // This is the parent.
    int status;

    // Wait on the child, and make sure we save its return code.
    waitpid(cpid, &status, 0);

    cout << "Child terminated with status: " << status << endl;
    cout << "Its output should be written here: " << file << endl;
  }

  return EXIT_SUCCESS;
}
