#include "udeabook.h"

int main()
{
    UdeABook red;

    if (!red.iniciar("datos/"))
    {
        return 1;
    }

    red.ejecutarMenu();

    return 0;
}
