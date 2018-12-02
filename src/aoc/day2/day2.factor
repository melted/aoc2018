USING: assocs io io.files io.encodings.utf8 kernel math
math.statistics sequences strings ;
IN: aoc.day2

: read-data ( -- seq ) "data/input2.txt" utf8 file-lines ;

: any-n ( seq n -- n ) [ = ] curry any? ;

: checksum ( seq -- n ) [ histogram values ] map 
    [ [ 2 any-n ] count ] [ [ 3 any-n ] count ] bi * ; 

: diffseq ( seq1 seq2 -- n ) 0 [ = [ 1 + ] unless ] 2reduce ;

: commonseq ( seq1 seq2 -- seq ) 
    "" [ over = [ 1string append ] [ drop ] if ] 2reduce ;

: small-diff? ( seq1 seq2 -- seq/f ) 
    2dup diffseq 1 = [ commonseq ] [ 2drop f ] if ;

: find-small-diff ( seqs seq -- seq/f )
    [ small-diff? ] curry map-find drop ;

: get-small-diff ( seq -- seq/f ) dup empty? [ drop f ]
    [ unclip dupd find-small-diff [ nip ] [ get-small-diff ] if* ] if ;

: solve ( -- n n ) read-data [ checksum ] [ get-small-diff ] bi ;