# Shanhj_Json

一个轻量的C++Json库，一共只有880行左右的代码，支持解析Json，定位出错位置，输出带缩进和不带缩进的Json。

# Demo1

```cpp {.line-numbers}
#include "Shanhj_Json.hpp"
#include <iostream>

using namespace std;
using namespace Shanhj_Json;

int main()
{
    JsonObject obj1, obj2;
    JsonArray arr1, arr2;
    obj1.insert("name", "Shanhj");
    obj1.insert("age", 21);
    obj1.insert("sex", true);
    obj1.insert("height", 173.1);
    arr1.insert("CPP");
    arr1.insert("C");
    arr1.insert("Java");
    obj1.insert("programLanguage", arr1);
    obj2.insert("name", "Yoimiya");
    obj2.insert("birthday", "6/21");
    obj2.insert("sex", false);
    obj1.insert("favorite game role", obj2);
    arr2.insert("Naraka");
    arr2.insert("Genshine Impact");
    obj1.insert("games played", arr2);
    cout << obj1.output_to_string() << endl;
    return 0;
}
```

带缩进的输出如下：

```json
{
    "age": 21,
    "favorite game role": {
        "birthday": "6/21",
        "name": "Yoimiya",
        "sex": false
    },
    "games played": [
        "Naraka",
        "Genshine Impact"
    ],
    "height": 173.100000,
    "name": "Shanhj",
    "programLanguage": [
        "CPP",
        "C",
        "Java"
    ],
    "sex": true
}
```

不带缩进的输出如下：

```json
{"age":21,"favorite game role":{"birthday":"6/21","name":"Yoimiya","sex":false},"games played":["Naraka","Genshine Impact"],"height":173.100000,"name":"Shanhj","programLanguage":["CPP","C","Java"],"sex":true}
```

# Demo2

```cpp {.line-numbers}
#include "Shanhj_Json.hpp"
#include <iostream>

using namespace std;
using namespace Shanhj_Json;

int main()
{
    JsonArray arr;
    arr.insert(114514);
    arr.insert(1919810);
    arr.insert(114.514);
    arr.insert("Shimo-Kitazawa");
    arr.insert(true);
    arr.insert(false);
    cout << arr.output_to_string() << endl;
    return 0;
}
```

带缩进的输出如下：

```json
[
    114514,
    1919810,
    114.514000,
    "Shimo-Kitazawa",
    true,
    false
]
```

不带缩进的输出如下：

```json
[114514,1919810,114.514000,"Shimo-Kitazawa",true,false]
```

# Demo3

```cpp {.line-numbers}
#include "Shanhj_Json.hpp"
#include <cstdio>
#include <iostream>

using namespace std;
using namespace Shanhj_Json;

int main()
{
    FILE *file = fopen("test.json", "r");
    char buff[4096];
    int len = fread(buff, 1, 4096, file);
    if (len <= 0) return 0;
    JsonObject obj;
    bool res;
    obj.parser_from_array(buff, res);
    if (res)
        cout << obj.output_to_string() << endl;
    return 0;
}
```

test.json文件内容

```
{"age":21,"favorite game role":{"birthday":"6/21","name":"Yoimiya","sex":false},"games played":["Naraka","Genshine Impact"],"height":173.100000,"name":"Shanhj","programLanguage":["CPP","C","Java"],"sex":true}
```

程序输出：

```json
{
    "age": 21,
    "favorite game role": {
        "birthday": "6/21",
        "name": "Yoimiya",
        "sex": false
    },
    "games played": [
        "Naraka",
        "Genshine Impact"
    ],
    "height": 173.100000,
    "name": "Shanhj",
    "programLanguage": [
        "CPP",
        "C",
        "Java"
    ],
    "sex": true
}
```