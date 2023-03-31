#ifndef SHANHJ_JSON_H
#define SHANHJ_JSON_H

#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace Shanhj_Json
{
#define parser_array_check(array_begin, array_end) \
    if (array_begin >= array_end)                  \
    {                                              \
        result = false;                            \
        return array_begin;                        \
    }

    using namespace std;

    typedef unsigned long ulong;

    class JsonArray;
    class JsonObject;

    enum value_type
    {
        TYPE_STRING,
        TYPE_INT,
        TYPE_DOUBLE,
        TYPE_BOOLEAN,
        TYPE_OBJECT,
        TYPE_ARRAY,
        TYPE_NULL
    };

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
        char *parser_from_array(char *array_begin, char *array_end, bool &result);

    private:
        // 记录键值为key的元素在哪个vector中的什么位置
        // 如果是bool类型，则pair的第二个值记录true(1)或false(0)
        // 如果是null，则pair的第二个值忽略
        map<string, pair<value_type, ulong>> position;
        vector<string> v_string;
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
        bool get_array(ulong index, JsonArray &result);
        // 清空所有值
        void clear();
        // 默认有4个空格的缩进，如果传入的indent<0则无缩进
        string output_to_string(long indent = 0);
        // 从字符串数组中构造json数组，返回构造结束时的指针位置，result存储构造结果，为false则表示解析出错
        char *parser_from_array(char *array_begin, char *array_end, bool &result);
        // 获取元素个数
        ulong size();
        // 移除第index个元素，移除后index之后的元素下标减1
        bool remove(ulong index);

    private:
        // 记录下标为index的元素是什么类型，以及在vector中的下标
        // 如果是bool类型，则第二个值记录true(1)或false(0)
        // 如果是null，则第二个值忽略
        list<pair<value_type, ulong>> position;
        vector<string> v_string;
        vector<int64_t> v_int;
        vector<double> v_double;
        vector<JsonObject> v_object;
        vector<JsonArray> v_array;
    };
    // 跳过空格和换行符，如果array到达array_end则返回false
    inline bool skip_space(char *&array, char *array_end);

    // 通过第一个字节的内容返回非ascii字符的utf-8编码的长度
    // 如果是ascii编码，则返回0
    inline uint8_t get_utf8_len(char first_c);

    // 返回出错位置的行和列
    // begin为json序列开始的位置
    string error_position(char *begin, char *error_pos);

    // 从带有转义字符的文本中获取一个二进制字符串，遇到 " 停止，如果合法返回true
    // 自动处理转义字符，结束后array将指向 " 的后一个位置
    bool get_binary_from_text(char *&array, char *array_end, string &result);

    // 将二进制字符串转成文本，特殊字符进行转义
    // 如果转换的内容不是utf-8格式，返回空字符串
    string binary_to_text(const string &binary);
}

bool Shanhj_Json::skip_space(char *&array, char *array_end)
{
    while (array < array_end && (*array == ' ' || *array == '\n'))
        array++;
    return array < array_end;
}

uint8_t Shanhj_Json::get_utf8_len(char first_c)
{
    uint8_t len = 0;
    while (first_c & 0x80)
    {
        len++;
        first_c <<= 1;
    }
    return len;
}

std::string Shanhj_Json::error_position(char *begin, char *error_pos)
{
    ulong line = 1, column = 0;
    while (begin <= error_pos)
    {
        auto len = get_utf8_len(*begin);
        if (len == -1)
        {
            return "not utf-8 text";
        }
        else if (len == 0) // ASCII码
        {
            if (*begin == '\n')
            {
                line++;
                column = 0;
            }
            else
                column++;
            begin++;
        }
        else // 非ASCII码
        {
            column++;
            begin += len;
        }
    }
    return "lines:" + to_string(line) + ",colum:" + to_string(column);
}

bool Shanhj_Json::get_binary_from_text(char *&array, char *array_end, string &result)
{
    if (array >= array_end) return false;
    while (*array != '\"')
    {
        auto len = get_utf8_len(*array);
        if (len == -1)
            return false;
        else if (len == 0) // ASCII码
        {
            if (*array == '\\') // 转义字符
            {
                array++;
                if (array >= array_end) return false;
                switch (*array)
                {
                case 'n':
                    result += '\n';
                    break;
                case '\"':
                    result += '\"';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case 'b':
                    result += '\b';
                    break;
                case 'f':
                    result += '\f';
                    break;
                case 't':
                    result += '\t';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case '/':
                    result += '/';
                    break;
                default: // 不合法的转义字符
                    return false;
                    break;
                }
            }
            else
                result += *array;
        }
        else // 非ASCII码
        {
            while (array < array_end && --len)
            {
                result += *array;
                array++;
            }
            result += *array;
        }
        array++;
        if (array >= array_end) return false;
    }
    array++;
    return true;
}

