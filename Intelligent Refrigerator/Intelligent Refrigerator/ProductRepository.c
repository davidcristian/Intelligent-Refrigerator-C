#include <stdlib.h>
#include <string.h>

#include "ProductRepository.h"

/// <summary>
/// Creates a new repository
/// </summary>
/// <returns>A pointer to the created repository</returns>
ProductRepo* createRepo()
{
	ProductRepo* repo = malloc(sizeof(ProductRepo));
	if (repo == NULL) return NULL;

	repo->products = malloc(REPOSITORY_INITIAL_SIZE * sizeof(Product*));
	if (repo->products == NULL)
	{
		free(repo);
		return NULL;
	}

	repo->capacity = REPOSITORY_INITIAL_SIZE;
	repo->length = 0;
	return repo;
}

/// <summary>
/// Destroys the repository and all contents
/// </summary>
/// <param name="repo">A pointer to the repository</param>
void destroyRepo(ProductRepo* repo)
{
	if (repo == NULL) return;

	for (int i = 0; i < repo->length; i++)
		destroyProduct(repo->products[i]);

	free(repo->products);
	free(repo);

	repo = NULL;
}

/// <summary>
/// Adds a product to the repository
/// </summary>
/// <param name="repo">A pointer to the repository</param>
/// <param name="p">The product to add</param>
/// <returns></returns>
int addProductRepo(ProductRepo* repo, Product* p)
{
	for (int i = 0; i < repo->length; i++)
	{
		Product* current = getProductAt(repo, i);

		if (strcmp(current->name, p->name) == 0 && current->category == p->category)
		{
			current->quantity += p->quantity;
			destroyProduct(p);
			return 1;
		}
	}

	if (repo->length == repo->capacity)
	{
		repo->capacity *= REPOSITORY_SIZE_SCALE;
		Product** tmp = NULL;

		while (tmp == NULL) tmp = realloc(repo->products, repo->capacity * sizeof(Product*));
		repo->products = tmp;
	}

	repo->products[repo->length++] = p;
	return 1;
}

/// <summary>
/// Removes a product from the repository
/// </summary>
/// <param name="repo">A pointer to the repository</param>
/// <param name="name">The name of the product</param>
/// <param name="category">The category of the product</param>
/// <returns></returns>
int removeProductRepo(ProductRepo* repo, char* name, Category category)
{
	for (int i = 0; i < repo->length; i++)
	{
		Product* current = getProductAt(repo, i);

		if (strcmp(current->name, name) == 0 && current->category == category)
		{
			destroyProduct(current);
			// Copy everything over by 1
			for (int j = i; j < repo->length - 1; j++)
			{
				repo->products[j] = repo->products[j + 1];
			}

			repo->products[repo->length--] = NULL;
			return 1;
		}
	}

	return 0;
}

/// <summary>
/// Updates a product with new values
/// </summary>
/// <param name="repo">A pointer to the repository</param>
/// <param name="name">The name of the product</param>
/// <param name="category">The category of the product</param>
/// <param name="quantity">The quantity of the product</param>
/// <param name="expiration">The expiration date of the product</param>
/// <returns></returns>
int updateProductRepo(ProductRepo* repo, char* name, Category category, double quantity, Date expiration)
{
	for (int i = 0; i < repo->length; i++)
	{
		Product* current = getProductAt(repo, i);

		if (strcmp(current->name, name) == 0 && current->category == category)
		{
			current->expiration = expiration;
			current->quantity = quantity;
			return 1;
		}
	}

	return 0;
}

/// <summary>
/// Gets the length of the repository
/// </summary>
/// <param name="repo">A pointer to the repository</param>
/// <returns>The number of elements in the repository</returns>
int getLength(ProductRepo* repo)
{
	return repo->length;
}

/// <summary>
/// Gets the product at the given index
/// </summary>
/// <param name="repo">A pointer to the repository</param>
/// <param name="index">The index to look at</param>
/// <returns>The product at the given index</returns>
Product* getProductAt(ProductRepo* repo, int index)
{
	if (repo == NULL) return NULL;
	if (index < 0 || index >= repo->length) return NULL;

	return repo->products[index];
}

/// <summary>
/// Sorts the repo by quantity
/// </summary>
/// <param name="repo">A pointer to the repository</param>
/// <param name="descending">1 if the sort should be descending, otherwise ascending</param>
void sortByQuantity(ProductRepo* repo, int descending)
{
	if (repo->length < 2)
		return;

	for (int i = 0; i < repo->length - 1; i++)
	{
		for (int j = 0; j < repo->length - i - 1; j++)
		{
			Product* current = getProductAt(repo, j);
			Product* next = getProductAt(repo, j + 1);

			if (descending == 0)
			{
				if (current->quantity > next->quantity)
				{
					Product tmp = *current;
					*current = *next;
					*next = tmp;
				}
			}
			else
			{
				if (current->quantity < next->quantity)
				{
					Product tmp = *current;
					*current = *next;
					*next = tmp;
				}
			}
		}
	}
}

/// <summary>
/// Sorts products by name
/// </summary>
/// <param name="repo">A pointer to the repository</param>
/// <param name="descending">1 if the sort should be descending, otherwise ascending</param>
void sortByName(ProductRepo* repo, int descending)
{
	if (repo->length < 2)
		return;

	for (int i = 0; i < repo->length - 1; i++)
	{
		for (int j = 0; j < repo->length - i - 1; j++)
		{
			Product* current = getProductAt(repo, j);
			Product* next = getProductAt(repo, j + 1);

			if (descending == 0)
			{
				if (strcmp(current->name, next->name) > 0)
				{
					Product tmp = *current;
					*current = *next;
					*next = tmp;
				}
			}
			else
			{
				if (strcmp(current->name, next->name) < 0)
				{
					Product tmp = *current;
					*current = *next;
					*next = tmp;
				}
			}
		}
	}
}
