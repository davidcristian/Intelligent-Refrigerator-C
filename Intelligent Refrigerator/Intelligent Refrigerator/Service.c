#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "Service.h"

/// <summary>
/// Creates the service
/// </summary>
/// <param name="repo">The product repository</param>
/// <param name="init">Initializes the repository with 10 values if 1</param>
/// <returns>A pointer to the service</returns>
Service* createService(ProductRepo* repo, int init)
{
	Service* serv = malloc(sizeof(Service));
	if (serv == NULL) return NULL;

	serv->undoStack = malloc(REPOSITORY_INITIAL_SIZE * sizeof(ProductRepo*));
	if (serv->undoStack == NULL)
	{
		free(serv);
		return NULL;
	}

	serv->redoStack = malloc(REPOSITORY_INITIAL_SIZE * sizeof(ProductRepo*));
	if (serv->redoStack == NULL)
	{
		free(serv->undoStack);
		free(serv);
		return NULL;
	}
	serv->undoCapacity = REPOSITORY_INITIAL_SIZE;
	serv->redoCapacity = REPOSITORY_INITIAL_SIZE;
	serv->undoLength = 0;
	serv->redoLength = 0;

	serv->repo = repo;
	if (init == 1)
	{
		addProductService(serv, "milk", dairy, 1, date(2022, 3, 15));
		addProductService(serv, "yogurt", dairy, 3.25, date(2022, 3, 14));
		addProductService(serv, "chicken", meat, 2.5, date(2022, 3, 26));
		addProductService(serv, "chocolate", sweets, 2, date(2022, 8, 22));
		addProductService(serv, "eggs", dairy, 6, date(2022, 3, 28));
		
		addProductService(serv, "beef", meat, 1.33, date(2022, 3, 17));
		addProductService(serv, "apples", fruit, 4, date(2022, 4, 12));
		addProductService(serv, "pears", fruit, 2.5, date(2022, 4, 15));
		addProductService(serv, "oranges", fruit, 6, date(2022, 4, 8));
		addProductService(serv, "sour_candy", sweets, 4, date(2023, 4, 13));
	}

	return serv;
}

/// <summary>
/// Destroys the service
/// </summary>
/// <param name="serv">The service to destroy</param>
void destroyService(Service* serv)
{
	if (serv == NULL) return;

	destroyRepo(serv->repo);

	for (int i = 0; i < serv->undoLength; i++)
		destroyRepo(serv->undoStack[i]);
	free(serv->undoStack);

	for (int i = 0; i < serv->redoLength; i++)
		destroyRepo(serv->redoStack[i]);
	free(serv->redoStack);

	free(serv);
	serv = NULL;
}

/// <summary>
/// Adds a product to the repository
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <param name="name">The name of the product</param>
/// <param name="category">The category of the product</param>
/// <param name="quantity">The quantity of the product</param>
/// <param name="expiration">The expiration date of the product</param>
/// <returns></returns>
int addProductService(Service* serv, char* name, Category category, double quantity, Date expiration)
{
	Product* p = createProduct(name, category, quantity, expiration);

	int ret = addProductRepo(serv->repo, p);
	if (ret == 0) destroyProduct(p);

	return ret;
}

/// <summary>
/// Deletes a product from the repository
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <param name="name">The name of the product</param>
/// <param name="category">The category of the product</param>
/// <returns></returns>
int deleteProductService(Service* serv, char* name, Category category)
{
	return removeProductRepo(getRepo(serv), name, category);
}

/// <summary>
/// Updates a product from the repository
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <param name="name">The name of the product</param>
/// <param name="category">The category of the product</param>
/// <param name="quantity">The quantity of the product</param>
/// <param name="expiration">The expiration date of the product</param>
/// <returns></returns>
int updateProductService(Service* serv, char* name, Category category, double quantity, Date expiration)
{
	return updateProductRepo(getRepo(serv), name, category, quantity, expiration);
}

/// <summary>
/// Gets the repo from the service
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <returns>A pointer to the repository</returns>
ProductRepo* getRepo(Service* serv)
{
	return serv->repo;
}

/// <summary>
/// Filters products by a given string
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <param name="name">A string to be found in the product names</param>
/// <returns>A pointer to a repository that contains the filtered products</returns>
ProductRepo* filterByString(Service* serv, char* name)
{
	ProductRepo* repo = getRepo(serv);
	ProductRepo* newRepo = createRepo();

	for (int i = 0; i < repo->length; i++)
	{
		Product* current = getProductAt(repo, i);

		if (strlen(name) == 0 || strstr(current->name, name) != NULL)
		{
			Product* p = createProduct(current->name, current->category, current->quantity, current->expiration);

			int ret = addProductRepo(newRepo, p);
			if (ret == 0) destroyProduct(p);
		}
	}

	return newRepo;
}

/// <summary>
/// Filteres products by category and expiration date
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <param name="category">The category of the products</param>
/// <param name="expiration">The amount of days until products expire</param>
/// <returns>A pointer to a new repository that contains the filtered products</returns>
ProductRepo* filterByCategoryAndExpiration(Service* serv, Category category, int expiration)
{
	ProductRepo* repo = getRepo(serv);
	ProductRepo* newRepo = createRepo();

	time_t now = time(NULL);
	struct tm productDate = { 0 };
	time_t productTime;
	double difference;

	for (int i = 0; i < repo->length; i++)
	{
		Product* current = getProductAt(repo, i);
		productDate.tm_year = current->expiration.year - 1900;
		productDate.tm_mon = current->expiration.month - 1;
		productDate.tm_mday = current->expiration.day;

		productTime = mktime(&productDate);
		difference = round(difftime(productTime, now) / 86400);

		if (difference <= expiration && (category == none || current->category == category))
		{
			Product* p = createProduct(current->name, current->category, current->quantity, current->expiration);

			int ret = addProductRepo(newRepo, p);
			if (ret == 0) destroyProduct(p);
		}
	}

	return newRepo;
}

