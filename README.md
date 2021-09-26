# NotAJSONParser_cpp - with handy console output
A C++17 version of NotAJSONParser  

## Example

```cpp
#include "najp.hpp"

najp n;
int main() {
  n.open("test.json");
  n.addelement("Name", "Phoebe");
  n.addelement("Age", 14);
  
  najp_array arr = {{"HTML", true}, {"CSS", true}, {"C++", true}, {"C", true}};
  n.addarray("Languages", arr);
  n.close();
}
```

Output:
```json
{
	"Name" : "Phoebe",
	"Age" : 13,
	"Programming Languages Learnt" : [
		"C",
		"C++",
		"CSS",
		"HTML"
	]
}
```
Console output:
```
$   File "test.json" opened ..
$ ✓ String element "Name" finished with no errors
$ ✓ Integer element "Age" finished with no errors
$ ✗ Integer element "Age" finished with one error: NAJP_TITLE_ALREADY_IN_USE
$ ✓ Array "Programming Languages Learnt" finished with no errors
$   File closed ..
```

Errors will be displayed in red, and successes will be displayed in green.  
Opening and closing of files is coloured normally and underlined.
