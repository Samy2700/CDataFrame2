#include "cdataframe.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Starting the DataFrame application...\n");

    // Create a new DataFrame
    DATAFRAME *df = create_dataframe();
    if (!df) {
        fprintf(stderr, "Failed to create DataFrame.\n");
        return EXIT_FAILURE;
    }

    // Define and add columns of various types
    COLUMN *intColumn = create_column(INT, "Integer Column");
    COLUMN *floatColumn = create_column(FLOAT, "Float Column");
    COLUMN *stringColumn = create_column(STRING, "String Column");

    // Check if columns are created successfully
    if (!intColumn || !floatColumn || !stringColumn) {
        fprintf(stderr, "Failed to create one or more columns.\n");
        free_dataframe(df);
        return EXIT_FAILURE;
    }

    // Add columns to the DataFrame
    add_column_to_dataframe(df, intColumn);
    add_column_to_dataframe(df, floatColumn);
    add_column_to_dataframe(df, stringColumn);

    // Inserting data into the DataFrame
    int intValue = 100;
    float floatValue = 1.23;
    char *stringValue = "example";

    for (int i = 0; i < 5; i++) { // Add 5 rows
        void *row[] = {
                &intValue,    // Integer data
                &floatValue,  // Float data
                stringValue   // String data
        };
        add_row_to_dataframe(df, row);
        intValue++;
        floatValue += 1.1f;
        // String remains constant; could vary if needed
    }

    // Display the full DataFrame contents
    printf("Complete DataFrame contents:\n");
    print_dataframe(df);

    // Cleanup: Free all resources associated with the DataFrame
    free_dataframe(df);

    return EXIT_SUCCESS;
}
