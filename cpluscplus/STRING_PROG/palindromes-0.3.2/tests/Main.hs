-----------------------------------------------------------------------------
-- |
-- Module      :  tests.Main
-- Copyright   :  (c) 2007 - 2011 Johan Jeuring
-- License     :  BSD3
--
-- Maintainer  :  johan@jeuring.net
-- Stability   :  experimental
-- Portability :  portable
--
-----------------------------------------------------------------------------

module Main where


import Data.Array
import Data.Char

import Test.QuickCheck
import Test.HUnit

import  Data.Algorithms.Palindromes.Palindromes 
import  qualified PalindromesConstantArguments as P
import qualified Data.ByteString as B
import qualified Data.ByteString.Char8 as BC

propPalindromesAroundCentres :: Property
propPalindromesAroundCentres = 
  forAll (arbitrary:: Gen [Char]) $ 
  \l -> let input = BC.pack l
        in palindromesAroundCentres input == longestPalindromesQ input

longestPalindromesQ    ::  B.ByteString -> [Int]
longestPalindromesQ input  =   
  let (afirst,alast)  =  (0,B.length input - 1)
      positions       =  [0 .. 2*(alast-afirst+1)]
  in  map (lengthPalindromeAround input) positions

lengthPalindromeAround  ::  B.ByteString 
                                 -> Int 
                                 -> Int
lengthPalindromeAround input position 
  | even position = 
      extendPalindromeAround (afirst+pos-1) (afirst+pos) 
  | odd  position = 
      extendPalindromeAround (afirst+pos-1) (afirst+pos+1) 
  where  pos             =  div position 2
         (afirst,alast)  =  (0,B.length input -1)
         extendPalindromeAround start end  = 
            if   start < 0 
              || end > alast-afirst 
              || B.index input start /= B.index input end
            then end-start-1
            else extendPalindromeAround (start-1) (end+1) 

propTextPalindrome :: Property
propTextPalindrome =
  forAll (arbitrary:: Gen [Char]) $ 
    \l -> let ltp   =  longestTextPalindrome (BC.pack l)
              ltp'  =  map toLower (filter myIsLetterC (unescape ltp))
          in ltp' == reverse ltp'

unescape :: String -> String
unescape [] = []
unescape cs = case readLitChar cs of
                (c,rest):xs -> c:unescape rest
                []          -> []   

testTextPalindrome1, testTextPalindrome2, testTextPalindrome3, testTextPalindrome4, 
  testTextPalindrome5, testTextPalindrome6, testTextPalindrome7, testTextPalindrome8,
  testTextPalindrome9, testTextPalindrome10, testTextPalindrome11 :: Test

testWordPalindrome1, testWordPalindrome2, testWordPalindrome3, testWordPalindrome4,
  testWordPalindrome5, testWordPalindrome6 :: Test

testTextPalindrome1 =
  TestCase (assertEqual 
              "textPalindrome1" 
              "\"a,ba.\"" 
              (longestTextPalindrome (BC.pack "abcdea,ba."))
           )
testTextPalindrome2 =
  TestCase (assertEqual 
              "textPalindrome2" 
              "\"a,ba\"" 
              (longestTextPalindrome (BC.pack "abcdea,ba"))
           )
testTextPalindrome3 =
  TestCase (assertEqual 
              "textPalindrome3" 
              "\".a,ba\"" 
              (longestTextPalindrome (BC.pack "abcde.a,ba"))
           )
testTextPalindrome4 =
  TestCase (assertEqual 
              "textPalindrome4" 
              "\".a,ba\"" 
              (longestTextPalindrome (BC.pack "abcde.a,baf"))
           )
testTextPalindrome5 =
  TestCase (assertEqual 
              "textPalindrome5" 
              "\".ab,a\"" 
              (longestTextPalindrome (BC.pack ".ab,acdef"))
           )
testTextPalindrome6 =
  TestCase (assertEqual 
              "textPalindrome6" 
              "\"ab,a\"" 
              (longestTextPalindrome (BC.pack "ab,acdef"))
           )
testTextPalindrome7 =
  TestCase (assertEqual 
              "textPalindrome7" 
              "\"ab,a.\"" 
              (longestTextPalindrome (BC.pack "ab,a.cdef"))
           )
testTextPalindrome8 =
  TestCase (assertEqual 
              "textPalindrome8" 
              "\".ab,a.\"" 
              (longestTextPalindrome (BC.pack "g.ab,a.cdef"))
           )
testTextPalindrome9 =
  TestCase (assertEqual 
              "textPalindrome9" 
              "" 
              (longestTextPalindrome B.empty)
           )

testTextPalindrome10 =
  TestCase (do string <- B.readFile "examples/palindromes/Damnitimmad.txt"
               assertEqual 
                 "textPalindrome10" 
                 (concatMap (\c -> case c of
                                     '\n' -> "\\n" 
                                     '\"' -> "\""--""\\\""
                                     d    -> [d]
                            )
                            (show string)
                 )
                 (longestTextPalindrome string)
           )

