USING: accessors arrays assocs calendar calendar.parser combinators hashtables
io io.files io.encodings.utf8 fry kernel locals math math.parser math.ranges math.statistics sequences sorting
splitting strings unicode ;
IN: aoc.day4

SINGLETONS: doze wake ;

: get-number ( str -- n ) [ digit? ] filter string>number ;

: parse-action ( str -- x )
    " " split [ empty? not ] filter unclip {
        { "Guard" [ first get-number ] }
        { "wakes" [ drop wake ] }
        { "falls" [ drop doze ] }
    } case ;

: parse-date ( str -- ts n ) 1 tail ":00" append ymdhms>timestamp dup minute>> ;

: parse-line ( str -- seq )
    "]" split first2 [ parse-date ] [ parse-action ] bi* 3array ;

: read-data ( -- seq ) "data/input4.txt" utf8 file-lines
    [ parse-line ] map ;

:: collect-data ( seq -- ht ht )
    32 <hashtable> :> snoozes
    32 <hashtable> :> minutes
    0 :> current-guard!
    0 :> start!
    seq [| val | val last number?
     [ val last current-guard! ] [
       val last doze? [ val second start! ] [
            val second start swap [ [a,b) ] [ swap - ] 2bi [ current-guard snoozes push-at ] [ current-guard minutes push-at ] bi*
        ] if
     ] if
    ] each snoozes minutes ;


: find-guard ( seq -- n n ) [ first ] sort-with collect-data [ sum ] assoc-map [ [ concat histogram ] assoc-map ] dip ;


: solve ( -- n n ) read-data find-guard ;