USING: assocs io io.files io.encodings.utf8 kernel math math.parser
math.statistics sequences splitting strings ;
IN: aoc.day3

: parse-line ( str -- seq ) " #@,:x" split
    [ empty? not ] filter
    [ string>number ] map ;

: read-data ( -- seq ) "data/input3.txt" utf8 file-lines
    [ parse-line ] map ;

