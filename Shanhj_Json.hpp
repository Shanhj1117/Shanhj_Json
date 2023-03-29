#ifndef SHANHJ_JSON_H
#define SHANHJ_JSON_H

#include <cstring>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#define skip_space(array)                   \
    while (*array == ' ' || *array == '\n') \
    array++

namespace Shanhj_Json
{
    using namespace std;
    typedef unsigned long ulong;

    enum value_type
    {
        TYPE_STRING,
        TYPE_INT,
        TYPE_DOUBLE,
        TYPE_BOOLEAN,
        TYPE_OBJECT,
        TYPE_ARRAY
    };

    class JsonArray;
    class JsonObject;

    class JsonObject
    {
    public:
        void insert(const string &key, const string &value);
        void insert(const string &key, const char *value);
        void insert(const string &key, bool value);
        void insert(const string &key, int value);
        void insert(const string &key, int64_t value);
        void insert(const string &key, double value);
        void insert(const string &key, const JsonObject &value);
        void insert(const string &key, const JsonArray &value);

        bool get_string(const string &key, string &result);
        bool get_boolean(const string &key, bool &result);
        bool get_int(const string &key, int64_t &result);
        bool get_double(const string &key, double &result);
        bool get_object(const string &key, JsonObject &result);
        bool get_array(const string &key, JsonArray &result);
        // 清空所有值
        void clear();

        // 默认有4个空格的缩进，如果传入的indent<0则无缩进
        string output_to_string(long indent = 0);
        // 从字符串数组中构造json对象，返回构造结束时的指针位置，result存储构造结果，为false则表示解析出错
        char *parser_from_array(char *array, bool &result);

    private:
        // 记录键值为key的元素在哪个vector中的什么位置
        map<string, pair<value_type, ulong>> position;
        vector<string> v_string;
        vector<bool> v_boolean;
        vector<int64_t> v_int;
        vector<double> v_double;
        vector<JsonObject> v_object;
        vector<JsonArray> v_array;
    };

    class JsonArray
    {
    public:
        void insert(const string &value);
        void insert(const char *value);
        void insert(bool value);
        void insert(int value);
        void insert(int64_t value);
        void insert(double value);
        void insert(const JsonObject &value);
        void insert(const JsonArray &value);

        bool get_string(ulong index, string &result);
        bool get_boolean(ulong index, bool &result);
        bool get_int(ulong index, int64_t &result);
        bool get_double(ulong index, double &result);
        bool get_object(ulong index, JsonObject &result);
        bool get_object(ulong index, JsonArray &result);
        // 清空所有值
        void clear();
        // 默认有4个空格的缩进，如果传入的indent<0则无缩进
        string output_to_string(long indent = 0);
        // 从字符串数组中构造json数组，返回构造结束时的指针位置，result存储构造结果，为false则表示解析出错
        char *parser_from_array(char *array, bool &result);
        // 获取元素个数
        ulong size();
        // 移除第index个元素，移除后index之后的元素下标减1
        bool remove(ulong index);

    private:
        // 记录下标为index的元素是什么类型，以及在vector中的下标
        list<pair<value_type, ulong>> position;
        vector<string> v_string;
        vector<bool> v_boolean;
        vector<int64_t> v_int;
        vector<double> v_double;
        vector<JsonObject> v_object;
        vector<JsonArray> v_array;
    };
}

void Shanhj_Json::JsonObject::insert(const string &key, const string &value)
{
    // 已经存在相同键值的变量，并且是同一类型的
    if (position.count(key) && position[key].first == TYPE_STRING)
    {
        v_string[position[key].second] = value;
    }
    else
    { // 不存在相同键值的变量，或者存在相同键值但类型不同的变量，插入新的值
        // 旧的键值对仍然留在vector里，但不再使用
        position[key] = {TYPE_STRING, v_string.size()};
        v_string.push_back(value);
    }
}

void Shanhj_Json::JsonObject::insert(const string &key, const char *value)
{
    // 已经存在相同键值的变量，并且是同一类型的
    if (position.count(key) && position[key].first == TYPE_STRING)
    {
        v_string[position[key].second] = value;
    }
    else
    { // 不存在相同键值的变量，或者存在相同键值但类型不同的变量，插入新的值
        // 旧的键值对仍然留在vector里，但不再使用
        position[key] = {TYPE_STRING, v_string.size()};
        v_string.push_back(value);
    }
}

