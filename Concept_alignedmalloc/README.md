# 비트 연산 예제
## 📢 개요
 변수의 비트 단위 조작 
      

## ▶️ 프로그램 기능
  ![Bitwise Operation](https://github.com/kbm0996/Practice-BitwiseOperation/blob/master/BitwiseOperation/figure/BitwiseOperation.jpg)

 **figure 1. Bitwise Operation*
 

## 📌 주요 소스

    ...
    scanf_s("%d", &iFlag_Input);
    if (iFlag_Input == 1)
    {
    	shBit = shBit | (0x1 << iIndex_Input - 1);
    }
    else if (iFlag_Input == 0)
    {
    	shBit = shBit ^ (0x1 << iIndex_Input - 1); // XOR 연산
    }


 ## 📌 출처 
 
 개요 예제 - https://www.programiz.com/c-programming/bitwise-operators

