// Vigen`er cipher.cpp : Defines the entry point for the console application.
//Implementing the historical version of Vigen'ere cipher (both encoding and decoding).
//The program takes as input arguments the name of a ﬁle to be encrypted/decrypted and the secret word used for encryption.
//It outputs encrypted/decrypted text into a ﬁle.
//

#include <stdio.h>
#include <string.h>


#define DEV 0	// Constant used to define development mode (no need of entering anything, working on predefined data) 0 - dev off, 1 - dev on

#define SIZE 255	// Maximum size of any text buffer
#define OPERATIONCANCEL -1	// Output state of a function - operiation canceled
#define ERROR 0	// Output state of a function - error occurs
#define OK 1	// Output state of a function - all is right
#define EXIT 2	// Output state of a function - user enter "exit" string

/**
* Function prototypes.
* Descriptions are avaliable near definition of function
**/
int isCharLetter(char c);
int isLettersOnly(char* string);
int isUppercase(char c);
void toLowercase(char *string);
void clearBuffer(char *buffer);

int clearTmp();
int dumpTmp(char *buffer);
int saveTmpToFile(char *filename);
int saveTmpToFileGUI();

int encryptDecrypt(char actualChar, char *secretWord, char *buffer, int &i, char *mode);
int mainGUI();

/**
* int main() - main function
* Input:
*	None
* Output:
*	OK - all is right
**/
int main()
{
	int outputState;	// Output state of mainGUI() function

	if (!clearTmp()) {
		printf("You have no rights to write in program folder. Program cannot run proper.\nPress \"ENTER\" to exit...\n");
		getchar();
	} else {
		do {
			outputState = mainGUI();
			if (outputState == OPERATIONCANCEL) printf("\nOperation canceled. Now you can perform other operation or exit with \"exit\" command\n\n");
		} while(outputState != EXIT);
	}
	return OK;
}

/**
* int isCharLetter(char c) - function to check whether the character is a letter
* Input:
*	@param c - char to check
* Output:
*	ERROR - char is non letter
*	OK - char is letter
**/
int isCharLetter(char c) {
	// Check if the character is in the range of letters in ASCII
	if ((c < 65 || c > 90) && (c < 97 || c > 122)) return ERROR;
	else return OK;
}

/**
* int isLettersOnly(char* string) - function to check whether string is letter only
* Input:
*	@param string - pointer to first element of string to check
* Output:
*	ERROR - string contains at least one non-letter char
*	OK - string is letter only
**/
int isLettersOnly(char* string) {
	unsigned int i;	// counter
	for(i=0;i<strlen(string);i++) {
		// check chars char by char until first occurence of non-letter is found
		if (isCharLetter(string[i]) == ERROR) return ERROR;
	}

	// If we have come so far it means that string is letter only
	return OK;
}

/**
* int isUppercase(char c) - function to check whether char is uppercase
* Input:
*	@param c - char to check
* Output:
*	OK - char is uppercase
*	ERROR - char is lowercase
**/
int isUppercase(char c) {
	// Check if the character is in the range of letters in ASCII and is lower than lower bound of lowercase letters code
	if (isCharLetter(c) && c < 97) return OK;
	else return ERROR;
}

/**
* void toLowercase(char *string) - function converting string to lowercase
* Input:
*	@param string - pointer to first element of string to check
* Output:
*	None
**/
void toLowercase(char *string) {
	unsigned int i;	// Counter
	for(i=0;i<strlen(string);i++) {
		// check string char by char for uppercase and convert it to lowercase if necessary
		if (isUppercase(string[i])) string[i] += 32;
	}
}

/**
* int clearTmp() - function to clearing tmp file
* Input:
*	None
* Output:
*	OK - tmp file cleared
*	ERROR - user have no rights to write in program folder or any other error with file handling occured
**/
int clearTmp() {
	FILE *tmp;	// Pointer to temporary file
	// Open and close tmp file in write mode to clear content of file
	tmp = fopen("temp", "wb");

	if (tmp) {
		fclose(tmp);
		return OK;
	} else return ERROR;
}

