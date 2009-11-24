#include <stdio.h>
#include "erl_nif.h"

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM info)
{
    return 0;
}

static int
reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM info)
{
    return 0;
}

static int
upgrade(ErlNifEnv* env, void** priv, void** old, ERL_NIF_TERM info)
{
    return 0;
}

static void
unload(ErlNifEnv* env, void* priv)
{
    return;
}

static ERL_NIF_TERM
size(ErlNifEnv* env, ERL_NIF_TERM arg)
{
    ErlNifBinary bin;

    if(!enif_inspect_binary(env, arg, &bin))
        return enif_make_badarg(env);

    return enif_make_ulong(env, bin.size);
}

static ErlNifFunc nifsize_funcs[] =
{
    {"size", 1, size}
};

ERL_NIF_INIT(nifsize, nifsize_funcs, load, reload, upgrade, unload)
