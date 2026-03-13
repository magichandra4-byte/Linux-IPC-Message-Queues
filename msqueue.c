#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

int main(int argc, char *argv[]) {
    key_t key;
    int msgid;

    if (argc != 2) {
        printf("Usage: %s writer|reader\n", argv[0]);
        return 1;
    }

    key = ftok("progfile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Message Queue ID: %d\n", msgid);

    if (strcmp(argv[1], "writer") == 0) {

        message.mesg_type = 1;

        printf("Enter Message: ");
        fgets(message.mesg_text, sizeof(message.mesg_text), stdin);

        message.mesg_text[strcspn(message.mesg_text, "\n")] = 0;

        msgsnd(msgid, &message, sizeof(message.mesg_text), 0);

        printf("Message sent: %s\n", message.mesg_text);
    }

    else if (strcmp(argv[1], "reader") == 0) {

        msgrcv(msgid, &message, sizeof(message.mesg_text), 1, 0);

        printf("Message received: %s\n", message.mesg_text);

        msgctl(msgid, IPC_RMID, NULL);
    }

    else {
        printf("Invalid argument. Use writer or reader.\n");
    }

    return 0;
}
