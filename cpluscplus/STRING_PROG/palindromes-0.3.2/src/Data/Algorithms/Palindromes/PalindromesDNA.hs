-- >let input = B.pack (map c2w "AATAATT")
-- >oddDNAPalindromesAroundCentres input

-----------------------------------------------------------------------------
-- 
-- Module      :  Data.Algorithms.Palindromes.PalindromesDNA
-- Copyright   :  (c) 2012 Johan Jeuring
-- License     :  BSD3
--
-- Maintainer  :  johan@jeuring.net
-- Stability   :  experimental
-- Portability :  portable
--
-----------------------------------------------------------------------------

module Data.Algorithms.Palindromes.PalindromesDNA
       (maximalEvenPalindromesDNA
       ,maximalEvenPalindromesLengthAtLeastDNA
       ,maximalEvenPalindromesLengthBetweenDNA
       ,maximalEvenPalindromesLengthAtMostDNA
       ,evenPalindromesLengthExactDNA
       ,maximalOddPalindromesDNA
       ,maximalOddPalindromesLengthAtLeastDNA
       ,maximalOddPalindromesLengthBetweenDNA
       ,maximalOddPalindromesLengthAtMostDNA
       ,oddPalindromesLengthExactDNA
       ,negateDNA
       ,isDNASymbol
       )  where
 
import Data.List (intercalate)
import Data.Char (toUpper)
import Data.Word (Word8)
import qualified Data.ByteString as B
import Data.ByteString.Internal as BI

-----------------------------------------------------------------------------
-- maximalEvenPalindromesDNA
-----------------------------------------------------------------------------

