#include "cdataframe.h"
#include <stdio.h>
#include <stdlib.h> // For dynamic allocation and system clears

int main() {
    DATAFRAME *df = create_dataframe();
    if (!df) {
        fprintf(stderr, "Failed to create a dataframe.\n");
        return 1;
    }

    int choice;
    char data_type;
    char proceed;
    printf("Do you want to fill the dataframe yourself (1) or use hardcoded data (2)? Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Please fill the dataframe:\n");
        fill_dataframe_from_user(df);
    } else {
        // Example predefined data for demonstration purposes
        int data1[] = {1, 2, 3, 4, 5};
        float data2[] = {1.1, 2.2, 3.3, 4.4, 5.5};
        void *predefined_data[] = {data1, data2};
        printf("Hard filling the dataframe with predefined data:\n");
        hard_fill_dataframe(df, predefined_data, 5, 2);
    }

    int running = 1;
    int row, col, value;
    char new_name[100];
    while (running) {
        printf("\nPress enter to continue to the menu...");
        scanf("%c", &proceed);  // Wait for user to press enter
        scanf("%c", &proceed);  // Handle the newline character left in the input buffer


        printf("\nMenu:\n");
        printf("\nMenu:\n");
        printf("1. Display the full dataframe\n");
        printf("2. Display a specific number of rows from the dataframe\n");
        printf("3. Display a specific number of columns from the dataframe\n");
        printf("4. Get and display a specific cell value\n");
        printf("5. Set a specific cell value\n");
        printf("6. Add a row of values\n");
        printf("7. Delete a row of values\n");
        printf("8. Add a column\n");
        printf("9. Delete a column\n");
        printf("10. Rename a column\n");
        printf("11. Check for a value\n");
        printf("12. Replace a cell's value\n");
        printf("13. Display column names\n");
        printf("14. Display number of rows\n");
        printf("15. Display number of columns\n");
        printf("16. Display number of cells equal to a value\n");
        printf("17. Display number of cells greater than a value\n");
        printf("18. Display number of cells less than a value\n");
        printf("19. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_full_dataframe(df);
                break;
            case 2:
                printf("Enter number of rows to display: ");
                scanf("%d", &row);
                display_dataframe_rows(df, row);
                break;
            case 3:
                printf("Enter number of columns to display: ");
                scanf("%d", &col);
                display_dataframe_columns(df, col);
                break;
            case 4:
                printf("Enter row and column index to get value: ");
                scanf("%d %d", &row, &col);
                printf("Value at [%d, %d]: %p\n", row, col, get_cell_value(df, row, col));
                break;
            case 5:
                printf("Enter row and column index to set value, followed by the integer value to set: ");
                int val;
                scanf("%d %d %d", &row, &col, &val);
                set_cell_value(df, row, col, &val);
                break;
            case 6:
                printf("Enter data for a new row (integers only for simplicity):\n");
                // Assuming rows are compatible with all columns
                int *new_row = malloc(df->column_count * sizeof(int));
                for (int i = 0; i < df->column_count; i++) {
                    printf("Enter value for column %d: ", i + 1);
                    scanf("%d", &new_row[i]);
                }
                add_row_to_dataframe(df, new_row);
                free(new_row);
                break;
            case 7:
                printf("Enter row index to delete: ");
                scanf("%d", &row);
                delete_row_from_dataframe(df, row);
                break;
            case 8:
                printf("Enter the data type for the new column (i for int, f for float, c for char, s for string): ");
                scanf(" %c", &data_type);
                COLUMN *new_column = NULL;
                switch (data_type) {
                    case 'i':
                        new_column = create_column(INT, "Integer Column");
                        break;
                    case 'f':
                        new_column = create_column(FLOAT, "Float Column");
                        break;
                    case 'c':
                        new_column = create_column(CHAR, "Char Column");
                        break;
                    case 's':
                        new_column = create_column(STRING, "String Column");
                        break;
                    default:
                        printf("Invalid type specified.\n");
                        continue;
                }
                add_column_to_dataframe(df, new_column);
                printf("New column added.\n");
                break;
            case 9:
                printf("Enter column index to delete: ");
                scanf("%d", &col);
                remove_column_from_dataframe(df, col);
                break;
            case 10:
                printf("Enter column index to rename: ");
                scanf("%d", &col);
                printf("Enter new name: ");
                scanf("%s", new_name);
                rename_column_title(df, col, new_name);
                break;
            case 11:
                printf("Enter value to check for: ");
                scanf("%d", &value); // Adjust for other data types as needed
                printf("Existence: %s\n", check_value_existence(df, &value) ? "Yes" : "No");
                break;
            case 12:
                // Assume access/replace functionality is properly implemented
                printf("Replacing a cell's value - not implemented in this example.\n");
                break;
            case 13:
                display_column_names(df);
                break;
            case 14:
                display_row_count(df);
                break;
            case 15:
                display_column_count(df);
                break;
            case 16:
                printf("Enter value to count cells equal to: ");
                scanf("%d", &value); // Adjust for other data types
                printf("Cells equal to %d: %d\n", value, count_cells_equal_to(df, &value));
                break;
            case 17:
                printf("Enter value to count cells greater than: ");
                scanf("%d", &value); // Adjust for other data types
                printf("Cells greater than %d: %d\n", value, count_cells_greater_than(df, &value));
                break;
            case 18:
                printf("Enter value to count cells less than: ");
                scanf("%d", &value); // Adjust for other data types
                printf("Cells less than %d: %d\n", value, count_cells_less_than(df, &value));
                break;
            case 19:
                running = 0; // Exit the loop
                break;
            default:
                printf("Invalid option, please try again.\n");
                break;
        }
    }

    // Free the dataframe resources
    free_dataframe(df);
    return 0;
}
