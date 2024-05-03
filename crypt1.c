#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// Fonction de chiffrement simple (à remplacer par une méthode de chiffrement plus sécurisée)
void simple_encrypt(char *data, int length, char key) {
    for (int i = 0; i < length; i++) {
        data[i] = data[i] ^ key;
    }
}

// Fonction de déchiffrement simple (à remplacer par une méthode de déchiffrement plus sécurisée)
void simple_decrypt(char *data, int length, char key) {
    simple_encrypt(data, length, key);
}

// Écriture des données chiffrées dans un fichier
void write_encrypted_file(const char *filename, char *data, int length, char key) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    simple_encrypt(data, length, key);

    ssize_t bytes_written = write(fd, data, length);
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

// Lecture et déchiffrement des données d'un fichier
void read_and_decrypt_file(const char *filename, char *buffer, int buffer_size, char key) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(fd, buffer, buffer_size);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    close(fd);

    simple_decrypt(buffer, bytes_read, key);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s [e|d] filename key\n", argv[0]);
        return 1;
    }

    char *mode = argv[1];
    char *filename = argv[2];
    char key = argv[3][0];

    if (strcmp(mode, "e") == 0) {
        // Mode chiffrement
        char data[100];
        printf("Entrez les données à chiffrer : ");
        fgets(data, sizeof(data), stdin);
        int length = strlen(data);
        write_encrypted_file(filename, data, length, key);
        printf("Les données ont été chiffrées et enregistrées dans le fichier : %s\n", filename);
    } else if (strcmp(mode, "d") == 0) {
        // Mode déchiffrement
        char data[100];
        read_and_decrypt_file(filename, data, sizeof(data), key);
        printf("Données déchiffrées : %s\n", data);
    } else {
        printf("Mode inconnu. Utilisez 'e' pour chiffrer ou 'd' pour déchiffrer.\n");
    }

    return 0;
}

