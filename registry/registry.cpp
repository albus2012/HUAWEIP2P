#include"registry.h"
Registry::Registry()
{
  std::cout << "it works" << std::endl;
}
void Registry::reg_insert(double num, reg_info regtable)
{  
  registry_table.insert(std::make_pair(num, regtable));
} 
  
  
 
int main()
{
  reg_info reg0;
  reg0.complete_level = 0;
  double int0 = 192168000201;
  Registry reg;
  reg.reg_insert(int0, reg0);
  
  return 1;
}