void Shanhj_Json::JsonObject::insert(const string &key, bool value)
{
    // 已经存在相同键值的变量，并且是同一类型的
    if (position.count(key) && position[key].first == TYPE_BOOLEAN)
    {
        v_boolean[position[key].second] = value;
    }
    else
    { // 不存在相同键值的变量，或者存在相同键值但类型不同的变量，插入新的值
        // 旧的键值对仍然留在vector里，但不再使用
        position[key] = {TYPE_BOOLEAN, v_boolean.size()};
        v_boolean.push_back(value);
    }
}

void Shanhj_Json::JsonObject::insert(const string &key, int value)
{
    // 已经存在相同键值的变量，并且是同一类型的
    if (position.count(key) && position[key].first == TYPE_INT)
    {
        v_int[position[key].second] = value;
    }
    else
    { // 不存在相同键值的变量，或者存在相同键值但类型不同的变量，插入新的值
        // 旧的键值对仍然留在vector里，但不再使用
        position[key] = {TYPE_INT, v_int.size()};
        v_int.push_back(value);
    }
}

void Shanhj_Json::JsonObject::insert(const string &key, int64_t value)
{
    // 已经存在相同键值的变量，并且是同一类型的
    if (position.count(key) && position[key].first == TYPE_INT)
    {
        v_int[position[key].second] = value;
    }
    else
    { // 不存在相同键值的变量，或者存在相同键值但类型不同的变量，插入新的值
        // 旧的键值对仍然留在vector里，但不再使用
        position[key] = {TYPE_INT, v_int.size()};
        v_int.push_back(value);
    }
}

void Shanhj_Json::JsonObject::insert(const string &key, double value)
{
    // 已经存在相同键值的变量，并且是同一类型的
    if (position.count(key) && position[key].first == TYPE_DOUBLE)
    {
        v_double[position[key].second] = value;
    }
    else
    { // 不存在相同键值的变量，或者存在相同键值但类型不同的变量，插入新的值
        // 旧的键值对仍然留在vector里，但不再使用
        position[key] = {TYPE_DOUBLE, v_double.size()};
        v_double.push_back(value);
    }
}

void Shanhj_Json::JsonObject::insert(const string &key, const JsonObject &value)
{
    // 已经存在相同键值的变量，并且是同一类型的
    if (position.count(key) && position[key].first == TYPE_OBJECT)
    {
        v_object[position[key].second] = value;
    }
    else
    { // 不存在相同键值的变量，或者存在相同键值但类型不同的变量，插入新的值
        // 旧的键值对仍然留在vector里，但不再使用
        position[key] = {TYPE_OBJECT, v_object.size()};
        v_object.push_back(value);
    }
}

void Shanhj_Json::JsonObject::insert(const string &key, const JsonArray &value)
{
    // 已经存在相同键值的变量，并且是同一类型的
    if (position.count(key) && position[key].first == TYPE_ARRAY)
    {
        v_array[position[key].second] = value;
    }
    else
    { // 不存在相同键值的变量，或者存在相同键值但类型不同的变量，插入新的值
        // 旧的键值对仍然留在vector里，但不再使用
        position[key] = {TYPE_ARRAY, v_array.size()};
        v_array.push_back(value);
    }
}

bool Shanhj_Json::JsonObject::get_string(const string &key, string &result)
{
    if (!position.count(key)) return false; // 不存在该键值
    auto pos = position[key];
    if (pos.first != TYPE_STRING) return false; // 不存在该类型的键值对
    result = v_string[pos.second];
    return true;
}

bool Shanhj_Json::JsonObject::get_boolean(const string &key, bool &result)
{
    if (!position.count(key)) return false; // 不存在该键值
    auto pos = position[key];
    if (pos.first != TYPE_BOOLEAN) return false; // 不存在该类型的键值对
    result = v_boolean[pos.second];
    return true;
}

bool Shanhj_Json::JsonObject::get_int(const string &key, int64_t &result)
{
    if (!position.count(key)) return false; // 不存在该键值
    auto pos = position[key];
    if (pos.first != TYPE_INT) return false; // 不存在该类型的键值对
    result = v_int[pos.second];
    return true;
}

bool Shanhj_Json::JsonObject::get_double(const string &key, double &result)
{
    if (!position.count(key)) return false; // 不存在该键值
    auto pos = position[key];
    if (pos.first != TYPE_DOUBLE) return false; // 不存在该类型的键值对
    result = v_double[pos.second];
    return true;
}

