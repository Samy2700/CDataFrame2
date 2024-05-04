#include "cdataframe.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

// Creates an empty dataframe
DATAFRAME *create_dataframe() {
    DATAFRAME *df = (DATAFRAME *)malloc(sizeof(DATAFRAME));
    if (!df) {
        fprintf(stderr, "Memory allocation failed for the dataframe.\n");
        return NULL;
    }
    df->columns = NULL;
    df->column_count = 0;
    df->max_columns = 0;
    return df;
}

// Fills the dataframe from user input
void fill_dataframe_from_user(DATAFRAME *df) {
    if (!df) {
        fprintf(stderr, "Dataframe is NULL.\n");
        return;
    }

    printf("Enter the number of columns: ");
    int num_columns;
    scanf("%d", &num_columns);
    df->columns = (COLUMN **)malloc(num_columns * sizeof(COLUMN *));
    df->max_columns = num_columns;
    df->column_count = 0;

    for (int i = 0; i < num_columns; i++) {
        char type[20], name[100];
        printf("Enter type and name of column %d (e.g., INT Age): ", i + 1);
        scanf("%s %s", type, name);
        ENUM_TYPE col_type = parse_type(type);
        COLUMN *new_column = create_column(col_type, name);
        if (!new_column) {
            continue;
        }
        add_column_to_dataframe(df, new_column);

        printf("Enter the number of rows for column '%s': ", name);
        int num_rows;
        scanf("%d", &num_rows);
        for (int j = 0; j < num_rows; j++) {
            printf("Enter data for row %d: ", j + 1);
            void *data = read_data_based_on_type(col_type);
            insert_value(new_column, data);
        }
    }
}

// Converts a string representation of a type into an ENUM_TYPE
ENUM_TYPE parse_type(const char *typeStr) {
    if (strcmp(typeStr, "INT") == 0) return INT;
    else if (strcmp(typeStr, "FLOAT") == 0) return FLOAT;
    else if (strcmp(typeStr, "DOUBLE") == 0) return DOUBLE;
    else if (strcmp(typeStr, "CHAR") == 0) return CHAR;
    else if (strcmp(typeStr, "STRING") == 0) return STRING;
    else if (strcmp(typeStr, "STRUCTURE") == 0) return STRUCTURE;
    return -1; // Invalid type
}


// Example of a hard_fill_dataframe function without dynamic type determination
void hard_fill_dataframe(DATAFRAME *df, void **data, unsigned int num_rows, unsigned int num_columns) {
    if (!df || !data) {
        fprintf(stderr, "Invalid arguments for hard filling the dataframe.\n");
        return;
    }

    for (unsigned int i = 0; i < num_columns; i++) {
        ENUM_TYPE fixed_type = INT; // Example fixed type, replace with actual logic
        COLUMN *col = create_column(fixed_type, "Predefined Column");
        add_column_to_dataframe(df, col);
        for (unsigned int j = 0; j < num_rows; j++) {
            insert_value(col, ((void **)data[i])[j]); // Assuming data is appropriately typed
        }
    }
}


// Reads data from standard input based on the specified type and returns a pointer to the data
void *read_data_based_on_type(ENUM_TYPE type) {
    void *data = NULL;
    switch (type) {
        case INT: {
            int *value = malloc(sizeof(int));
            printf("Enter an integer: ");
            scanf("%d", value);
            data = value;
            break;
        }
        case FLOAT: {
            float *value = malloc(sizeof(float));
            printf("Enter a float: ");
            scanf("%f", value);
            data = value;
            break;
        }
        case DOUBLE: {
            double *value = malloc(sizeof(double));
            printf("Enter a double: ");
            scanf("%lf", value);
            data = value;
            break;
        }
        case CHAR: {
            char *value = malloc(sizeof(char));
            printf("Enter a char: ");
            scanf(" %c", value);
            data = value;
            break;
        }
        case STRING: {
            char *value = malloc(256); // Define a reasonable max length for strings
            printf("Enter a string: ");
            scanf("%255s", value); // Avoid buffer overflow
            data = value;
            break;
        }
        case STRUCTURE: {
            // Assuming CustomStructure has been defined with known fields
            CustomStructure *value = malloc(sizeof(CustomStructure));
            printf("Enter structure values (id, value): ");
            scanf("%d %lf", &value->id, &value->value);
            data = value;
            break;
        }
        default:
            printf("Unsupported data type.\n");
            break;
    }
    return data;
}

