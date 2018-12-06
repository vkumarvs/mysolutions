-----------------------------------------------------------------------------
-- 
-- Module      :  Data.Algorithms.Palindromes.Palindromes
-- Copyright   :  (c) 2007 - 2012 Johan Jeuring
-- License     :  BSD3
--
-- Maintainer  :  johan@jeuring.net
-- Stability   :  experimental
-- Portability :  portable
--
-----------------------------------------------------------------------------


module Data.Algorithms.Palindromes.Palindromes 
       (longestPalindrome
       ,maximalPalindromes
       ,maximalPalindromesLengthAtLeast
       ,maximalPalindromesLengthBetween
       ,maximalPalindromesLengthAtMost
       ,palindromesLengthExact
       ,lengthLongestPalindrome
       ,lengthMaximalPalindromes
       ,longestTextPalindrome
       ,maximalTextPalindromesLengthAtLeast
       ,longestWordPalindrome
       ,maximalWordPalindromesLengthAtLeast
       ,palindromesAroundCentres
       ,myIsLetterC
       )  where
 
import Data.List (maximumBy,intercalate)
import Data.Word
import Data.Char (toLower,isPunctuation,isSpace,isControl)
import Data.Array (Array(),bounds,listArray,(!)) 
import qualified Data.ByteString as B
import Data.ByteString.Internal

-----------------------------------------------------------------------------
-- longestPalindrome
-----------------------------------------------------------------------------

