#include<map>
#include<iostream>


typedef struct 
{
  //the complete_level tell how much the moden has updated at least
  // 0---20%---40%---60%---80%---100%
  //---0--|--1--|--2--|--3--|--4--|-5

  int complete_level;
}reg_info;

class Registry
{
  typedef std::map<double, reg_info> Registry_table;
 public:
  Registry()
  {
    std::cout << "it works" << std::endl;
  }
  void reg_insert(double num, reg_info regtable);
  
 private:
  Registry_table registry_table;
};

