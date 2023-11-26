#include "mrng.h"
#include "GNSS.h"
#include <cstring>
int main (int argc, char * argv[]){
    char* dummy= get_str_of_option(argv,argv+argc,"-m");
    if (strcmp(dummy,"1")==0)
        gnss(argc,argv);
    else if (strcmp(dummy,"2")==0)
        mrng(argc,argv);

    return 0;

}