/**
* int dumpTmp(char *buffer) - function dumping content of buffer to tmp file
* Input:
*	@param buffer - pointer to first element of buffer string
* Output:
*	OK - dump complete
*	ERROR - problem with file handling occurred
**/
int dumpTmp(char *buffer) {
	FILE* tmp;	// Pointer to temporary file
	tmp = fopen("temp", "ab");

	if (tmp) {
		fprintf(tmp, "%s", buffer);
		fclose(tmp);
		return OK;
	} else {
		return ERROR;
	}
}

/**
* void clearBuffer(char *buffer) - function clearing content of buffer
* Input:
*	@param buffer - pointer to first element of buffer string
* Output:
*	None
**/
void clearBuffer(char *buffer) {
	unsigned int i;	// Counter
	for(i=0;i<SIZE;i++) buffer[i] = '\0';	// Set content to '\0' char by char
}

/**
* int saveTmpToFile(char *filename) - function copying content of tmp file to user-defined path
* Input:
*	@param filename - pointer to firest element to file path string
* Output:
*	OK - file saved without any error
*	ERROR - error with file handling occured
**/
int saveTmpToFile(char *filename) {
	FILE* tmp;	// Pointer to temporary file
	FILE* dest;	// Pointer to output file
	char c;	// Actual char

	// Open proper files in proper mode
	tmp = fopen("temp", "rb");
	dest = fopen(filename, "wb");

	if (tmp == NULL || dest == NULL) return ERROR;	// If any error with file handling occured return ERROR

	// Copy content of tmp to user-defined file char by char
	while((c = getc(tmp)) != EOF) fputc(c, dest);

	// Close opened files
	fclose(tmp);
	fclose(dest);

	return OK;
}

/**
* int saveTmpToFileGUI() - function creating GUI for saveTmpToFile function
* Input:
*	None
* Output:
*	OPERATIONCANCEL - operation canceled by user
*	OK - all is right
**/
int saveTmpToFileGUI() {
	char buffer[SIZE];	// Internal buffer
	FILE *file;	// Pointer to file

	// Get filename from user until filesave will be complete or user enter "cancel"
	do {
		printf("Save as: ");
		if (!DEV) {
			scanf("%255[^\n]s", buffer);
			getchar();
		} else {
			printf("dest.txt");
			strcpy(buffer, "dest.txt");
		}

		if (stricmp(buffer, "cancel") == 0) {
			return OPERATIONCANCEL;	// If user entered "cancel" stop function
		}

		file = fopen(buffer, "wb");
		if (file == NULL) printf("Problem with creating file. Try again or enter \"cancel\" to cancel.\n\n");
	} while(file == NULL);

	fclose(file);	// Close file

	if (saveTmpToFile(buffer) == ERROR) {
		printf("Error occurs while saving file.\n");
		clearTmp();
		return ERROR;
	}

	printf("File saved.\n");
	clearTmp();
	return OK;
}

/**
* int encryptDecrypt(char actualChar, char *secretWord, char *buffer, int &i, char *mode) - function encrypting/decrypting char with respect to secret word
* Input:
*	@param actualChar - char to encrypt/decrypt
*	@param secretWord - pointer to first element of secret word string
*	@param buffer - pointer to first element of global buffer string
*	@param i - reference to global counter value
*	@param mode - pointer to first element of mode string describing mode (encrypting/decrypting)
* Output:
*	ERROR - any error occured
*	OK - all is right
**/
int encryptDecrypt(char actualChar, char *secretWord, char *buffer, int &i, char *mode) {
	int secretWordLen = strlen(secretWord);	// Length of secret word
	int check=0;	// Internal flag of case of letter
	int charAsInt;	// char converted to ASCII value
	int actualCharInAlphabet;	// Position in alphabet of secret word char

	// If length of buffer string is near global limit of string lenght dump that to tmp file
	if (strlen(buffer) >= SIZE-1) {
		if (!dumpTmp(buffer)) return ERROR;	// If error with dumping occured return error and stop encryption/decryption
		clearBuffer(buffer);	// Buffer is dumped so we clearing it
	}

	if (i >= secretWordLen) i = 0;	// If all secret word content is used we go back to the beginning

	if (isCharLetter(actualChar)) {	// We encoding/decoding only letters
		if (isUppercase(actualChar)) {
			// If letter is uppercase we converting them to lowercase and set flag to 1 (later we'll know that it was uppercase letter, work on only lowercase is simpler)
			actualChar += 32;
			check = 1;
		}

		charAsInt = actualChar;	// Converting char to ASCII value
		actualCharInAlphabet = secretWord[i]-97;	// Denoting proper char form secret word as him position in alphabet
		if (stricmp(mode, "encrypt") == 0) charAsInt += actualCharInAlphabet;	// If we encrypt file, we add position in alphabet of secret word char to current letter
		else charAsInt -= actualCharInAlphabet;	// If we decrypt file, we subtract position in alphabet of secret word char to current letter

		if (charAsInt > 122) charAsInt -= 26;	// If we exceed range of ASCII letters we subtract 26 to obltain proper letter
		else if (charAsInt < 97) charAsInt += 26;	// If we are below range of ASCII letters we add 26 to obltain proper letter

		if (check == 1) {	// If letter was uppercase we convert them back to uppercase
			charAsInt -= 32;
			check = 0;
		}

		actualChar = (char)charAsInt;	// Converting int value to char
		strcat(buffer, &actualChar);	// Add current char to buffer string
		i++;	// Increment value of internal counter
	} else {
		// If char is no letter simply add him to buffer without any operations
		strcat(buffer, &actualChar);
	}

	// If we have come so far it means that all is right
	return OK;
}