// Frees resources associated with the dataframe
void free_dataframe(DATAFRAME *df) {
    if (df == NULL) return;

    // Loop through each column and delete it using delete_column
    for (unsigned int i = 0; i < df->column_count; i++) {
        if (df->columns[i] != NULL) {
            delete_column(df->columns[i]);  // Ensure this function frees all resources within the column
        }
    }

    // Free the array of column pointers
    free(df->columns);

    // Finally free the dataframe structure itself
    free(df);
}

// Adds a column to the dataframe
int add_column_to_dataframe(DATAFRAME *df, COLUMN *col) {
    if (df == NULL || col == NULL) return -1;
    if (df->column_count == df->max_columns) {
        size_t new_size = df->max_columns == 0 ? 1 : df->max_columns * 2;
        COLUMN **new_cols = realloc(df->columns, new_size * sizeof(COLUMN *));
        if (!new_cols) return -1;
        df->columns = new_cols;
        df->max_columns = new_size;
    }
    df->columns[df->column_count++] = col;
    return 0;
}

// Displays the entire dataframe
void display_full_dataframe(DATAFRAME *df) {
    if (!df || !df->columns) {
        printf("The dataframe is empty or uninitialized.\n");
        return;
    }
    printf("Dataframe contains %u columns:\n", df->column_count);
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Column %d: %s\n", i + 1, df->columns[i]->title);
        print_col(df->columns[i]);  // Assuming print_col() is a function to print column data
    }
}

void display_dataframe_rows(DATAFRAME *df, unsigned int rows) {
    if (!df || !df->columns) {
        printf("The dataframe is empty or uninitialized.\n");
        return;
    }
    printf("Displaying up to %u rows from each of the %u columns:\n", rows, df->column_count);
    for (unsigned int i = 0; i < df->column_count; i++) {
        COLUMN *col = df->columns[i];
        printf("Column %d (%s):\n", i + 1, col->title);

        // Print each row in the column up to the specified limit
        for (unsigned int j = 0; j < rows && j < col->size; j++) {
            switch (col->column_type) {
                case INT:
                    printf("%d: %d\n", j + 1, *((int*)col->data[j]));
                    break;
                case FLOAT:
                    printf("%d: %f\n", j + 1, *((float*)col->data[j]));
                    break;
                case DOUBLE:
                    printf("%d: %lf\n", j + 1, *((double*)col->data[j]));
                    break;
                case STRING:
                    printf("%d: %s\n", j + 1, (char*)col->data[j]);
                    break;
                case STRUCTURE:
                    CustomStructure *cs = (CustomStructure*)col->data[j];
                    printf("%d: ID = %d, Value = %.2f\n", j + 1, cs->id, cs->value);
                    break;
                default:
                    printf("%d: Unhandled data type.\n", j + 1);
                    break;
            }
        }
    }
}


void add_row_to_dataframe(DATAFRAME *df, void **row_data) {
    if (!df || !df->columns) {
        printf("Dataframe is not properly initialized.\n");
        return;
    }
    for (unsigned int i = 0; i < df->column_count; i++) {
        if (insert_value(df->columns[i], row_data[i]) != 0) {
            printf("Failed to insert data in column %d.\n", i + 1);
        }
    }
}

void delete_row_from_dataframe(DATAFRAME *df, unsigned int row_index) {
    if (!df) {
        printf("Dataframe is not initialized.\n");
        return;
    }

    // Check each column to ensure the row index is valid before deletion
    for (unsigned int i = 0; i < df->column_count; i++) {
        if (row_index >= df->columns[i]->size) {
            printf("Row index %u is out of bounds for column %u.\n", row_index, i + 1);
            return;  // If any column doesn't have enough rows, abort the operation
        }
    }

    // Decrease the size of each column by one, effectively hiding the last row
    // This is a logical deletion and does not free any memory
    for (unsigned int i = 0; i < df->column_count; i++) {
        df->columns[i]->size--;
    }
}


void display_dataframe_columns(DATAFRAME *df, unsigned int columns) {
    if (!df || !df->columns) {
        printf("Dataframe is empty or uninitialized.\n");
        return;
    }
    printf("Displaying the first %u columns out of %u total columns:\n", columns, df->column_count);
    for (unsigned int i = 0; i < columns && i < df->column_count; i++) {
        printf("Column %u (%s):\n", i + 1, df->columns[i]->title);
        print_col(df->columns[i]);  // Assumes a function to print column data
    }
}


