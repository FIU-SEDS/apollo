#pragma once 

#include <string>
#include <string_view>

class State{
  public:
    State(std::string& state);
    ~State();

    void setState(std::string& state);
    void getState(std::string_view state);
  private:
    std::string state = "INIT";
};
