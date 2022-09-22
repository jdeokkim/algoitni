# 스택

스택 (stack)은 마지막으로 넣은 항목이 가장 먼저 나오는 후입선출 (last-in, first-out, LIFO)의 특성을 가진 자료 구조이다. 접시 더미를 예시로 들면, 우리는 접시가 하나 필요할 때 접시 더미의 중간에서 접시를 꺼내는 것은 너무 번거롭기 때문에 그냥 맨 위에 있는 접시 하나를 집어들게 되는데, 바로 이것이 스택의 특성이라고 볼 수 있다.

스택은 컴퓨터에서 특히 중요한 역할을 한다. 우리가 프로그램을 실행하면 컴퓨터의 주기억장치 (main memory)에 프로그램의 모든 명령어 (instruction)가 저장되고, 중앙 처리 장치는 이 명령어들을 하나씩 읽으면서 프로그램을 실행하게 된다. 만약, 중앙 처리 장치가 실행해야 하는 명령어가 함수 호출 명령이라면, 중앙 처리 장치는 현재 실행 중인 함수가 종료된 후에 다음으로 실행할 명령어의 메모리 주소를 중앙 처리 장치 내부 또는 주기억장치에 스택의 형태로 저장한다. 이렇듯 스택은 가장 기초적이면서 중요한 자료 구조이다.

## 시간 복잡도

스택의 삽입, 삭제 및 가장 최근에 추가된 항목 확인에 대한 시간 복잡도는 `O(1)`이다.

## API 설계

```c
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* | 매크로 정의... | */

#define STACK_INIT_CAPACITY 32

/* | 자료형 선언 및 정의... | */

/* 스택에 저장되는 항목의 자료형. */
typedef int Item;

/* 스택을 나타내는 추상 자료형. */
typedef struct Stack Stack;

/* | 라이브러리 함수... | */

/* 스택을 생성한다. */
Stack *stack_create(void);

/* 스택에 할당된 메모리를 해제한다. */
void stack_release(Stack *s);

/* 스택에 새로운 항목을 추가한다. */
bool stack_push(Stack *s, Item i);

/* 스택에서 가장 최근에 추가된 항목을 꺼낸다. */
bool stack_pop(Stack *s, Item *const i);

/* 스택이 비어 있는지 확인한다. */
bool stack_is_empty(Stack *s);

/* 스택에 저장된 항목의 개수를 반환한다. */
size_t stack_size(Stack *s);

#endif // `STACK_H`
```