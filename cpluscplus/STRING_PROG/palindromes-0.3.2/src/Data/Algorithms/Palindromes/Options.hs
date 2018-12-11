-----------------------------------------------------------------------------
--
-- Module      :  Data.Algorithms.Palindromes.Options
-- Copyright   :  (c) 2007 - 2012 Johan Jeuring
-- License     :  BSD3
--
-- Maintainer  :  johan@jeuring.net
-- Stability   :  experimental
-- Portability :  portable
--
-----------------------------------------------------------------------------
module Data.Algorithms.Palindromes.Options where

import System.Console.GetOpt 

import qualified Data.ByteString as B

import Data.Algorithms.Palindromes.Palindromes
import Data.Algorithms.Palindromes.PalindromesDNA

-----------------------------------------------------------------------------
-- flags
-----------------------------------------------------------------------------

data Flag  =  Help
           |  StandardInput
           |  LongestPalindrome
           |  LengthLongestPalindrome
           |  LongestTextPalindrome
           |  LongestWordPalindrome
           |  MaximalPalindromes
           |  LengthMaximalPalindromes
           |  MaximalTextPalindromes
           |  MaximalWordPalindromes
           |  LengthAtLeast Int
           |  LengthAtMost Int
           |  LengthExact Int 
           |  DNA 
           |  Odd deriving Show
          
isHelp :: Flag -> Bool
isHelp Help                        =  True
isHelp _                           =  False      

isDNA :: Flag -> Bool
isDNA DNA                          =  True
isDNA _                            =  False      

isOdd :: Flag -> Bool
isOdd Odd                          =  True
isOdd _                            =  False      

isStandardInput :: Flag -> Bool
isStandardInput StandardInput      =  True
isStandardInput _                  =  False      

isLengthAtLeast :: Flag -> Bool
isLengthAtLeast (LengthAtLeast _)  =  True
isLengthAtLeast _                  =  False      

isLengthAtMost :: Flag -> Bool
isLengthAtMost (LengthAtMost _)    =  True
isLengthAtMost _                   =  False      

isLengthExact :: Flag -> Bool
isLengthExact (LengthExact _)      =  True
isLengthExact _                    =  False      

isLength       :: Flag -> Bool
isLength flag  =  isLengthAtLeast flag || isLengthAtMost flag || isLengthExact flag

getLength :: Flag -> Int
getLength (LengthAtLeast n)  =  n 
getLength (LengthAtMost  n)  =  n 
getLength (LengthExact   n)  =  n 
getLength _                  =  error "No length specified"

-- I am using single letter options here (except for help): getOpt handles 
-- options too flexible: in case a letter within a multiple letter option is 
-- recognized, it is taken as a single letter option.
options :: [OptDescr Flag] 
options = 
  [Option "h" ["help"] (NoArg Help)
     "This message"
  ,Option "i" [] (NoArg StandardInput)
     "Read input from standard input"
  ,Option "p" [] (NoArg LongestPalindrome) 
     "Longest palindrome (default)"
  ,Option "l" [] (NoArg LengthLongestPalindrome)
     "Length of the longest palindrome"
  ,Option "t" [] (NoArg LongestTextPalindrome)
     "Longest palindrome ignoring case, spacing and punctuation"
  ,Option "v" [] (NoArg LongestWordPalindrome)
     "Longest text palindrome preceded and followed by punctuation symbols (if any)"
  ,Option "m" [] (NoArg MaximalPalindromes)
     "Maximal palindrome around each position in the input"
  ,Option "k" [] (NoArg LengthMaximalPalindromes)
     "Length of the longest palindrome around each position in the input"
  ,Option "s" [] (NoArg MaximalTextPalindromes)
     "Maximal text palindrome around each position in the input"
  ,Option "w" [] (NoArg MaximalWordPalindromes)
     "Longest word palindrome around each position in the input"
  ,Option "a" [] (ReqArg (LengthAtLeast . (read :: String -> Int)) "arg")
     "Maximal palindromes of length at least [arg]"
  ,Option "b" [] (ReqArg (LengthAtMost . (read :: String -> Int)) "arg")
     "Maximal palindromes (possibly cut off) of length at most [arg]"
  ,Option "c" [] (ReqArg (LengthExact . (read :: String -> Int)) "arg")
     "Maximal alindromes (possibly cut off) of length exactly [arg]"
  ,Option "d" [] (NoArg DNA)
     "Use DNA equality"
  ,Option "o" [] (NoArg Odd)
     "Return palindromes of odd length (for palindromes in DNA; the default here is even length)"
  ]

-- This needs to be refactored seriously!
handleOptions :: [Flag] -> (B.ByteString -> String,Bool)
handleOptions flags = 
  let lengthFlags  =  filter isLength flags
      dna          =  any isDNA flags
      isodd        =  any isOdd flags
      flags'       =  filter (\f -> not (isLength f || isStandardInput f || isDNA f)) flags
      fromfile     =  not (any isStandardInput flags)
      function     =  case flags' of
                        []        -> longestPalindrome
                        (flag:_)  -> 
                          case flag of 
                            Help                      ->  
                              const (usageInfo headerHelpMessage options)
                            LongestPalindrome        ->  longestPalindrome
                            LengthLongestPalindrome  ->  lengthLongestPalindrome
                            LongestTextPalindrome    ->  longestTextPalindrome 
                            LongestWordPalindrome    ->  longestWordPalindrome
                            f                        ->  dispatchMaximalFlags dna isodd lengthFlags f 
  in (function,fromfile)

