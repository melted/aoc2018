USING: grouping io io.files io.encodings.utf8 kernel
 locals math sequences sorting strings unicode ;
IN: aoc.day5

: read-data ( -- seq ) "data/input5.txt" utf8 file-contents ;

: volatile? ( ch ch -- ? ) [ = ] [ [ ch>lower ] bi@ = ] 2bi [ not ] when ;

: one-round ( seq -- seq )
    dup 2 <clumps> [ dup first2 volatile?
        [ dup 32 0 rot set-nth 32 1 rot set-nth ] [ drop ] if ]
        each [ 32 = not ] filter ;

:: iterate-reduction ( seq -- n )
    seq length :> current-length!
    seq one-round :> current-seq!
    [ current-seq length current-length = ]
        [ current-seq length current-length!
          current-seq one-round current-seq! ]
    until current-length ;

:: remove-char ( seq n -- seq )
    seq [ dup n 65 + = [ n 97 + = ] dip or not ] filter ;

:: try-removal ( seq -- n )
    26 <iota> :> tests
    tests [ seq swap remove-char iterate-reduction ] map natural-sort first ;

: solve ( -- n n ) read-data [ iterate-reduction ] [ try-removal ] bi ;