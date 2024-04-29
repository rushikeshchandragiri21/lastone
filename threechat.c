#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MAX_MSG_SIZE 1024
#define MSG_TYPE 1

struct message_buffer {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct message_buffer message;

    // Generate a unique key
    key = ftok(".", 'A');

    // Create a message queue
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Chat Application\n");
    printf("Type 'exit' to end the chat.\n");

    // Fork child processes to send and receive messages
    for (int i = 0; i < 2; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            while (1) {
                // Receive message from the queue
                if (msgrcv(msgid, &message, MAX_MSG_SIZE, MSG_TYPE, 0) == -1) {
                    perror("msgrcv");
                    exit(1);
                }

                // Print received message
                printf("Received: %s", message.msg_text);

                // Check if the received message is 'exit'
                if (strcmp(message.msg_text, "exit\n") == 0) {
                    break;
                }
            }
            exit(0);
        }
    }

    // Parent process
    while (1) {
        // Input message from user
        printf("You: ");
        fgets(message.msg_text, MAX_MSG_SIZE, stdin);

        // Send message to the queue
        message.msg_type = MSG_TYPE;
        if (msgsnd(msgid, &message, MAX_MSG_SIZE, 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        // Check if the sent message is 'exit'
        if (strcmp(message.msg_text, "exit\n") == 0) {
            break;
        }
    }

    // Wait for child processes to finish
    for (int i = 0; i < 2; ++i) {
        wait(NULL);
    }

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
