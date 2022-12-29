##Code Style Guidelines

1. all functions and variable lowerCamelCase 
2. All Class UpperCamelCase
3. Prefix for members class and variable:
    - member class: m_member;
    - varible in function: v_variable;
    - pointer: p_pointer; 
    - global g_global;
    - static s_static;
    - const c_const; 
4. Doxygen standard used in header files.
5. copyright header ? (file, author, date, brief)

code example: 

int g_version; 
static int sg_counter;
const std::string cg_name;

Class Module
{
    int m_member;
    std::string m_memberString;
    int* p_pointer;
    std::unique_ptr<int> p_smartPointer;


public:
    void someFunc(int lhs, int rhs);
    int anotherFunc(std::string word);

};

void Module::someFunc()
{
    int v_variabe;
    int (*pf_someFunctionPointer)(std::string);
}