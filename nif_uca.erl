-module(nif_uca).
-export([start/0, collate/2]).

start() ->
    erlang:load_nif("nif_uca", 0).

collate(_A, _B) ->
    nif_error(?LINE).    

nif_error(Line) ->
    exit({nif_not_loaded,module,?MODULE,line,Line}).
