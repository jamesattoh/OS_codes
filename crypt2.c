#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

void handle_errors(void) {
    fprintf(stderr, "An error occurred.\n");
    exit(EXIT_FAILURE);
}

// Génère une clé AES-256 et un vecteur d'initialisation (IV) aléatoires
void generate_key_and_iv(unsigned char *key, unsigned char *iv) {
    RAND_bytes(key, EVP_CIPHER_key_length(EVP_aes_256_cbc()));
    RAND_bytes(iv, EVP_CIPHER_iv_length(EVP_aes_256_cbc()));
}

// Chiffre les données à l'aide d'AES-256-CBC
void aes_encrypt(unsigned char *data, int length, unsigned char *key, unsigned char *iv, unsigned char *encrypted) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        handle_errors();
    }

    int out_len, final_len;
    if (!EVP_EncryptUpdate(ctx, encrypted, &out_len, data, length)) {
        handle_errors();
    }

    if (!EVP_EncryptFinal_ex(ctx, encrypted + out_len, &final_len)) {
        handle_errors();
    }

    EVP_CIPHER_CTX_free(ctx);
}

// Déchiffre les données à l'aide d'AES-256-CBC
void aes_decrypt(unsigned char *encrypted, int length, unsigned char *key, unsigned char *iv, unsigned char *decrypted) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        handle_errors();
    }

    int out_len, final_len;
    if (!EVP_DecryptUpdate(ctx, decrypted, &out_len, encrypted, length)) {
        handle_errors();
    }

    if (!EVP_DecryptFinal_ex(ctx, decrypted + out_len, &final_len)) {
        handle_errors();
    }

    EVP_CIPHER_CTX_free(ctx);
}

// Écriture des données chiffrées dans un fichier
void write_encrypted_file(const char *filename, char *data, int length, unsigned char *key, unsigned char *iv) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    unsigned char encrypted[length];
    aes_encrypt((unsigned char *)data, length, key, iv, encrypted);

    ssize_t bytes_written = write(fd, iv, EVP_CIPHER_iv_length(EVP_aes_256_cbc()));
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    bytes_written = write(fd, encrypted, length);
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

// Lecture et déchiffrement des données d'un fichier
void read_and_decrypt_file(const char *filename, char *buffer, int buffer_size, unsigned char *key) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    unsigned char iv[EVP_CIPHER_iv_length(EVP_aes_256_cbc())];
    ssize_t bytes_read = read(fd, iv, EVP_CIPHER_iv_length(EVP_aes_256_cbc()));
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    unsigned char encrypted[buffer_size];
    bytes_read = read(fd, encrypted, buffer_size);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    close(fd);

    aes_decrypt(encrypted, bytes_read, key, iv, (unsigned char *)buffer);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [e|d] filename\n", argv[0]);
        return 1;
    }

    char *mode = argv[1];
    char *filename = argv[2];

    unsigned char key[EVP_CIPHER_key_length(EVP_aes_256_cbc())];
    unsigned char iv[EVP_CIPHER_iv_length(EVP_aes_256_cbc())];

    generate_key_and_iv(key, iv);

    if (strcmp(mode, "e") == 0) {
        // Mode chiffrement
        char data[100];
        printf("Entrez les données à chiffrer : ");
        fgets(data, sizeof(data), stdin);
        int length = strlen(data);
        write_encrypted_file(filename, data, length, key, iv);
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

