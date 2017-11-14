#include <stdio.h>
#include <stdlib.h>
typedef int ElementType;
#ifndef _AvlTree_H

void FatalError(char string[])
{
	printf("%s\n", string);
	system("PAUSE");
	exit(0);
}

struct AvlNode;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;

Position SingleRotateWithLeft(Position K2);
Position SingleRotateWithRight(Position K2);
Position DoubleRotateWithLeft(Position K3);
Position DoubleRotateWithRight(Position K3);

AvlTree Initialize(void);
AvlTree MakeEmpty(AvlTree T);
Position Find(ElementType X, AvlTree T);
Position FindMin(AvlTree T);
Position FindMax(AvlTree T);
AvlTree Insert(ElementType X, AvlTree T);
AvlTree Delete_R(ElementType X, AvlTree T);
AvlTree Delete(ElementType X, AvlTree T);
Position FindPrevious(ElementType X, AvlTree T);
ElementType Retrieve(Position P);
void ShowTree(AvlTree);
int Height(Position P);

#endif /* _AvlTee_H */

/* Place in the implementation file */
struct AvlNode
{
	ElementType Element;
	AvlTree Left;
	AvlTree Right;
	int Height;
};

int Height(Position P)
{
	if (P == NULL)
		return -1;
	else
		return P->Height;
}

AvlTree Initialize(void)
{
	AvlTree T;

	T = malloc(sizeof(struct AvlNode));
	if (T == NULL)
		FatalError("Out of space while initializing AvlTree");
	T->Element = -1;
	T->Height = 0;
	T->Left = T->Right = NULL;

	return T;
}

AvlTree MakeEmpty(AvlTree T)
{
	if (T->Left != NULL)
		T->Left = MakeEmpty(T->Left);
	if (T->Right != NULL)
		T->Right = MakeEmpty(T->Right);
	if (T->Element != -1)
	{
		free(T);
		T = NULL;
	}

	return T;
}

Position Find(ElementType X, AvlTree T)
{
	if (T == NULL)
		return NULL;
	if (X < T->Element)
		return Find(X, T->Left);
	else if (X > T->Element)
		return Find(X, T->Right);
	else
		return T;
}

Position FindMin(AvlTree T)
{
	if (T == NULL)
		return NULL;
	else
	{
		if (T->Left == NULL)
			return T;
		else
			return FindMin(T->Left);
	}
}

Position FindMax(AvlTree T)
{
	if (T == NULL)
		return NULL;
	else
	{
		if (T->Right == NULL)
			return T;
		else
			return FindMax(T->Right);
	}
}

AvlTree Insert(ElementType X, AvlTree T)
{
	if (T == NULL)
	{
		T = malloc(sizeof(struct AvlNode));
		if (T == NULL)
			FatalError("Out of space while creating a new cell.\n");
		else
		{
			T->Element = X;
			T->Height = 0;
			T->Left = T->Right = NULL;
		}
	}
	else if (X < T->Element)
	{
		T->Left = Insert(X, T->Left);
		if (Height(T->Left) - Height(T->Right) == 2 && T->Element != -1)
			if (X < T->Left->Element)
				T = SingleRotateWithLeft(T);
			else
				T = DoubleRotateWithLeft(T);
	}
	else if (X > T->Element)
	{
		T->Right = Insert(X, T->Right);
		if (Height(T->Right) - Height(T->Left) == 2 && T->Element != -1)
			if (X > T->Right->Element)
				T = SingleRotateWithRight(T);
			else
				T = DoubleRotateWithRight(T);
	}

	else
		printf("%d is already in the tree.\n", X);

	if (Height(T->Left) > Height(T->Right))
		T->Height = Height(T->Left) + 1;
	else
		T->Height = Height(T->Right) + 1;

	return T;
}

Position SingleRotateWithLeft(Position K2)
{
	Position K1;

	K1 = K2->Left;
	K2->Left = K1->Right;
	K1->Right = K2;

	K2->Height = (Height(K2->Left) > Height(K2->Right)) ? Height(K2->Left) : Height(K2->Right);
	K2->Height++;
	K1->Height = (Height(K1->Left) > K2->Height) ? Height(K1->Left) : K2->Height;
	K1->Height++;

	return K1;
}

