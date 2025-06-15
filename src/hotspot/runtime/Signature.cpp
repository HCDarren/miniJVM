#include "runtime/Signature.hpp"
#include <vector>
#include <tuple>
#include <cassert>
#include "utilities/GlobalDefinitions.hpp"

namespace mini_jvm
{

    std::tuple<int, std::vector<int>> Signature::parseParameterAndReturn(const std::string &methodSignature)
    {
        std::vector<int> parameters;
        int i = 0;
        while (methodSignature[i] != ')')
        {
            switch (methodSignature[i])
            {
            case 'B':
                parameters.push_back(T_BYTE);
                break;
            case 'C':
                parameters.push_back(T_CHAR);
                break;
            case 'D':
                parameters.push_back(T_DOUBLE);
                break;
            case 'F':
                parameters.push_back(T_FLOAT);
                break;
            case 'I':
                parameters.push_back(T_INT);
                break;
            case 'J':
                parameters.push_back(T_LONG);
                break;
            case 'S':
                parameters.push_back(T_SHORT);
                break;
            case 'Z':
                parameters.push_back(T_BOOLEAN);
                break;
            case '[':
            {
                int arrayComponentType = ++i;
                while (methodSignature[arrayComponentType] == '[')
                {
                    arrayComponentType++;
                }
                i = arrayComponentType;
                parameters.push_back(T_ARRAY);
                break;
            }
            case 'L':
            {
                int objectType = i++;
                while (methodSignature[objectType] != ';')
                {
                    objectType++;
                }
                i = objectType;
                parameters.push_back(T_OBJECT);
                break;
            }
            }
            i++;
        }
        while (methodSignature[i])
        {
            switch (methodSignature[i])
            {
            case 'B':
                return std::make_tuple(T_BYTE, parameters);
            case 'C':
                return std::make_tuple(T_CHAR, parameters);
            case 'D':
                return std::make_tuple(T_DOUBLE, parameters);
            case 'F':
                return std::make_tuple(T_FLOAT, parameters);
            case 'I':
                return std::make_tuple(T_INT, parameters);
            case 'J':
                return std::make_tuple(T_LONG, parameters);
            case 'S':
                return std::make_tuple(T_SHORT, parameters);
            case 'Z':
                return std::make_tuple(T_BOOLEAN, parameters);
            case 'V':
                return std::make_tuple(T_VOID, parameters);
            case '[':
            {
                int arrayComponentType = i++;
                while (methodSignature[arrayComponentType] != '[')
                {
                    arrayComponentType++;
                }
                i = arrayComponentType;

                return std::make_tuple(T_OBJECT, parameters);
            }
            case 'L':
            {
                int objectType = i++;
                while (methodSignature[objectType] != ';')
                {
                    objectType++;
                }
                i = objectType;

                return std::make_tuple(T_OBJECT, parameters);
            }
            }
            i++;
        }
        assert(false);
        return {};
    }
} // namespace name