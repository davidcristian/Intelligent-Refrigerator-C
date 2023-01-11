#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Product.h"

/// <summary>
/// Creates a new date
/// </summary>
/// <param name="year">The year</param>
/// <param name="month">The month</param>
/// <param name="day">The day</param>
/// <returns>A new date</returns>
Date date(int year, int month, int day)
{
	Date date = { year = year, month = month, day = day };
	return date;
}

/// <summary>
/// Creates a new product
/// </summary>
/// <param name="name">The name of the product</param>
/// <param name="category">The category of the product</param>
/// <param name="quantity">The quantity of the product</param>
/// <param name="expiration">The expiration date of the product</param>
/// <returns></returns>
Product* createProduct(char* name, Category category, double quantity, Date expiration)
{
	Product* p = malloc(sizeof(Product));
	if (p == NULL) return NULL;

	p->name = malloc(sizeof(char) * (strlen(name) + 1));
	if (p->name == NULL)
	{
		free(p);
		return NULL;
	}
	strcpy(p->name, name);

	p->expiration = expiration;
	p->category = category;
	p->quantity = quantity;

	return p;
}

/// <summary>
/// Destroys the product
/// </summary>
/// <param name="p">A pointer to the product</param>
void destroyProduct(Product* p)
{
	if (p == NULL) return;

	free(p->name);
	free(p);

	p = NULL;
}

/// <summary>
/// Gets the name of the product
/// </summary>
/// <param name="p">A pointer to the product</param>
/// <returns>the name of the product</returns>
char* getName(Product* p)
{
	if (p == NULL) return NULL;

	return p->name;
}

/// <summary>
/// Gets the category of the product
/// </summary>
/// <param name="p">A pointer to the product</param>
/// <returns>The category of the product</returns>
Category getCategory(Product* p)
{
	if (p == NULL) return -1;

	return p->category;
}

/// <summary>
/// Gets the quantity of the product
/// </summary>
/// <param name="p">A pointer to the product</param>
/// <returns>The quantity of the product</returns>
double getQuantity(Product* p)
{
	if (p == NULL) return -1;

	return p->quantity;
}

/// <summary>
/// Gets the expiration date
/// </summary>
/// <param name="p">A pointer to the product</param>
/// <returns>The expiration date of the product</returns>
Date getExpiration(Product* p)
{
	Date date = { 0 };
	if (p == NULL) return date;

	return p->expiration;
}

/// <summary>
/// The string representation of the product
/// </summary>
/// <param name="p">A pointer to the product</param>
/// <param name="str">The string to print to</param>
void toString(Product* p, char str[])
{
	if (p == NULL) return;
	sprintf(str, "Product %s is part of the \"%s\" category, there is %g of it in the fridge, and it expires on %d/%02d/%02d.", p->name, category_name[p->category], p->quantity, p->expiration.year, p->expiration.month, p->expiration.day);
}
