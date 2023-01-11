#include <string.h>
#include <assert.h>

#include "Product.h"
#include "ProductRepository.h"
#include "Service.h"

/// <summary>
/// Runs tests for the domain
/// </summary>
void testDomain()
{
	Date d1 = date(2022, 3, 16);

	Product* product = createProduct("test", none, 1, d1);

	assert(strcmp(getName(product), "test") == 0);
	assert(getCategory(product) == none);
	assert(getQuantity(product) == 1);

	Date d2 = getExpiration(product);
	assert(d1.year == d2.year);
	assert(d1.month == d2.month);
	assert(d1.day == d2.day);
	
	char productString[256];
	toString(product, productString);
	assert(strcmp(productString, "Product test is part of the \"none\" category, there is 1 of it in the fridge, and it expires on 2022/03/16.") == 0);

	destroyProduct(product);
}

/// <summary>
/// Runs tests for the repository
/// </summary>
void testRepo()
{
	ProductRepo* repo = createRepo();
	assert(getLength(repo) == 0);

	Product* p1 = createProduct("test4", none, 6, date(2022, 3, 15));
	Product* p2 = createProduct("test1", none, 3, date(2022, 3, 16));
	Product* p3 = createProduct("test6", none, 5, date(2022, 3, 17));
	Product* p4 = createProduct("test7", none, 4, date(2022, 3, 18));
	Product* p5 = createProduct("test2", none, 1, date(2022, 3, 19));

	addProductRepo(repo, p1);
	assert(getLength(repo) == 1);

	addProductRepo(repo, p5);
	assert(getLength(repo) == 2);

	assert(getProductAt(repo, 0) == p1);
	assert(getProductAt(repo, 1) == p5);
	assert(getProductAt(repo, 2) == NULL);

	removeProductRepo(repo, p5->name, p5->category);
	assert(getLength(repo) == 1);
	assert(getProductAt(repo, 1) == NULL);

	updateProductRepo(repo, p1->name, p1->category, 5, date(2022, 3, 20));
	assert(getProductAt(repo, 0) == p1);
	assert(p1->quantity == 5);

	addProductRepo(repo, p2);
	addProductRepo(repo, p3);
	addProductRepo(repo, p4);

	sortByQuantity(repo, 0);
	assert(getProductAt(repo, 0)->quantity == 3);
	assert(getProductAt(repo, 1)->quantity == 4);
	assert(getProductAt(repo, 2)->quantity == 5);
	assert(getProductAt(repo, 3)->quantity == 5);

	sortByName(repo, 0);
	assert(strcmp(getProductAt(repo, 0)->name, "test1") == 0);
	assert(strcmp(getProductAt(repo, 1)->name, "test4") == 0);
	assert(strcmp(getProductAt(repo, 2)->name, "test6") == 0);
	assert(strcmp(getProductAt(repo, 3)->name, "test7") == 0);

	destroyRepo(repo);
}

/// <summary>
/// Runs tests for the service
/// </summary>
void testService()
{
	Product *p1, *p2, *p3;

	ProductRepo* repo = createRepo();
	Service* serv = createService(repo, 0);
	ProductRepo *quantityFilter, *categoryAndExpiration;

	assert(getRepo(serv) == repo);

	addProductService(serv, "test1", none, 1, date(2222, 3, 15));
	addProductService(serv, "test2", none, 2, date(2022, 3, 16));
	addProductService(serv, "test3", none, 3, date(2022, 3, 17));

	assert(getLength(repo) == 3);
	p1 = getProductAt(repo, 0);
	p2 = getProductAt(repo, 1);
	p3 = getProductAt(repo, 2);

	deleteProductService(serv, p3->name, p3->category);
	assert(getLength(repo) == 2);
	
	updateProductService(serv, p2->name, p2->category, 3, date(2022, 3, 20));
	assert(getProductAt(repo, 1) == p2);
	assert(p2->quantity == 3);

	quantityFilter = filterByString(serv, "1");
	assert(getLength(quantityFilter) == 1);
	assert(strcmp(getProductAt(quantityFilter, 0)->name, "test1") == 0);
	destroyRepo(quantityFilter);
	
	categoryAndExpiration = filterByCategoryAndExpiration(serv, none, 7);
	assert(getLength(categoryAndExpiration) == 1);
	assert(strcmp(getProductAt(categoryAndExpiration, 0)->name, "test2") == 0);
	destroyRepo(categoryAndExpiration);

	addToUndoStack(serv);
	assert(serv->undoLength == 1);
	deleteProductService(serv, "test2", none);
	assert(getRepo(serv)->length == 1);
	undoOperation(serv);
	assert(getRepo(serv)->length == 2);
	redoOperation(serv);
	assert(getRepo(serv)->length == 1);

	destroyService(serv);
}

/// <summary>
/// Runts all tests
/// </summary>
void runAllTests()
{
	testDomain();
	testRepo();
	testService();
}
