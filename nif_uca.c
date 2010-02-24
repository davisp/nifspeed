#include <stdio.h>
#include "unicode/ucol.h"
#include "unicode/ucasemap.h"
#include "erl_nif.h"

ErlNifResourceType* UCOLL_TP = NULL;

typedef struct {
    UCollator*      coll;
} uca_data;

static void
free_coll(ErlNifEnv* env, void* coll)
{
    uca_data* ptr = (uca_data*) coll;
    if(ptr->coll) ucol_close(ptr->coll);
    // I assume I'm not responsible for freeing coll.
}

static ERL_NIF_TERM
make_atom(ErlNifEnv* env, const char* atom)
{
    ERL_NIF_TERM ret;
    if(enif_make_existing_atom(env, atom, &ret))
    {
        return ret;
    }

    return enif_make_atom(env, atom);
}

static ERL_NIF_TERM
make_ok(ErlNifEnv* env, ERL_NIF_TERM data)
{
    ERL_NIF_TERM ok = make_atom(env, "ok");
    return enif_make_tuple2(env, ok, data);
}

static ERL_NIF_TERM
make_error(ErlNifEnv* env, const char* reason)
{
    ERL_NIF_TERM error = make_atom(env, "error");
    ERL_NIF_TERM msg = make_atom(env, reason);
    return enif_make_tuple2(env, error, msg);
}

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    char* name = "nif_uca.coll";
    int flags = ERL_NIF_RT_CREATE;

    UCOLL_TP = enif_open_resource_type(env, name, free_coll, flags, NULL);
    if(UCOLL_TP == NULL)
    {
        return -1;
    }

    return 0;
}

static ERL_NIF_TERM
open(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    uca_data* ptr = NULL;
    UErrorCode status = U_ZERO_ERROR;
    ERL_NIF_TERM ret;

    ptr = (uca_data*) enif_alloc_resource(env, UCOLL_TP, sizeof(uca_data));
    if(ptr == NULL)
    {
        return make_error(env, "alloc_resource_failed");
    }

    ptr->coll = ucol_open("", &status);
    if(U_FAILURE(status))
    {
        return make_error(env, "ucol_open_failed");
    }

    ret = enif_make_resource(env, ptr);
    enif_release_resource(env, ptr);
    return make_ok(env, ret);
}

static ERL_NIF_TERM
collate(ErlNifEnv* env, int args, const ERL_NIF_TERM argv[])
{
    uca_data* ptr = NULL;

    ErlNifBinary lhs;
    ErlNifBinary rhs;
    
    UErrorCode status = U_ZERO_ERROR;
    UCharIterator iter1;
    UCharIterator iter2;

    int result = 0;

    if(!enif_get_resource(env, argv[0], UCOLL_TP, (void**) &ptr))
    {
        return make_error(env, "bad_collator");
    }

    if(!enif_inspect_binary(env, argv[1], &lhs))
    {
        return enif_make_badarg(env);
    }

    if(!enif_inspect_binary(env, argv[2], &rhs))
    {
        return enif_make_badarg(env);
    }

    uiter_setUTF8(&iter1, (char*) lhs.data, lhs.size);
    uiter_setUTF8(&iter2, (char*) rhs.data, rhs.size);

    result = ucol_strcollIter(ptr->coll, &iter1, &iter2, &status);
    
    if(U_FAILURE(status))
    {
        return make_error(env, "collation_failed");
    }
    else
    {
        return enif_make_int(env, result);
    }
}

static ErlNifFunc nif_uca_funcs[] =
{
    {"open", 0, open},
    {"collate", 3, collate}
};

ERL_NIF_INIT(nif_uca, nif_uca_funcs, load, NULL, NULL, NULL)