/// <summary>
/// Adds a repository state to the undo stack
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <param name="repo">The repository to be saved</param>
void addToUndoStack(Service* serv)
{
	// Clear redo stack
	for (int i = 0; i < serv->redoLength; i++)
		destroyRepo(serv->redoStack[i]);
	serv->redoLength = 0;

	// Realloc undo stack if needed
	if (serv->undoLength == serv->undoCapacity)
	{
		serv->undoCapacity *= REPOSITORY_SIZE_SCALE;
		ProductRepo** tmp = NULL;

		while (tmp == NULL) tmp = realloc(serv->undoStack, serv->undoCapacity * sizeof(ProductRepo*));
		serv->undoStack = tmp;
	}

	// Deep copy repo to undo stack
	ProductRepo* repoCopy = createRepo();
	for (int i = 0; i < serv->repo->length; i++)
	{
		Product* current = getProductAt(serv->repo, i);

		Product* p = createProduct(current->name, current->category, current->quantity, current->expiration);
		int ret = addProductRepo(repoCopy, p);
		if (ret == 0) destroyProduct(p);
	}
	serv->undoStack[serv->undoLength++] = repoCopy;
}

/// <summary>
/// Pops the last element from the undo stack
/// </summary>
/// <param name="serv">A pointer to the service</param>
void popUndoStack(Service* serv)
{
	if (serv->undoLength == 0) return;
	destroyRepo(serv->undoStack[serv->undoLength-- - 1]);
}

/// <summary>
/// Undoes the previous operation
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <returns>1 if the operation was successfull,
///			 0, otherwise</returns>
int undoOperation(Service* serv)
{
	if (serv->undoLength == 0) return 0;
	ProductRepo* repo = serv->undoStack[serv->undoLength - 1];

	// Realloc redo stack if needed
	if (serv->redoLength == serv->redoCapacity)
	{
		serv->redoCapacity *= REPOSITORY_SIZE_SCALE;
		ProductRepo** tmp = NULL;

		while (tmp == NULL) tmp = realloc(serv->redoStack, serv->redoCapacity * sizeof(ProductRepo*));
		serv->redoStack = tmp;
	}

	// Deep copy repo to redo stack
	ProductRepo* redoCopy = createRepo();
	for (int i = 0; i < serv->repo->length; i++)
	{
		Product* current = getProductAt(serv->repo, i);
		Product* p = createProduct(current->name, current->category, current->quantity, current->expiration);
		
		int ret = addProductRepo(redoCopy, p);
		if (ret == 0) destroyProduct(p);
	}
	serv->redoStack[serv->redoLength++] = redoCopy;

	// Do the undo
	ProductRepo* repoCopy = createRepo();
	for (int i = 0; i < repo->length; i++)
	{
		Product* current = getProductAt(repo, i);
		Product* p = createProduct(current->name, current->category, current->quantity, current->expiration);
		
		int ret = addProductRepo(repoCopy, p);
		if (ret == 0) destroyProduct(p);
	}

	destroyRepo(serv-> repo);
	serv->repo = repoCopy;

	destroyRepo(repo);
	serv->undoLength--;
	return 1;
}

/// <summary>
/// Redoes the previous operation
/// </summary>
/// <param name="serv">A pointer to the service</param>
/// <returns>1, if the operation was successfull,
///			 0, otherwise</returns>
int redoOperation(Service* serv)
{
	if (serv->redoLength == 0) return 0;
	ProductRepo* repo = serv->redoStack[serv->redoLength - 1];

	// Realloc undo stack if needed
	if (serv->undoLength == serv->undoCapacity)
	{
		serv->undoCapacity *= REPOSITORY_SIZE_SCALE;
		ProductRepo** tmp = NULL;

		while (tmp == NULL) tmp = realloc(serv->undoStack, serv->undoCapacity * sizeof(ProductRepo*));
		serv->undoStack = tmp;
	}

	// Deep copy repo to undo stack
	ProductRepo* undoCopy = createRepo();
	for (int i = 0; i < serv->repo->length; i++)
	{
		Product* current = getProductAt(serv->repo, i);
		Product* p = createProduct(current->name, current->category, current->quantity, current->expiration);
		
		int ret = addProductRepo(undoCopy, p);
		if (ret == 0) destroyProduct(p);
	}
	serv->undoStack[serv->undoLength++] = undoCopy;

	// Do the redo
	ProductRepo* repoCopy = createRepo();
	for (int i = 0; i < repo->length; i++)
	{
		Product* current = getProductAt(repo, i);
		Product* p = createProduct(current->name, current->category, current->quantity, current->expiration);
		
		int ret = addProductRepo(repoCopy, p);
		if (ret == 0) destroyProduct(p);
	}

	destroyRepo(serv->repo);
	serv->repo = repoCopy;

	destroyRepo(repo);
	serv->redoLength--;
	return 1;
}
