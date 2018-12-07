open TextIO
open List

fun readStream (s : instream) (acc : string list) : string list =
    case inputLine s of
         NONE => rev acc
       | SOME x => readStream s (x::acc)

fun readData () =
    let
        val input = openIn "data/input7.txt"
        val xs = readStream input []
        val p = map (String.tokens (fn c => c = #" ")) xs
    in
        map (fn l => (nth (l,1), nth (l,7))) p
    end

val deps = readData ()

structure StringyOrd : ORD_KEY =
    struct
        type ord_key = String.string
        val compare = String.compare
    end

structure StringSet = SplaySetFn(StringyOrd)

val available = StringSet.addList(StringSet.empty, revAppend (ListPair.unzip deps))

fun removeDeps _ [] = []
  | removeDeps done ((a,b)::xs) = if StringSet.member (done, a)
                                    then removeDeps done xs
                                    else (a, b)::removeDeps done xs

fun blocked (d : (string * string) list) = StringSet.addList(StringSet.empty, map #2 d)

fun nextTasks deps avail =
    let
        val block = blocked deps
        val left = StringSet.difference(avail, block)
    in
        StringSet.listItems left
    end

fun solve deps avail =
    let
        fun worker deps avail done acc =
            if StringSet.isEmpty avail
                then acc
                else let
                    val t = hd (nextTasks deps avail)
                    val newDone = StringSet.add (done, t)
                    val newDeps = removeDeps newDone deps
                in
                    worker newDeps (StringSet.delete(avail,t)) newDone (acc^t)
                end
    in
        worker deps avail StringSet.empty ""
    end

fun solve2 deps avail =
    let
        fun updateWorkers [] acc = acc
          | updateWorkers ((a, 1)::ws) (xs, ys) = updateWorkers ws (xs, a::ys))
          | updateWorkers ((a, n)::ws) (xs, ys) = updateWorkers ws ((a, n-1)::xs, ys)
        fun allocateWorkers [] = []
          | allocateWorkers (t::ts) = (t, ord (hd (explode t))-4)::allocateWorkers ts
        fun worker ws deps avail done time =
            let
                val (cws, prod) = updateWorkers ws ([], [])
                val newDone = StringSet.addList (done, prod)
                val newDeps = removeDeps newDone deps
                val tasks = nextTasks newDeps avail
                val alloc = Int.min (5 - length cws, length tasks)
                val newAvail = StringSet.difference (avail, StringSet.addList (StringSet.empty, take (tasks, alloc)))
                val nws = cws@(allocateWorkers tasks)
            in
                if StringSet.numItems newDone = 26
                    then time
                    else worker nws newDeps newAvail newDone (time+1)
            end
    in
        worker [] deps avail StringSet.empty 0
    end

val result = solve deps available
val result2 = solve2 deps available

(*val _ = OS.Process.exit OS.Process.success*)