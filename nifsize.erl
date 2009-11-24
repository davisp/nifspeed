-module(nifsize).
-export([start/0, size/1]).

start() ->
    erlang:load_nif("nifsize", 0).

size(_A) ->
    nif_error(?LINE).

nif_error(Line) ->
    exit({nif_not_loaded,module,?MODULE,line,Line}).
