- [Shanhj\_Json](#shanhj_json)
- [Demo1-输出json对象](#demo1-输出json对象)
- [Demo2-输出json数组](#demo2-输出json数组)
- [Demo3-解析json对象](#demo3-解析json对象)
- [Demo4-解析json数组](#demo4-解析json数组)
- [Demo5-错误定位](#demo5-错误定位)

# Shanhj_Json

一个轻量的C++Json库，一共只有880行左右的代码。

功能如下：

- 解析utf-8编码的Json
- 定位出错位置
- 输出带缩进和不带缩进的Json。

限制点：

- 只支持utf-8格式的json数据，对于json中的`\uxxxx`转义Unicode无法解析。
- JsonObject只能解析Json对象，即`{***}`格式的json。
- JsonArray只能解析Json数组，即`[***]`格式的json。
- 解析json时遇到第一个合法对象即停止，对于后面的数据合法与否不做检查，比如
    ```json
    {"legal expression": 123}"123"
    ```
    是可以通过解析的，因为在遇到第二个花括号即停止解析了，后面的内容不会继续。

**对于嵌套深度没有限制。**

# Demo1-输出json对象

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

```json {.line-numbers}
{
    "age": 21,
    "favorite game role": {
        "birthday": "6\/21",
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

```json {.line-numbers}
{"age":21,"favorite game role":{"birthday":"6/21","name":"Yoimiya","sex":false},"games played":["Naraka","Genshine Impact"],"height":173.100000,"name":"Shanhj","programLanguage":["CPP","C","Java"],"sex":true}
```

# Demo2-输出json数组

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

```json {.line-numbers}
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

```json {.line-numbers}
[114514,1919810,114.514000,"Shimo-Kitazawa",true,false]
```

# Demo3-解析json对象

```cpp {.line-numbers}
#include "Shanhj_Json.hpp"
#include <cstdio>
#include <iostream>

using namespace std;
using namespace Shanhj_Json;

int main()
{
    FILE *file = fopen("test3.json", "r");
    char buff[4096];
    int len = fread(buff, 1, 4096, file);
    if (len <= 0) return 0;
    JsonObject obj;
    bool res;
    obj.parser_from_array(buff, res);
    if (res)
    {
        string value;
        if (obj.get_string("name", value))
            cout << "name:" << value << endl;
        else
            cout << "name:null" << endl;

        int64_t age;
        if (obj.get_int("age", age))
            cout << "age:" << age << endl;
        else
            cout << "age:null" << endl;

        JsonObject gamerole;
        if (obj.get_object("favorite game role", gamerole))
            cout << "favorite game role:"
                 << gamerole.output_to_string() << endl;
        else
            cout << "favorite game role:null" << endl;

        JsonArray games;
        if (obj.get_array("games played", games))
            cout << "games played:" << games.output_to_string() << endl;
        else
            cout << "games played:null" << endl;
    }
    return 0;
}
```

test3.json文件内容

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

程序输出：

```
name:Shanhj
age:21
favorite game role:{
    "birthday": "6\/21",
    "name": "Yoimiya",
    "sex": false
}
games played:[
    "Naraka",
    "Genshine Impact"
]
```

# Demo4-解析json数组

test.json文件内容

```json
[114514, true, "1919810", false, null]
```

```cpp
#include "../Shanhj_Json.hpp"
#include <cstdio>
#include <iostream>

using namespace std;
using namespace Shanhj_Json;

char buff[4096];

int main()
{
    FILE *file = fopen("test4.json", "r");
    auto len = fread(buff, 1, 4096, file);
    if (len <= 0) return 0;
    JsonArray arr;
    bool res;
    auto end_pos = arr.parser_from_array(buff, res);
    if (res)
    {
        int64_t num;
        if (arr.get_int(0, num))
            cout << "index0:" << num << endl;

        bool flag;
        if (arr.get_boolean(1, flag))
            cout << "index1:" << (flag ? "true" : "false") << endl;

        string text;
        if (arr.get_string(2, text))
            cout << "index2:" << text << endl;

        if (arr.get_boolean(3, flag))
            cout << "index3:" << (flag ? "true" : "false") << endl;

        arr.remove(1);

        cout << arr.output_to_string() << endl;
    }
    return 0;
}
```

输出如下：

```
index0:114514
index1:true  
index2:1919810
index3:false
[
    114514,
    "1919810",
    false,
    null
]
```

# Demo5-错误定位

错误定位功能演示

下面是一个错误格式的json数据：

```jso {.line-numbers}
{
    "legal expression1": 3,
    "Illegal expression": 1 + 2,
    "legal expression2": 3
}
```

```cpp
#include "Shanhj_Json.hpp"
#include <cstdio>
#include <iostream>

using namespace std;
using namespace Shanhj_Json;

char buff[4096];

int main()
{
    FILE *file = fopen("test.json", "r");
    auto len = fread(buff, 1, 4096, file);
    if (len <= 0) return 0;
    JsonObject obj;
    bool res;
    auto end_pos = obj.parser_from_array(buff, res);
    if (res)
        cout << "pass" << endl;
    else
        cout << "error:" << error_position(buff, end_pos) << endl;
    return 0;
}
```

输出如下：

```
error:lines:3,colum:29
```