std::string Shanhj_Json::binary_to_text(const string &binary)
{
    string result;
    for (int i = 0; i < binary.size(); i++)
    {
        auto len = get_utf8_len(binary[i]);
        if (len == 0) // ASCII字符
        {
            switch (binary[i])
            {
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\\':
                result += "\\\\";
                break;
            case '/':
                result += "\\/";
                break;
            case '\"':
                result += "\\\"";
                break;
            default:
                result += binary[i];
                break;
            }
        }
        else // 非ASCII字符
        {
            while (--len)
            {
                result += binary[i];
                i++;
            }
            result += binary[i];
        }
    }
    return result;
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
        position[key].second = value;
    else
        position[key] = {TYPE_BOOLEAN, value};
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
    result = pos.second;
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
    v_double.clear();
    v_int.clear();
    v_object.clear();
    v_string.clear();
}

std::string Shanhj_Json::JsonObject::output_to_string(long indent)
{
    string result;
    result += "{";
    if (position.size())
    {
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
            result += binary_to_text(entry.first);
            result += "\":";
            if (indent >= 0) result += ' ';
            switch (entry.second.first)
            {
            case TYPE_STRING:
                result += '\"';
                result += binary_to_text(v_string[entry.second.second]);
                result += '\"';
                break;
            case TYPE_BOOLEAN:
                result += entry.second.second ? "true" : "false";
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
            case TYPE_NULL:
                result += "null";
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
    }
    result += "}";
    return result;
}

char *Shanhj_Json::JsonObject::parser_from_array(char *array_begin, char *array_end, bool &result)
{
    parser_array_check(array_begin, array_end);
    ulong value_order = 0;
    clear();
    if (!skip_space(array_begin, array_end))
    {
        result = false;
        return array_begin;
    }
    if (*array_begin != '{')
    {
        result = false;
        return array_begin;
    }
    array_begin++;
    parser_array_check(array_begin, array_end);
    while (array_begin < array_end)
    {
        if (!skip_space(array_begin, array_end))
        {
            result = false;
            return array_begin;
        }
        if (*array_begin != '\"')
        {
            if (*array_begin == '}' && value_order == 0) break;
            result = false;
            return array_begin;
        }
        array_begin++;
        parser_array_check(array_begin, array_end);
        string key; // 获取键值
        if (!get_binary_from_text(array_begin, array_end, key))
        {
            result = false;
            return array_begin;
        }
        if (!skip_space(array_begin, array_end))
        {
            result = false;
            return array_begin;
        }
        if (*array_begin != ':')
        {
            result = false;
            return array_begin;
        }
        array_begin++;
        parser_array_check(array_begin, array_end);
        if (!skip_space(array_begin, array_end))
        {
            result = false;
            return array_begin;
        }
        // 获取值
        if (*array_begin == '\"') // 字符串类型
        {
            array_begin++;
            parser_array_check(array_begin, array_end);
            string str;
            if (!get_binary_from_text(array_begin, array_end, str))
            {
                result = false;
                return array_begin;
            }
            insert(key, str);
        }
        else if (*array_begin == 't') // 布尔类型，true
        {
            char tmp[5];
            memcpy(tmp, array_begin, 4);
            tmp[4] = 0;
            if (strcmp(tmp, "true") == 0)
                insert(key, true);
            else
            {
                result = false;
                return array_begin;
            }
            array_begin += 4;
            parser_array_check(array_begin, array_end);
        }
        else if (*array_begin == 'f') // 布尔类型，false
        {
            char tmp[6];
            memcpy(tmp, array_begin, 5);
            tmp[5] = 0;
            if (strcmp(tmp, "false") == 0)
                insert(key, false);
            else
            {
                result = false;
                return array_begin;
            }
            array_begin += 5;
            parser_array_check(array_begin, array_end);
        }
        else if ((*array_begin >= '0' && *array_begin <= '9') || *array_begin == '-') // 数字类型
        {
            string num;
            if (*array_begin == '-')
            {
                num += '-';
                array_begin++;
                parser_array_check(array_begin, array_end);
            }
            if (*array_begin == '0') // 0开头
            {
                array_begin++;
                parser_array_check(array_begin, array_end);
                if (*array_begin != '.' && (*array_begin < '0' || *array_begin > '9'))
                    insert(key, 0);
                else if (*array_begin == '.')
                {
                    num = "0.";
                    array_begin++;
                    parser_array_check(array_begin, array_end);
                    bool flag = 0;
                    while (*array_begin >= '0' && *array_begin <= '9')
                    {
                        flag = 1;
                        num += *array_begin;
                        array_begin++;
                        parser_array_check(array_begin, array_end);
                    }
                    if (!flag)
                    {
                        result = false;
                        return array_begin;
                    }
                    insert(key, stod(num));
                }
            }
            else if (*array_begin >= '1' && *array_begin <= '9') // 1-9开头
            {
                ulong dot_cnt = 0; // 统计小数点个数
                num += *array_begin;
                array_begin++;
                parser_array_check(array_begin, array_end);
                while ((*array_begin >= '0' && *array_begin <= '9') || *array_begin == '.')
                {
                    if (*array_begin == '.') dot_cnt++;
                    num += *array_begin;
                    array_begin++;
                    parser_array_check(array_begin, array_end);
                }
                if (dot_cnt > 1 || num.back() == '.')
                {
                    result = false;
                    return array_begin;
                }
                if (dot_cnt == 1) // 有小数点，为浮点数
                    insert(key, stod(num));
                else
                    insert(key, stoll(num));
            }
            else // 非数字开头，错误
            {
                result = false;
                return array_begin;
            }
        }
        else if (*array_begin == '{') // json对象
        {
            JsonObject jsobj;
            bool flag;
            array_begin = jsobj.parser_from_array(array_begin, array_end, flag);
            parser_array_check(array_begin, array_end);
            if (flag)
                insert(key, jsobj);
            else
            {
                result = false;
                return array_begin;
            }
        }
        else if (*array_begin == '[') // json数组
        {
            JsonArray jsary;
            bool flag;
            array_begin = jsary.parser_from_array(array_begin, array_end, flag);
            parser_array_check(array_begin, array_end);
            if (flag)
                insert(key, jsary);
            else
            {
                result = false;
                return array_begin;
            }
        }
        else if (*array_begin == 'n') // null
        {
            char tmp[5];
            memcpy(tmp, array_begin, 4);
            tmp[4] = 0;
            if (strcmp(tmp, "null") == 0)
            {
                position[key] = {TYPE_NULL, 0};
            }
            else
            {
                result = false;
                return array_begin;
            }
            array_begin += 4;
            parser_array_check(array_begin, array_end);
        }
        else // 格式错误
        {
            result = false;
            return array_begin;
        }
        value_order++;
        if (!skip_space(array_begin, array_end))
        {
            result = false;
            return array_begin;
        }
        if (*array_begin == '}')
            break;
        if (*array_begin == ',')
        {
            array_begin++;
            parser_array_check(array_begin, array_end);
            continue;
        }
        {
            result = false;
            return array_begin;
        }
    }
    array_begin++;
    result = true;
    return array_begin;
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
    position.push_back({TYPE_BOOLEAN, value});
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
    result = iter->second;
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

bool Shanhj_Json::JsonArray::get_array(ulong index, JsonArray &result)
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
    if (position.size())
    {
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
                result += binary_to_text(v_string[entry.second]);
                result += '\"';
                break;
            case TYPE_BOOLEAN:
                result += entry.second ? "true" : "false";
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
            case TYPE_NULL:
                result += "null";
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
    }
    result += "]";
    return result;
}

void Shanhj_Json::JsonArray::clear()
{
    position.clear();
    v_array.clear();
    v_double.clear();
    v_int.clear();
    v_object.clear();
    v_string.clear();
}

char *Shanhj_Json::JsonArray::parser_from_array(char *array_begin, char *array_end, bool &result)
{
    if (array_begin >= array_end)
    {
        result = false;
        return array_begin;
    }
    ulong value_order = 0;
    clear();
    if (!skip_space(array_begin, array_end))
    {
        result = false;
        return array_begin;
    }
    if (*array_begin != '[')
    {
        result = false;
        return array_begin;
    }
    array_begin++;
    parser_array_check(array_begin, array_end);
    while (array_begin < array_end)
    {
        if (!skip_space(array_begin, array_end))
        {
            result = false;
            return array_begin;
        }
        if (*array_begin == '\"') // 字符串类型
        {
            array_begin++;
            parser_array_check(array_begin, array_end);
            string str;
            if (!get_binary_from_text(array_begin, array_end, str))
            {
                result = false;
                return array_begin;
            }
            insert(str);
        }
        else if (*array_begin == 't') // 布尔类型，true
        {
            char tmp[5];
            memcpy(tmp, array_begin, 4);
            tmp[4] = 0;
            if (strcmp(tmp, "true") == 0)
                insert(true);
            else
            {
                result = false;
                return array_begin;
            }
            array_begin += 4;
            parser_array_check(array_begin, array_end);
        }
        else if (*array_begin == 'f') // 布尔类型，false
        {
            char tmp[6];
            memcpy(tmp, array_begin, 5);
            tmp[5] = 0;
            if (strcmp(tmp, "false") == 0)
                insert(false);
            else
            {
                result = false;
                return array_begin;
            }
            array_begin += 5;
            parser_array_check(array_begin, array_end);
        }
        else if ((*array_begin >= '0' && *array_begin <= '9') || *array_begin == '-') // 数字类型
        {
            string num;
            if (*array_begin == '-')
            {
                num += '-';
                array_begin++;
                parser_array_check(array_begin, array_end);
            }
            if (*array_begin == '0') // 0开头
            {
                array_begin++;
                parser_array_check(array_begin, array_end);
                if (*array_begin != '.' && (*array_begin < '0' || *array_begin > '9'))
                    insert(0);
                else if (*array_begin == '.')
                {
                    num = "0.";
                    array_begin++;
                    parser_array_check(array_begin, array_end);
                    bool flag = 0;
                    while (*array_begin >= '0' && *array_begin <= '9')
                    {
                        flag = 1;
                        num += *array_begin;
                        array_begin++;
                        parser_array_check(array_begin, array_end);
                    }
                    if (!flag)
                    {
                        result = false;
                        return array_begin;
                    }
                    insert(stod(num));
                }
            }
            else if (*array_begin >= '1' && *array_begin <= '9')
            {
                ulong dot_cnt = 0; // 统计小数点个数
                num += *array_begin;
                array_begin++;
                parser_array_check(array_begin, array_end);
                while ((*array_begin >= '0' && *array_begin <= '9') || *array_begin == '.')
                {
                    if (*array_begin == '.') dot_cnt++;
                    num += *array_begin;
                    array_begin++;
                    parser_array_check(array_begin, array_end);
                }
                if (dot_cnt > 1 || num.back() == '.')
                {
                    result = false;
                    return array_begin;
                }
                if (dot_cnt == 1) // 有小数点，为浮点数
                    insert(stod(num));
                else
                    insert(stoll(num));
            }
            else
            {
                result = false;
                return array_begin;
            }
        }
        else if (*array_begin == '{') // json对象
        {
            JsonObject jsobj;
            bool flag;
            array_begin = jsobj.parser_from_array(array_begin, array_end, flag);
            parser_array_check(array_begin, array_end);
            if (flag)
                insert(jsobj);
            else
            {
                result = false;
                return array_begin;
            }
        }
        else if (*array_begin == '[') // json数组
        {
            JsonArray jsary;
            bool flag;
            array_begin = jsary.parser_from_array(array_begin, array_end, flag);
            parser_array_check(array_begin, array_end);
            if (flag)
                insert(jsary);
            else
            {
                result = false;
                return array_begin;
            }
        }
        else if (*array_begin == 'n') // null
        {
            char tmp[5];
            memcpy(tmp, array_begin, 4);
            tmp[4] = 0;
            if (strcmp(tmp, "null") == 0)
            {
                position.push_back({TYPE_NULL, 0});
            }
            else
            {
                result = false;
                return array_begin;
            }
            array_begin += 4;
            parser_array_check(array_begin, array_end);
        }
        else if (*array_begin == ']' && value_order == 0)
            break;
        else // 格式错误
        {
            result = false;
            return array_begin;
        }
        if (!skip_space(array_begin, array_end))
        {
            result = false;
            return array_begin;
        }
        value_order++;
        if (*array_begin == ']')
            break;
        if (*array_begin == ',')
        {
            array_begin++;
            parser_array_check(array_begin, array_end);
            continue;
        }
        result = false;
        return array_begin;
    }
    array_begin++;
    result = true;
    return array_begin;
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

#endif