#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "UI.h"

/// <summary>
/// Creates the user interface
/// </summary>
/// <param name="serv">The service</param>
/// <returns>A pointer to the user interface</returns>
UI* createUI(Service* serv)
{
	UI* ui = malloc(sizeof(UI));
	if (ui == NULL) return NULL;
	
	ui->serv = serv;
	return ui;
}

/// <summary>
/// Destroys the user interface
/// </summary>
/// <param name="ui">The user interface to destroy</param>
void destroyUI(UI* ui)
{
	if (ui == NULL) return;

	destroyService(ui->serv);
	free(ui);

	ui = NULL;
}

/// <summary>
/// Reads a valid integer from the user
/// </summary>
/// <param name="message">The message to display as a prompt</param>
/// <returns>A valid integer</returns>
int readInteger(const char* message)
{
	char input[16] = { 0 };
	int conversionResult = 0;

	int flag = 0;
	while (flag == 0)
	{
		printf(message);
		scanf("%15s", input);

		flag = sscanf(input, "%d", &conversionResult);
		if (flag == 0) printf("ERROR: Invalid input!\n");
	}

	return conversionResult;
}

/// <summary>
/// Reads a valid double from the user
/// </summary>
/// <param name="message">The message to display as a prompt</param>
/// <returns>A valid integer</returns>
double readDouble(const char* message)
{
	char input[32] = { 0 };
	double conversionResult;
	
	int flag = 0;
	while (flag == 0)
	{
		printf(message);
		scanf("%31s", input);

		flag = sscanf(input, "%lf", &conversionResult);
		if (flag == 0) printf("ERROR: Invalid input!\n");
	}

	return conversionResult;
}

/// <summary>
/// Reads a valid date from the user
/// </summary>
/// <param name="message">THe message to display as a prompt</param>
/// <returns>A valid date</returns>
Date readDate(const char* message)
{
	int monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int year, month, day;

	printf("%s\n", message);
	year = readInteger("Year: ");
	while (year < 2000 || year > 2100)
	{
		printf("ERROR: Invalid year!\n");
		year = readInteger("Year: ");
	}
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
		monthDays[1] = 29; // Leap year

	month = readInteger("Month: ");
	while (month < 1 || month > 12)
	{
		printf("ERROR: Invalid month!\n");
		month = readInteger("Month: ");
	}

	day = readInteger("Day: ");
	while (day < 1 || day > monthDays[month - 1])
	{
		printf("ERROR: Invalid day!\n");
		day = readInteger("Day: ");
	}

	Date date = { year = year, month = month, day = day };
	return date;
}

/// <summary>
/// Prints the user interface
/// </summary>
/// <param name="menu_options">The menu options</param>
/// <param name="menu_length">The amount of menu options</param>
void print_menu(char* menu_options[], int* menu_length)
{
	for (int i = 0; i < *menu_length; i++)
	{
		printf("%s\n", menu_options[i]);
	}
}

/// <summary>
/// Reads a product and adds it to the repository
/// </summary>
/// <param name="ui">A pointer to the user interface</param>
/// <returns>1 if the product was added successfully,
///			 0, otherwise</returns>
int addProductUI(UI* ui)
{
	char name[64];
	Category category;
	double quantity;
	Date expiration;

	printf("Name: ");
	scanf("%63s", name);
	
	printf("Available categories:\n");
	for (int i = CATEGORY_START; i <= CATEGORY_END; i++)
		printf("%d - %s\n", i, category_name[i]);

	category = readInteger("Category: ");
	while (category < CATEGORY_START || category > CATEGORY_END)
	{
		printf("ERROR: Invalid category!\n");
		category = readInteger("Category: ");
	}
	
	quantity = readDouble("Quantity: ");
	expiration = readDate("Expiration date:");

	return addProductService(ui->serv, name, category, quantity, expiration);
}

/// <summary>
/// Deletes a product from the repository
/// </summary>
/// <param name="ui">A pointer to the user interface</param>
/// <returns>1 if the product was deleted successfuly,
///			 0, otherwise</returns>
int deleteProductUI(UI* ui)
{
	char name[64];
	Category category;

	printf("Name: ");
	scanf("%63s", name);

	printf("Available categories:\n");
	for (int i = CATEGORY_START; i <= CATEGORY_END; i++)
		printf("%d - %s\n", i, category_name[i]);

	category = readInteger("Category: ");
	while (category < CATEGORY_START || category > CATEGORY_END)
	{
		printf("ERROR: Invalid category!\n");
		category = readInteger("Category: ");
	}

	return deleteProductService(ui->serv, name, category);
}

/// <summary>
/// Updates a product in the repository
/// </summary>
/// <param name="ui">A pointer to the user interface</param>
/// <returns>1 if the product was updated successfully,
///			 0, otherwise</returns>
int updateProductUI(UI* ui)
{
	char name[64];
	Category category;
	double quantity;
	Date expiration;

	printf("Name: ");
	scanf("%63s", name);

	printf("Available categories:\n");
	for (int i = CATEGORY_START; i <= CATEGORY_END; i++)
		printf("%d - %s\n", i, category_name[i]);

	category = readInteger("Category: ");
	while (category < CATEGORY_START || category > CATEGORY_END)
	{
		printf("ERROR: Invalid category!\n");
		category = readInteger("Category: ");
	}

	quantity = readDouble("Quantity: ");
	expiration = readDate("Expiration date:");

	return updateProductService(ui->serv, name, category, quantity, expiration);
}

