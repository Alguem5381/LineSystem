
#include "object.h"
#include "fileMenager.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wchar.h>
#include <locale.h>
#include <doubleList.h>
#include <math.h>
#include <wchar.h>

int main()
{
    setlocale(LC_ALL, "");
    
    Object obj;
    defineObject(&obj);

    loadData(&obj);

    saveObject(&obj);
    deleteObject(&obj);

    return 0;
}

