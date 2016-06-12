import System.IO
import System.Exit
import Data.List
import Control.Monad
import Data.String

data Tree a = Nil | Node a  a (Tree a) (Tree a) deriving (Show, Read, Eq)
--                     info type child  sibling

data SymTab b = Empty | Symbol [String] (Tree String) deriving (Show, Read, Eq)
--                         values  complex

getPrim str = do
	if (length str == 0)
		then do
		[]
	else if ((str!!0 >= 'a' && str!!0 <= 'z') || (str!!0 >= 'A' && str!!0 <= 'Z') || ( str!!0 >= '0' && str!!0 <= '9'))
		then do
		[str!!0] ++ getPrim (drop 1 str)
	else do
		[]

dropPrim str = do
	if (length str == 0)
		then do
		[]
	else if ((str!!0 >= 'a' && str!!0 <= 'z') || (str!!0 >= 'A' && str!!0 <= 'Z') || ( str!!0 >= '0' && str!!0 <= '9'))
		then do
		[] ++ dropPrim (drop 1 str)
	else do
		str

varName str = do
	if (length str == 0)
		then do
		""
	else if ((str!!0 >= 'a' && str!!0 <= 'z') || (str!!0 >= 'A' && str!!0 <= 'Z') || ( str!!0 >= '0' && str!!0 <= '9'))
		then do
		[(str!!0)] ++ varName (drop 1 str)
	else do
		""

dropVarname str = do
	if (length str == 0)
		then do
		""
	else if ((str!!0 >= 'a' && str!!0 <= 'z') || (str!!0 >= 'A' && str!!0 <= 'Z') || ( str!!0 >= '0' && str!!0 <= '9'))
		then do
		"" ++ dropVarname (drop 1 str)
	else do
		str

tokenize :: String -> [String]
tokenize str = do
	if (length str == 0)
		then do
		[]
	else if (str!!0 == ' ' || str!!0 == '\t')
		then do
		let s = drop 1 str
		tokenize s
	else if (str!!0 == '(' || str!!0 == ')' || str!!0 == ',' || str!!0 == '[' || str!!0 == ']' || str!!0 == '&' )
		then do
			let s = drop 1 str
			[(str!!0)] : tokenize s
	else if (str!!0 == '-')
		then do
		if ((length str > 1) && str!!1 == '>')
			then do
			let s = drop 2 str
			"->" : tokenize s
		else do
			["ERR"]
	else if (str!!0 == '`')
		then do
			let s = drop 1 str
			if ((length s > 0) && ((s!!0 >= 'a' && s!!0 <= 'z') || (s!!0 >= 'A' && s!!0 <= 'Z')))
				then do
				let t = "`" ++ varName s
				t : tokenize (dropVarname s)
			else do
				["ERR"]
	else if ((str!!0 >= 'a' && str!!0 <= 'z') || (str!!0 >= 'A' && str!!0 <= 'Z'))
		then do
			let s = getPrim str
			let t = dropPrim str
			if (s == "int" || s == "real" || s == "str")
				then do
				[s] ++ tokenize t
			else do
				["ERR"]
	else do
		 ["ERR"]


getInfo (Node info ty child sibling) = info


getType (Node info ty child sibling) = ty


getChild (Node info ty child sibling) = child


getSib (Node info ty child sibling) = sibling


verifyArgs inp = do -- (Nil,[])
		if (inp!!0 == ")")
			then do
			(Nil, inp)
		else do
			let (c1,arr) = categorize inp
			if ((length arr > 0) && (arr!!0 == ","))
				then do
				let arr1 = drop 1 arr
				let (c2,arr2) = verifyArgs arr1
				((Node (getInfo c1) (getType c1) (getChild c1) c2), arr2)
			else if ((length arr > 0))
				then do
				(c1, arr)
			else do
				((Node "ERR" "-" Nil Nil), [])


insertSib c1 c2 = do
	if ( c1 == Nil)
		then do
		Node "" "4" Nil c2
	else do
		let a = getInfo c1
		let b = getType c1
		let child = getChild c1
		let sib = getSib c1
		if ( sib == Nil )
			then do
			Node a b child c2
		else do
			Node a b child (insertSib sib c2)