bool Shanhj_Json::JsonObject::get_object(const string &key, JsonObject &result)
{
    if (!position.count(key)) return false; // 不存在该键值
    auto pos = position[key];
    if (pos.first != TYPE_OBJECT) return false; // 不存在该类型的键值对
    result = v_object[pos.second];
    return true;
}

bool Shanhj_Json::JsonObject::get_array(const string &key, JsonArray &result)
{
    if (!position.count(key)) return false; // 不存在该键值
    auto pos = position[key];
    if (pos.first != TYPE_ARRAY) return false; // 不存在该类型的键值对
    result = v_array[pos.second];
    return true;
}

void Shanhj_Json::JsonObject::clear()
{
    position.clear();
    v_array.clear();
    v_boolean.clear();
    v_double.clear();
    v_int.clear();
    v_object.clear();
    v_string.clear();
}

std::string Shanhj_Json::JsonObject::output_to_string(long indent)
{
    string result;
    result += "{";
    if (indent >= 0) result += '\n';
    bool flag = 0;
    for (auto entry : position)
    {
        if (!flag)
            flag = 1;
        else
        {
            result += ',';
            if (indent >= 0) result += '\n';
        }
        if (indent >= 0)
        {
            for (int i = 0; i < indent + 4; i++) // 缩进
                result += ' ';
        }
        result += '\"';
        result += entry.first;
        result += "\":";
        if (indent >= 0) result += ' ';
        switch (entry.second.first)
        {
        case TYPE_STRING:
            result += '\"';
            result += v_string[entry.second.second];
            result += '\"';
            break;
        case TYPE_BOOLEAN:
            result += v_boolean[entry.second.second] ? "true" : "false";
            break;
        case TYPE_INT:
            result += to_string(v_int[entry.second.second]);
            break;
        case TYPE_DOUBLE:
            result += to_string(v_double[entry.second.second]);
            break;
        case TYPE_OBJECT:
            result += v_object[entry.second.second].output_to_string(indent >= 0 ? indent + 4 : -1);
            break;
        case TYPE_ARRAY:
            result += v_array[entry.second.second].output_to_string(indent >= 0 ? indent + 4 : -1);
            break;
        default:
            break;
        }
    }
    if (indent >= 0)
    {
        result += '\n';
        for (int i = 0; i < indent; i++)
            result += ' ';
    }
    result += "}";
    return result;
}

char *Shanhj_Json::JsonObject::parser_from_array(char *array, bool &result)
{
    clear();
    skip_space(array);
    if (*array != '{')
    {
        result = false;
        return array;
    }
    array++;
    while (1)
    {
        skip_space(array);
        if (*array != '\"')
        {
            result = false;
            return array;
        }
        array++;
        string key;
        while (*array != '\"')
        {
            key += *array;
            array++;
        }
        array++;
        skip_space(array);
        if (*array != ':')
        {
            result = false;
            return array;
        }
        array++;
        skip_space(array);
        if (*array == '\"') // 字符串类型
        {
            array++;
            string str;
            while (*array != '\"')
            {
                str += *array;
                array++;
            }
            insert(key, str);
            array++;
        }
        else if (*array == 't') // 布尔类型，true
        {
            char tmp[5];
            memcpy(tmp, array, 4);
            tmp[4] = 0;
            if (strcmp(tmp, "true") == 0)
                insert(key, true);
            else
            {
                result = false;
                return array;
            }
            array += 4;
        }
        else if (*array == 'f') // 布尔类型，false
        {
            char tmp[6];
            memcpy(tmp, array, 5);
            tmp[5] = 0;
            if (strcmp(tmp, "false") == 0)
                insert(key, false);
            else
            {
                result = false;
                return array;
            }
            array += 5;
        }
        else if (*array >= '0' && *array <= '9') // 数字类型
        {
            string num;
            if (*array == 0) // 0开头
            {
                array++;
                if (*array == ' ')
                    insert(key, 0);
                else if (*array == '.')
                {
                    num = "0.";
                    array++;
                    bool flag = 0;
                    while (*array >= '0' && *array <= '9')
                    {
                        flag = 1;
                        num += *array;
                        array++;
                    }
                    if (!flag)
                    {
                        result = false;
                        return array;
                    }
                    insert(key, stod(num));
                }
            }
            else
            {
                ulong dot_cnt = 0; // 统计小数点个数
                num += *array;
                array++;
                while ((*array >= '0' && *array <= '9') || *array == '.')
                {
                    if (*array == '.') dot_cnt++;
                    num += *array;
                    array++;
                }
                if (dot_cnt > 1 || num.back() == '.')
                {
                    result = false;
                    return array;
                }
                if (dot_cnt == 1) // 有小数点，为浮点数
                    insert(key, stod(num));
                else
                    insert(key, stoll(num));
            }
        }
        else if (*array == '{') // json对象
        {
            JsonObject jsobj;
            bool flag;
            array = jsobj.parser_from_array(array, flag);
            if (flag)
                insert(key, jsobj);
            else
            {
                result = false;
                return array;
            }
        }
        else if (*array == '[') // json数组
        {
            JsonArray jsary;
            bool flag;
            array = jsary.parser_from_array(array, flag);
            if (flag)
                insert(key, jsary);
            else
            {
                result = false;
                return array;
            }
        }
        else // 格式错误
        {
            result = false;
            return array;
        }
        skip_space(array);
        if (*array == '}')
            break;
        if (*array == ',')
        {
            array++;
            continue;
        }
        {
            result = false;
            return array;
        }
    }
    array++;
    result = true;
    return array;
}

