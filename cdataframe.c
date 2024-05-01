#include "cdataframe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create an empty dataframe with predefined maximum capacity
DATAFRAME *create_dataframe() {
    DATAFRAME *df = (DATAFRAME *)malloc(sizeof(DATAFRAME));
    if (!df) {
        fprintf(stderr, "Failed to allocate memory for the dataframe.\n");
        return NULL;
    }
    df->columns = (COLUMN **)malloc(sizeof(COLUMN *) * 10); // Default capacity
    if (!df->columns) {
        free(df);
        fprintf(stderr, "Failed to allocate memory for columns in the dataframe.\n");
        return NULL;
    }
    df->column_count = 0;
    df->max_columns = 10;
    return df;
}

// Add a column to the dataframe
void add_column_to_dataframe(DATAFRAME *df, COLUMN *col) {
    if (df->column_count >= df->max_columns) {
        // Reallocate memory to accommodate more columns
        df->max_columns += 10;
        df->columns = (COLUMN **)realloc(df->columns, sizeof(COLUMN *) * df->max_columns);
        if (!df->columns) {
            fprintf(stderr, "Failed to reallocate memory for columns.\n");
            return;
        }
    }
    df->columns[df->column_count++] = col;
}

// Remove a column from the dataframe by its index
void remove_column_from_dataframe(DATAFRAME *df, unsigned int index) {
    if (index >= df->column_count) {
        fprintf(stderr, "Index out of bounds for removing column.\n");
        return;
    }
    delete_column(&df->columns[index]); // Utilize the delete_column function from column.c
    for (unsigned int i = index; i < df->column_count - 1; i++) {
        df->columns[i] = df->columns[i + 1];
    }
    df->column_count--;
}

// Free all memory associated with the dataframe
void free_dataframe(DATAFRAME *df) {
    if (!df) return;
    for (unsigned int i = 0; i < df->column_count; i++) {
        delete_column(&df->columns[i]);
    }
    free(df->columns);
    free(df);
}

// Retrieve a column from the dataframe by index
COLUMN *get_column(DATAFRAME *df, unsigned int index) {
    if (index >= df->column_count) {
        fprintf(stderr, "Index out of bounds for getting column.\n");
        return NULL;
    }
    return df->columns[index];
}

// Print the entire dataframe
void print_dataframe(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is NULL\n");
        return;
    }
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Column %u: %s\n", i, df->columns[i]->title);
        print_col(df->columns[i]);
    }
}

// Initialize a dataframe with empty columns based on its capacity
void initialize_empty_dataframe(DATAFRAME *df) {
    if (!df) return;
    for (unsigned int i = 0; i < df->max_columns; i++) {
        df->columns[i] = create_column(INT, ""); // Initialize with default type INT
    }
    df->column_count = df->max_columns;
}

// Prompt user to fill the dataframe
void fill_dataframe_from_user(DATAFRAME *df) {
    if (!df) return;
    printf("Enter data for each column as prompted.\n");

    char buffer[256]; // Buffer to hold string inputs

    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Filling data for column: %s\n", df->columns[i]->title);
        switch (df->columns[i]->column_type) {
            case INT: {
                int value;
                printf("Enter an integer for column '%s': ", df->columns[i]->title);
                scanf("%d", &value);
                insert_value(df->columns[i], &value);
                break;
            }
            case FLOAT: {
                float value;
                printf("Enter a float for column '%s': ", df->columns[i]->title);
                scanf("%f", &value);
                insert_value(df->columns[i], &value);
                break;
            }
            case DOUBLE: {
                double value;
                printf("Enter a double for column '%s': ", df->columns[i]->title);
                scanf("%lf", &value);
                insert_value(df->columns[i], &value);
                break;
            }
            case CHAR: {
                char value;
                printf("Enter a char for column '%s': ", df->columns[i]->title);
                scanf(" %c", &value);
                insert_value(df->columns[i], &value);
                break;
            }
            case STRING: {
                printf("Enter a string for column '%s': ", df->columns[i]->title);
                scanf("%s", buffer);
                insert_value(df->columns[i], buffer);
                break;
            }
            case STRUCTURE: {
                // CustomStructure input handling
                CustomStructure cs;
                printf("Enter ID and value for column '%s': ", df->columns[i]->title);
                scanf("%d %lf", &cs.id, &cs.value);
                insert_value(df->columns[i], &cs);
                break;
            }
            default:
                printf("Unsupported type for user input.\n");
                break;
        }
    }
}

// Hard-fill the dataframe with provided data for testing purposes
void hard_fill_dataframe(DATAFRAME *df, void **data, unsigned int num_rows, unsigned int num_columns) {
    if (!df || !data) return;
    if (num_columns > df->column_count) {
        fprintf(stderr, "More columns provided than exist in the dataframe.\n");
        return;
    }
    for (unsigned int i = 0; i < num_columns; i++) {
        for (unsigned int j = 0; j < num_rows; j++) {
            insert_value(df->columns[i], ((void **)data[i])[j]);
        }
    }
}

// Display the entire contents of the dataframe
void display_full_dataframe(DATAFRAME *df) {
    if (!df || df->column_count == 0) {
        printf("Dataframe is empty or NULL.\n");
        return;
    }
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Column %u (%s):\n", i, df->columns[i]->title);
        print_col(df->columns[i]);
    }
}

