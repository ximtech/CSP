# CSP

[![tests](https://github.com/ximtech/CSP/actions/workflows/cmake-ci.yml/badge.svg?branch=main)](https://github.com/ximtech/CSP/actions/workflows/cmake-ci.yml)
[![codecov](https://codecov.io/gh/ximtech/CSP/branch/main/graph/badge.svg?token=POFfaMxu0z)](https://codecov.io/gh/ximtech/CSP)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/97090da742044ae9ac1efa7e13a329c8)](https://app.codacy.com/gh/ximtech/CSP/dashboard)

CSP (C Server Pages) - server-side view rendering technology written on C. 
It is designed to be familiar for users of technologies such as JSP, GSP, Freemarker etc. and to be usable in embedded applications

### Dependencies
- [FileUtils](https://github.com/ximtech/FileUtils)

### Features

- Pure C implementation
- Groovy like scripting language
- Supports variables, loops, conditions, includes, collections, ternary and elvis operators...
- Simple and user-friendly design 


### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME CSP
        GITHUB_REPOSITORY ximtech/CSP
        GIT_TAG origin/main)

target_link_libraries(${PROJECT_NAME} CSP)
```

```cmake
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
# For Clion STM32 plugin generated Cmake use 
target_link_libraries(${PROJECT_NAME}.elf CSP)
```

### Header includes

```c
#include "CSPTemplate.h"
#include "CSPRenderer.h"
```

### CSP Basics

CSP supports the usage of `${}` for parameter binding and evaluation:

#### First create file with `.csp` extension and add 

```html
<!--index.csp-->
<html>
  <body>
    Hello ${param}
  </body>
</html>
```

#### Then create template and renderer

```c
CspTemplate *cspTemplate = newCspTemplate("index.csp"); // compile template file
CspObjectMap *params = newCspParamObjMap(8);    // create map to store parameters
cspAddStrToMap(params, "param", "World!!!");  // add parameter name and value

CspRenderer *renderer = NEW_CSP_RENDERER(cspTemplate, params);  // create renderer from template and add parameters
CspTableString *result = renderCspTemplate(renderer);
if (!isCspTemplateOk(cspTemplate)) {    // check that no errors
    printf("%s\n", cspTemplateErrorMessage(cspTemplate));
    return 1;
}

printf("%s", result->value);

// Release resources
deleteCspRenderer(renderer);
deleteCspTemplate(cspTemplate);

```
#### Output:
```html
<!--index.csp-->
<html>
  <body>
    Hello World!!!
  </body>
</html>
```

#### Array as parameter
```c
CspObjectMap *params = newCspParamObjMap(8);    // create map to store parameters
CspObjectArray *array = newCspParamObjArray(4); // create array object
cspAddStrToArray(array, "Action");
cspAddIntToArray(array, 123);
cspAddFloatToArray(array, 1.1f);
cspAddVecToMap(array, params, "array"); // add array to root params and assign name
```

#### Map as parameter
```c
CspObjectMap *params = newCspParamObjMap(8);    // create map to store parameters
CspObjectMap *innerObj = newCspParamObjMap(8);  // create map object
cspAddStrToMap(innerObj, "str", "test");        // add key/value pairs
cspAddIntToMap(innerObj, "int", 1);
cspAddFloatToMap(innerObj, "decimal", 3.14f);
cspAddMapToMap(innerObj, paramMap, "obj");  // add map to root params and assign name
```

Then you can access map values as follows:
```text
${obj.str}  // "test"
${obj.int}  // 1
${obj.decimal}  // 3.14
```

***Note:*** Array and map can support nested array and map parameters. 
- Use functions `cspAddMapToMap()`, `cspAddVecToMap()`, `cspAddMapToArray()` and `cspAddArrayToArray()`
- Check `CSPInterpreter.h` for other param functions

## CSP syntax

The syntax of the template derives from the high level programming languages grammar like Java, but enhances it with specific constructs.

### Template placeholder

The placeholder expressions are surrounded by `${}`:
```html
<html>
  <body>
    Hello ${params.name}
  </body>
</html>
```

### Strings

#### Single-quoted string

Single-quoted strings are a series of characters surrounded by single quotes:
```text
${'a single-quoted string'}
```

#### Double-quoted string

Double-quoted strings are a series of characters surrounded by double quotes:
```text
${"a double-quoted string"}
```

***Note:*** To escape a double quote, you can use the backslash character: "A double quote: \"".


#### String concatenation

Strings can be concatenated with the `+` operator:

```text
${'a' + 'b'}    // "ab"
```

#### String multiplication

```text
${'one' * 3} // repeat string 3 times: "oneoneone"
```

### Numbers

By default, CSP supports `int32_t` and `float` numeric types. To change size of types define macros:
```c
#define CSP_INT_TYPE int32_t
#define CSP_FLOAT_TYPE float
```

#### Literals
```text
${1}    // unsigned `int` literal
${-1}   // signed `int` literal
${1.0}  // unsigned `float/double` literal
${-1.0}  // signed `float/double` literal
```

### Reserved keywords

- Lower or upper case `NULL` literal:
```text
${NULL}
${null}
```
- Lower or upper case Booleans:
```text
${true}
${false}
${TRUE}
${FALSE}
```

### Arrays

CSP uses a comma-separated array of values, surrounded by square brackets, to denote arrays:
```text
${[]}       // empty array
${[1, 2, 3]}
${['one', 'two']}
${[1.0] + 2.0}  // add value to array -> [1.0, 2.0]
${[0...5]}      // fill array in range -> [0, 1, 2, 3, 4, 5]
```
***Note:*** Range values must be `int` type and compile known constants

You can also create arrays containing values of heterogeneous types
```text
${[1, 2...5, 'a', true, param.name]}
```
Array here contains a `number`, `range`, `string`, `boolean` and `parameter` value

### Maps

Sometimes called dictionaries or associative arrays in other languages. 
Maps associate keys to values, separating keys and values with colons, and each key/value pairs with commas, and the whole keys and values surrounded by square brackets.

```text
${[:]}  // empty map
${['key1': 1, 'key2': 'two', 'key3': 3.0, 'key4': param.name]}
${['red': '#FF0000', 'green': '#00FF00', 'blue': '#0000FF']}
```
The last one is Map of `string` color names, associated with their hexadecimal-coded html colors

***Note:*** Key names must be a `string` literals

### Arithmetic operators

CSP template supports the usual familiar arithmetic operators like `Groovy` programming language

| Operator | Purpose        |
|----------|----------------|
| `+`      | addition       |
| `-`      | subtraction    |
| `*`      | multiplication |
| `/`      | division       |
| `%`      | remainder      |
| `**`     | power          |

Here are a few examples of usage of those operators:
```text
${1 + 2}
${4 - 3} 
${3 * 5}
${3 / 2} 
${10 % 3}
${2 ** 3}
```

### Unary operators
The `+` and `-` operators are also available as unary operators:
```text
${+3 == 3}
${-4 == 0 - 4}
${--1 == 1}
```
**Note:** the `++` and `--` is used only as unary operators and can be stacked `----1`

### Relational operators

| Operator | Purpose               |
|----------|-----------------------|
| `==`     | equal                 |
| `!=`     | not equal             |
| `<`      | less than             |
| `<=`     | less than or equal    |
| `>`      | greater than          |
| `>=`     | greater than or equal |

Here are some examples of simple number and string comparisons using these operators:

```text
${1 + 2 == 3}
${3 != 4}

${-2 < 3}
${2 <= 2}
${3 <= 4}

${5 > 1}
${5 >= -2}

${2 == '2'}
${3.2 == '3.2'}
${'foo' == 'baz'}
${'foo' != 'baz'}
```

### Logical operators

CSP offers three logical operators for boolean expressions:

- `&&`: logical "and"
- `||`: logical "or"
- `!`: logical "not"

```text
${!false}       // "not" false is true
${true && true} // true "and" true is true
${true || false} // true "or" false is true
```

### Precedence

The logical "not" has a higher priority than the logical "and".

`${!false && false}`

The result is `false`, because `not` has a higher precedence than `and`

The logical "and" has a higher priority than the logical "or".

`${true || true && false }`

Here result is `true`, because `and` has a higher precedence than `or`, therefore the `or` is executed last and returns `true`

### Short-circuiting

The logical `||` operator supports short-circuiting: if the left operand is `true`, it knows that the result will be `true` in any case, so it won’t evaluate the right operand. The right operand will be evaluated only if the left operand is `false`.

Likewise for the logical `&&` operator: if the left operand is `false`, it knows that the result will be `false` in any case, so it won’t evaluate the right operand. The right operand will be evaluated only if the left operand is `true`.

### Conditional operators

#### Not operator
```text
${!true}     // the negation of `true` is `false`
${!false}    // the negation of `false` is `true`
${!'foo'}    // 'foo' is a non-empty string, evaluating to `true`, so negation returns `false`
${!''}       // empty string, evaluating to `false`, so negation returns `true`
```

#### Ternary operator
- The ternary operator is a shortcut expression that is equivalent to an `if/else` branch assigning some value to a variable.
```text
${param != null ? 'Found' : 'Not found'}
```

#### Elvis operator
- The "Elvis operator" is a shortening of the ternary operator. One instance of where this is handy is for returning a 'default' value if an expression resolves to `false`
```text
${user.name ? user.name : 'Anonymous'}  // with the ternary operator, you have to repeat the value you want to view
${user.name ?: 'Anonymous'}    // with the Elvis operator the value, which is tested, is used if it is not `false`
```

#### Parameter escaping

Sometimes there is need to skip parameters `${}`. For example JS have [string interpolation](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals)
that contains same parameter enclosing symbols that in CSP and can cause conflict situations.
Thus, to ignore parameter enclose it by backtick (\`) characters:
```javascript
`some text with ${expression} param`
```

### Limitations

1. The `++` (increment) and `--` (decrement) operators are not available
2. Assignment arithmetic operators (`+=`, `-=`, `*=`, `/=`, `%=`, `**=`) are not available
3. Bitwise and bit shift operators (`&`, `|`, `^`, `-`, `<<`, `>>`) not supported

## Template Tags

### Variables
Variables can be defined within a CSP template using the `set` tag:
```html
<csp:set var="number" value="${1 + 2}"/>
<csp:set var="fromParam" value="${param.name}"/>
<csp:set var="list" value="${['one', name, 123, 1.23, 'some']}"/>
<csp:set var="map" value="${['key1': 1, 'key2': 'two', 'key3': 3.0, 'key4': param.name]}"/>
```

#### Description
- `var` - The name of the variable
- `value` - The initial value

### if tag
The logical if tag to switch on an expression

```html
<csp:if test="${name == 'fred'}">
    <p>Hello ${name}!</p>
</csp:if>
```

#### Description
- `test` - The expression to test

### elseif tag
The logical elseif tag

```html
<csp:if test="${name == 'Fred'}">
    <p>Hello Fred!</p>
</csp:if>
<csp:elseif test="${name == 'Bob'}">
    <p>Hello Bob!</p>
</csp:elseif>
```

#### Description
- `test` - The expression to test

### else tag
The logical else tag

```html
<csp:if test="${name == 'Fred'}">
    <p>Hello Fred!</p>
</csp:if>
<csp:else>
     Hello ${name}! Do I know you?
</csp:else>
```

### loop tag
Uses a "for(var in collection)" like loop to iterate over each element of the specified object

```html
<csp:loop var="book" in="${books}">
    <p>Title: ${book.title}</p>
    <p>Author: ${book.author}</p>
</csp:loop>
```

- Using the `status` parameter to alternate the coloring of a table’s rows:
```html
<tbody>
  <csp:loop status="i" var="item" in="${itemList}">
    <!-- Alternate CSS classes for the rows. -->
    <tr class="${ (i % 2) == 0 ? 'a' : 'b' }">
      <td>${item.id}</td>
      <td>${item.parentId}</td>
      <td>${item.type}</td>
      <td>${item.status}</td>
    </tr>
  </csp:loop>
</tbody>
```

- Using `set` tag variable as a counter
```html
<ul class="list-group container">
<csp:set var="cnt" value="${1}"/>

<csp:loop var="val" in="${list}">
    <li class="list-group-item">${cnt}</li>
    <csp:set var="cnt" value="${cnt + 1}"/>
</csp:loop>
</ul>
```

#### Description
- `in` - The object to iterate over
- `var` - The name of the item
- `status` (optional) - The name of a variable to store the iteration index in. Starts with 0 and increments for each iteration

### render tag
Applies an inbuilt or user-defined CSP template so that templates can be shared and reused

- Example template `include.csp`:
```html
<p>${title}</p>
<p>${author}</p>
```

- This template can now be reused whether you have parameters as `title` and `author`:
```html
<csp:render template="../templates/include.csp"/>
```

#### Description
- `template` - The name of the template to apply


