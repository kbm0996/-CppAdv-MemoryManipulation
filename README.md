# 메모리 제어하기
## 📢 개요
 C언어와 같은 저수준(Low-Level)에 가까운 프로그래밍 언어는 메모리 관리와 같은 하드웨어 제어가 가능하다는 특징이 있다. 이는 메모리를 프로그래머가 마음대로 관리 할 수 있다는 장점이 될 수도 있고, 예상치 못한 메모리 관련 에러가 발생할 위험이 있다는 단점이 되기도 한다.
  
## 📑 구성
  자세한 내용은 하위 디렉토리 참조

### 💻 Concept_alignedmalloc
  메모리 할당시 시작 주소를 특정 비트로 정렬해주는 _aligned_malloc() 함수의 원리

### 💻 FilePacking
  다수의 파일을 하나의 파일로 포장(Packing)하는 예제. 압축(Compression)이 아니므로 용량이 줄어들지는 않음

### 💻 MemoryTracer_malloc
  malloc()으로 메모리를 할당했을 때 template 함수를 이용해 해제를 했는지 안했는지 여부를 알 수 있도록 구현

### 💻 MemoryTracer_new
  new를 override하여 메모리를 할당할 때마다 singleton class에 그 정보를 저장, 추적하도록 구현
