#include "cdataframe.h"
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_COLUMN_CAPACITY 50

// Function to create a new dataframe
DATAFRAME *create_dataframe() {
    DATAFRAME *df = (DATAFRAME *)malloc(sizeof(DATAFRAME));
    if (!df) {
        fprintf(stderr, "Failed to allocate memory for dataframe.\n");
        return NULL;
    }

    df->columns = (COLUMN **)malloc(INITIAL_COLUMN_CAPACITY * sizeof(COLUMN *));
    if (!df->columns) {
        free(df);
        fprintf(stderr, "Failed to allocate memory for columns array.\n");
        return NULL;
    }

    df->column_count = 0;
    df->max_columns = INITIAL_COLUMN_CAPACITY;
    return df;
}

// Function to add a column to the dataframe
void add_column_to_dataframe(DATAFRAME *df, COLUMN *col) {
    if (!df || !col) {
        fprintf(stderr, "Invalid dataframe or column.\n");
        return;
    }

    if (df->column_count == df->max_columns) {
        // Resize the array if the current capacity is reached
        unsigned int new_capacity = df->max_columns * 2;
        COLUMN **new_columns = (COLUMN **)realloc(df->columns, new_capacity * sizeof(COLUMN *));
        if (!new_columns) {
            fprintf(stderr, "Failed to resize columns array.\n");
            return;
        }
        df->columns = new_columns;
        df->max_columns = new_capacity;
    }

    df->columns[df->column_count] = col;
    df->column_count++;
}

// Function to remove a column from the dataframe
void remove_column_from_dataframe(DATAFRAME *df, unsigned int index) {
    if (!df || index >= df->column_count) {
        fprintf(stderr, "Invalid index or dataframe.\n");
        return;
    }

    delete_column(&df->columns[index]); // Free the column

    // Shift all columns to fill the gap
    for (unsigned int i = index; i < df->column_count - 1; i++) {
        df->columns[i] = df->columns[i + 1];
    }
    df->column_count--;
}

// Function to free the entire dataframe
void free_dataframe(DATAFRAME *df) {
    if (!df) return;

    // Free all columns
    for (unsigned int i = 0; i < df->column_count; i++) {
        delete_column(&df->columns[i]);
    }

    // Free the columns array and dataframe itself
    free(df->columns);
    free(df);
}

// Function to get a column by index
COLUMN *get_column(DATAFRAME *df, unsigned int index) {
    if (!df || index >= df->column_count) {
        fprintf(stderr, "Invalid index.\n");
        return NULL;
    }
    return df->columns[index];
}

// Function to print the dataframe
void print_dataframe(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is NULL\n");
        return;
    }

    printf("Dataframe with %u columns:\n", df->column_count);
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Column %d: %s\n", i, df->columns[i]->title);
        print_col(df->columns[i]);
    }
}

// Display the entire dataframe
void display_full_dataframe(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is NULL\n");
        return;
    }

    printf("Dataframe with %u columns:\n", df->column_count);
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Column %d: %s\n", i, df->columns[i]->title);
        print_col(df->columns[i]);
    }
}

// Display a specific number of dataframe rows
void display_dataframe_rows(DATAFRAME *df, unsigned int rows) {
    if (!df) {
        printf("Dataframe is NULL\n");
        return;
    }

    printf("Dataframe with %u columns, displaying up to %u rows:\n", df->column_count, rows);
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Column %d: %s\n", i, df->columns[i]->title);
        for (unsigned int j = 0; j < rows && j < df->columns[i]->size; j++) {
            char str[256];
            convert_value(df->columns[i], j, str, sizeof(str));
            printf("[%u] %s\n", j, str);
        }
    }
}

// Rename the title of a column in the dataframe
void rename_column_title(DATAFRAME *df, unsigned int column_index, const char *new_title) {
    if (df && column_index < df->column_count && new_title) {
        free(df->columns[column_index]->title);  // Free existing title
        df->columns[column_index]->title = strdup(new_title);  // Assign new title
    }
}

// Display column names
void display_column_names(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is NULL\n");
        return;
    }

    printf("Column names:\n");
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("%d. %s\n", i, df->columns[i]->title);
    }
}

// Add a row of values to the dataframe
void add_row_to_dataframe(DATAFRAME *df, void **row_data) {
    if (!df || !row_data) {
        fprintf(stderr, "Invalid dataframe or row data.\n");
        return;
    }

    for (unsigned int i = 0; i < df->column_count; i++) {
        if (i < df->column_count && row_data[i] != NULL) {
            insert_value(df->columns[i], row_data[i]);
        } else {
            // Handle missing data for the column if any
            insert_value(df->columns[i], NULL);
        }
    }
}

// Delete a row from the dataframe (assume same size for all columns)
void delete_row_from_dataframe(DATAFRAME *df, unsigned int row_index) {
    if (!df || row_index >= df->columns[0]->size) {
        fprintf(stderr, "Invalid dataframe or row index.\n");
        return;
    }

    for (unsigned int i = 0; i < df->column_count; i++) {
        COLUMN *col = df->columns[i];
        free(col->data[row_index]);  // Free the data at the specified row
        // Shift remaining data up
        for (unsigned int j = row_index; j < col->size - 1; j++) {
            col->data[j] = col->data[j + 1];
        }
        col->size--;
    }
}

// Function to check the existence of a value in the dataframe
int check_value_existence(DATAFRAME *df, void *value) {
    if (!df || !value) return 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        if (count_occurrences(df->columns[i], value) > 0) {
            return 1; // Found the value in one of the columns
        }
    }
    return 0;
}

// Access/replace the value in a dataframe cell
void *get_cell_value(DATAFRAME *df, unsigned int row, unsigned int column) {
    if (!df || column >= df->column_count || row >= df->columns[column]->size) {
        fprintf(stderr, "Invalid row or column index.\n");
        return NULL;
    }
    return df->columns[column]->data[row];
}

void set_cell_value(DATAFRAME *df, unsigned int row, unsigned int column, void *value) {
    if (!df || column >= df->column_count || row >= df->columns[column]->size) {
        fprintf(stderr, "Invalid row or column index.\n");
        return;
    }
    // Assuming value is of the correct type and already allocated
    free(df->columns[column]->data[row]);  // Free existing data
    df->columns[column]->data[row] = value;
}

// Display number of rows
void display_row_count(DATAFRAME *df) {
    if (!df || df->column_count == 0) {
        printf("No rows in the dataframe.\n");
        return;
    }
    printf("Number of rows: %u\n", df->columns[0]->size);
}

// Display number of columns
void display_column_count(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is NULL\n");
        return;
    }
    printf("Number of columns: %u\n", df->column_count);
}

// Count cells with specific conditions
int count_cells_equal_to(DATAFRAME *df, void *value) {
    int count = 0;
    if (!df) return 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        count += count_occurrences(df->columns[i], value);
    }
    return count;
}

int count_cells_greater_than(DATAFRAME *df, void *value) {
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        count += count_greater_than(df->columns[i], value);
    }
    return count;
}

int count_cells_less_than(DATAFRAME *df, void *value) {
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        count += count_less_than(df->columns[i], value);
    }
    return count;
}