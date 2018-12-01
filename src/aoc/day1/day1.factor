USING: hash-sets io io.files io.encodings.utf8 kernel locals math
math.parser sequences sets ;
IN: aoc.day1

: read-data ( -- seq ) "data/input1.txt" utf8 file-lines 
    [ string>number ] map ;

:: find-rep ( seq scanned acc! -- n )
   seq [ acc + dup dup acc! scanned in? [ scanned adjoin f ] unless ] map-find
   [ drop seq scanned acc find-rep ] unless ;

: solve ( -- n n ) read-data
    [ sum ] [ 32 <hash-set> 0 find-rep ] bi ;