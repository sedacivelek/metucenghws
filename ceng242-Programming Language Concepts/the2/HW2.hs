module HW2 (
    parse, -- reexport for easy terminal use
    foldAndPropagateConstants,
    assignCommonSubexprs,
    reducePoly,
    eval
) where

import Expression
import Parser
import Data.Map
import Data.List
import Pprint
-- Do not change the module definition and imports above! Feel free
-- to modify the parser, but please remember that it is just a helper
-- module and is not related to your grade. You should only submit
-- this file. Feel free to import other modules, especially Data.List!

eval:: [(String,Int)]-> ExprV -> ExprV
eval list (UnaryOperation Minus (Leaf (Constant a))) = Leaf (Constant (negate a))
eval list (UnaryOperation Minus (Leaf (Variable a))) = if (member a (fromList(list)))
                                                        then Leaf(Constant (negate (fromList(list)! a)))
                                                        else UnaryOperation Minus (Leaf (Variable a))
eval list (BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Constant a2))) = (Leaf (Constant (a1+a2)))
eval list (BinaryOperation Times (Leaf (Constant a1)) (Leaf (Constant a2))) = (Leaf (Constant (a1*a2)))
eval list (BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Variable a2))) = if ((member a2 (fromList(list))) == True)
                            then ( Leaf (Constant (a1+((fromList(list))! a2 ) ) ) )
                            else(BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Variable a2)))
eval list (BinaryOperation Plus (Leaf (Variable a2)) (Leaf (Constant a1))) = if ((member a2 (fromList(list))) == True)
                            then ( Leaf (Constant (a1+((fromList(list))! a2 ) ) ) )
                            else(BinaryOperation Plus (Leaf (Variable a2)) (Leaf (Constant a1)))
eval list (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Variable a2))) = if ((member a1 (fromList(list))))
                            then (if(member a2 (fromList(list)))
                                then (Leaf (Constant ((fromList(list) ! a1)+(fromList(list) !a2))))
                                else (BinaryOperation Plus (Leaf (Constant (fromList(list)! a1))) (Leaf (Variable a2))) )
                            else (if (member a2 (fromList(list)))
                                then (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Constant (fromList(list)!a2))))
                                else (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Variable a2))))                            
eval list (BinaryOperation Times (Leaf (Constant a1))(Leaf (Variable a2))) = if (member a2 (fromList(list))) == True
                            then (Leaf (Constant (a1*((fromList(list))! a2 ))))
                            else(BinaryOperation Times (Leaf (Constant a1)) (Leaf (Variable a2)))
eval list (BinaryOperation Times (Leaf (Variable a2))(Leaf (Constant a1))) = if (member a2 (fromList(list))) == True
                            then (Leaf (Constant (a1*((fromList(list))! a2 ))))
                            else(BinaryOperation Times (Leaf (Variable a2)) (Leaf (Constant a1)))
eval list (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Variable a2))) = if ((member a1 (fromList(list))))
                            then (if(member a2 (fromList(list)))
                                then (Leaf (Constant ((fromList(list) ! a1)+(fromList(list) !a2))))
                                else (BinaryOperation Times (Leaf (Constant (fromList(list)! a1))) (Leaf (Variable a2))) )
                            else (if (member a2 (fromList(list)))
                                then (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Constant (fromList(list)!a2))))
                                else (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Variable a2))))
eval list a=a

simple :: [(String,Int)] -> ExprV -> ExprV
simple list (UnaryOperation Minus a) = eval list (UnaryOperation Minus (simple list a))
simple list (BinaryOperation Plus a1 a2) = eval list  (BinaryOperation Plus (simple list a1) (simple list a2))
simple list (BinaryOperation Times a1 a2) = eval list (BinaryOperation Times (simple list a1) (simple list a2))
simple list a=a

listg :: (String,ExprV) -> [(String, Int)] -> [(String,Int)]
listg  (s,(Leaf (Constant a))) list = list++[(s,a)]
listg a list = list

folding :: [(String,Int)] -> [(String,ExprV)] -> [(String,ExprV)]
folding _ [] = []
folding list seq = [((fst (head seq)),(simple list (snd (head seq))))]++folding ((listg ((fst (head seq)),(simple list (snd (head seq))))) list ) (drop 1 seq)

foldAndPropagateConstants :: [(String, ExprV)] -> [(String, ExprV)]
foldAndPropagateConstants seq = folding [] seq

sub:: [ExprV] -> ExprV -> [ExprV]
sub list (BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Constant a2))) = list++[(BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Constant a2)))]
sub list (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Constant a2))) = list++[(BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Constant a2)))]
sub list (BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Variable a2))) = list++[(BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Variable a2)))]
sub list (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Variable a2))) = list++[(BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Variable a2)))]
sub list (BinaryOperation Plus a1 a2) = list++(sub list a1)++(sub list a2) 

