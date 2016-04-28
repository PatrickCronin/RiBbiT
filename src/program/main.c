#include <stdio.h>
#include <stdlib.h>
#include "../data_types/misc_structures/data.h"
#include "../data_types/misc_structures/bigthing.h"
#include "../data_types/list_structures/ulvlist.h"
#include "algorithm.h"
#include "module.h"
#include "parse_args.h"
#include "parse_xml_input.h"
#include "progress.h"

int main (int argc, char * argv[])
{
    bigthing b;
    int next_stc_id;

    /* Pre-initialize bigthing struct */
    b = bigthing_constructor();

    /* Load data */
    b->pd = data_initialize();
    parse_program_arguments(b->pd, argc, argv);
    parse_input_file(b->pd);
    data_calculate_global_helpers(b->pd);

    /* Prepare recursive algorithm */
    bigthing_init2(b);
    bigthing_prepare_subject_days_left(b);
    next_stc_id = bigthing_prepare_stcs(b);
    bigthing_create_necessary_free_periods(b, next_stc_id);
    bigthing_sanity_check(b);

    /* Run any mod_pregen functions before we start
    ** generating the timetables */
    module_run_pregens(b);

    /* Initialize the progress counter */
    progress_init(b->progress);

    /* Run the algorithm */
    /* algorithm_generate_timetable(b, ulvlist_copy_shallow(b->stcs[0])); */
    algorithm_generate_timetable_2(b);
    
    /* Free things that we don't need */
    mpz_clear(b->num_solutions);
    return(1);
}