/// <summary>
/// Prints the list of products with no filters
/// </summary>
/// <param name="ui">A pointer to the user interface</param>
void listProducts(UI* ui)
{
	ProductRepo* repo = getRepo(ui->serv);
	if (getLength(repo) == 0)
	{
		printf("INFO: There are no products in the fridge.\n");
	}
	else
	{
		for (int i = 0; i < getLength(repo); i++)
		{
			Product* product = getProductAt(repo, i);
			char productString[256];
			toString(product, productString);
			printf("%s\n", productString);
		}
	}
}

/// <summary>
/// Prints the list of products that contain a given string in their name
/// and sorts them in ascending order by quantity
/// </summary>
/// <param name="ui">A pointer to the user interface</param>
void listProductsQuantity(UI* ui)
{
	char input[64];
	int c;
	
	printf("Search: ");
	while ((c = getchar()) != '\n' && c != EOF);
	
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = 0;

	ProductRepo* repo = filterByString(ui->serv, input);
	if (getLength(repo) == 0)
	{
		printf("INFO: There are no product names that contain the given string.\n");
	}
	else
	{
		sortByQuantity(repo, 0);

		for (int i = 0; i < getLength(repo); i++)
		{
			Product* product = getProductAt(repo, i);
			char productString[256];
			toString(product, productString);
			printf("%s\n", productString);
		}
	}

	destroyRepo(repo);
}

/// <summary>
/// Prints the list of products in ascending order by their name
/// </summary>
/// <param name="ui">A pointer to the user interface</param>
void listProductsName(UI* ui)
{
	ProductRepo* repo = getRepo(ui->serv);
	if (getLength(repo) == 0)
	{
		printf("INFO: The repository is empty.\n");
	}
	else
	{
		sortByName(repo, 0);

		for (int i = 0; i < getLength(repo); i++)
		{
			Product* product = getProductAt(repo, i);
			char productString[256];
			toString(product, productString);
			printf("%s\n", productString);
		}
	}

	destroyRepo(repo);
}

/// <summary>
/// Prints the list of products in a given category that have expired
/// or expire in the upcoming X amount of days given by the user
/// </summary>
/// <param name="ui">A pointer to the user interface</param>
void listProductsExpiring(UI* ui)
{
	Category category;
	int expiration;

	printf("Categories:\n");
	for (int i = none; i <= CATEGORY_END; i++)
		printf("%d - %s\n", i, category_name[i]);

	category = readInteger("Category: ");
	while (category < none || category > CATEGORY_END)
	{
		printf("ERROR: Invalid category!\n");
		category = readInteger("Category: ");
	}

	expiration = readInteger("Expires within days: ");

	ProductRepo* repo = filterByCategoryAndExpiration(ui->serv, category, expiration);
	if (getLength(repo) == 0)
	{
		printf("INFO: There are no products from the given category that expire in %d days in the fridge.\n", expiration);
	}
	else
	{
		for (int i = 0; i < getLength(repo); i++)
		{
			Product* product = getProductAt(repo, i);
			char productString[256];
			toString(product, productString);
			printf("%s\n", productString);
		}
	}

	destroyRepo(repo);
}

/// <summary>
/// Starts the user interface and handles the options
/// chosen by the user
/// </summary>
/// <param name="ui">A pointer to the user interface</param>
void startUI(UI* ui)
{
	char* menu_options[] = {
		"0. Exit",
		"1. List all products",
		"2. Display products containing text (empty = all) sorted in ascending order by quantity",
		"3. Add a product",
		"4. Delete a product",
		"5. Update a product",
		"6. List products sorted in ascending order by name",
		"7. Display all products in given category (none = all) that have expired or expire in the given number of days",
		"8. Undo the previous operation",
		"9. Redo the previously undone operation"
	};
	int menu_length = sizeof(menu_options) / sizeof(menu_options[0]);
	int menu_selection = -1;

	printf("Welcome to the Admin Panel of the Intelligent Refrigerator by Home SmartApps.\n");
	while (menu_selection != 0)
	{
		print_menu(menu_options, &menu_length);
		menu_selection = readInteger("Option: ");

		switch (menu_selection)
		{
			case 0:
				printf("INFO: Quitting.\n");
				break;
			case 1:
				listProducts(ui);
				break;
			case 2:
				listProductsQuantity(ui);
				break;
			case 3:
				addToUndoStack(ui->serv);

				if (addProductUI(ui) == 1)
					printf("INFO: Product added successfully.\n");
				else
				{
					popUndoStack(ui->serv);
					printf("ERROR: Could not add product due to memory issues.\n");
				}
				break;
			case 4:
				addToUndoStack(ui->serv);

				if (deleteProductUI(ui) == 1)
					printf("INFO: Product deleted successfully.\n");
				else
				{
					popUndoStack(ui->serv);
					printf("ERROR: The product was not deleted because it does not exist!\n");
				}
				break;
			case 5:
				addToUndoStack(ui->serv);

				if (updateProductUI(ui) == 1)
					printf("INFO: Product updated successfully.\n");
				else
				{
					popUndoStack(ui->serv);
					printf("ERROR: The product was not updated because it does not exist!\n");
				}
				break;
			case 6:
				listProductsName(ui);
				break;
			case 7:
				listProductsExpiring(ui);
				break;
			case 8:
				if (undoOperation(ui->serv) == 1)
					printf("INFO: Successfully undone previous operation.\n");
				else
					printf("ERROR: Failed to undo previous operation.\n");
				break;
			case 9:
				if (redoOperation(ui->serv) == 1)
					printf("INFO: Successfully redone previous operation.\n");
				else
					printf("ERROR: Failed to redo previous operation.\n");
				break;
			default:
				printf("ERROR: Invalid menu option!\n");
		}

		printf("\n");
	}
}