verifyFunc inp = do -- (Nil, [])
		let arr = drop 1 inp
		let (c1, arr1) = verifyArgs arr
		if ((length arr1 > 0) && (arr1!!0 == ")"))
			then do
			let arr2 = drop 1 arr1
			if (arr2!!0 == "->")
				then do
				let arr3 = drop 1 arr2
				let (c2,arr4) = categorize arr3
				let c3 = insertSib c1 c2
				((Node "()" "2" c3 Nil), arr4)
			else do
				((Node "ERR" "-" Nil Nil), [])
		else do
			((Node "ERR" "-" Nil Nil), [])


verifyList inp = do --(Nil, [])
		let arr = drop 1 inp
		let (c1,arr1) = categorize arr
		if ((length arr1 > 0) && (arr1!!0 == "]"))
			then do
			let arr2 = drop 1 arr1
			let c2 = Node "[]" "3" c1 Nil
			(c2, arr2)
		else do
			((Node "ERR" "-" Nil Nil), [])


categorize inp = do
		if (length inp > 0)
			then do
			if (((inp!!0)!!0) == '`')
				then do
				let c1 = Node (inp!!0) "4" Nil Nil
				(c1,(drop 1 inp))
			else if ((inp!!0) == "int" || (inp!!0) == "real" || (inp!!0) == "str")
				then do
				let c1 = Node (inp!!0) "1" Nil Nil
				(c1, (drop 1 inp))
			else if ((inp!!0) == "(")
				then do
				verifyFunc inp
			else if ((inp!!0) == "[")
				then do
				verifyList inp
			else do
				((Node "ERR" "-" Nil Nil), [])
		else do
			((Node "ERR" "-" Nil Nil), [])


constraintGen arr = do
		let (c1,arr1) = categorize arr
		if ((length arr1) > 0 && (arr1!!0 == "&"))
			then do
			let arr2 = drop 1 arr1
			let (c2,arr3) = categorize arr2
			if ((length arr3) == 0)
				then do
				(c1,c2)
			else do
				((Node "ERR" "-" Nil Nil), (Node "ERR" "-" Nil Nil))
		else do
			((Node "ERR" "-" Nil Nil), (Node "ERR" "-" Nil Nil))


checkErr (Node a b child sib) = a == "ERR"


isPrim (Node a b child sib) = b == "1"


isFunc (Node a b child sib) = b == "2"


isList (Node a b child sib) = b == "3"


isVar (Node a b child sib) = b == "4"


getValues (Symbol values comp) = values


getComp :: SymTab [[Char]] -> Tree [Char]
getComp (Symbol values comp) = comp


findVal values a = do
		if (length values == 0)
			then do
			-1
		else do
			let v1 = head values
			if (v1 == a)
				then do
				1
			else do
				findVal (tail values) a

findSym (Node a b child sib) tab = do
				if (length tab == 0)
					then do
					Empty
				else do
					let st = head tab
					let values = getValues st
					let comp = getComp st
					let val = findVal values a
					if ( val == 1)
						then do
						Symbol values comp
					else do
						findSym (Node a b child sib) (tail tab)


findAndInsert a2 a1 tab = do
			if (length tab == 0)
				then do
				tab
			else do
				let st = head tab
				let values = getValues st
				let comp = getComp st
				let val = findVal values a2
				if ( val == 1)
					then do
					let vals = a1: values
					let newSym = Symbol vals comp
					newSym : (tail tab)
				else do
					st : findAndInsert a2 a1 (tail tab)


checkRecursiveFunc origNode chkNode firstTime tab = do
				if (chkNode == Nil)
					then do
					False
				else do
					if (isRecursive origNode chkNode firstTime tab)
						then do
						True
					else do
						let sib = getSib chkNode
						checkRecursiveFunc origNode sib firstTime tab


isRecursiveSub origNode chkNode firstTime tab = do
					if (isFunc chkNode)
						then do
						checkRecursiveFunc origNode chkNode firstTime tab
					else if (isList chkNode)
						then do
						isRecursive origNode (getChild chkNode) firstTime tab
					else if (isVar chkNode)
						then do
						let s1 = findSym chkNode tab
						if (s1 == Empty)
							then do
							False
						else if (getComp s1 == Nil)
							then do
							False
						else do
							isRecursive origNode (getComp s1) firstTime tab
					else do
						False
		


