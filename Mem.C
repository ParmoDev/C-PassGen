#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <iostream>

int check_password_strength(char* password) {
    int strength = 0;
    int length = strlen(password);
    int num_uppercase = 0;
    int num_lowercase = 0;
    int num_numbers = 0;
    int num_special_chars = 0;

    // Check password length
    if (length >= 12) {
        strength++;
    }

    // Check for uppercase letters
    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) {
            num_uppercase++;
        }
    }
    if (num_uppercase > 0) {
        strength++;
    }

    // Check for lowercase letters
    for (int i = 0; i < length; i++) {
        if (islower(password[i])) {
            num_lowercase++;
        }
    }
    if (num_lowercase > 1) {
        strength++;
    }

    // Check for numbers
    for (int i = 0; i < length; i++) {
        if (isdigit(password[i])) {
            num_numbers++;
        }
    }
    if (num_numbers > 0) {
        strength++;
    }

    // Check for special characters
    for (int i = 0; i < length; i++) {
        if (!isalnum(password[i])) {
            num_special_chars++;
        }
    }
    if (num_special_chars > 0) {
        strength++;
    }

    return strength;
}

int generatePassword(int length) {
    char password[128];
    char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$^&*?";

    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        password[i] = characters[rand() % (sizeof(characters) - 1)];
    }

    char strength[20]; 
    switch(check_password_strength(password)) {
        case 0:
            strcpy(strength, "Very weak");
            break;
        case 1:
            strcpy(strength, "Weak");
            break;
        case 2:
            strcpy(strength, "Medium");
            break;
        case 3:
            strcpy(strength, "Strong");
            break;
        case 4:
            strcpy(strength, "Very strong");
            break;
        case 5:
            strcpy(strength, "Extremely strong");
            break;
        default:
            strcpy(strength, "Unknown");
            break;
    }

    printf("%s || Password strength: %d/5 > %s\n", password, check_password_strength(password), strength);

    char copy;
    printf("Do you want to copy the password to your clipboard? (y/n)\n> ");
    scanf("%s", &copy);

    if (copy == 'y') {
        // Copy password to clipboard
        OpenClipboard(0);
        EmptyClipboard();
        
        const size_t ln = strlen(password) + 1;
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, ln);
        memcpy(GlobalLock(hMem), password, ln);
        GlobalUnlock(hMem);

        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
    }

    char save;
    printf("Do you want to save the password to a file? (y/n)\n> ");
    scanf("%s", &save);

    if (save == 'y') {
        FILE *fptr; 
        fptr = fopen("passwords.txt", "a");

        if (fptr == NULL) {
            printf("Error!");
            exit(1);
        }

        char noteYN;
        char note[60];
        printf("Do you want to add a note to the password? (y/n)\n> ");
        scanf("%s", &noteYN);

        if (noteYN == 'y') {
            printf("Enter note: ");
            getchar();
            fgets(note, 60, stdin);
        }

        if (!note) {
            strncpy(note, "No note", 60);
        }

        fprintf(fptr, "\n%s - %s", password, note);
    }

    return 0;
}

int main() {
    int mode;
    printf("1. Generate password (random)\n2. Print saved passwords\n> ");
    scanf("%d", &mode);

    if (mode == 1) {
        int length;

        printf("How long do you want your password to be? (max: 128)\n> ");
        scanf("%d", &length);

        if (length > 128) {
            printf("Password cannot be longer than 128 characters. Please try again\n");
            main();
        }

        generatePassword(length);

        char newPass;
        printf("Do you want to generate another password? (y/n)\n> ");
        scanf("%s", &newPass);

        if (newPass == 'y') {
            main();
        }
    } else if (mode == 2) {
        FILE *fptr = fopen("passwords.txt", "r");
        
        if (fptr == NULL) {
            printf("fptr is null. (likely cause 'passwords.txt' does not exist or is empty)\n");
            getchar();
        }

        // Determine file size
        fseek(fptr, 0, SEEK_END);
        long size = ftell(fptr);
        rewind(fptr);

        // Allocate memory for file contents
        char *buffer = (char *)malloc(size + 1);

        if (buffer == NULL) {
            printf("Error allocating memory for file contents\n");
            getchar();
        }

        // Read file contents into buffer
        fgets(buffer, size, fptr);
        
        buffer[size] = '\0';

        while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
            printf("%s", buffer);
        }

        fclose(fptr);
        printf("\nFinished.\n");
        getchar();
    }

    getchar();

    return 0;
}