-- | longestPalindrome returns the longest palindrome in a string.
longestPalindrome        :: B.ByteString -> String
longestPalindrome input  = 
  let (maxLength,pos)    =  maximumBy 
                              (\(l,_) (l',_) -> compare l l') 
                              (zip (palindromesAroundCentres input) [0..])    
  in showPalindrome input (maxLength,pos)

-----------------------------------------------------------------------------
-- maximalPalindromes
-----------------------------------------------------------------------------

-- | maximalPalindromes returns the maximal palindrome around each position
--   in a string. 
maximalPalindromes        :: B.ByteString -> String
maximalPalindromes input  = 
    intercalate "\n" 
  $ map (showPalindrome input) 
  $ zip (palindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalPalindromesLengthAtLeast
-----------------------------------------------------------------------------

-- | maximalPalindromesLengthAtLeast returns the longest palindrome around 
--   each position in a string. The integer argument is used to only show 
--   palindromes of length at least this integer.
maximalPalindromesLengthAtLeast          :: Int -> B.ByteString -> String
maximalPalindromesLengthAtLeast m input  = 
    intercalate "\n" 
  $ map (showPalindrome input) 
  $ filter ((m<=) . fst)
  $ zip (palindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalPalindromesLengthBetween
-----------------------------------------------------------------------------

-- | maximalPalindromesLengthBetween returns the longest palindrome around each 
--   position in a string. The integer arguments are used to only show palindromes
--   of length in between the specified lengths.
maximalPalindromesLengthBetween          :: Int -> Int -> B.ByteString -> String
maximalPalindromesLengthBetween m n input  = 
    intercalate "\n" 
  $ map (showPalindrome input) 
  $ filter (\(pl,_) -> pl >= m && pl <= n)
  $ zip (palindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalPalindromesLengthAtMost
-----------------------------------------------------------------------------

-- | maximalPalindromesLengthAtMost returns the longest palindrome around each 
--   position in a string. The integer arguments are used to only show palindromes
--   of length in between the specified lengths.
maximalPalindromesLengthAtMost          :: Int -> B.ByteString -> String
maximalPalindromesLengthAtMost m input  = 
    intercalate "\n" 
  $ map (showPalindrome input) 
  $ filter ((<=m) . fst)
  $ zip (palindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- palindromesLengthExact
-----------------------------------------------------------------------------

-- | palindromesLengthExact returns the longest palindrome around each 
--   position in a string. The integer arguments are used to only show palindromes
--   of length in between the specified lengths.
palindromesLengthExact          :: Int -> B.ByteString -> String
palindromesLengthExact m input  = 
    intercalate "\n" 
  $ map (showPalindrome input . \(_,p) -> (m,p)) 
  $ filter (\(l,_) -> m<=l && (odd l == odd m))
  $ zip (palindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- lengthLongestPalindrome
-----------------------------------------------------------------------------

-- | lengthLongestPalindrome returns the length of the longest palindrome in 
--   a string.
lengthLongestPalindrome  :: B.ByteString -> String
lengthLongestPalindrome  =  show . maximum . palindromesAroundCentres

-----------------------------------------------------------------------------
-- lengthLongestPalindromes
-----------------------------------------------------------------------------

-- | lengthMaximalPalindromes returns the lengths of the longest palindrome  
--   around each position in a string.
lengthMaximalPalindromes  :: B.ByteString -> String
lengthMaximalPalindromes  =  show . palindromesAroundCentres

-----------------------------------------------------------------------------
-- longestTextPalindrome
-----------------------------------------------------------------------------

-- | longestTextPalindrome returns the longest text palindrome in a string,
--   ignoring spacing, punctuation symbols, and case of letters.
longestTextPalindrome        :: B.ByteString -> String
longestTextPalindrome input  = 
  let textInput          =  B.map myToLower (B.filter myIsLetterW input)
      positionTextInput  =  listArrayl0 (B.findIndices myIsLetterW input)
  in  longestTextPalindromeBS input textInput positionTextInput 

longestTextPalindromeBS  ::  B.ByteString -> B.ByteString -> Array Int Int -> String
longestTextPalindromeBS input textInput positionTextInput  = 
  let (len,pos) = maximumBy 
                    (\(l,_) (l',_) -> compare l l') 
                    (zip (palindromesAroundCentres textInput) [0..])    
  in showTextPalindrome input positionTextInput (len,pos) 

-----------------------------------------------------------------------------
-- longestTextPalindromes
-----------------------------------------------------------------------------

-- | longestTextPalindromes returns the longest text palindrome around each
--   position in a string. The integer argument is used to only show palindromes
--   of length at least this integer.
maximalTextPalindromesLengthAtLeast          :: Int -> B.ByteString -> String
maximalTextPalindromesLengthAtLeast m input  = 
  let textInput          =  B.map myToLower (B.filter myIsLetterW input)
      positionTextInput  =  listArrayl0 (B.findIndices myIsLetterW input)
  in  intercalate "\n" 
    $ maximalTextPalindromesLengthAtLeastBS m textInput positionTextInput input

maximalTextPalindromesLengthAtLeastBS :: Int -> B.ByteString -> Array Int Int -> B.ByteString -> [String]
maximalTextPalindromesLengthAtLeastBS m textInput positionTextInput input  = 
    map (showTextPalindrome input positionTextInput) 
  $ filter ((m<=) . fst)
  $ zip (palindromesAroundCentres textInput) [0..]

-----------------------------------------------------------------------------
-- palindromesAroundCentres 
--
-- The function that implements the palindrome finding algorithm.
-- Used in all the above interface functions.
-----------------------------------------------------------------------------

-- | palindromesAroundCentres is the central function of the module. It returns
--   the list of lenghths of the longest palindrome around each position in a
--   string.
palindromesAroundCentres        :: B.ByteString -> [Int]
palindromesAroundCentres input  =  reverse $ extendPalindrome input 0 0 []

extendPalindrome :: B.ByteString -> Int -> Int -> [Int] -> [Int]
extendPalindrome input rightmost currentPalindrome currentMaximalPalindromes 
  | rightmost > last
      -- reached the end of the array
      =  finalPalindromes currentPalindrome currentMaximalPalindromes (currentPalindrome:currentMaximalPalindromes)
  | rightmost-currentPalindrome == first ||
    B.index input rightmost /= B.index input (rightmost-currentPalindrome-1)
    -- the current palindrome extends to the start of the array, or
    -- it cannot be extended 
      =  moveCenter input rightmost (currentPalindrome:currentMaximalPalindromes) currentMaximalPalindromes currentPalindrome 
  | otherwise                                           
      -- the current palindrome can be extended
      =  extendPalindrome input (rightmost+1) (currentPalindrome+2) currentMaximalPalindromes      
  where  first = 0
         last  = B.length input - 1

moveCenter :: B.ByteString -> Int -> [Int] -> [Int] -> Int -> [Int]
moveCenter input rightmost currentMaximalPalindromes previousMaximalPalindromes centreDistance
  | centreDistance == 0
      -- the last centre is on the last element: try to extend the tail of length 1
      =  extendPalindrome input (rightmost+1) 1 currentMaximalPalindromes
  | centreDistance-1 == head previousMaximalPalindromes
      -- the previous element in the centre list reaches exactly to the end of the last 
      -- tail palindrome use the mirror property of palindromes to find the longest tail palindrome
      =  extendPalindrome input rightmost (head previousMaximalPalindromes) currentMaximalPalindromes
  | otherwise
      -- move the centres one step add the length of the longest palindrome to the centres
      =  moveCenter input rightmost (min (head previousMaximalPalindromes) (centreDistance-1):currentMaximalPalindromes) (tail previousMaximalPalindromes) (centreDistance-1)

finalPalindromes :: Int -> [Int] -> [Int] -> [Int]
finalPalindromes nrOfCenters previousMaximalPalindromes currentMaximalPalindromes  
  | nrOfCenters == 0
      =  currentMaximalPalindromes
  | nrOfCenters > 0
      =  finalPalindromes (nrOfCenters-1) (tail previousMaximalPalindromes) (min (head previousMaximalPalindromes) (nrOfCenters-1):currentMaximalPalindromes)
  | otherwise  =  error "finalCentres: input < 0"               

-----------------------------------------------------------------------------
-- longestWordPalindromes
-----------------------------------------------------------------------------

-- | longestWordPalindromes returns the longest word palindrome around each
--   position in a string. The integer argument is used to only show 
--   palindromes of length at least this integer.
maximalWordPalindromesLengthAtLeast          :: Int -> B.ByteString -> String
maximalWordPalindromesLengthAtLeast m input  = 
  let textInput          =  B.map myToLower (B.filter myIsLetterW input)
      positionTextInput  =  listArrayl0 (B.findIndices myIsLetterW input)
  in  intercalate "\n" 
    $ maximalWordPalindromesLengthAtLeastBS m input textInput positionTextInput 


maximalWordPalindromesLengthAtLeastBS :: Int -> B.ByteString -> B.ByteString -> Array Int Int -> [String]
maximalWordPalindromesLengthAtLeastBS m input textInput positionTextInput = 
    map (showTextPalindrome input positionTextInput) 
  $ filter ((m<=) . fst)
  $ zip (wordPalindromesAroundCentres input textInput positionTextInput) [0..]

-- | longestWordPalindrome returns the longest text palindrome preceded and 
--   followed by non-letter symbols (if any). 	
longestWordPalindrome :: B.ByteString -> String
longestWordPalindrome input =
  let textInput          =  B.map myToLower (B.filter myIsLetterW input)
      positionTextInput  =  listArrayl0 (B.findIndices myIsLetterW input)
      (len,pos)          =  maximumBy 
                              (\(w,_) (w',_) -> compare w w') 
                              (zip (wordPalindromesAroundCentres input textInput positionTextInput) [0..])    
  in showTextPalindrome input positionTextInput (len,pos)

-----------------------------------------------------------------------------
-- wordPalindromesAroundCentres 
--
-- This is the function palindromesAroundCentres, extended with the longest
-- word palindromes around each centre.
-----------------------------------------------------------------------------

-- | wordPalindromesAroundCentres returns the same lengths of palindromes as 
--   palindromesAroundCentres, but at the same time also the length of the 
--   longest word palindromes around the centres.
wordPalindromesAroundCentres  ::  B.ByteString -> B.ByteString -> Array Int Int -> [Int]
wordPalindromesAroundCentres input textInput positionTextInput  =   
  let tfirst = 0
  in reverse $ map (head . snd) $ extendTailWord input textInput positionTextInput [] tfirst (0,[0]) 

-- extendTailWordold textInput positionTextInput input n current centres = extendTailWord input textInput positionTextInput centres n current

extendTailWord :: B.ByteString -> B.ByteString -> Array Int Int -> [(Int,[Int])] -> Int -> (Int,[Int]) -> [(Int,[Int])] 
extendTailWord input textInput positionTextInput centres n current@(currentTail,currentTailWords)  
  | n > alast                          =  
      -- reached the end of the text input array                                     
      finalWordCentres input textInput positionTextInput (current:centres) currentTail centres (1+length centres)
  | n-currentTail == afirst            =  
      -- the current longest tail palindrome extends to the start of the text input array
      extendWordCentres input textInput positionTextInput (current:centres) n centres currentTail
  | B.index textInput n == B.index textInput (n-currentTail-1)     =  
      -- the current longest tail palindrome can be extended
      -- check whether or not the extended palindrome is a wordpalindrome
      if surroundedByPunctuation (positionTextInput!(n-currentTail-1)) (positionTextInput!n) input
      then extendTailWord input textInput positionTextInput centres (n+1) (currentTail+2,currentTail+2:currentTailWords) 
      else extendTailWord input textInput positionTextInput centres (n+1) (currentTail+2,currentTailWords)       
  | otherwise                          =  
      -- the current longest tail palindrome cannot be extended                 
      extendWordCentres input textInput positionTextInput (current:centres) n centres currentTail
  where  (afirst,alast)  =  (0,B.length textInput -1)

extendWordCentres :: B.ByteString -> B.ByteString -> Array Int Int -> [(Int,[Int])] -> Int -> [(Int,[Int])] -> Int -> [(Int,[Int])]
extendWordCentres input textInput positionTextInput centres n tcentres centreDistance
  | centreDistance == 0                =  
      -- the last centre is on the last element: 
      -- try to extend the tail of length 1
      if surroundedByPunctuation (positionTextInput!n) (positionTextInput!n) input
      then extendTailWord input textInput positionTextInput centres (n+1) (1,[1,0]) 
      else extendTailWord input textInput positionTextInput centres (n+1) (1,[0]) 
  | centreDistance-1 == fst (head tcentres)  =  
      -- the previous element in the centre list 
      -- reaches exactly to the end of the last 
      -- tail palindrome use the mirror property 
      -- of palindromes to find the longest tail 
      -- palindrome
      let (currentTail,oldWord:oldWords) = head tcentres
      in if surroundedByPunctuation (positionTextInput!(n-currentTail)) (positionTextInput!(n-1)) input
         then if oldWord == currentTail
		      then extendTailWord input textInput positionTextInput centres n (head tcentres) 
		      else extendTailWord input textInput positionTextInput centres n (currentTail,currentTail:oldWord:oldWords) 
		 else if oldWord == currentTail && oldWord > 0
			  then extendTailWord input textInput positionTextInput centres n (currentTail, tail (snd (head tcentres)))  
		      else extendTailWord input textInput positionTextInput centres n (head tcentres) 
  | otherwise                          =  
      -- move the centres one step
      -- add the length of the longest palindrome 
      -- to the centres
      let newTail   =  min (fst (head tcentres)) (centreDistance-1)
          oldWord   =  head (snd (head tcentres))
          newWords | oldWord < newTail  
	                   = if surroundedByPunctuation (positionTextInput!(n-newTail+1)) (positionTextInput!n) input
		                 then newTail:snd (head tcentres) 
		                 else snd (head tcentres) 
		            | null (tail (snd (head tcentres)))
			           = snd (head tcentres) 
			        | otherwise 
			           = tail (snd (head tcentres))
      in extendWordCentres input textInput positionTextInput ((newTail,newWords):centres) n (tail tcentres) (centreDistance-1)

finalWordCentres :: B.ByteString -> B.ByteString -> Array Int Int -> [(Int,[Int])] -> Int -> [(Int,[Int])] -> Int -> [(Int,[Int])]
finalWordCentres input textInput positionTextInput centres n tcentres mirrorPoint 
  | n == 0     =  centres
  | n > 0      =  let tlast                       =  B.length textInput - 1
                      (oldTail,oldWord:oldWords)  =  head tcentres
                      newTail                     =  min oldTail (n-1)
                      newWord                     =  min oldWord (n-1)
                      tailFirstMirror             =  min tlast (div (mirrorPoint - newTail) 2)
                      tailLastMirror              =  min tlast (if odd newTail then div (mirrorPoint + newTail) 2 else div (mirrorPoint + newTail) 2 - 1)
                      wordFirstMirror             =  min tlast (div (mirrorPoint - newWord) 2)
                      wordLastMirror              =  min tlast (if odd newWord then div (mirrorPoint + newTail) 2 else div (mirrorPoint + newTail) 2 - 1)
                      newWords | surroundedByPunctuation (positionTextInput!tailFirstMirror) (positionTextInput!tailLastMirror) input
		                                             =    if newWord == newTail
	                                                      then newTail:oldWords
		                                                  else newTail:oldWord:oldWords
		                       | surroundedByPunctuation (positionTextInput!wordFirstMirror) (positionTextInput!wordLastMirror) input ||
			                     null oldWords       =    newWord:oldWords
			                   | otherwise           =    oldWords
                 in  finalWordCentres input textInput positionTextInput ((newTail,newWords):centres) (n-1) (tail tcentres)  (mirrorPoint+1)
  | otherwise  = error "finalWordCentres: input < 0"        

-----------------------------------------------------------------------------
-- Showing palindromes and other text related functionality
-----------------------------------------------------------------------------

showPalindrome :: B.ByteString -> (Int,Int) -> String
showPalindrome input (len,pos) = 
  let startpos = pos `div` 2 - len `div` 2
  in show $ B.take len $ B.drop startpos input 

showTextPalindrome :: B.ByteString -> Array Int Int -> (Int,Int) -> String
showTextPalindrome input positionTextInput (len,pos) = 
  let startpos   =  pos `div` 2 - len `div` 2
      endpos     =  if odd len 
                    then pos `div` 2 + len `div` 2 
                    else pos `div` 2 + len `div` 2 - 1
      (pfirst,plast) = bounds positionTextInput
      (ifirst,ilast) = (0,1 + B.length input)
  in  if endpos < startpos
      then []
      else let start      =  if startpos > pfirst
                             then (positionTextInput!(startpos-1))+1
                             else ifirst 
               end        =  if endpos < plast
                             then (positionTextInput!(endpos+1))-1
                             else ilast
           in  show (B.take (end-start+1) (B.drop start input))

{- Using this code instead of the last else above shows text palindromes without 
   all punctuation around it. Right now this punctuation is shown.

      else let start      =  positionArray!!!startpos
               end        =  positionArray!!!endpos
-}

-- For palindromes in strings, punctuation, spacing, and control characters
-- are often ignored

myIsLetterW     ::  Word8 -> Bool
myIsLetterW c'  =   not (isPunctuation c)
                &&  not (isControl c)
                &&  not (isSpace c)
  where c = w2c c'

myIsLetterC    ::  Char -> Bool
myIsLetterC c  =   not (isPunctuation c)
               &&  not (isControl c)
               &&  not (isSpace c)

myToLower  :: Word8 -> Word8
myToLower  = c2w . toLower . w2c

surroundedByPunctuation :: Int -> Int -> B.ByteString -> Bool
surroundedByPunctuation begin end input 
  | begin > afirst  && end < alast   =  not (myIsLetterW (B.index input (begin-1))) && not (myIsLetterW (B.index input (end+1)))
  | begin <= afirst && end < alast   =  not (myIsLetterW (B.index input (end+1)))
  | begin <= afirst && end >= alast  =  True
  | begin > afirst  && end >= alast  =  not (myIsLetterW (B.index input (begin-1)))
  | otherwise                        =  error "surroundedByPunctuation"
  where (afirst,alast) = (0,B.length input - 1)

-----------------------------------------------------------------------------
-- Array utils
-----------------------------------------------------------------------------

listArrayl0         :: [a] -> Array Int a
listArrayl0 string  =  listArray (0,length string - 1) string
