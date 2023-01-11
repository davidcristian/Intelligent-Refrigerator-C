#pragma once
#include "ProductRepository.h"

typedef struct
{
	ProductRepo* repo;

	ProductRepo** undoStack;
	int undoCapacity;
	int undoLength;

	ProductRepo** redoStack;
	int redoCapacity;
	int redoLength;
} Service;

Service* createService(ProductRepo* repo, int init);
void destroyService(Service* serv);

int addProductService(Service* serv, char* name, Category category, double quantity, Date expiration);
int deleteProductService(Service* serv, char* name, Category category);
int updateProductService(Service* serv, char* name, Category category, double quantity, Date expiration);

ProductRepo* getRepo(Service* serv);
ProductRepo* filterByString(Service* serv, char* name);
ProductRepo* filterByCategoryAndExpiration(Service* serv, Category category, int expiration);

void addToUndoStack(Service* serv);
void popUndoStack(Service* serv);
int undoOperation(Service* serv);
int redoOperation(Service* serv);
