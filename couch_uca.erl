-module(couch_uca).
-export([start_driver/1, collate/2]).

start_driver(LibDir) ->
    case erl_ddll:load_driver(LibDir, "couch_uca") of
    ok ->
        ok;
    {error, already_loaded} ->
        ok = erl_ddll:reload_driver(LibDir, "couch_uca");
    {error, Error} ->
        exit(erl_ddll:format_error(Error))
    end.

drv_port() ->
    case get(couch_drv_port) of
    undefined ->
        Port = open_port({spawn, "couch_uca"}, []),
        put(couch_drv_port, Port),
        Port;
    Port ->
        Port
    end.

collate(A, B) ->
    collate(A, B, []).

collate(A, B, Options) when is_binary(A), is_binary(B) ->
    Operation =
    case lists:member(nocase, Options) of
        true -> 1; % Case insensitive
        false -> 0 % Case sensitive
    end,

    SizeA = size(A),
    SizeB = size(B),
    Bin = <<SizeA:32/native, A/binary, SizeB:32/native, B/binary>>,
    [Result] = erlang:port_control(drv_port(), Operation, Bin),
    % Result is 0 for lt, 1 for eq and 2 for gt. Subtract 1 to return the
    % expected typical -1, 0, 1
    Result - 1.
