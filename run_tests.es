
main([]) ->
    Count = 500000,
    Size = 1,
    
    Left = << <<"a">> || _ <- lists:seq(0, Size) >>,
    Right = list_to_binary([<<"b">>, Left]),

    io:format("Running tests...~n", []),

    couch_uca:start_driver("."),
    nif_uca:start(),

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
  Exp = nif_uca:collate(Left, Right),
  run_nif_uca(Count-1, Right, Left, Exp * -1).

