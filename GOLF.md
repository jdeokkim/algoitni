# 목차

아래 내용은 [C99 표준](https://en.cppreference.com/w/c/99) 및 [GCC v9.4.0](https://gcc.gnu.org/) 컴파일러를 기준으로 작성되었습니다.

* [`main()` 함수](#main-함수)
* [표준 입출력](#표준-입출력)
* [표현식과 연산자](#표현식과-연산자)
* [반복문과 배열](#반복문과-배열)
* [문자와 문자열](#문자와-문자열)
* [참고 자료](#참고-자료)

## `main()` 함수

- `main()` 함수를 포함한 **모든 함수의 반환형과 매개 변수 자료형은 생략 가능**하며, 이 경우에 반환형과 매개 변수 자료형은 모두 `int`가 된다. 또한, 매개 변수 목록이 아예 생략되어 있는 경우, 그 함수는 정해지지 않은 개수의 매개 변수를 받는 함수가 된다.

```c
#include <stdio.h>

/*
    `main(void)`: 매개 변수가 아예 없다는 뜻.
    `main()`: 매개 변수가 있으나, 몇 개인지는 정해지지 않았다는 뜻.
*/
main(){
    return 0;
}
```

<br />

- GCC 등의 C 컴파일러에서는 `#include <stdio.h>` 등의 전처리기 지시문을 생략할 수 있으며, `main()` 함수의 `return 0;`도 생략 가능하다.

```c
main(){
    /* TODO: ... */
}
```

<br />

- **`main()` 함수의 매개 변수 목록을 이용하면, 하나 이상의 `int` 변수를 간편하게 생성할 수 있다.** 이때, `main()` 함수의 첫 번째 매개 변수는 `int main (int argc, char *argv[]) { ... }`에서 `argc`에 대응하는 값을 가지게 된다.

```c
main(i,j,k){
    /* i = `argc`, j, k는 초기화 필요함! */
}
```

<br />

- `main()` 함수 바깥에 선언된 전역 변수들은 항상 0으로 초기화된다.

```c
i;main(){
    /* i = 0 */
}
```

<br />

- 코드에 반복되는 키워드 등이 존재한다면, 상황에 따라 `#define`을 이용하여 코드 길이를 줄일 수 있다. 특히, 매크로 이름 뒤에 문자열 리터럴 (string literal)이 올 경우에는 공백 문자를 생략 가능하다.

```c
#define D double
#define R return

/*
#define S "hello"
*/
#define S"hello"

#define L(x,y) for((x)=0;(x)<(y);(x)++)
```

<br />

## 표준 입출력

- **입력받아야 하는 값이 문자 1글자**일 때는 `scanf()` 대신 [`getchar()`](https://en.cppreference.com/w/c/io/getchar)를 사용할 수 있다.

```c
main(c){
//  scanf("%c",&c);printf("%d",c%2);
//  c=getchar();printf("%d",c%2);
    puts(getchar()%2?"1":"0");
}
```

<br />

- 여러 개의 문자를 하나씩 입력받을 때는 `" %c"`를 이용하여 모든 공백 문자를 무시할 수 있다.

```c
main(c){
    scanf(" %c",&c);
}
```

<br />

- **`gets()` 함수는 [C 프로그래밍을 할 때 절대로 사용해서는 안되는 위험한 함수 중 하나지만](https://stackoverflow.com/questions/1694036/why-is-the-gets-function-so-dangerous-that-it-should-not-be-used)**, 숏코딩을 할 때만큼은 유용하게 사용할 수 있다.

```c
S[21];main(){
//  scanf("%s",S);
    gets(S);

    puts(S);
}
```

<br />

- 문자를 출력할 때는 문자 리터럴 (character literal) 대신 그 문자에 대응하는 ASCII 코드를 사용할 수 있다.

```c
main(){
//  putchar('A');
    putchar(65);
}
```

<br />

- **형식 문자열이 없는 단순 문자열을 출력**할 때는 `printf()` 대신 [`puts()`](https://en.cppreference.com/w/c/io/puts)를 사용할 수 있는데, `puts()` 함수는 마지막에 개행 문자를 출력한다는 점에 유의하도록 한다.

```c
main(){
//  printf("YES\n");
    puts("YES");
}
```

```c
main(){
//  printf("\n");
    puts("");
}
```

<br />

## 표현식과 연산자

- `,` 연산자를 이용하면 여러 개의 **표현식** (expression)을 하나로 묶을 수 있다. 여기서 표현식은 연산자와 피연산자로 이루어진 식을 의미하며, `break`나 `continue`, `return` 등의 문장 (statement)과는 다른 개념이므로 주의하도록 한다.

```c
main(a,b){
//  if(a){a+=b;b++;}
    if(a)a+=b,b++;
}
```

<br />

- `if-else` 대신 삼항 연산자 (ternary operator)를 이용하여 조건문을 만들 수도 있다.

```c
main(a,b){
//  if(a!=b)puts("RUN");
//  a!=b?puts("RUN"):0;

    // https://gcc.gnu.org/onlinedocs/gcc/Conditionals.html
    a==b?:puts("RUN");
}
```

<br />

- `!` 연산자를 이용하면 특정 값이 0인지 여부를 간결하게 판단할 수 있다.

```c
main(a){
//  a==0?0:puts("RUN");
    !a?0:puts("RUN");
}
```

<br />

- 두 값이 서로 다른 값인지 확인할 때는 `!=` 연산자 대신 `^` 또는 `-` 연산자를 사용할 수 있다.

```c
main(a,b){
//  a!=b?puts("RUN"):0;
    a^b?puts("RUN"):0;
}
```

<br />

- `(x > 0) && (y > 0)`일 경우, `(x >= y) = (x / y)`가 성립한다.

```c
main(a,b){
    a=2,b=1;

//  puts(a>=b?"true":"false");
    puts(a/b?"true":"false");
}
```

<br />

- **논리 연산자 `&&`와 `||`의 왼쪽 표현식과 오른쪽 표현식 중 하나라도 논리 표현식 (boolean expression)일 경우**, `&&`와 `||`를 각각 `&`과 `|`로 대체할 수 있다. 단, 이 방법은 각 표현식에 포함된 [연산자의 우선순위](https://en.cppreference.com/w/c/language/operator_precedence)에 따라 올바르게 동작하지 않을 수도 있다.

```c
main(a,b){
    a=4,b=-1;

//  puts(a==4&&b!=0?"true":"false");
//  puts(a==4&&b?"true":"false");
    puts(a==4&b?"true":"false");
}
```

<br />

## 반복문과 배열

- 대부분의 경우, `while` 문보다는 `for` 문이 코드 길이가 더 짧다.

```c
main(){
//  while(1)break;
    for(;;)break;
}
```

<br />

- 반복문 코드를 작성할 때는 `main()` 함수의 매개 변수를 이용할 수 있다.

```c
main(i){
//  for(int i=1;i<=5;i++)putchar('a');
    for(;i<6;i++)putchar('a');
}
```

<br />

- `for`문의 조건식 (`cond-expression`)과 증감식 (`iteration-expression`)을 적절히 배치하면 코드 길이를 더 줄일 수 있다.

```c
main(i){
//  for(;i<6;i++)putchar('a');
    for(;i++<6;)putchar('a');
}
```

<br />

- **배열의 이름은 그 배열의 첫 번째 원소를 가리키는 메모리 주소이다.**

```c
v[3];main(i){
//  for(;i++<4;)scanf("%d",&v[i-2]);
    for(;i++<4;)scanf("%d",v+i-2);
}
```

```c
v[]={1,2,3};main(i){
//  printf("%d\n",v[0]);
    printf("%d\n",*v);
}
```

<br />

- C99 표준의 [복합 리터럴 (compound literal)](https://en.cppreference.com/w/c/language/compound_literal)을 이용하면 이름 없는 상수 배열을 만들 수 있다.

```c
double v[]={.5,.65,.8};main(){printf("%f\n",v[1]);}
```

```c
main(){printf("%f\n",(double[]){.5,.65,.8}[1]);}
```

<br />

## 문자와 문자열

- 문자열을 입력받을 때는 `char` 배열 대신 `int` 배열에 문자열을 저장할 수도 있다.

```c
main(n){char s[100];scanf("%d",&n);for(;n--;)scanf("%s",s),puts(s);}
```

```c
s[100];main(n){scanf("%d",&n);for(;n--;)scanf("%s",s),puts(s);}
```

<br />

- 문자열의 끝을 확인할 때는 `strlen()` 함수를 사용하는 대신, 각 문자가 널 문자인지를 검사하는 것이 좋다.

```c
i;main(){char*s="RUN";
//  for(;i<strlen(s);i++)putchar(s[i]);
    for(;s[i];i++)putchar(s[i]);
//  for(;*s;s++)putchar(*s);
}
```

<br />

## 참고 자료

- https://blog.merovius.de/posts/2013-10-11-how-to-cgolf/
- https://codegolf.stackexchange.com/questions/2203/tips-for-golfing-in-c
- https://en.cppreference.com/w/c/language/function_definition
- https://en.cppreference.com/w/c/language/main_function
- https://en.cppreference.com/w/c/language/operator_precedence
- http://graphics.stanford.edu/~seander/bithacks.html