/**
* int mainGUI() - function creating GUI
* Input:
*	None
* Outpu:
*	OPERATIONCANCEL - operation canceled by user
*	EXIT - user enter "exit" command
*	ERROR - error occured
*	OK - all is right
**/
int mainGUI() {
	char buffer[SIZE];	// Internal buffer string
	char secretWord[SIZE];	// Secret word string
	char mode[10];	// Mode string (to choose encode/decode)
	char actualChar;	// Actual procesed char
	FILE *sourceFile = NULL;	// Pointer to source file
	int check;	// Internal flag (actual letter is uppercase (1) or lowercase(0))
	int i;	// Internal counter

	do {
		printf("Enter path to file to encrypt/decrypt: ");
		if (!DEV) {
			scanf("%255[^\n]s", buffer);
			getchar();
		} else {
			printf("test.txt");
			strcpy(buffer, "test.txt");
		}

		if (stricmp(buffer, "cancel") == 0) return OPERATIONCANCEL;
		if (stricmp(buffer, "exit") == 0) return EXIT;

		sourceFile = fopen(buffer, "rb");
		if (sourceFile == NULL) printf("File not found or other error occurs. Try again or enter \"cancel\" to cancel.\n\n");
	} while(sourceFile == NULL);

	do {
		printf("Enter \"secret word\": ");
		if (!DEV) {
			scanf("%255[^\n]s", secretWord);
			getchar();
		} else {
			printf("tajne");
			strcpy(secretWord, "tajne");
		}

		if (stricmp(secretWord, "cancel") == 0) return OPERATIONCANCEL;

		check = isLettersOnly(secretWord);
		if (check == ERROR) printf("Only letters allowed. Try again or enter \"cancel\" to cancel.\n\n");
	} while(check == ERROR); 

	toLowercase(secretWord);

	do {
		check = 1;
		printf("Select mode (encrypt/decrypt): ");
		scanf("%255[^\n]s", mode);
		getchar();

		if (stricmp(mode, "cancel") == 0) return OPERATIONCANCEL;

		if (stricmp(mode, "encrypt") != 0 && stricmp(mode, "decrypt") != 0) {
			printf("Only \"encrypt\" and \"decrypt\" allowed. Try again or enter \"cancel\" to cancel.\n\n");
			check = 0;
		}
	} while(check == 0); 

	i = 0;
	clearTmp();
	clearBuffer(buffer);

	while((actualChar = fgetc(sourceFile))!=EOF) {
		// Direct chars char by char to encryptDecrypt function
		if (!encryptDecrypt(actualChar, secretWord, buffer, i, mode)) {
			printf("Error occurs. Operation canceled\n");
			clearTmp();
			clearBuffer(buffer);
			return ERROR;
		}
	}
	dumpTmp(buffer);

	fclose(sourceFile);
	if (saveTmpToFileGUI() == OPERATIONCANCEL) return OPERATIONCANCEL;		

	printf("\nNow you can encrypt/decrypt next file. To exit just type in \"exit\" and tap ENTER\n");
	return OK;
}
