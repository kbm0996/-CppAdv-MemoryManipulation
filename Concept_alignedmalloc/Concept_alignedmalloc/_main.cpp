/*
* nMalloc.cpp
*
*  Created on: Dec 5, 2014
*      Author: prasadnair
*/

#include <stdlib.h>
#include <stdio.h>

void * aligned_malloc(size_t size, int align) {
	/* 0���� ���� ����Ʈ�� ������ �� ���� */
	if (align < 0) {
		return NULL;
	}

	/* Ŭ���̾�Ʈ���� �ʿ��� �޸� ���� �Ҵ� - ���� ũ�� -
	* malloc()�� ���� ��ȯ�� �޸� �ּҸ� ������ ������ ���ϱ�
	*/
	void *ptr;

	/* (void *)�� align-1 ũ�⸦ �߰��Ͽ� �ּ� �ϳ��� ���ĵ� �޸� ������ ������ ������
	* �Ҵ��Ҷ� ����� �ּҸ� p�� ������Ű���� �߰��� ���̴�. �� ���� free()�� ȣ���Ҷ� ��Ȱ���Ѵ�.
	*/
	void *p = malloc(size + align - 1 + sizeof(void*));

	if (p != NULL) {

		/* ������ ���ڿ� ���� �޸� �ּ� ���� */
		//  2�� ��� ����(1, 2, 4, 8, 16, 32 ...)�� ������ �������� ������ �� ����.
		//
		// - �ݳ��� : size &= ~(align - 1)
		//
		// - �ݿø� : size = (size + align - 1) & ~(align - 1);
		//
		// - bool power_of_two = !(alignment & (alignment-1)) 
		//	 "align - 1"�� 2�� �ŵ������� �ٻ��� ������, �̸� �̿��� 2�� �ŵ����� ���� ��� ��Ʈ���� ���� �� ����. 
		//	~�� ��� ��Ʈ�� ������Ŵ
		//
		// ��) 4
		//	4		= 00000100
		//	4 - 1	= 00000011
		//	& -----------------
		//	0		= 00000000
		//
		// ��) 16
		//	16		= 00010000
		//	16 - 1	= 00001111
		//	& -----------------
		//	0		= 00000000
		//
		// ��) 5
		//	5		= 00000101
		//	4 - 1	= 00000100
		//	& ----------------
		//	4		= 00000100
		// 2�� �ŵ������� �ƴ�!

		ptr = (void*)(((size_t)p + sizeof(void*) + align - 1) & ~(align - 1));
		// �ݿø��ؼ� align byte�� �´� ���� �ּҰ��� ã�� ��

		/* ��ü �޸� ������ ���� �ּ� ���� malloc()�� �ּ� ���� */
		*((void**)((size_t)ptr - sizeof(void*))) = p;

		/* ���ĵ� �޸� �ּ� ��ȯ */
		return ptr;
	}
	return NULL;
}

void aligned_free(void *p) {
	/* ��ü �޸� ������ ���� �κп� ����� �޸� �ּ� ȹ�� */
	void *ptr = *((void**)((size_t)p - sizeof(void*)));
	free(ptr);
	return;
}


int  main()
{

	int align;
	for (align = 2; align < 5000000; align = align * 2)
	{
		//bool power_of_two = !(align & (align - 1));
		//printf("!(%d & (%d - 1) = %d\n ", align, align, power_of_two);

		int *p = (int *)aligned_malloc(1024, align);
		printf("Aligned Byte %07d	:	%p %d \n", align, p, ((long int)p) % align);
		aligned_free(p);
	}
	return 0;
}