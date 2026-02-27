# Just Engine

Just a game engine

### Code style
1. .clang-format

2. Naming style
    - C++ classes/structs: **F**Name
    - enums: **E**Name
    - types: **T**Name
    - global variables: **g**Name
    - boolean variables: **b**Name
    - CamelCase for: classes/structs, class members and methods (EXCLUDE booleans)
    - lowerCamelCase: static functions, local variables
    - _lowerCamelCase: function arguments

3. Classes/structs header structure
    - each group has it's own public/protected/private sections
    - //- Types : inner class types declaration/definition
    - //- Events : events/delegates declarations and variables
    - //- Variables : class members
    - //- Lifecycle : main methods (constructors, destructor, tick, update, etc)
    - //- Methods : other methods (getters, setters, other logic)