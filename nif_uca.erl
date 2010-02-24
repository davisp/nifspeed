-module(nif_uca).
-export([open/0, collate/3]).
-on_load(load_nif/0).

load_nif() ->
    erlang:load_nif("nif_uca", 0).

open() ->
    nif_error(?LINE).

collate(_Coll, _A, _B) ->
    nif_error(?LINE).    

nif_error(Line) ->
    exit({nif_not_loaded,module,?MODULE,line,Line}).
