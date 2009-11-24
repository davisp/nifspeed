#include <stdio.h>
#include "unicode/ucol.h"
#include "unicode/ucasemap.h"
#include "erl_nif.h"

typedef struct {
    int refcnt;
    UCollator* coll;
} uca_data;

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    UErrorCode status = U_ZERO_ERROR;
    uca_data* ptr = (uca_data*) enif_alloc(env, sizeof(uca_data));
    ptr->refcnt = 1;

    ptr->coll = ucol_open("", &status);
    if(U_FAILURE(status)) return -1;

    *priv = (void*) ptr;

    return 0;
}

static int
reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    return 0;
}

static int
upgrade(ErlNifEnv* env, void** priv, void** old_priv,
          ERL_NIF_TERM load_info)
{
    uca_data* ptr = (uca_data*) *priv;
    ptr->refcnt++;
    return 0;
}

static void
unload(ErlNifEnv* env, void* priv)
{
    uca_data* ptr = (uca_data*) priv;
    if(--ptr->refcnt == 0)
    {
        enif_free(env, priv);
    }
    return;
}

static ERL_NIF_TERM
collate(ErlNifEnv* env, ERL_NIF_TERM a, ERL_NIF_TERM b)
{
    uca_data* ptr = (uca_data*) enif_get_data(env);

    ErlNifBinary bin1;
    ErlNifBinary bin2;

    UErrorCode status = U_ZERO_ERROR;
    UCharIterator iter1;
    UCharIterator iter2;

    int result = 0;

    if(!ptr) return -1;
    if(!enif_inspect_binary(env, a, &bin1)) return -1;
    if(!enif_inspect_binary(env, b, &bin2)) return -1;

    uiter_setUTF8(&iter1, (char*) bin1.data, bin1.size);
    uiter_setUTF8(&iter2, (char*) bin2.data, bin2.size);

    result = ucol_strcollIter(ptr->coll, &iter1, &iter2, &status);
    
    if(U_FAILURE(status))
    {
        return enif_make_badarg(env);
    }
    else
    {
        return enif_make_int(env, result);
    }
}

static ErlNifFunc nif_uca_funcs[] =
{
    {"collate", 2, collate}
};

ERL_NIF_INIT(nif_uca, nif_uca_funcs, load, reload, upgrade, unload)
