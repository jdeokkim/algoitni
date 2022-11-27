# 목차

아래 내용은 [C99 표준](https://en.cppreference.com/w/c/99) 및 [GCC v9.4.0](https://gcc.gnu.org/) 컴파일러를 기준으로 작성되었습니다.

* [`main() 함수`](#main-함수)
* [참고 자료](#참고-자료)

## `main()` 함수

- `main()` 함수를 포함한 **모든 함수의 반환형과 매개 변수 자료형은 생략 가능**하며, 이 경우에 반환형과 매개 변수 자료형은 모두 `int`가 된다. 또한, 매개 변수 목록이 아예 생략되어 있는 경우, 그 함수는 정해지지 않은 개수의 매개 변수를 받는 함수가 된다.

```c
#include <stdio.h>

/*
    `main(void)`: 매개 변수가 아예 없다는 뜻.
    `main()`: 매개 변수가 있으나, 몇 개인지는 정해지지 않았다는 뜻.
*/
main() {
    return 0;
}
```

<br />

- GCC 등의 C 컴파일러에서는 `#include <stdio.h>` 등의 전처리기 지시문을 생략할 수 있으며, `main()` 함수의 `return 0;`도 생략 가능하다.

```c
main() {
    /* TODO: ... */
}
```

<br />

- **`main()` 함수의 매개 변수 목록을 이용하면, 하나 이상의 `int` 변수를 간편하게 생성할 수 있다.** 이때, `main()` 함수의 첫 번째 매개 변수는 `int main (int argc, char *argv[]) { ... }`에서 `argc`에 대응하는 값을 가지게 된다.

```c
main(i, j, k) {
    /* i = `argc`, j, k는 초기화 필요함! */
}
```

<br />

## 참고 자료

- https://codegolf.stackexchange.com/questions/2203/tips-for-golfing-in-c
- https://en.cppreference.com/w/c/language/function_definition
- https://en.cppreference.com/w/c/language/main_function