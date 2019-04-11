/*
* nMalloc.cpp
*
*  Created on: Dec 5, 2014
*      Author: prasadnair
*/

#include <stdlib.h>
#include <stdio.h>

void * aligned_malloc(size_t size, int align) {
	/* 0보다 작은 바이트를 정렬할 수 없음 */
	if (align < 0) {
		return NULL;
	}

	/* 클라이언트에서 필요한 메모리 공간 할당 - 인자 크기 -
	* malloc()에 의해 반환된 메모리 주소를 저장할 공간에 더하기
	*/
	void *ptr;

	/* (void *)의 align-1 크기를 추가하여 최소 하나의 정렬된 메모리 영역을 수용한 이유는
	* 할당할때 사용한 주소를 p에 유지시키려고 추가한 것이다. 이 값은 free()를 호출할때 재활용한다.
	*/
	void *p = malloc(size + align - 1 + sizeof(void*));

	if (p != NULL) {

		/* 정렬할 인자에 따라 메모리 주소 정렬 */
		//  2의 모든 지수(1, 2, 4, 8, 16, 32 ...)는 간단한 조작으로 정렬할 수 있음.
		//
		// - 반내림 : size &= ~(align - 1)
		//
		// - 반올림 : size = (size + align - 1) & ~(align - 1);
		//
		// - bool power_of_two = !(alignment & (alignment-1)) 
		//	 "align - 1"은 2의 거듭제곱에 근사한 값으로, 이를 이용해 2의 거듭제곱 밑의 모든 비트들을 얻을 수 있음. 
		//	~은 모든 비트를 반전시킴
		//
		// 예) 4
		//	4		= 00000100
		//	4 - 1	= 00000011
		//	& -----------------
		//	0		= 00000000
		//
		// 예) 16
		//	16		= 00010000
		//	16 - 1	= 00001111
		//	& -----------------
		//	0		= 00000000
		//
		// 예) 5
		//	5		= 00000101
		//	4 - 1	= 00000100
		//	& ----------------
		//	4		= 00000100
		// 2의 거듭제곱이 아님!

		ptr = (void*)(((size_t)p + sizeof(void*) + align - 1) & ~(align - 1));
		// 반올림해서 align byte에 맞는 시작 주소값을 찾는 것

		/* 전체 메모리 영역의 시작 주소 위에 malloc()의 주소 저장 */
		*((void**)((size_t)ptr - sizeof(void*))) = p;

		/* 정렬된 메모리 주소 반환 */
		return ptr;
	}
	return NULL;
}

void aligned_free(void *p) {
	/* 전체 메모리 영역의 시작 부분에 저장된 메모리 주소 획득 */
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