Position SingleRotateWithRight(Position K2)
{
	Position K1;

	K1 = K2->Right;
	K2->Right = K1->Left;
	K1->Left = K2;

	K2->Height = (Height(K2->Left) > Height(K2->Right)) ? Height(K2->Left) : Height(K2->Right);
	K2->Height++;
	K1->Height = (Height(K1->Right) > K2->Height) ? Height(K1->Right) : K2->Height;
	K1->Height++;

	return K1;
}

Position DoubleRotateWithLeft(Position K3)
{
	K3->Left = SingleRotateWithRight(K3->Left);

	return SingleRotateWithLeft(K3);
}

Position DoubleRotateWithRight(Position K3)
{
	K3->Right = SingleRotateWithLeft(K3->Right);

	return SingleRotateWithRight(K3);
}

Position FindPrevious(ElementType X, AvlTree T)
{
	Position P = T;

	while (1)
	{
		if (X<P->Element && P->Left->Element != X)
			P = P->Left;
		else if (X> P->Element && P->Right->Element != X)
			P = P->Right;
		else
			return P;
	}
}

AvlTree Delete_R(ElementType X, AvlTree T)
{
	if (T->Left == NULL && T->Right == NULL)
	{
		free(T);
		T = NULL;
	}
	else if (X < T->Element)
	{
		T->Left = Delete_R(X, T->Left);
			if (Height(T->Right) - Height(T->Left) == 2 && T->Element != -1)
				if (Height(T->Right->Right) > Height(T->Right->Left))
					T = SingleRotateWithRight(T);
				else
					T = DoubleRotateWithRight(T);
		if (Height(T->Left) > Height(T->Right))
			T->Height = Height(T->Left) + 1;
		else
			T->Height = Height(T->Right) + 1;
	}
	else if (X > T->Element)
	{
		T->Right = Delete_R(X, T->Right);
			if (Height(T->Left) - Height(T->Right) == 2 && T->Element != -1)
				if (Height(T->Left->Left) > Height(T->Left->Right))
					T = SingleRotateWithLeft(T);
				else
					T = DoubleRotateWithLeft(T);
		if (Height(T->Left) > Height(T->Right))
			T->Height = Height(T->Left) + 1;
		else
			T->Height = Height(T->Right) + 1;
	}
	else
	{
		if (T->Right != NULL)
		{
			T->Element = FindMin(T->Right)->Element;
			T->Right = Delete_R(T->Element, T->Right);
			if (Height(T->Left) - Height(T->Right) == 2 && T->Element != -1)
				if (X > T->Left->Element)
					T = SingleRotateWithLeft(T);
				else
					T = DoubleRotateWithLeft(T);
			if (Height(T->Left) > Height(T->Right))
				T->Height = Height(T->Left) + 1;
			else
				T->Height = Height(T->Right) + 1;
		}
		else
		{
			T->Element = FindMax(T->Left)->Element;
			T->Left = Delete_R(T->Element, T->Left);
			if (Height(T->Right) - Height(T->Left) == 2 && T->Element != -1)
				if (X < T->Left->Element)
					T = SingleRotateWithRight(T);
				else
					T = DoubleRotateWithRight(T);
			if (Height(T->Left) > Height(T->Right))
				T->Height = Height(T->Left) + 1;
			else
				T->Height = Height(T->Right) + 1;
		}
	}

	return T;
}

AvlTree Delete(ElementType X, AvlTree T)
{
	if (Find(X, T) != NULL)
	{
		T = Delete_R(X, T);
		return T;
	}
	else
	{
		printf("The element '%d' is not found.\n", X);
		return T;
	}
}

ElementType Retrieve(Position P)
{
	return P->Element;
}

#include <time.h>

int main(void)
{
	AvlTree T = Initialize();
	int i, tmp;
	
	printf("Initializing successfully!\n");
	printf("Inserting...\n");
	srand((unsigned int)time(NULL));
	for (i = 0; i != 20; ++i)
	{
		tmp = rand() % 20 + 1;
		printf("Inserting %d...\n", tmp);
		T = Insert(tmp, T);
	}
	printf("Deleting...\n");
	for (i = 0; i != 20; ++i)
	{
		tmp = rand() % 20 + 1;
		printf("Deleting %d...\n", tmp);
		T = Delete(tmp, T);
	}
	printf("Making the tree empty...\n");
	T = MakeEmpty(T);
	printf("Test is over!\n");
	system("PAUSE");

	return 0;
}