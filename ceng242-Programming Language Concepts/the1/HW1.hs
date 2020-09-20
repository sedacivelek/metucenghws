module HW1 (
    form,
    constGrid,
    flatten,
    access,
    slice,
    vcat,
    hcat,
    without,
    matches2d,
) where

-- do not modify the module declaration above!
-- this will ensure that you cannot load (compile)
-- the module without implementing all of the functions.

-- If you have functions you do not want to implement,
-- leave them as undefined or make them have another
-- default value. If you fully remove any of their definitions,
-- that will be a compilation error during evaluation,
-- and you will be eligible for (yay!) a 5 point deduction
-- (that's bad for your grade). Runtime errors in your code 
-- (rather than compilation errors) are acceptable and will simply
-- result in you getting zero from the specific test case causing
-- an error.

-------------------------
-- Fellowship of the Grid (25, 5, 5, 5 points)
form :: [a] -> (Int, Int) -> [[a]] 
form [] _  = []
form x (r,c) = (take c x) : (form (drop c x) (r,c))
constGrid :: a -> (Int, Int) -> [[a]]
constGrid v (r,c) = [ [v|x<-[1..c]] | x<-[1..r]]

flatten :: [[a]] -> [a]
flatten x = foldr (++) [] x

access :: [[a]] -> (Int, Int) -> a
access x (r,c) = (x !! r) !! c
----------------------------
-- The Two Signatures (10, 5, 5, 10 points)
slice :: [[a]] -> (Int, Int) -> (Int, Int) -> [[a]]
slice x (i1,i2) (j1,j2) = take(i2-i1) (drop i1 (sliced x (i1,i2) (j1,j2))) 

sliced :: [[a]]-> (Int,Int) -> (Int, Int)-> [[a]]
sliced [] _ _ = []
sliced x (i1,i2) (j1,j2) = (take (j2-j1) (drop j1 (head x))) : (sliced (tail x) (i1,i2) (j1,j2))

vcat :: [[a]] -> [[a]] -> [[a]]
vcat x y = (++) x y 

hcat :: [[a]] -> [[a]] -> [[a]]
hcat [] [] = []
hcat x y = ((head x)++(head y)) : (hcat (tail x) (tail y))

without :: [[a]] -> (Int, Int) -> (Int, Int) -> [[a]]
without x (i1,i2) (j1,j2) = (take i1 (withouth x (j1,j2))) ++ (drop i2(withouth x(j1,j2)))

withouth :: [[a]] -> (Int,Int) -> [[a]]
withouth [] _ = []
withouth x (j1,j2) = ((take j1 (head x))++ (drop j2 (head x))):(withouth (tail x)(j1,j2))
----------------------------
-- Return of the Non-trivial (30 points, 15 subject to runtime constraints)
matches2d :: Eq a => [[a]] -> [[a]] -> [(Int,Int)]
matches2d x y = flatten (slicein x  y (length x) (length (head(x))) 0 (length y) 0 (length (head y)))

slicein x  pattern p q i1 i2 j1 j2 = if (i1 < (p-i2))
    then (slicej (  (take i2 x) ) pattern q i1 i2 j1 j2) :(slicein (drop 1 x) pattern p q (i1+1) i2 j1 j2) 
    else [(slicej x pattern q i1 i2 j1 j2)]

slicej x pattern q i1 i2 j1 j2 = if (j1<(q-j2))
    then if (map (take j2) x == pattern)
        then (i1,j1) : (slicej (map (drop 1) x) pattern q i1 i2 (j1+1) j2)
        else slicej (map (drop 1) x) pattern q i1 i2 (j1+1) j2
    else if (x == pattern) 
        then [(i1,j1)]
        else []

----------------------------
-- What is undefined? Just a value that will cause an error
-- when evaluated, from the GHC implementation:
-- undefined = error "Prelude.undefined"
-- But it allows your module to be compiled
-- since the function definitions will exist.
