#pragma once
#include "Product.h"

#define REPOSITORY_INITIAL_SIZE 32
#define REPOSITORY_SIZE_SCALE 2

typedef struct
{
	Product** products;
	int capacity;
	int length;
} ProductRepo;

ProductRepo* createRepo();
void destroyRepo(ProductRepo* repo);

int addProductRepo(ProductRepo* repo, Product* p);
int removeProductRepo(ProductRepo* repo, char* name, Category category);
int updateProductRepo(ProductRepo* repo, char* name, Category category, double quantity, Date expiration);
Product* getProductAt(ProductRepo* repo, int index);

int getLength(ProductRepo* repo);
void sortByQuantity(ProductRepo* repo, int descending);
void sortByName(ProductRepo* repo, int descending);
