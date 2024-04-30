#include "cdataframe.h"
#include <stdio.h>

// Helper function to get character input from the user
char get_char_input(const char *prompt) {
    char input;
    printf("%s", prompt);
    scanf(" %c", &input);  // Read a single character input
    return input;
}

// Helper function to get integer input from the user
int get_int_input(const char *prompt) {
    int input;
    printf("%s", prompt);
    scanf("%d", &input);  // Read an integer input
    return input;
}

int main() {
    // Create a new dataframe
    DATAFRAME *df = create_dataframe();
    if (df == NULL) {
        fprintf(stderr, "Failed to create dataframe.\n");
        return EXIT_FAILURE;
    }

    // Create a column for characters
    COLUMN *mycol = create_column(CHAR, "Character Column");
    if (mycol == NULL) {
        fprintf(stderr, "Failed to create column.\n");
        free_dataframe(df);
        return EXIT_FAILURE;
    }

    // Add the column to the dataframe
    add_column_to_dataframe(df, mycol);

    // Input characters into the column
    char input_char = get_char_input("Enter a character: ");
    if (!insert_value(mycol, &input_char)) {
        fprintf(stderr, "Failed to insert value into the column.\n");
        free_dataframe(df);
        return EXIT_FAILURE;
    }

    input_char = get_char_input("Enter another character: ");
    if (!insert_value(mycol, &input_char)) {
        fprintf(stderr, "Failed to insert value into the column.\n");
        free_dataframe(df);
        return EXIT_FAILURE;
    }

    // Display the full dataframe
    printf("Contents of the dataframe:\n");
    display_full_dataframe(df);

    // Create another column for integers
    COLUMN *intCol = create_column(INT, "Integer Column");
    if (intCol == NULL) {
        fprintf(stderr, "Failed to create integer column.\n");
        free_dataframe(df);
        return EXIT_FAILURE;
    }

    // Add the integer column to the dataframe
    add_column_to_dataframe(df, intCol);

    // Input integers into the new column
    int input_int = get_int_input("Enter an integer: ");
    if (!insert_value(intCol, &input_int)) {
        fprintf(stderr, "Failed to insert integer into the column.\n");
        free_dataframe(df);
        return EXIT_FAILURE;
    }

    input_int = get_int_input("Enter another integer: ");
    if (!insert_value(intCol, &input_int)) {
        fprintf(stderr, "Failed to insert integer into the column.\n");
        free_dataframe(df);
        return EXIT_FAILURE;
    }

    // Display updated dataframe
    printf("Updated contents of the dataframe:\n");
    display_full_dataframe(df);

    // Clean up and free the allocated dataframe and its columns
    free_dataframe(df);

    return EXIT_SUCCESS;
}
