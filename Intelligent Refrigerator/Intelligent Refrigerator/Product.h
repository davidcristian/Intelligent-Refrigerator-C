#pragma once

typedef enum { none, dairy, sweets, meat, fruit } Category;
#define CATEGORY_START dairy
#define CATEGORY_END fruit

static const char* const category_name[] =
{
	[none] = "none",
	[dairy] = "dairy",
	[sweets] = "sweets",
	[meat] = "meat",
	[fruit] = "fruit"
};

typedef struct
{
	int year;
	int month;
	int day;
} Date;
Date date(int year, int month, int day);

typedef struct
{
	char* name;
	Category category;
	double quantity;
	Date expiration;
} Product;

Product* createProduct(char* name, Category category, double quantity, Date expiration);
void destroyProduct(Product* p);

char* getName(Product* p);
Category getCategory(Product* p);
double getQuantity(Product* p);
Date getExpiration(Product* p);

void toString(Product* p, char str[]);