void remove_column_from_dataframe(DATAFRAME *df, unsigned int index) {
    if (!df || index >= df->column_count) {
        printf("Invalid column index or empty dataframe.\n");
        return;
    }
    // Free the column resources
    delete_column(df->columns[index]);  // Assumes free_column correctly frees all column data

    // Shift all columns to the left to fill the gap
    for (unsigned int i = index; i < df->column_count - 1; i++) {
        df->columns[i] = df->columns[i + 1];
    }
    df->column_count--;

    // Reduce memory usage if necessary
    if (df->column_count > 0 && df->column_count == df->max_columns / 4) {
        COLUMN **new_columns = realloc(df->columns, (df->max_columns / 2) * sizeof(COLUMN *));
        if (new_columns) {
            df->columns = new_columns;
            df->max_columns /= 2;
        }
    }
}



void rename_column_title(DATAFRAME *df, unsigned int column_index, const char *new_title) {
    if (!df || column_index >= df->column_count || !new_title) {
        printf("Invalid column index or new title.\n");
        return;
    }
    char *new_title_copy = strdup(new_title);
    if (!new_title_copy) {
        printf("Failed to allocate memory for new title.\n");
        return;
    }
    free(df->columns[column_index]->title);
    df->columns[column_index]->title = new_title_copy;
}


int check_value_existence(DATAFRAME *df, void *value) {
    if (!df || !value) {
        printf("Invalid dataframe or value.\n");
        return 0;
    }
    for (unsigned int i = 0; i < df->column_count; i++) {
        for (unsigned int j = 0; j < df->columns[i]->size; j++) {
            if (compare_values(df->columns[i]->column_type, df->columns[i]->data[j], value) == 0) {
                return 1;  // Value found
            }
        }
    }
    return 0;  // Value not found
}


void *get_cell_value(DATAFRAME *df, unsigned int row, unsigned int column) {
    if (!df || column >= df->column_count || row >= df->columns[column]->size) {
        printf("Invalid row or column index.\n");
        return NULL;
    }
    return df->columns[column]->data[row];
}

void set_cell_value(DATAFRAME *df, unsigned int row, unsigned int column, void *value) {
    if (!df || column >= df->column_count || row >= df->columns[column]->size || !value) {
        printf("Invalid row or column index, or null value provided.\n");
        return;
    }

    // Free the current memory if necessary
    if (df->columns[column]->column_type == STRING) {
        free(df->columns[column]->data[row]);  // Only free if it's a string or similarly dynamically allocated type
    }

    // Assign the new value based on the type
    switch (df->columns[column]->column_type) {
        case INT:
            // Assuming value is a pointer to the integer to be assigned
            *(int *)(df->columns[column]->data[row]) = *(int *)value;
            break;
        case FLOAT:
            *(float *)(df->columns[column]->data[row]) = *(float *)value;
            break;
        case DOUBLE:
            *(double *)(df->columns[column]->data[row]) = *(double *)value;
            break;
        case STRING:
            // Duplicate the string to handle it safely
            df->columns[column]->data[row] = strdup((char *)value);
            break;
        case STRUCTURE:
            // If value is assumed to be a pointer to CustomStructure, allocate new memory and copy the data
            df->columns[column]->data[row] = malloc(sizeof(CustomStructure));
            if (df->columns[column]->data[row]) {
                memcpy(df->columns[column]->data[row], value, sizeof(CustomStructure));
            } else {
                printf("Memory allocation failed for structure.\n");
            }
        default:
            printf("Unhandled data type.\n");
            break;
    }
}


void display_column_names(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is uninitialized.\n");
        return;
    }
    printf("Column names:\n");
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Column %u: %s\n", i + 1, df->columns[i]->title);
    }
}


void display_row_count(DATAFRAME *df) {
    if (!df || df->column_count == 0 || !df->columns[0]) {
        printf("Dataframe is empty or not properly initialized.\n");
    } else {
        printf("Number of rows: %u\n", df->columns[0]->size);
    }
}

void display_column_count(DATAFRAME *df) {
    if (!df) {
        printf("Dataframe is empty or not properly initialized.\n");
    } else {
        printf("Number of columns: %u\n", df->column_count);
    }
}

int count_cells_equal_to(DATAFRAME *df, void *value) {
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        for (unsigned int j = 0; j < df->columns[i]->size; j++) {
            if (compare_values(df->columns[i]->column_type, df->columns[i]->data[j], value) == 0) {
                count++;
            }
        }
    }
    return count;
}

int count_cells_greater_than(DATAFRAME *df, void *value) {
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

int count_cells_less_than(DATAFRAME *df, void *value) {
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