dispatchMaximalFlags  ::  Bool -> Bool -> [Flag] -> Flag -> B.ByteString -> String
dispatchMaximalFlags True False lengthFlags f 
  | null lengthFlags
      = case f of 
          MaximalPalindromes        ->  maximalEvenPalindromesDNA . B.filter isDNASymbol 
          LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
          MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
          MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
          _                         ->  error "handleOptions, no length flags" 
  | length lengthFlags == 1
      = let l = getLength (head lengthFlags) in 
        if isLengthAtLeast (head lengthFlags)
        then case f of  
               MaximalPalindromes        ->  maximalEvenPalindromesLengthAtLeastDNA l . B.filter isDNASymbol 
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
               MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
               MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
               _                         ->  error "handleOptions length at least" 
        else if isLengthAtMost (head lengthFlags)
        then case f of  
               MaximalPalindromes        ->  maximalEvenPalindromesLengthAtMostDNA l . B.filter isDNASymbol 
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
               MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
               MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
               _                         ->  error "handleOptions length at most" 
        else case f of  
               MaximalPalindromes        ->  evenPalindromesLengthExactDNA l . B.filter isDNASymbol 
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
               MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
               MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
               _                         ->  error "handleOptions length exact" 
  | length lengthFlags == 2 && length (filter isLengthAtMost lengthFlags) == 1 && length (filter isLengthAtLeast lengthFlags) == 1
      = let m = minimum (map getLength lengthFlags); n = maximum (map getLength lengthFlags) 
        in case f of 
             MaximalPalindromes        ->  maximalEvenPalindromesLengthBetweenDNA m n . B.filter isDNASymbol 
             LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
             MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
             MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
             _                         ->  error ("handleOptions two length flags: " ++ show f)
  | otherwise 
      = error "handleOptions"
dispatchMaximalFlags True True lengthFlags f 
  | null lengthFlags
      = case f of 
          MaximalPalindromes        ->  maximalOddPalindromesDNA . B.filter isDNASymbol 
          LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
          MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
          MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
          _                         ->  error "handleOptions, no length flags" 
  | length lengthFlags == 1
      = let l = getLength (head lengthFlags) in 
        if isLengthAtLeast (head lengthFlags)
        then case f of  
               MaximalPalindromes        ->  maximalOddPalindromesLengthAtLeastDNA l . B.filter isDNASymbol 
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
               MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
               MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
               _                         ->  error "handleOptions length at least" 
        else if isLengthAtMost (head lengthFlags)
        then case f of  
               MaximalPalindromes        ->  maximalOddPalindromesLengthAtMostDNA l . B.filter isDNASymbol 
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
               MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
               MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
               _                         ->  error "handleOptions length at most" 
        else case f of  
               MaximalPalindromes        ->  oddPalindromesLengthExactDNA l . B.filter isDNASymbol 
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
               MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
               MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
               _                         ->  error "handleOptions length exact" 
  | length lengthFlags == 2 && length (filter isLengthAtMost lengthFlags) == 1 && length (filter isLengthAtLeast lengthFlags) == 1
      = let m = minimum (map getLength lengthFlags); n = maximum (map getLength lengthFlags) 
        in case f of 
             MaximalPalindromes        ->  maximalOddPalindromesLengthBetweenDNA m n . B.filter isDNASymbol 
             LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported for DNA palindromes" 
             MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported for DNA palindromes" 
             MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromesLengthAtLeast not supported for DNA palindromes" 
             _                         ->  error ("handleOptions two length flags: " ++ show f)
  | otherwise = error "handleOptions"
dispatchMaximalFlags False _ lengthFlags f
  | null lengthFlags
      = case f of 
          MaximalPalindromes        ->  maximalPalindromes
          LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported" 
          MaximalTextPalindromes    ->  maximalTextPalindromesLengthAtLeast 0
          MaximalWordPalindromes    ->  maximalWordPalindromesLengthAtLeast 0
          _                         ->  error "handleOptions, no length flags" 
  | length lengthFlags == 1
      = let l = getLength (head lengthFlags) in 
        if isLengthAtLeast (head lengthFlags)
        then case f of  
               MaximalPalindromes        ->  maximalPalindromesLengthAtLeast l
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported"
               MaximalTextPalindromes    ->  maximalTextPalindromesLengthAtLeast l
               MaximalWordPalindromes    ->  maximalWordPalindromesLengthAtLeast l
               _                         ->  error "handleOptions length at least" 
        else if isLengthAtMost (head lengthFlags)
        then case f of  
               MaximalPalindromes        ->  maximalPalindromesLengthAtMost l
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option LengthMaximalPalindromes not supported"
               MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option MaximalTextPalindromes not supported"
               MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option MaximalWordPalindromes not supported"
               _                         ->  error "handleOptions length at most" 
        else case f of  
               MaximalPalindromes        ->  palindromesLengthExact l
               LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option not supported"
               MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option not supported"
               MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option not supported"
               _                         ->  error "handleOptions length exact" 
  | length lengthFlags == 2 && length (filter isLengthAtMost lengthFlags) == 1 && length (filter isLengthAtLeast lengthFlags) == 1
      = let m = minimum (map getLength lengthFlags); n = maximum (map getLength lengthFlags) 
        in case f of 
             MaximalPalindromes        ->  maximalPalindromesLengthBetween m n
             LengthMaximalPalindromes  ->  error "dispatchMaximalFlags: option not supported"
             MaximalTextPalindromes    ->  error "dispatchMaximalFlags: option not supported"
             MaximalWordPalindromes    ->  error "dispatchMaximalFlags: option not supported"
             _                         ->  error ("handleOptions two length flags: " ++ show f)
  | otherwise 
      = error "handleOptions"

headerHelpMessage :: String
headerHelpMessage = 
     "*********************\n"
  ++ "* Palindrome Finder *\n"
  ++ "* version 0.3.2     *\n"
  ++ "*********************\n"
  ++ "Usage:"
