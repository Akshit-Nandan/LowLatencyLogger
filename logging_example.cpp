#include "logging.h"


int main(){
    using namespace Common;
    std::string str = "Example String";
    const char* cstr = "Example C String";
    int i_val = 101;
    double d_val = 22.0/7;

    Logger logger("example.log");
    logger.log("Starting to log\n");
    logger.log("Here are the values \nstd:string: %,\ncstring: %,\ninteger: %,\nValue of pi: %",
    str,cstr,i_val,d_val);
}