isRecursive origNode chkNode firstTime tab= do
		if (chkNode == Nil)
			then do
			False
		else do
			if (firstTime)
				then do
				let ft = False
				isRecursiveSub origNode chkNode ft tab
			else do
				if (origNode == chkNode)
					then do
					True
				else do
					isRecursiveSub origNode chkNode firstTime tab


appendValues value s2 tab = do
			let st = head tab
			let values = getValues st
			let valuesIns = getValues s2
			let comp = getComp st
			let val = findVal values value
			if (val == 1)
				then do
				let newVals = values ++ valuesIns
				let newSt = Symbol newVals comp
				newSt : (tail tab)
			else do
				st : appendValues value s2 (tail tab)


equalSet values1 values2 = do
			if (length values1 == 0)
				then do
				True
			else do
				let val1 = head values1
				let val2 = head values2
				if ( val1 == val2 )
					then do
					equalSet (tail values1) (tail values2)
				else do
					False


removeSym s2 tab = do
			let st = head tab
			let values = getValues st
			let values1 = getValues s2
			if ((length values) == (length values1))
				then do
				if (equalSet values values1)
					then do
					tail tab
				else do
					st : removeSym s2 (tail tab)
			else do
				st : removeSym s2 (tail tab)


insertComp sym node tab = do
			let st = head tab
			let values = getValues st
			let values1 = getValues sym
			let val = findVal values (values1!!0)
			if (val == 1)
				then do
				let newSym = Symbol values1 node
				newSym : (tail tab)
			else do
				st : insertComp sym node (tail tab)


isNotNil c1 = do
	if ( c1 == Nil )
		then do
		False
	else do
		True


compareFuncs c1 c2 tab = do
		let a1 = getInfo c1
		let a2 = getInfo c2
		let b1 = getType c1
		let b2 = getType c2
		let child1 = getChild c1
		let child2 = getChild c2
		let sib1 = getSib c1
		let sib2 = getSib c2
		if ( c1 == Nil && c2 == Nil)
			then do
			(True, tab)
		else do
			let (bool, tab1) = unite c1 c2 tab
			if ( bool )
				then do
				if ( (sib1 == Nil && (isNotNil sib2)) || ((isNotNil sib1) && sib2 == Nil))
					then do
					(False, tab1)
				else do
					compareFuncs sib1 sib2 tab1
			else do
				(False, tab1)