void Shanhj_Json::JsonArray::insert(const string &value)
{
    position.push_back({TYPE_STRING, v_string.size()});
    v_string.push_back(value);
}

void Shanhj_Json::JsonArray::insert(const char *value)
{
    position.push_back({TYPE_STRING, v_string.size()});
    v_string.push_back(value);
}

void Shanhj_Json::JsonArray::insert(bool value)
{
    position.push_back({TYPE_BOOLEAN, v_boolean.size()});
    v_boolean.push_back(value);
}
void Shanhj_Json::JsonArray::insert(int value)
{
    position.push_back({TYPE_INT, v_int.size()});
    v_int.push_back(value);
}
void Shanhj_Json::JsonArray::insert(int64_t value)
{
    position.push_back({TYPE_INT, v_int.size()});
    v_int.push_back(value);
}
void Shanhj_Json::JsonArray::insert(double value)
{
    position.push_back({TYPE_DOUBLE, v_double.size()});
    v_double.push_back(value);
}
void Shanhj_Json::JsonArray::insert(const JsonObject &value)
{
    position.push_back({TYPE_OBJECT, v_object.size()});
    v_object.push_back(value);
}

void Shanhj_Json::JsonArray::insert(const JsonArray &value)
{
    position.push_back({TYPE_ARRAY, v_array.size()});
    v_array.push_back(value);
}

bool Shanhj_Json::JsonArray::get_string(ulong index, string &result)
{
    if (index >= position.size()) return false;
    auto iter = position.begin();
    while (index--)
        iter++;
    if (iter->first != TYPE_STRING) return false;
    result = v_string[iter->second];
    return true;
}
bool Shanhj_Json::JsonArray::get_boolean(ulong index, bool &result)
{
    if (index >= position.size()) return false;
    auto iter = position.begin();
    while (index--)
        iter++;
    if (iter->first != TYPE_BOOLEAN) return false;
    result = v_boolean[iter->second];
    return true;
}
bool Shanhj_Json::JsonArray::get_int(ulong index, int64_t &result)
{
    if (index >= position.size()) return false;
    auto iter = position.begin();
    while (index--)
        iter++;
    if (iter->first != TYPE_INT) return false;
    result = v_int[iter->second];
    return true;
}
bool Shanhj_Json::JsonArray::get_double(ulong index, double &result)
{
    if (index >= position.size()) return false;
    auto iter = position.begin();
    while (index--)
        iter++;
    if (iter->first != TYPE_DOUBLE) return false;
    result = v_double[iter->second];
    return true;
}
bool Shanhj_Json::JsonArray::get_object(ulong index, JsonObject &result)
{
    if (index >= position.size()) return false;
    auto iter = position.begin();
    while (index--)
        iter++;
    if (iter->first != TYPE_OBJECT) return false;
    result = v_object[iter->second];
    return true;
}

bool Shanhj_Json::JsonArray::get_object(ulong index, JsonArray &result)
{
    if (index >= position.size()) return false;
    auto iter = position.begin();
    while (index--)
        iter++;
    if (iter->first != TYPE_ARRAY) return false;
    result = v_array[iter->second];
    return true;
}