-- | maximalEvenPalindromesDNA returns the maximal even-length DNA palindrome
--   around each position in a string. 
maximalEvenPalindromesDNA        :: B.ByteString -> String
maximalEvenPalindromesDNA input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input) 
  $ zip (evenDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalEvenPalindromesLengthAtLeastDNA
-----------------------------------------------------------------------------

-- | maximalEvenPalindromesLengthAtLeastDNA returns the maximal even-length 
--   DNA palindrome around each position in a string. The integer argument is
--   used to only show palindromes of length at least this integer.
maximalEvenPalindromesLengthAtLeastDNA          :: Int -> B.ByteString -> String
maximalEvenPalindromesLengthAtLeastDNA m input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input) 
  $ filter ((m<=) . fst)
  $ zip (evenDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalEvenPalindromesLengthBetweenDNA
-----------------------------------------------------------------------------

-- | maximalEvenPalindromesLengthBetweenDNA returns the maximal even-length 
--   palindrome around each position in a string. The integer arguments are 
--   used to only show (or cut off to) palindromes of length in between the 
--   specified lengths.
maximalEvenPalindromesLengthBetweenDNA          :: Int -> Int -> B.ByteString -> String
maximalEvenPalindromesLengthBetweenDNA m n input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input) 
  $ filter (\(pl,_) -> m <= pl && pl <= n)
  $ zip (evenDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalEvenPalindromesLengthAtMostDNA
-----------------------------------------------------------------------------

-- | maximalEvenPalindromesLengthAtMostDNA returns the maximal even-length 
--   palindrome around each position in a string. The integer argument is 
--   used to only show (or cut off to) palindromes of length at most the 
--   specified length.
maximalEvenPalindromesLengthAtMostDNA          :: Int -> B.ByteString -> String
maximalEvenPalindromesLengthAtMostDNA m input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input) 
  $ filter ((<=m) . fst)
  $ zip (evenDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- evenPalindromesLengthExactDNA
-----------------------------------------------------------------------------

-- | evenPalindromesLengthExactDNA returns the maximal even-length palindrome 
--   around each position in a string. The integer argument is used to only 
--   show (or cut off to) the palindrome of exactly this length.
evenPalindromesLengthExactDNA          :: Int -> B.ByteString -> String
evenPalindromesLengthExactDNA m input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input . \(_,p) -> (m,p)) 
  $ filter (\(l,_) -> m<=l && (odd l == odd m))
  $ zip (evenDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- evenDNAPalindromesAroundCentres 
--
-- The function that implements the palindrome finding algorithm.
-- Used in all the above interface functions.
-----------------------------------------------------------------------------

-- | evenDNAPalindromesAroundCentres is the central function of the module. It 
--   returns the list of lenghths of maximal even-length palindromes around 
--   each position in a string.
evenDNAPalindromesAroundCentres        :: B.ByteString -> [Int]
evenDNAPalindromesAroundCentres input  =  reverse $ evenExtendTail input 0 0 []

evenExtendTail :: B.ByteString -> Int -> Int -> [Int] -> [Int]
evenExtendTail input n currentTail centres 
  | n > inputlast                                            =  evenFinalCentres currentTail centres (currentTail:centres)
      -- reached the end of the array                                     
  | n-currentTail == inputfirst                              =  evenExtendCentres input n (currentTail:centres) centres currentTail 
      -- the current longest tail palindrome extends to the start of the array
  | B.index input n `eqDNA` B.index input (n-currentTail-1)  =  evenExtendTail input (n+1) (currentTail+2) centres      
      -- the current longest tail palindrome can be extended
  | otherwise                                                =  evenExtendCentres input n (currentTail:centres) centres currentTail
      -- the current longest tail palindrome cannot be extended
  where  inputfirst = 0
         inputlast  = B.length input - 1

evenExtendCentres :: B.ByteString -> Int -> [Int] -> [Int] -> Int -> [Int]
evenExtendCentres input n centres tcentres centreDistance
  | centreDistance == 0                =  evenExtendTail input (n+1) 0 centres
      -- the last centre is on the last element: try to extend the tail of length 1
  | centreDistance-2 == head tcentres  =  evenExtendTail input n (head tcentres) centres
      -- the previous element in the centre list reaches exactly to the end of the last 
      -- tail palindrome use the mirror property of palindromes to find the longest tail palindrome
  | otherwise                          =  evenExtendCentres input n (min (head tcentres) (centreDistance-2):centres) (tail tcentres) (centreDistance-2)
      -- move the centres one step add the length of the longest palindrome to the centres

evenFinalCentres :: Int -> [Int] -> [Int] -> [Int]
evenFinalCentres n tcentres centres  
  | n == 0     =  centres
  | n > 0      =  evenFinalCentres (n-2) (tail tcentres) (min (head tcentres) (n-2):centres)
  | otherwise  =  error "finalCentres: input < 0"               

-----------------------------------------------------------------------------
-- maximalOddPalindromesDNA
-----------------------------------------------------------------------------

-- | maximalOddPalindromesDNA returns the maximal odd-length palindrome 
--   around each position in a string. 
maximalOddPalindromesDNA        :: B.ByteString -> String
maximalOddPalindromesDNA input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input) 
  $ zip (oddDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalOddPalindromesLengthAtLeastDNA
-----------------------------------------------------------------------------

-- | maximalOddPalindromesLengthAtLeastDNA returns the maximal odd-length 
--   palindrome around each position in a string. The integer argument is 
--   used to only show palindromes of length at least this integer.
maximalOddPalindromesLengthAtLeastDNA          :: Int -> B.ByteString -> String
maximalOddPalindromesLengthAtLeastDNA m input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input) 
  $ filter ((m<=) . fst)
  $ zip (oddDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalOddPalindromesLengthBetweenDNA
-----------------------------------------------------------------------------

-- | maximalOddPalindromesLengthBetweenDNA returns the maximal odd-length 
--   palindrome around each position in a string. The integer arguments are 
--   used to only show (or cut off to) palindromes of length in between the 
--   specified lengths.
maximalOddPalindromesLengthBetweenDNA          :: Int -> Int -> B.ByteString -> String
maximalOddPalindromesLengthBetweenDNA m n input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input . \(pl,pc) -> (min pl n,pc)) 
  $ filter (\(pl,_) -> m <= pl)
  $ zip (oddDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- maximalOddPalindromesLengthAtMostDNA
-----------------------------------------------------------------------------

-- | maximalOddPalindromesLengthAtMostDNA returns the maximal odd-length 
--   palindrome around each position in a string. The integer argument is 
--   used to only show (or cut off to) palindromes of length at most the 
--   specified length.
maximalOddPalindromesLengthAtMostDNA          :: Int -> B.ByteString -> String
maximalOddPalindromesLengthAtMostDNA m input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input) 
  $ filter ((<=m) . fst)
  $ zip (oddDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- oddPalindromesLengthExactDNA
-----------------------------------------------------------------------------

-- | oddPalindromesLengthExactDNA returns the maximal odd-length palindrome 
--   around each position in a string. The integer arguments are used to only 
--   show (or cut off to) palindromes of exactly the specified length.
oddPalindromesLengthExactDNA          :: Int -> B.ByteString -> String
oddPalindromesLengthExactDNA m input  = 
    intercalate "\n" 
  $ map (showPalindromeDNA input . \(_,p) -> (m,p)) 
  $ filter (\(l,_) -> m<=l && (odd l == odd m))
  $ zip (oddDNAPalindromesAroundCentres input) [0..]

-----------------------------------------------------------------------------
-- oddDNAPalindromesAroundCentres
--
-- The function that implements the palindrome finding algorithm.
-- Used in all the above interface functions.
-----------------------------------------------------------------------------

-- | oddDNAPalindromesAroundCentres is the central function of the module. It 
--   returns the list of lenghths of the maximal odd-length palindrome around 
--   each position in a string.
oddDNAPalindromesAroundCentres        :: B.ByteString -> [Int]
oddDNAPalindromesAroundCentres input  =  if B.null input
	                                     then []
	                                     else reverse $ oddExtendTail input 1 1 []

oddExtendTail :: B.ByteString -> Int -> Int -> [Int] -> [Int]
oddExtendTail input n currentTail centres 
  | n > inputlast                                            =  oddFinalCentres currentTail centres (currentTail:centres)
      -- reached the end of the array                                     
  | n-currentTail <= inputfirst                              =  oddExtendCentres input n (currentTail:centres) centres currentTail 
      -- the current longest tail palindrome extends to the start of the array
  | B.index input n `eqDNA` B.index input (n-currentTail-1)  =  oddExtendTail input (n+1) (currentTail+2) centres      
      -- the current longest tail palindrome can be extended
  | otherwise                                                =  oddExtendCentres input n (currentTail:centres) centres currentTail
      -- the current longest tail palindrome cannot be extended
  where  inputfirst = 0
         inputlast  = B.length input - 1

oddExtendCentres :: B.ByteString -> Int -> [Int] -> [Int] -> Int -> [Int]
oddExtendCentres input n centres tcentres centreDistance
  | centreDistance == 1 || null tcentres  =  oddExtendTail input (n+1) 1 centres
      -- the last centre is on the last element: try to extend the tail of length 1
  | centreDistance-2 == head tcentres     =  oddExtendTail input n (head tcentres) centres
      -- the previous element in the centre list reaches exactly to the end of the last 
      -- tail palindrome use the mirror property of palindromes to find the longest tail palindrome
  | otherwise                             =  oddExtendCentres input n (min (head tcentres) (centreDistance-2):centres) (tail tcentres) (centreDistance-2)
      -- move the centres one step add the length of the longest palindrome to the centres

oddFinalCentres :: Int -> [Int] -> [Int] -> [Int]
oddFinalCentres n tcentres centres  
  | n > 1      =  oddFinalCentres (n-2) (tail tcentres) (min (head tcentres) (n-2):centres)
  | otherwise  =  centres

-----------------------------------------------------------------------------
-- Equality on DNA
-----------------------------------------------------------------------------

negateDNA      :: Char -> Char
negateDNA 'A'  =  'T'
negateDNA 'T'  =  'A'
negateDNA 'C'  =  'G'
negateDNA 'G'  =  'C'
negateDNA _    =  error "negateDNA: not a DNA character"

eqDNA      :: Word8 -> Word8 -> Bool
eqDNA l r  =  let cl = BI.w2c l
                  cr = BI.w2c r
              in toUpper cl == negateDNA (toUpper cr)

-----------------------------------------------------------------------------
-- Showing palindromes and other text related functionality
-----------------------------------------------------------------------------

showPalindromeDNA :: B.ByteString -> (Int,Int) -> String
showPalindromeDNA input (len,pos) = 
  let startpos = pos - len `div` 2
  in (show startpos ++) . (" to " ++) . (show (startpos+len) ++) . ("\t" ++) . (show (B.take len $ B.drop startpos input) ++) . ("\t" ++) $ show len

isDNASymbol    :: Word8 -> Bool
isDNASymbol w  =  w2c w `elem` "ACTGactg"