-- 1 = Primitive
-- 2 = Function
-- 3 = List
-- 4 = Variable
unite c1 c2 tab = do
		let a1 = getInfo c1
		let a2 = getInfo c2
		let b1 = getType c1
		let b2 = getType c2
		let child1 = getChild c1
		let child2 = getChild c2
		let sib1 = getSib c1
		let sib2 = getSib c2
		if (c1 == Nil || c2 == Nil)
			then do
			(False, tab)
		else if ((isPrim c1) && (isPrim c2))
			then do
			if ((getInfo c1) == (getInfo c2))
				then do
				(True, tab)
			else do
				(False, tab)
		else if ((isFunc c1) && (isFunc c2))
			then do
			compareFuncs child1 child2 tab
		else if ((isList c1) && (isList c2))
			then do
			unite child1 child2 tab
		else if ((isVar c1) && (isVar c2))
			then do
			let s1 = findSym c1 tab
			let s2 = findSym c2 tab
			let comp1 = getComp s1
			let comp2 = getComp s2
			if ( s1 == Empty)
				then do
				if (s2 == Empty)
					then do
					let val = a1 : a2 : []
					let s3 = Symbol val Nil
					(True, s3 : tab)
				else do
					let tab1 = findAndInsert a2 a1 tab
					if (isRecursive comp2 comp2 True tab1)
						then do
						(False, tab1)
					else do
						(True, tab1)
			else if ( comp1 == Nil )
				then do
				if (s2 == Empty)
					then do
					let tab1 = findAndInsert a1 a2 tab
					(True, tab1)
				else if (comp2 == Nil)
					then do
					let tab1 = removeSym s2 tab
					let val = getValues s1
					let tab2 = appendValues (val!!0) s2 tab1
					(True, tab2)
				else do
					let tab1 = removeSym s1 tab
					let val = getValues s2
					let tab2 = appendValues (val!!0) s1 tab1
					if (isRecursive comp2 comp2 True tab2)
						then do
						(False, tab2)
					else do
						(True, tab2)
			else if ( s2 == Empty )
				then do
				let tab1 = findAndInsert a1 a2 tab
				if (isRecursive comp1 comp1 True tab1)
					then do
					(False, tab1)
				else do
					(True, tab1)
			else if ( comp2 == Nil)
				then do
				let tab1 = removeSym s2 tab
				let val = getValues s1
				let tab2 = appendValues (val!!0) s2 tab1
				if (isRecursive c2 c2 True tab2)
					then do
					(False, tab2)
				else do
					(True, tab2)
			else do
				unite comp1 comp2 tab
		else if ((isVar c1) && (isPrim c2 || isFunc c2 || isList c2))
			then do
			let s1 = findSym c1 tab
			let comp1 = getComp s1
			if ( s1 == Empty )
				then do
				let newSym = Symbol (a1:[]) c2
				let tab1 = newSym : tab
				if (isRecursive c2 c2 True tab1)
					then do
					(False, tab1)
				else do
					(True, tab1)
			else if ( comp1 == Nil )
				then do
				let tab1 = insertComp s1 c2 tab
				if (isRecursive c2 c2 True tab1)
					then do
					(False, tab1)
				else do
					(True, tab1)
			else do
				unite comp1 c2 tab
		else if ((isPrim c1 || isFunc c1 || isList c1) && (isVar c2))
			then do 
			let s2 = findSym c2 tab
			let comp2 = getComp s2
			if ( s2 == Empty )
				then do
				let newSym = Symbol (a2:[]) c1
				let tab1 = newSym : tab
				if (isRecursive c1 c1 True tab1)
					then do
					(False, tab1)
				else do
					(True, tab1)
			else if (comp2 == Nil)
				then do
				let tab1 = insertComp s2 c1 tab
				if (isRecursive c1 c1 True tab1)
					then do
					(False, tab1)
				else do
					(True, tab1)
			else do
				unite c1 comp2 tab
		else if (((isFunc c1) && (isList c2 || isPrim c2)) || (isPrim c1 && (isFunc c2 || isList c2)) || (isList c1 && (isPrim c2 || isFunc c2)))
			then do
			(False, tab)
		else do
			(False, tab)


displayFunc (Node a b child sib) tab = do
				if ( sib == Nil)
					then do
					let c1 = Node a b child sib
					putStr ") ->"
					display c1 tab
				else do
					let c1 = Node a b child sib
					display c1 tab
					let sib1 = getSib sib
					if ( sib1 == Nil)
						then do
						putStr ""
						displayFunc sib tab
					else do
						putStr ", "
						displayFunc sib tab


display (Node a b child sib) tab = do
				if ( b == "2")
					then do
					putStr "("
					displayFunc child tab
				else if ( b == "3")
					then do
					putStr "["
					display child tab
					putStr "]"
				else if ( b == "4")
					then do
					let s1 = findSym (Node a b child sib) tab
					if ( s1 == Empty )
						then do
						putStr (a)
					else do
						let comp = getComp s1
						if ( comp == Nil )
							then do
							let val = getValues s1
							putStr (val!!0)
						else do
							display comp tab
				else do
					putStr (a)


--Edit Remove symbol function
unify table = do
	-- Add condition for empty input
	a <- getLine
	if (a == "QUIT")
		then do  
		exitSuccess 
	else do
		let x = tokenize a
		if ((["ERR"] `intersect` x)== ["ERR"])
			then do 
			putStrLn "ERR"
			exitSuccess
		else do 
			let (c1,c2) = constraintGen x
			if ((checkErr c1) || (checkErr c2))
				then do 
				putStrLn "ERR"
				exitSuccess
			else do 
				let (bool, tab) = unite c1 c2 table
				if (bool)
					then do
					display c1 tab
					putStrLn ""
					unify tab
				else do
					putStrLn "BOTTOM"
					exitSuccess

main = do
	let table = [] 
	unify table
