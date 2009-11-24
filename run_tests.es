
main([]) ->
    Count = 500000,
    Size = 4096,
    
    Bin = << <<"a">> || _ <- lists:seq(1, Size) >>,
    
    io:format("Running tests...~n", []),

    ok = drvsize:start(),
    ok = nifsize:start(),

    timeit("bifsize", fun() -> run_bifsize(Count, Bin, Size) end),
    timeit("drvsize", fun() -> run_drvsize(Count, Bin, Size) end),
    timeit("nifsize", fun() -> run_nifsize(Count, Bin, Size) end),
    ok.

timeit(Name, Fun) ->
  Before = erlang:now(),
  Fun(),
  Length = float(timer:now_diff(erlang:now(), Before)) / 1000000.0,
  io:format("~s: ~f s~n", [Name, Length]).

run_bifsize(0, _, _) ->
  ok;
run_bifsize(Count, Bin, Exp) ->
  Exp = erlang:size(Bin),
  run_bifsize(Count-1, Bin, Exp).

run_drvsize(0, _, _) ->
  ok;
run_drvsize(Count, Bin, Exp) ->
  Exp = drvsize:size(Bin),
  run_drvsize(Count-1, Bin, Exp).

run_nifsize(0, _, _) ->
  ok;
run_nifsize(Count, Bin, Exp) ->
  Exp = nifsize:size(Bin),
  run_nifsize(Count-1, Bin, Exp).

