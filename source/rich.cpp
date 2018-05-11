#include <iostream>
#include "sim_data.hpp"
#include "my_main_loop.hpp"

using namespace std;

int main(void)
{
  try{
    SimData sim_data(read_number("impactor_width.txt"));
    my_main_loop(sim_data.getSim());
  }
  catch(const UniversalError& eo){
    cout << eo.GetErrorMessage() << endl;
  }
  return 0;
}
