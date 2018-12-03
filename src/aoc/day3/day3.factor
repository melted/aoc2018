USING: arrays combinators.smart io io.files io.encodings.utf8
fry kernel locals math math.parser math.ranges sequences
splitting strings ;
IN: aoc.day3

: parse-line ( str -- seq ) " #@,:x" split
    [ empty? not ] filter
    [ string>number ] map ;

: read-data ( -- seq ) "data/input3.txt" utf8 file-lines
    [ parse-line ] map ;

: pos>n ( x y -- n ) 1000 * + ;

: n>pos ( n -- x y ) 1000 [ mod ] [ /i ] 2bi ;

: inc-pos ( arr x y -- ) pos>n swap [ 1 + ] change-nth ;

: get-pos ( arr x y -- n ) pos>n swap nth ;

:: get-area ( x y dx dy -- seq )
    x x dx + [a,b)
    y y dy + [a,b)
    cartesian-product concat ;

: inc-area ( seq arr -- ) [ 1 tail first4 get-area ] dip
    '[ _ swap first2 inc-pos ] each ;

:: make-fabric ( seq -- arr )
    1000000 0 <array> :> fabric
       seq [ fabric inc-area ] each fabric ;

: check-area? ( seq arr -- ? )
    [ first4 get-area ] dip
    '[ _ swap first2 get-pos 2 < ] all? ;

: check-overlap ( seq arr -- n )
    '[ unclip swap _ check-area? [ drop f ] unless ] map-find drop ;

: solve ( -- n n )
    read-data dup make-fabric
    [ [ 1 > ] count nip ]
    [ check-overlap ] 2bi ;