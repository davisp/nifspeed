-module(drvsize).
-export([start/0, size/1]).

start() ->
    case erl_ddll:load_driver(".", "drvsize") of
    ok ->
        ok;
    {error, already_loaded} ->
        ok = erl_ddll:reload_driver(".", "drvsize");
    {error, Error} ->
        exit(erl_ddll:format_error(Error))
    end,
    put(drvport, open_port({spawn, "drvsize"}, [binary])),
    ok.

size(Bin) when is_binary(Bin) ->
    <<Ret:32/integer>> = erlang:port_control(get(drvport), 0, Bin),
    Ret.

