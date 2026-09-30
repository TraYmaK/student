check_values([H|T]) :-
    between(1, 4, H),
    check_values(T).

check_values([]).

check_uniqueness([H|T]) :-
    \+ member(H, T),
    check_uniqueness(T).

check_uniqueness([]).

check_rows(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16) :-
    check_uniqueness([X1, X2, X3, X4]),
    check_uniqueness([X5, X6, X7, X8]),
    check_uniqueness([X9, X10, X11, X12]),
    check_uniqueness([X13, X14, X15, X16]).

check_columns(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16) :-
    check_uniqueness([X1, X5, X9, X13]),
    check_uniqueness([X2, X6, X10, X14]),
    check_uniqueness([X3, X7, X11, X15]),
    check_uniqueness([X4, X8, X12, X16]).

check_squares(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16) :-
    check_uniqueness([X1, X2, X5, X6]),
    check_uniqueness([X3, X4, X7, X8]),
    check_uniqueness([X9, X10, X13, X14]),
    check_uniqueness([X11, X12, X15, X16]).

print_sudoku(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16) :-
    write([X1, X2, X3, X4]), nl,
    write([X5, X6, X7, X8]), nl,
    write([X9, X10, X11, X12]), nl,
    write([X13, X14, X15, X16]).

sudoku(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16) :-
    check_values([X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16]),
    check_rows(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16),
    check_columns(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16),
    check_squares(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16),
    print_sudoku(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15, X16).