sub list (BinaryOperation Times (Leaf (Constant a1)) (Leaf (Constant a2))) = list++[(BinaryOperation Times (Leaf (Constant a1)) (Leaf (Constant a2)))]
sub list (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Constant a2))) = list++[(BinaryOperation Times (Leaf (Variable a1)) (Leaf (Constant a2)))]
sub list (BinaryOperation Times (Leaf (Constant a1)) (Leaf (Variable a2))) = list++[(BinaryOperation Times (Leaf (Constant a1)) (Leaf (Variable a2)))]
sub list (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Variable a2))) = list++[(BinaryOperation Times (Leaf (Variable a1)) (Leaf (Variable a2)))]
sub list (BinaryOperation Times a1 a2) = list++(sub list a1)++(sub list a2)

sub list (UnaryOperation Minus (Leaf (Constant a))) = list++[(UnaryOperation Minus (Leaf (Constant a)))]
sub list (UnaryOperation Minus (Leaf (Variable a))) = list++[(UnaryOperation Minus (Leaf (Variable a)))]
sub list (UnaryOperation Minus a) = list++(sub list a)
sub list expr = list

count n (x:xs) = fromEnum (n == x) + count n xs
count _ []     = 0  

filtre:: [ExprV]->[ExprV]
filtre [] = []
filtre a = if ((count (head a) a)>1)
                    then [head a]++(filtre (Data.List.filter (/=(head a)) a))
                    else filtre (tail a)


hepsi:: Int ->[ExprV]->  [(String,ExprV)]
hepsi i list = common i (filtre list)
common:: Int->[ExprV]-> [(String,ExprV)]
common _  [] = []
common i coms = [("$"++(show i),(head coms))]++(common (i+1)(tail coms))

insertin:: (String, ExprV) -> ExprV ->ExprV
insertin (s,e) (BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Constant a2))) = if((BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Constant a2)))==e)
                                            then (Leaf (Variable s))
                                            else (BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Constant a2)))
insertin (s,e) (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Constant a2))) = if((BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Constant a2)))==e)
                                            then (Leaf (Variable s))
                                            else (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Constant a2)))
insertin (s,e) (BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Variable a2))) = if((BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Variable a2)))==e)
                                            then (Leaf (Variable s))
                                            else (BinaryOperation Plus (Leaf (Constant a1)) (Leaf (Variable a2)))
insertin (s,e) (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Variable a2))) = if((BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Variable a2)))==e)
                                            then (Leaf (Variable s))
                                            else (BinaryOperation Plus (Leaf (Variable a1)) (Leaf (Variable a2)))
insertin (s,e) (BinaryOperation Times (Leaf (Constant a1)) (Leaf (Constant a2))) = if((BinaryOperation Times (Leaf (Constant a1)) (Leaf (Constant a2)))==e)
                                            then (Leaf (Variable s))
                                            else (BinaryOperation Times (Leaf (Constant a1)) (Leaf (Constant a2)))
insertin (s,e) (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Constant a2))) = if((BinaryOperation Times (Leaf (Variable a1)) (Leaf (Constant a2)))==e)
                                            then (Leaf (Variable s))
                                            else (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Constant a2)))
insertin (s,e) (BinaryOperation Times (Leaf (Constant a1)) (Leaf (Variable a2))) = if((BinaryOperation Times (Leaf (Constant a1)) (Leaf (Variable a2)))==e)
                                            then (Leaf (Variable s))
                                            else (BinaryOperation Times (Leaf (Constant a1)) (Leaf (Variable a2)))
insertin (s,e) (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Variable a2))) = if((BinaryOperation Times (Leaf (Variable a1)) (Leaf (Variable a2)))==e)
                                            then (Leaf (Variable s))
                                            else (BinaryOperation Times (Leaf (Variable a1)) (Leaf (Variable a2)))
insertin (s,e) (UnaryOperation Minus (Leaf (Constant a))) = if (e==(UnaryOperation Minus (Leaf (Constant a))))
                                            then (Leaf (Variable s))
                                            else (UnaryOperation Minus (Leaf (Constant a)))
insertin (s,e) (UnaryOperation Minus (Leaf (Variable a))) = if (e==(UnaryOperation Minus (Leaf (Variable a))))
                                            then (Leaf (Variable s))
                                            else (UnaryOperation Minus (Leaf (Variable a)))
            