// Display a specified number of rows from the dataframe
void display_dataframe_rows(DATAFRAME *df, unsigned int rows) {
    if (!df) {
        printf("Dataframe is NULL.\n");
        return;
    }
    printf("Displaying first %u rows of each column:\n", rows);
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Column %s:\n", df->columns[i]->title);
        for (unsigned int j = 0; j < rows && j < df->columns[i]->size; j++) {
            char value[256];
            convert_value(df->columns[i], j, value, sizeof(value));
            printf("%s\n", value);
        }
    }
}

// Display a specified number of columns from the dataframe
void display_dataframe_columns(DATAFRAME *df, unsigned int columns) {
    if (!df || columns > df->column_count) {
        printf("Dataframe is NULL or column count exceeded.\n");
        return;
    }
    printf("Displaying first %u columns:\n", columns);
    for (unsigned int i = 0; i < columns; i++) {
        printf("Column %s:\n", df->columns[i]->title);
        print_col(df->columns[i]);
    }
}

// Add a row of data to the dataframe
void add_row_to_dataframe(DATAFRAME *df, void **row_data) {
    if (!df || !row_data) {
        printf("Dataframe or row data is NULL.\n");
        return;
    }
    if (df->column_count == 0) {
        printf("Dataframe has no columns.\n");
        return;
    }
    for (unsigned int i = 0; i < df->column_count; i++) {
        insert_value(df->columns[i], row_data[i]);
    }
}

// Delete a row from the dataframe
void delete_row_from_dataframe(DATAFRAME *df, unsigned int row_index) {
    if (!df) {
        printf("Dataframe is NULL.\n");
        return;
    }
    for (unsigned int i = 0; i < df->column_count; i++) {
        if (row_index < df->columns[i]->size) {
            // Remove the row by shifting all rows up
            for (unsigned int j = row_index; j < df->columns[i]->size - 1; j++) {
                df->columns[i]->data[j] = df->columns[i]->data[j + 1];
            }
            df->columns[i]->size--;
        }
    }
}

// Rename a column in the dataframe
void rename_column_title(DATAFRAME *df, unsigned int column_index, const char *new_title) {
    if (!df || column_index >= df->column_count) {
        printf("Dataframe is NULL or index out of bounds.\n");
        return;
    }
    free(df->columns[column_index]->title);
    df->columns[column_index]->title = strdup(new_title);
}

// Check if a value exists anywhere in the dataframe
int check_value_existence(DATAFRAME *df, void *value) {
    if (!df) return 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        if (count_occurrences(df->columns[i], value) > 0) {
            return 1;
        }
    }
    return 0;
}

// Get a value from a specific cell in the dataframe
void *get_cell_value(DATAFRAME *df, unsigned int row, unsigned int column) {
    if (!df || column >= df->column_count || row >= df->columns[column]->size) {
        printf("Dataframe is NULL or index out of bounds.\n");
        return NULL;
    }
    return df->columns[column]->data[row];
}

// Set a value for a specific cell in the dataframe
void set_cell_value(DATAFRAME *df, unsigned int row, unsigned int column, void *value) {
    if (!df || column >= df->column_count || row >= df->columns[column]->size) {
        printf("Dataframe is NULL or index out of bounds.\n");
        return;
    }
    // Free the old value and set the new one
    free(df->columns[column]->data[row]);
    df->columns[column]->data[row] = value ? memcpy(malloc(sizeof(value)), value, sizeof(value)) : NULL;
}

// Display all column names in the dataframe
void display_column_names(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is NULL.\n");
        return;
    }
    printf("Column names:\n");
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("%s\n", df->columns[i]->title);
    }
}

#include "cdataframe.h"
#include <stdio.h>

// Display the total number of rows in the dataframe
void display_row_count(DATAFRAME *df) {
    if (!df || df->column_count == 0) {
        printf("Dataframe is empty or NULL.\n");
        return;
    }
    // Assuming all columns have the same number of rows
    printf("Total number of rows: %u\n", df->columns[0]->size);
}

// Display the total number of columns in the dataframe
void display_column_count(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is NULL.\n");
        return;
    }
    printf("Total number of columns: %u\n", df->column_count);
}

// Count cells in the dataframe equal to a specified value
int count_cells_equal_to(DATAFRAME *df, void *value) {
    if (!df || df->column_count == 0) {
        return 0; // No columns to search
    }
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        count += count_occurrences(df->columns[i], value);
    }
    return count;
}

// Count cells in the dataframe with values greater than the specified value
int count_cells_greater_than(DATAFRAME *df, void *value) {
    if (!df || df->column_count == 0) {
        return 0;
    }
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        for (unsigned int j = 0; j < df->columns[i]->size; j++) {
            if (compare_values(df->columns[i]->column_type, df->columns[i]->data[j], value) > 0) {
                count++;
            }
        }
    }
    return count;
}

// Count cells in the dataframe with values less than the specified value
int count_cells_less_than(DATAFRAME *df, void *value) {
    if (!df || df->column_count == 0) {
        return 0;
    }
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        for (unsigned int j = 0; j < df->columns[i]->size; j++) {
            if (compare_values(df->columns[i]->column_type, df->columns[i]->data[j], value) < 0) {
                count++;
            }
        }
    }
    return count;
}