std::string Shanhj_Json::JsonArray::output_to_string(long indent)
{
    string result;
    result += '[';
    if (indent >= 0)
    {
        result += '\n';
        for (int i = 0; i < indent + 4; i++) // 缩进
            result += ' ';
    }
    bool flag = 0;
    for (auto entry : position)
    {
        if (!flag)
            flag = 1;
        else
        {
            result += ',';
            if (indent >= 0)
            {
                result += '\n';
                for (int i = 0; i < indent + 4; i++) // 缩进
                    result += ' ';
            }
        }
        switch (entry.first)
        {
        case TYPE_STRING:
            result += '\"';
            result += v_string[entry.second];
            result += '\"';
            break;
        case TYPE_BOOLEAN:
            result += v_boolean[entry.second] ? "true" : "false";
            break;
        case TYPE_INT:
            result += to_string(v_int[entry.second]);
            break;
        case TYPE_DOUBLE:
            result += to_string(v_double[entry.second]);
            break;
        case TYPE_OBJECT:
            result += v_object[entry.second].output_to_string(indent + 4);
            break;
        case TYPE_ARRAY:
            result += v_array[entry.second].output_to_string(indent + 4);
            break;
        default:
            break;
        }
    }
    if (indent >= 0)
    {
        result += '\n';
        for (int i = 0; i < indent; i++)
            result += ' ';
    }
    result += "]";
    return result;
}

void Shanhj_Json::JsonArray::clear()
{
    position.clear();
    v_array.clear();
    v_boolean.clear();
    v_double.clear();
    v_int.clear();
    v_object.clear();
    v_string.clear();
}

char *Shanhj_Json::JsonArray::parser_from_array(char *array, bool &result)
{
    clear();
    skip_space(array);
    if (*array != '[')
    {
        result = false;
        return array;
    }
    array++;
    while (1)
    {
        skip_space(array);
        if (*array == '\"') // 字符串类型
        {
            array++;
            string str;
            while (*array != '\"')
            {
                str += *array;
                array++;
            }
            insert(str);
            array++;
        }
        else if (*array == 't') // 布尔类型，true
        {
            char tmp[5];
            memcpy(tmp, array, 4);
            tmp[4] = 0;
            if (strcmp(tmp, "true") == 0)
                insert(true);
            else
            {
                result = false;
                return array;
            }
            array += 4;
        }
        else if (*array == 'f') // 布尔类型，false
        {
            char tmp[6];
            memcpy(tmp, array, 5);
            tmp[5] = 0;
            if (strcmp(tmp, "false") == 0)
                insert(false);
            else
            {
                result = false;
                return array;
            }
            array += 5;
        }
        else if (*array >= '0' && *array <= '9') // 数字类型
        {
            string num;
            if (*array == 0) // 0开头
            {
                array++;
                if (*array == ' ')
                    insert(0);
                else if (*array == '.')
                {
                    num = "0.";
                    array++;
                    bool flag = 0;
                    while (*array >= '0' && *array <= '9')
                    {
                        flag = 1;
                        num += *array;
                        array++;
                    }
                    if (!flag)
                    {
                        result = false;
                        return array;
                    }
                    insert(stod(num));
                }
            }
            else
            {
                ulong dot_cnt = 0; // 统计小数点个数
                num += *array;
                array++;
                while ((*array >= '0' && *array <= '9') || *array == '.')
                {
                    if (*array == '.') dot_cnt++;
                    num += *array;
                    array++;
                }
                if (dot_cnt > 1 || num.back() == '.')
                {
                    result = false;
                    return array;
                }
                if (dot_cnt == 1) // 有小数点，为浮点数
                    insert(stod(num));
                else
                    insert(stoll(num));
            }
        }
        else if (*array == '{') // json对象
        {
            JsonObject jsobj;
            bool flag;
            array = jsobj.parser_from_array(array, flag);
            if (flag)
                insert(jsobj);
            else
            {
                result = false;
                return array;
            }
        }
        else if (*array == '[') // json数组
        {
            JsonArray jsary;
            bool flag;
            array = jsary.parser_from_array(array, flag);
            if (flag)
                insert(jsary);
            else
            {
                result = false;
                return array;
            }
        }
        else // 格式错误
        {
            result = false;
            return array;
        }
        skip_space(array);
        if (*array == ']')
            break;
        if (*array == ',')
        {
            array++;
            continue;
        }
        result = false;
        return array;
    }
    array++;
    result = true;
    return array;
}

Shanhj_Json::ulong Shanhj_Json::JsonArray::size()
{
    return position.size();
}

bool Shanhj_Json::JsonArray::remove(ulong index)
{
    if (index >= position.size()) return false;
    auto iter = position.begin();
    while (index--)
        iter++;
    position.erase(iter);
    return true;
}

#undef skip_space

#endif