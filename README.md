# NotAJSONParser_cpp
A C++ version of NotAJSONParser  
View [this other branch](https://github.com/phoebe-leong/NotAJSONParser_cpp/tree/console_out) for a version with handy console output for debugging

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