insertin (s,e) (BinaryOperation Plus a1 a2) = BinaryOperation Plus (insertin (s,e) a1) (insertin (s,e) a2)
insertin (s,e) (BinaryOperation Times a1 a2) = BinaryOperation Times (insertin (s,e) a1) (insertin (s,e) a2)
insertin (s,e) (UnaryOperation Minus a) = UnaryOperation Minus (insertin (s,e) a)
insertin (s,e) a = a
repo::[(String,ExprV)]->ExprV->ExprV
repo [] exp = exp
repo (l:ls) exp = repo ls (insertin l exp)


assign:: Int -> ExprV -> [(String,ExprV)]-> ([(String,ExprV)],ExprV)
assign i a arr= if(((repo (hepsi i (sub [] a)) a)==a)) then (arr,a)
else assign (i+(length arr)) (repo (hepsi (i+(length arr)) (sub [] a)) a) (arr++(hepsi (i+(length arr)) (sub [] a)))

assignCommonSubexprs :: ExprV -> ([(String, ExprV)], ExprV)
assignCommonSubexprs exp = assign 0 exp []

leafop :: Value -> [Int]
leafop (Constant a) = [a]
leafop (Variable a) = [0,1]

listadd :: [Int]->[Int]->[Int]->[Int]
listadd new (x:xs) (y:ys) = (new++[(x+y)])++listadd new xs ys
listadd _ [] [] = []
listadd new x [] = lastof new x
listadd new [] y = lastof new y 

lastof:: [Int] -> [Int]-> [Int]
lastof a b = a++b

mult a (x:xs)= (a*x):mult a xs
mult _ [] = []

listmul:: Int->[Int]->[Int]->[Int]->[Int]
listmul i new (x:xs) y = listadd new (mult x ((replicate i 0)++y)) (listmul (i+1) new xs y)
listmul _ _ [] _ = []


polleaf:: [Int]->ExprV -> [Int]
polleaf new (UnaryOperation Minus (Leaf a)) = (mult (-1) (leafop a))
polleaf new (UnaryOperation Minus a) = mult (-1) (polleaf new a)
polleaf new (BinaryOperation Plus (Leaf a) (Leaf b)) = listadd new (leafop a) (leafop b)
polleaf new (BinaryOperation Plus a (Leaf b)) = listadd new (leafop b) (polleaf new a)
polleaf new (BinaryOperation Plus (Leaf a) b) = listadd new (leafop a) (polleaf new b)
polleaf new (BinaryOperation Plus a b) = listadd new (polleaf new a) (polleaf new b)
polleaf new (BinaryOperation Times (Leaf a) (Leaf b)) = listmul 0 new (leafop a) (leafop b)
polleaf new (BinaryOperation Times a (Leaf b)) = listmul 0 new (leafop b) (polleaf new a)
polleaf new (BinaryOperation Times (Leaf a) b) = listmul 0 new (leafop a) (polleaf new b)
polleaf new (BinaryOperation Times a b) = listmul 0 new (polleaf new a) (polleaf new b)
polleaf _ _ = []

varım::ExprV->String
varım (Leaf (Variable a)) = a
varım (UnaryOperation Minus a) = varım a
varım (BinaryOperation op a b) = if((varım a)=="") then varım b 
                                                    else (varım a)   
varım a = ""
polim::Int->[Int]-> [(Int,Int)]
polim _ [] = []
polim i (x:xs) = if (x/=0) then (i,x):(polim (i+1) xs)
                            else polim (i+1) xs

recons:: Int-> Int -> String -> ExprV
recons a i str  | (i==0) = Leaf(Constant a)
                | (i==1 && a==1) = Leaf(Variable str)
                | (i==1 && a==(-1)) = UnaryOperation Minus (Leaf (Variable str))
                | (i==1 && a/=(-1) && a/=1) = BinaryOperation Times (Leaf (Constant a)) (Leaf (Variable str))
                | i>=2 =  (BinaryOperation Times (recons a (i-1) str) (Leaf (Variable str)))

summin:: Int->[(Int,Int)]->String-> ExprV
summin  i arr str   |i==0  = recons (snd (arr!!i)) (fst (arr!!i)) str
                    |(i==1) = BinaryOperation Plus (recons (snd(arr!!(i-1))) (fst (arr!!(i-1))) str ) (recons (snd (arr!!i)) (fst (arr!!i)) str)
                    |i>=1 = BinaryOperation Plus (summin (i-1) arr str) (recons (snd (arr!!i)) (fst (arr!!i)) str)
reducePoly :: ExprV -> ExprV
reducePoly expr = summin (length (polim 0 (polleaf [] expr))-1) (polim 0 (polleaf [] expr)) (varım expr)

-- an extra dummy variable, so as to not crash the GUI
notImpl :: ExprV
notImpl = Leaf $ Variable "Not Implemented"

