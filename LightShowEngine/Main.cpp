#include "main.h"

#ifdef main
#undef main
#endif // main


int main() {
    Application thisApplication; //the application

    thisApplication.run();

    return 0; //return zero, indicating no errors.
}
