//
// Created by lucie on 15/08/2022.
//

#ifndef REDUCED_BENCH_SQL_H
#define REDUCED_BENCH_SQL_H

#include "../parser.h"


namespace Database {

void sql_insert(settings &set);
bool is_in_db(settings &set);
};


#endif //REDUCED_BENCH_SQL_H