testTextPalindrome11 =
  TestCase (do string <- B.readFile "examples/palindromes/pal17.txt"
               assertEqual 
                 "textPalindrome11" 
                 (concatMap (\c -> case c of
                                     '\n' -> "\\n" 
                                     '\"' -> "\""
                                     d    -> [d]
                            )
                  (show string))
                 (longestTextPalindrome string)
           )

testWordPalindrome1 =
  TestCase (assertEqual
              "wordPalindrome" 
              "\" is non si, \"" 
              (longestWordPalindrome (BC.pack "what is non si, not?"))
           )

testWordPalindrome2 =
  TestCase (assertEqual
              "wordPalindrome" 
              "\" is non si\"" 
              (longestWordPalindrome (BC.pack "what is non si"))
           )

testWordPalindrome3 =
  TestCase (assertEqual
              "wordPalindrome" 
              "\"is non si, \"" 
              (longestWordPalindrome (BC.pack "is non si, not?"))
           )

testWordPalindrome4 =
  TestCase (assertEqual
              "wordPalindrome" 
              "" 
              (longestWordPalindrome (BC.pack "aaaaba"))
           )

testWordPalindrome5 =
  TestCase (assertEqual
              "wordPalindrome" 
              "\" a\"" 
              (longestWordPalindrome (BC.pack "aaaab a"))
           )

testWordPalindrome6 =
  TestCase (assertEqual
              "wordPalindrome" 
              "\" waaw \"" 
              (longestWordPalindrome (BC.pack "w waaw wo waw"))
           )

tests :: Test
tests = TestList [TestLabel "testTextPalindrome1"  testTextPalindrome1
                 ,TestLabel "testTextPalindrome2"  testTextPalindrome2
                 ,TestLabel "testTextPalindrome3"  testTextPalindrome3
                 ,TestLabel "testTextPalindrome4"  testTextPalindrome4
                 ,TestLabel "testTextPalindrome5"  testTextPalindrome5
                 ,TestLabel "testTextPalindrome6"  testTextPalindrome6
                 ,TestLabel "testTextPalindrome7"  testTextPalindrome7
                 ,TestLabel "testTextPalindrome8"  testTextPalindrome8
                 ,TestLabel "testTextPalindrome9"  testTextPalindrome9
                 ,TestLabel "testTextPalindrome10" testTextPalindrome10
                 ,TestLabel "testTextPalindrome11" testTextPalindrome11
                 ,TestLabel "testWordPalindrome1"  testWordPalindrome1
                 ,TestLabel "testWordPalindrome2"  testWordPalindrome2
                 ,TestLabel "testWordPalindrome3"  testWordPalindrome3
                 ,TestLabel "testWordPalindrome4"  testWordPalindrome4
                 ,TestLabel "testWordPalindrome5"  testWordPalindrome5
                 ,TestLabel "testWordPalindrome6"  testWordPalindrome6
                 ]

main :: IO Counts
main = do 
          quickCheck  propPalindromesAroundCentres
          quickCheck  propTextPalindrome
          runTestTT tests


{-
2nd property falsified by
"m\159:t\231\202\r\STX-me\230\&9JS/\EM'5\164\171\148\&5A@\196\242\f\157jY\NULB,\134\179\ESCS`:\ff\203\b\130\&0\DC3Yni>L"

"\GS[\242\tx\ENQ3\247\&3\130(\NUL?zX\215\DC3"

"\213\SI6+\ESCU:1\165\254\228\SUB9\200\231\USM,3\227\&3\176\214X\203\SOH\130UI9\154\239<w\231kPbmvY|!sc\133\b$#v\203LM\235H"

-}

{-

-- Code for benchmarking. Needs to go in a separate file.

import Criterion.Main
import Data.Algorithms.Palindromes.Palindromes
import System.IO
import qualified Data.ByteString as B

main :: IO ()
main = 
  do fnenhl <- openFile  "examples/palindromes/Damnitimmad.txt" ReadMode-- "../../TestSources/Bibles/engelskingjames.txt" ReadMode
     hSetEncoding fnenhl latin1 
     inputenB <- B.hGetContents fnenhl
--     fnnlhl <- openFile  "../../TestSources/Bibles/nederlands.txt" ReadMode
--     hSetEncoding fnnlhl latin1 
--    inputnlB <- B.hGetContents fnnlhl
     defaultMain
       [
--        bench "longestPalindromeConstantArguments Dutch" (nf CA.longestPalindrome inputnlB)--,
--        bench "longestPalindrome Dutch" (nf longestPalindrome inputnlB)--,
--       bench "longestPalindromeConstantArguments English" (nf CA.longestPalindrome inputenB)--,
        bench "longestPalindrome English" (nf longestPalindrome inputenB)--,--,
       ]       

-}
{-

To compare my solution and Rampion's lazy solution:

       [bench "lengthLongestPalindromes" (nf (palindromesAroundCentres (==) . listArrayl0) input)
       ,bench "Rampion's solution" (nf maximalPalindromeLengths input)
       ]

-}

