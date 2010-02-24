
main([]) ->
    Count = 1000,
    Size = 1000000,

    Sizes = [10, 100, 1000, 10000, 100000, 1000000, 5000000],
    lists:foreach(fun(X) ->
        io:format("Size: ~p~n", [X]),
        test(Count, X) end,
    Sizes).

test(Count, Size) ->
    
    Left = << <<"a">> || _ <- lists:seq(0, Size) >>,
    Right = list_to_binary([<<"b">>, Left]),

    io:format("Running tests...~n", []),

    couch_uca:start_driver("."),
    {ok, Coll} = nif_uca:open(),

    timeit("couch_uca", fun() -> run_couch_uca(Count, Left, Right, -1) end),
    timeit("nif_uca", fun() -> run_nif_uca(Count, Left, Right, -1) end),
    ok.

timeit(Name, Fun) ->
  Before = erlang:now(),
  Fun(),
  Length = float(timer:now_diff(erlang:now(), Before)) / 1000000.0,
  io:format("~s: ~f s~n", [Name, Length]).

run_couch_uca(0, _, _, _) ->
  ok;
run_couch_uca(Count, Left, Right, Exp) ->
  Exp = couch_uca:collate(Left, Right),
  run_couch_uca(Count-1, Right, Left, Exp * -1).

run_nif_uca(0, _, _, _) ->
  ok;
run_nif_uca(Count, Left, Right, Exp) ->
  {ok, Coll} = nif_uca:open(),
  Exp = nif_uca:collate(Coll, Left, Right),
  run_nif_uca(Count-1, Right, Left, Exp * -1).

