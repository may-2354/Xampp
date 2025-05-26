#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Function to analyze logs using a keyword and write results to an output file
void keywordAnalyzer(const char* inputFile, const char* outputFile, const char* keyword) {
    int pipefd[2];
    pid_t cpid1, cpid2;

    printf("Setting up the pipe for inter-process communication...\n");

    if (pipe(pipefd) == -1) {
        perror("Error: Failed to create pipe");
        exit(EXIT_FAILURE);
    }

    printf("Creating first child process to read from input file...\n");
    cpid1 = fork();
    if (cpid1 == -1) {
        perror("Error: Failed to fork first child");
        exit(EXIT_FAILURE);
    }

    if (cpid1 == 0) {  // First child process
        printf("  [Child 1] PID: %d | Parent PID: %d\n", getpid(), getppid());

        close(pipefd[0]);  // Close unused read end

        printf("  [Child 1] Opening input file: %s\n", inputFile);
        int logFd = open(inputFile, O_RDONLY);
        if (logFd == -1) {
            perror("Error: Unable to open input file");
            exit(EXIT_FAILURE);
        }

        printf("  [Child 1] Redirecting input and output...\n");
        dup2(logFd, STDIN_FILENO);      // stdin <- input file
        dup2(pipefd[1], STDOUT_FILENO); // stdout -> pipe

        close(pipefd[1]);
        close(logFd);

        printf("  [Child 1] Executing 'cat' to pass file content to pipe...\n");
        execlp("cat", "cat", (char*)NULL);
        perror("Error: execlp 'cat' failed");
        exit(EXIT_FAILURE);
    }

    printf("Creating second child process to filter content using 'grep'...\n");
    cpid2 = fork();
    if (cpid2 == -1) {
        perror("Error: Failed to fork second child");
        exit(EXIT_FAILURE);
    }

    if (cpid2 == 0) {  // Second child process
        printf("  [Child 2] PID: %d | Parent PID: %d\n", getpid(), getppid());

        close(pipefd[1]);  // Close unused write end

        printf("  [Child 2] Redirecting pipe input to stdin...\n");
        dup2(pipefd[0], STDIN_FILENO);  // stdin <- pipe

        printf("  [Child 2] Opening/creating output file: %s\n", outputFile);
        int outputFd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outputFd == -1) {
            perror("Error: Unable to open output file");
            exit(EXIT_FAILURE);
        }

        dup2(outputFd, STDOUT_FILENO);  // stdout -> output file

        close(pipefd[0]);
        close(outputFd);

        printf("  [Child 2] Executing 'grep -i %s' to search for the keyword...\n", keyword);
        execlp("grep", "grep", "-i", keyword, (char*)NULL);
        perror("Error: execlp 'grep' failed");
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);

    printf("Waiting for both child processes to complete...\n");
    waitpid(cpid1, NULL, 0);  // Wait for first child
    waitpid(cpid2, NULL, 0);  // Wait for second child

    printf("Keyword search completed successfully!\n");
    printf("         Results saved in: %s\n", outputFile);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <inputFile> <outputFile> <keyword>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* inputFile = argv[1];  
    const char* outputFile = argv[2]; 
    const char* keyword = argv[3]; 

    printf("Starting keyword analyzer...\n");
    printf("Parent PID: %d\n", getpid());
    printf("Input File  : %s\n", inputFile);
    printf("Output File : %s\n", outputFile);
    printf("Keyword     : %s\n", keyword);

    keywordAnalyzer(inputFile, outputFile, keyword);

    printf("Program finished.\n");
    return 0;
}
